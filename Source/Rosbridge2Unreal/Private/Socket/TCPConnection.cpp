#include "TCPConnection.h"

#include "LogCategory.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "jsoncons_unreal_wrapper.h"


bool UTCPConnection::Initialize(FString IPAddress, int Port, TransportMode Mode)
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("Rosbridge TCP client"), false);
	
	TSharedRef<FInternetAddr> InternetAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bIPValid;
	InternetAddress->SetIp(*IPAddress, bIPValid);
	InternetAddress->SetPort(Port);

	int32 NewSize = 0;
	Socket->SetReceiveBufferSize(65536, NewSize); //16bit address, used by ROSBridge as a default
	
	if (!bIPValid)
	{
		UE_LOG(LogROSBridge, Error, TEXT("Given IP address is invalid: %s"), *IPAddress);
		return false;
	}

	if (!Socket->Connect(*InternetAddress))
	{
		UE_LOG(LogROSBridge, Error, TEXT("Could not connect to: %s:%d"), *IPAddress, Port);
		return false;
	}

	CurrentTransportMode = Mode;
	
	// Setting up and starting the receiver thread
	ReceiverThread = FRunnableThread::Create(this, TEXT("ROSBridgeReceiverThread"), 0, TPri_Normal);

	return true;
}

void UTCPConnection::Uninitialize()
{
	bTerminateReceiverThread = true;
	if (ReceiverThread) {
		ReceiverThread->WaitForCompletion();
	}
	if (Socket) {
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
	}
}
bool UTCPConnection::SendMessage(ROSData& Data) const
{
	if(CurrentTransportMode == TransportMode::BSON)
	{
		std::vector<uint8> EncodedData;
		jsoncons::bson::encode_bson(Data,EncodedData);
		return SendMessage(EncodedData.data(), EncodedData.size());
	}else if(CurrentTransportMode == TransportMode::JSON)
	{
		std::string EncodedData;
		Data.dump(EncodedData, jsoncons::indenting::no_indent);
		return SendMessage(reinterpret_cast<const uint8*>(EncodedData.c_str()), EncodedData.length());
	}
	return false;
}

bool UTCPConnection::SendMessage(FString Data) const
{
	return SendMessage(reinterpret_cast<const uint8*>(TCHAR_TO_UTF8(*Data)), Data.Len());
}

bool UTCPConnection::SendMessage(const uint8_t *Data, const unsigned int Length) const
{
	uint8_t BytesSendTotal = 0;

	for(uint8_t SendTries = 0; SendTries < 3; SendTries++)
	{
		int32 LastBytesSend = 0;

		if(Socket->Send(Data + BytesSendTotal, Length - BytesSendTotal, LastBytesSend))
		{
			BytesSendTotal += LastBytesSend;
			if(BytesSendTotal == Length) return true;
		}
	}
	return false;
}

