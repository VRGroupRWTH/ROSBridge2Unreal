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

void UROSMsgTransform::SetTranslationFromFVector(const FVector& InVector)
{
	Tx = InVector.X;
	Ty = InVector.Y;
	Tz = InVector.Z;
}

void UROSMsgTransform::SetRotationFromQuad(const FQuat& InQuat)
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
	DataHelpers::Append<ROSData>(OutMessage, "translation", ROSData());
	DataHelpers::Append<double>(OutMessage, "/translation/x", Tx);
	DataHelpers::Append<double>(OutMessage, "/translation/y", Ty);
	DataHelpers::Append<double>(OutMessage, "/translation/z", Tz);
	DataHelpers::Append<ROSData>(OutMessage, "rotation", ROSData());
	DataHelpers::Append<double>(OutMessage, "/rotation/x", Rx);
	DataHelpers::Append<double>(OutMessage, "/rotation/y", Ry);
	DataHelpers::Append<double>(OutMessage, "/rotation/z", Rz);
	DataHelpers::Append<double>(OutMessage, "/rotation/w", Rw);
}

bool UROSMsgTransform::FromData(const ROSData& Message)
{
	return DataHelpers::Extract<double>(Message, "/translation/x", Tx)
	&& DataHelpers::Extract<double>(Message, "/translation/y", Ty)
	&& DataHelpers::Extract<double>(Message, "/translation/z", Tz)
	&& DataHelpers::Extract<double>(Message, "/rotation/x", Rx)
	&& DataHelpers::Extract<double>(Message, "/rotation/y", Ry)
	&& DataHelpers::Extract<double>(Message, "/rotation/z", Rz)
	&& DataHelpers::Extract<double>(Message, "/rotation/w", Rw);
}