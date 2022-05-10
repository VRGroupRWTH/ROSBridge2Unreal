#include "TCPConnection.h"

#include "LogCategory.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include <fstream>
#include "jsoncons_unreal_wrapper.h"


bool UTCPConnection::Initialize(FString IPAddress, int Port, ETransportMode Mode)
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("Rosbridge TCP client"), false);

	TSharedRef<FInternetAddr> InternetAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bIPValid;
	InternetAddress->SetIp(*IPAddress, bIPValid);
	InternetAddress->SetPort(Port);

	Socket->SetReceiveBufferSize(65536, ReceiveBufferSize); //16bit address, used by ROSBridge as a default

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

bool UTCPConnection::SendMessage(const uint8_t* Data, const unsigned int Length) const
{
	int32 BytesSend = 0;
	return Socket->Send(Data, Length, BytesSend) && BytesSend == Length;
}

//ReceiverThread function
uint32 UTCPConnection::Run()
{
	bReceiverThreadRunning = true;

	TArray<uint8> BinaryBuffer;
	BinaryBuffer.Reserve(ReceiveBufferSize); // at least accomodate the whole socket buffer
	uint32 PendingData = 0;
	jsoncons::json_decoder<jsoncons::ojson> Decoder;
	jsoncons::json_parser Parser;

	while (!bTerminateReceiverThread) {

		//Check Connection State
		const ESocketConnectionState ConnectionState = Socket->GetConnectionState();
		if (ConnectionState == SCS_NotConnected) {
			FPlatformProcess::Sleep(0.5);
			continue;
		}
		else if (ConnectionState == SCS_ConnectionError) {
			UE_LOG(LogROSBridge, Error, TEXT("Error on connection"));
			ReportError(ETransportError::SocketError);
			bReceiverThreadRunning = false;
			return EXIT_FAILURE;
		}

		if (!Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromMilliseconds(100)))
		{
			continue; // check if any errors occured
		}

		if (CurrentTransportMode == ETransportMode::BSON) {
			if (!Socket->HasPendingData(PendingData) || PendingData < 4)
			{
				continue; //wait further
			}

			int32 BSONMessageLength = 0;
			int32 BytesRead = 0;
			if (!Socket->Recv(reinterpret_cast<uint8*>(&BSONMessageLength), sizeof(BSONMessageLength), BytesRead) || BytesRead < sizeof(BSONMessageLength))
			{
				UE_LOG(LogROSBridge, Error, TEXT("Failed to receive BSON message length. Closing receiver thread."));
				ReportError(ETransportError::SocketError);
				bReceiverThreadRunning = false;
				return EXIT_FAILURE;
			}

			//Initialize Buffer
			if (BinaryBuffer.Num() < BSONMessageLength) BinaryBuffer.SetNumZeroed(BSONMessageLength, false);
			FMemory::Memcpy(BinaryBuffer.GetData(), &BSONMessageLength, sizeof(BSONMessageLength));

			//Receive BSONMessageLength bytes in binary_buffer
			int32 TotalBytesRead = sizeof(BSONMessageLength);
			while (TotalBytesRead < BSONMessageLength)
			{
				if (!Socket->Recv(BinaryBuffer.GetData() + TotalBytesRead, BSONMessageLength - TotalBytesRead, BytesRead))
				{
					UE_LOG(LogROSBridge, Error, TEXT("Failed to receive from socket. Closing receiver thread."));
					ReportError(ETransportError::SocketError);
					bReceiverThreadRunning = false;
					return EXIT_FAILURE;
				}
				TotalBytesRead += BytesRead;
			}

			ROSData Data;
			try {
				Data = jsoncons::bson::decode_bson<ROSData>(BinaryBuffer.GetData(), BinaryBuffer.GetData() + BSONMessageLength);
			}
			catch (jsoncons::ser_error e)
			{
				UE_LOG(LogROSBridge, Error, TEXT("Error while parsing BSON message (Ignoring message)"));
				continue;
			}

			if (IncomingMessageCallback && !bTerminateReceiverThread) IncomingMessageCallback(Data);

		}
		else if (CurrentTransportMode == ETransportMode::JSON) {

			if (!Socket->HasPendingData(PendingData)) continue;

			//Ensure space in buffer
			if (static_cast<uint32>(BinaryBuffer.Num()) < PendingData) BinaryBuffer.SetNumUninitialized(PendingData, false);

			int32 BytesRead = 0;
			if (!Socket->Recv(BinaryBuffer.GetData(), PendingData, BytesRead))
			{
				UE_LOG(LogROSBridge, Error, TEXT("Failed to receive from socket. Closing receiver thread."));
				ReportError(ETransportError::SocketError);
				bReceiverThreadRunning = false;
				return EXIT_FAILURE;
			}

			if (BytesRead <= 0) continue;

			try {
				Parser.update(reinterpret_cast<char*>(BinaryBuffer.GetData()), BytesRead);
				do
				{
					Parser.parse_some(Decoder);

					if (Parser.done() || Parser.state() == jsoncons::json_parse_state::before_done)
					{
						ROSData Data = Decoder.get_result();
						if (IncomingMessageCallback && !bTerminateReceiverThread) IncomingMessageCallback(Data);
						Parser.reset();
					}
				} while (!Parser.source_exhausted());
			}
			catch (jsoncons::ser_error e)
			{
				UE_LOG(LogROSBridge, Fatal, TEXT("Error while parsing JSON message: %hs"), e.what());
				throw;
			}
		}
	}

	bReceiverThreadRunning = false;
	return EXIT_SUCCESS;
}