//ReceiverThread function
uint32 UTCPConnection::Run()
{
	bReceiverThreadRunning = true;
	
	TArray<uint8> BinaryBuffer;
	BinaryBuffer.Reserve(65536); //16bit address, used by ROSBridge as a default
	uint32 PendingData = 0;
	uint32 RemainingData = 0;

	while (!bTerminateReceiverThread) {

		//Check Connection State
		const ESocketConnectionState ConnectionState = Socket->GetConnectionState();
		if (ConnectionState == SCS_NotConnected) {
			FPlatformProcess::Sleep(0.5);
			continue;
		} else if (ConnectionState == SCS_ConnectionError){
			UE_LOG(LogROSBridge, Error, TEXT("Error on connection"));
			ReportError(TransportError::SocketError);
			bReceiverThreadRunning = false;
			return EXIT_FAILURE;
		}

		if (!Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromMilliseconds(100)))
		{
			continue; // check if any errors occured
		}

		if(CurrentTransportMode == TransportMode::BSON){
			if(!Socket->HasPendingData(PendingData) || PendingData < 4)
			{
				continue; //wait further
			}

			int32 BSONMessageLength = 0;
			int32 BytesRead = 0;
			if(!Socket->Recv(reinterpret_cast<uint8*>(&BSONMessageLength), sizeof(BSONMessageLength), BytesRead) || BytesRead < sizeof(BSONMessageLength))
			{
				UE_LOG(LogROSBridge, Error, TEXT("Failed to receive BSON message length. Closing receiver thread."));
				ReportError(TransportError::SocketError);
				bReceiverThreadRunning = false;
				return EXIT_FAILURE;
			}

			//Initialize Buffer
			if(BinaryBuffer.Num() < BSONMessageLength) BinaryBuffer.SetNumZeroed(BSONMessageLength, false);
			FMemory::Memcpy(BinaryBuffer.GetData(),&BSONMessageLength,sizeof(BSONMessageLength));
				
			//Receive BSONMessageLength bytes in binary_buffer
			int32 TotalBytesRead = sizeof(BSONMessageLength);
			while(TotalBytesRead < BSONMessageLength)
			{
				if(!Socket->Recv(BinaryBuffer.GetData() + TotalBytesRead, BSONMessageLength - TotalBytesRead, BytesRead))
				{
					UE_LOG(LogROSBridge, Error, TEXT("Failed to receive from socket. Closing receiver thread."));
					ReportError(TransportError::SocketError);
					bReceiverThreadRunning = false;
					return EXIT_FAILURE;
				}
				TotalBytesRead += BytesRead;
			}

			ROSData Data;
			try{
				Data = jsoncons::bson::decode_bson<ROSData>(BinaryBuffer.GetData(), BinaryBuffer.GetData() + BSONMessageLength);
			}catch(jsoncons::ser_error e)
			{
				UE_LOG(LogROSBridge, Error, TEXT("Error while parsing BSON message (Ignoring message)"));
				continue;
			}
			
			if (IncomingMessageCallback && !bTerminateReceiverThread) IncomingMessageCallback(Data);
			
		} else if(CurrentTransportMode == TransportMode::JSON) {

			if(!Socket->HasPendingData(PendingData)) continue;
			
			//Ensure space in buffer
			if(static_cast<uint32>(BinaryBuffer.Num()) < PendingData + RemainingData) BinaryBuffer.SetNumZeroed(PendingData + RemainingData, false);
			
			int32 BytesRead = 0;
			if(!Socket->Recv(BinaryBuffer.GetData() + RemainingData, PendingData, BytesRead, ESocketReceiveFlags::WaitAll))
			{
				UE_LOG(LogROSBridge, Error, TEXT("Failed to receive from socket. Closing receiver thread."));
				ReportError(TransportError::SocketError);
				bReceiverThreadRunning = false;
				return EXIT_FAILURE;
			}

			if(BytesRead <= 0) continue;

			//Split multiple jsons into single ones
			uint64 StartChar = 0;
			int64 OpenScopes = 0;
			for(uint64 i = 0; i < BytesRead + RemainingData; ++i)
			{
				if(BinaryBuffer[i] == '{' && OpenScopes == 0) StartChar = i;
				if(BinaryBuffer[i] == '{') OpenScopes++;
				if(BinaryBuffer[i] == '}') OpenScopes--;
				if(OpenScopes < 0){
					StartChar = i;
					OpenScopes = 0;
				}
				if(OpenScopes == 0 && i - StartChar > 0)
				{
					ROSData Data;
					try{
						Data = jsoncons::ojson::parse(jsoncons::ojson::string_view_type(reinterpret_cast<char*>(BinaryBuffer.GetData() + StartChar), i - StartChar + 1));
						if (IncomingMessageCallback && !bTerminateReceiverThread) IncomingMessageCallback(Data);
					}catch(jsoncons::ser_error e)
					{
						UE_LOG(LogROSBridge, Error, TEXT("Error while parsing JSON message (Ignoring message): %hs"), e.what());
						continue;
					}
					StartChar = i + 1;
				}
			}

			RemainingData = BytesRead + RemainingData - StartChar;
			if(RemainingData >= 0) //Remaining Data (incomplete JSON)
			{
				FMemory::Memmove(BinaryBuffer.GetData(),BinaryBuffer.GetData() + StartChar, RemainingData);
			}
		}
	}

	bReceiverThreadRunning = false;
	return EXIT_SUCCESS;
}


void UTCPConnection::RegisterIncomingMessageCallback(TFunction<void(ROSData)> CallbackFunction)
{
	IncomingMessageCallback = CallbackFunction;
}

void UTCPConnection::ReportError(const TransportError Error) const
{
	switch(Error)
	{
	case TransportError::SocketError:
		UE_LOG(LogROSBridge, Error, TEXT("Socket Error in ROSBridge"));	
		break;
	case TransportError::ConnectionClosed:
		UE_LOG(LogROSBridge, Error, TEXT("Socket Connection Closed"));	
		break;
	}
}

void UTCPConnection::SetTransportMode(const TransportMode Mode)
{
	CurrentTransportMode = Mode;
}

bool UTCPConnection::IsHealthy() const
{
	return bReceiverThreadRunning;
}

TransportMode UTCPConnection::GetTransportMode() const
{
	return CurrentTransportMode;
}
