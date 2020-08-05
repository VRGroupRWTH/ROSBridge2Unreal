#include "Messages/internal/ROSAuthMessage.h"


#include <hlslcc/hlslcc/src/hlslcc_lib/ast.h>
#include <openssl/sha.h>

#include "DataHelpers.h"

void UROSAuthMessage::ToData(ROSData& Message) const
{
	const FString Random = EncodeSHA512(FString::FromInt(FMath::Rand())).Left(10);
	const FString Time = FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp());
	const FString EndTime = FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp() + 31556952); // Now + 1 Year
	const FString Mac = EncodeSHA512(Secret + Client + Destination + Random + Time + Level + EndTime);
	
	DataHelpers::AppendString(Message, "op", "auth");
	DataHelpers::AppendString(Message, "mac", Mac);
	DataHelpers::AppendString(Message, "client", Client);
	DataHelpers::AppendString(Message, "dest", Destination);
	DataHelpers::AppendString(Message, "rand", Random);
	DataHelpers::AppendString(Message, "t", Time);
	DataHelpers::AppendString(Message, "level", Level);
	DataHelpers::AppendString(Message, "end", EndTime);
}

bool UROSAuthMessage::FromData(const ROSData& Message)
{
	return false; //Will never happen. Sending Authentications only.
}

FString UROSAuthMessage::EncodeSHA512(const FString Input)
{
	unsigned char Hash[SHA512_DIGEST_LENGTH];
	const std::string Message = TCHAR_TO_UTF8(*Input);
	SHA512(reinterpret_cast<const unsigned char*>(Message.c_str()), Message.length(), Hash);

	FString Result = "";
	for(uint8 i = 0; i < SHA512_DIGEST_LENGTH; i++)
	{
		Result += FString::Printf(TEXT("%02x"), Hash[i]);
	}
	
	return Result;
}
