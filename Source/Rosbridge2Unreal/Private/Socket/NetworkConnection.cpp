#include "Socket/NetworkConnection.h"


bool UNetworkConnection::SendMessage(const ROSData& Data) const
{
	if (CurrentTransportMode == ETransportMode::BSON)
	{
		std::vector<uint8> EncodedData;
		jsoncons::bson::encode_bson(Data,EncodedData);
		return SendMessage(EncodedData.data(), EncodedData.size());
	}
	else if (CurrentTransportMode == ETransportMode::JSON)
	{
		std::string EncodedData;
		Data.dump(EncodedData, jsoncons::indenting::no_indent);
		return SendMessage(reinterpret_cast<const uint8*>(EncodedData.c_str()), EncodedData.length());
	}
	return false;
}


void UNetworkConnection::ReportError(ETransportError Error) const
{
	switch(Error)
	{
	case ETransportError::SocketError:
		UE_LOG(LogROSBridge, Error, TEXT("Socket Error in ROSBridge"));	
		break;
	case ETransportError::ConnectionClosed:
		UE_LOG(LogROSBridge, Error, TEXT("Socket Connection Closed"));	
		break;
	};
}
