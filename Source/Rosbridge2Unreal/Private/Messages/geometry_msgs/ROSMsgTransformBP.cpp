#include "Messages/geometry_msgs/ROSMsgTransformBP.h"
#include "DataHelpers.h"

UROSMsgTransformBP* UROSMsgTransformBP::CreateFromTransform(const FTransform& Transform)
{
	return CreateFromTranslationRotation(Transform.GetTranslation(), Transform.GetRotation());
}

UROSMsgTransformBP* UROSMsgTransformBP::CreateFromTranslationRotation(const FVector& Translation, const FQuat& Rotation)
{
	UROSMsgTransformBP* Message = NewObject<UROSMsgTransformBP>();
	Message->Translation = Translation;
	Message->Rotation = Rotation;
	return Message;
}

UROSMsgTransformBP* UROSMsgTransformBP::Create(const float& Tx, const float& Ty, const float& Tz, const float& Rx, const float& Ry, const float& Rz, const float& Rw)
{
	UROSMsgTransformBP* Message = NewObject<UROSMsgTransformBP>();
	Message->Translation.X = Tx;
	Message->Translation.Y = Ty;
	Message->Translation.Z = Tz;
	Message->Rotation.X = Rx;
	Message->Rotation.Y = Ry;
	Message->Rotation.Z = Rz;
	Message->Rotation.W = Rw;
	return Message;
}

FTransform UROSMsgTransformBP::AsTransform() const
{
	return FTransform(Rotation,Translation);
}

void UROSMsgTransformBP::ToData(ROSData& Message) const
{
	DataHelpers::AppendSubDocument(Message, "translation", ROSData());
	DataHelpers::AppendFloat(Message, "/translation/x", Translation.X);
	DataHelpers::AppendFloat(Message, "/translation/y", Translation.Y);
	DataHelpers::AppendFloat(Message, "/translation/z", Translation.Z);
	DataHelpers::AppendSubDocument(Message, "rotation", ROSData());
	DataHelpers::AppendFloat(Message, "/rotation/x", Rotation.X);
	DataHelpers::AppendFloat(Message, "/rotation/y", Rotation.Y);
	DataHelpers::AppendFloat(Message, "/rotation/z", Rotation.Z);
	DataHelpers::AppendFloat(Message, "/rotation/w", Rotation.W);
}

bool UROSMsgTransformBP::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractFloat(Message, "/translation/x", Translation.X)
	&& DataHelpers::ExtractFloat(Message, "/translation/y", Translation.Y)
	&& DataHelpers::ExtractFloat(Message, "/translation/z", Translation.Z)
	&& DataHelpers::ExtractFloat(Message, "/rotation/x", Rotation.X)
	&& DataHelpers::ExtractFloat(Message, "/rotation/y", Rotation.Y)
	&& DataHelpers::ExtractFloat(Message, "/rotation/z", Rotation.Z)
	&& DataHelpers::ExtractFloat(Message, "/rotation/w", Rotation.W);
}