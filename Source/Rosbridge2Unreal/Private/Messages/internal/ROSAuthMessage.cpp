#include "Messages/internal/ROSAuthMessage.h"

THIRD_PARTY_INCLUDES_START
#define UI UI_ST
#include <openssl/evp.h>
#undef UI
THIRD_PARTY_INCLUDES_END


#include "DataHelpers.h"

void UROSAuthMessage::ToData(ROSData& OutMessage) const
{
	const FString Random = EncodeSHA512(FString::FromInt(FMath::Rand())).Left(10);
	const int64 Time = FDateTime::UtcNow().ToUnixTimestamp();
	const int64 EndTime = FDateTime::UtcNow().ToUnixTimestamp() + 31556952; // Now + 1 Year
	const FString Mac = EncodeSHA512(FString::Printf(TEXT("%s%s%s%s%lld%s%lld"), *Secret , *Client , *Destination , *Random , Time , *Level , EndTime));

	DataHelpers::AppendString(OutMessage, "op", "auth");
	DataHelpers::AppendString(OutMessage, "mac", Mac);
	DataHelpers::AppendString(OutMessage, "client", Client);
	DataHelpers::AppendString(OutMessage, "dest", Destination);
	DataHelpers::AppendString(OutMessage, "rand", Random);
	DataHelpers::AppendInt64(OutMessage, "t", Time);
	DataHelpers::AppendString(OutMessage, "level", Level);
	DataHelpers::AppendInt64(OutMessage, "end", EndTime);
}

bool UROSAuthMessage::FromData(const ROSData& Message)
{
	return false; //Will never happen. Sending Authentications only.
}

FString UROSAuthMessage::EncodeSHA512(const FString Input)
{
	unsigned char Hash[EVP_MAX_MD_SIZE];
	unsigned HashLength = 0;
	const std::string Message = TCHAR_TO_UTF8(*Input);

	EVP_MD_CTX* Context = EVP_MD_CTX_create();
	EVP_DigestInit_ex(Context,  EVP_sha512(), nullptr);
	EVP_DigestUpdate(Context, Message.c_str(), Message.length());
	EVP_DigestFinal_ex(Context, Hash, &HashLength);
	EVP_MD_CTX_destroy(Context);

	FString Result = "";
	for(uint8 i = 0; i < HashLength; i++)
	{
		Result += FString::Printf(TEXT("%02x"), Hash[i]);
	}
	
	return Result;
}
