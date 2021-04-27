#include "Messages/geometry_msgs/ROSMsgTransform.h"
#include "DataHelpers.h"

UROSMsgTransform* UROSMsgTransform::CreateFromTransform(const FTransform& Transform)
{
	return CreateFromTranslationRotation(Transform.GetTranslation(), Transform.GetRotation());
}

UROSMsgTransform* UROSMsgTransform::CreateFromTranslationRotation(const FVector& Translation, const FQuat& Rotation)
{
	UROSMsgTransform* Message = NewObject<UROSMsgTransform>();
	Message->SetTranslationFromFVector(Translation);
	Message->SetRotationFromQuad(Rotation);
	return Message;
}

UROSMsgTransform* UROSMsgTransform::Create(double Tx, double Ty, double Tz, double Rx, double Ry, double Rz, double Rw)
{
	UROSMsgTransform* Message = NewObject<UROSMsgTransform>();
	Message->Tx = Tx;
	Message->Ty = Ty;
	Message->Tz = Tz;
	Message->Rx = Rx;
	Message->Ry = Ry;
	Message->Rz = Rz;
	Message->Rw = Rw;
	return Message;
}

FVector UROSMsgTransform::TranslationAsFVector() const
{
	return FVector(Tx,Ty,Tz);
}

FQuat UROSMsgTransform::RotationAsQuad() const
{
	return FQuat(Rx,Ry,Rz,Rw);
}

FTransform UROSMsgTransform::AsTransform() const
{
	return FTransform(RotationAsQuad(),TranslationAsFVector());
}

void UROSMsgTransform::SetTranslationFromFVector(const FVector InVector)
{
	Tx = InVector.X;
	Ty = InVector.Y;
	Tz = InVector.Z;
}

void UROSMsgTransform::SetRotationFromQuad(const FQuat InQuat)
{
	Ry = InQuat.Y;
	Rz = InQuat.Z;
	Rw = InQuat.W;
	Rx = InQuat.X;
}

void UROSMsgTransform::SetFromTransform(const FTransform InTransform)
{
	SetRotationFromQuad(InTransform.GetRotation());
	SetTranslationFromFVector(InTransform.GetLocation());
}

void UROSMsgTransform::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendSubDocument(OutMessage, "translation", ROSData());
	DataHelpers::AppendDouble(OutMessage, "/translation/x", Tx);
	DataHelpers::AppendDouble(OutMessage, "/translation/y", Ty);
	DataHelpers::AppendDouble(OutMessage, "/translation/z", Tz);
	DataHelpers::AppendSubDocument(OutMessage, "rotation", ROSData());
	DataHelpers::AppendDouble(OutMessage, "/rotation/x", Rx);
	DataHelpers::AppendDouble(OutMessage, "/rotation/y", Ry);
	DataHelpers::AppendDouble(OutMessage, "/rotation/z", Rz);
	DataHelpers::AppendDouble(OutMessage, "/rotation/w", Rw);
}

bool UROSMsgTransform::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractDouble(Message, "/translation/x", Tx)
	&& DataHelpers::ExtractDouble(Message, "/translation/y", Ty)
	&& DataHelpers::ExtractDouble(Message, "/translation/z", Tz)
	&& DataHelpers::ExtractDouble(Message, "/rotation/x", Rx)
	&& DataHelpers::ExtractDouble(Message, "/rotation/y", Ry)
	&& DataHelpers::ExtractDouble(Message, "/rotation/z", Rz)
	&& DataHelpers::ExtractDouble(Message, "/rotation/w", Rw);
}