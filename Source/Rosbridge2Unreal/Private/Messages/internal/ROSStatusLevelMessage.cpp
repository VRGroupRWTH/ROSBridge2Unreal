#include "Messages/internal/ROSStatusLevelMessage.h"
#include "DataHelpers.h"

void UROSStatusLevelMessage::ToData(ROSData& Message) const
{
	DataHelpers::AppendString(Message, "op", "set_level");
	DataHelpers::AppendString(Message, "id", ID);

	switch (Level) {
	case EStatusMessageLevel::Error:
		DataHelpers::AppendString(Message, "level", "error");
		break;
	case EStatusMessageLevel::Warning:
		DataHelpers::AppendString(Message, "level", "warning");
		break;
	case EStatusMessageLevel::Info:
		DataHelpers::AppendString(Message, "level", "info");
		break;
	}
	
}

bool UROSStatusLevelMessage::FromData(const ROSData& Message)
{
	DataHelpers::ExtractString(Message, "id", ID); //optional
	
	FString InLevel;
	DataHelpers::ExtractString(Message, "level", InLevel);
	if(InLevel.Equals("error", ESearchCase::IgnoreCase))
	{
		Level = EStatusMessageLevel::Error;
		return true;
	}
	if(InLevel.Equals("warning", ESearchCase::IgnoreCase))
	{
		Level = EStatusMessageLevel::Warning;
		return true;
	}
	if(InLevel.Equals("info", ESearchCase::IgnoreCase))
	{
		Level = EStatusMessageLevel::Info;
		return true;
	}
	
	return false;
}