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
	uint32 PendingData = 0;

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

		if (!Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(1)))
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
			BinaryBuffer.SetNumZeroed(BSONMessageLength, false);
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
				UE_LOG(LogROSBridge, Error, TEXT("Error while parsing BSON message (Ignoring message): %hs"), e.what());
				continue;
			}
			
			if (IncomingMessageCallback && !bTerminateReceiverThread) IncomingMessageCallback(Data);
			
		} else if(CurrentTransportMode == TransportMode::JSON) {

			int32 TotalBytesRead = 0;
			while (Socket->HasPendingData(PendingData)) {
				//Ensure space in buffer
				BinaryBuffer.SetNumZeroed(TotalBytesRead + PendingData, false);
				
				int32 BytesRead = 0;
				if(!Socket->Recv(BinaryBuffer.GetData() + TotalBytesRead, PendingData, BytesRead))
				{
					UE_LOG(LogROSBridge, Error, TEXT("Failed to receive from socket. Closing receiver thread."));
					ReportError(TransportError::SocketError);
					bReceiverThreadRunning = false;
					return EXIT_FAILURE;
				}
				TotalBytesRead += BytesRead;
			}

			if(TotalBytesRead == 0) continue;
			
			//NULL termination done automatically
			BinaryBuffer.SetNumZeroed(TotalBytesRead + 1, false);

			//Split multiple jsons into single ones
			uint64 StartChar = 0;
			uint64 OpenScopes = 0;
			for(uint64 i = 0; i < BinaryBuffer.Num(); i++)
			{
				if(BinaryBuffer[i] == '{') OpenScopes++;
				if(BinaryBuffer[i] == '}') OpenScopes--;
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
