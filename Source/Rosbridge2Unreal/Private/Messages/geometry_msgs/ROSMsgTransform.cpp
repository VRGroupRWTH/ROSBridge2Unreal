// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSMsgTransform.h"
#include "DataHelpers.h"

UROSMsgTransform* UROSMsgTransform::CreateFromTransform(const FTransform& Transform)
{
	return CreateFromTranslationRotation(Transform.GetTranslation(), Transform.GetRotation());
}

UROSMsgTransform* UROSMsgTransform::CreateFromTranslationRotation(const FVector& Translation, const FQuat& Rotation)
{
	UROSMsgTransform* Message = NewObject<UROSMsgTransform>();
	Message->Tx = Translation.X;
	Message->Ty = Translation.Y;
	Message->Tz = Translation.Z;
	Message->Ry = Rotation.Y;
	Message->Rz = Rotation.Z;
	Message->Rw = Rotation.W;
	Message->Rx = Rotation.X;
	return Message;
}

UROSMsgTransform* UROSMsgTransform::Create(const double& Tx, const double& Ty, const double& Tz, const double& Rx, const double& Ry, const double& Rz, const double& Rw)
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

void UROSMsgTransform::ToData(ROSData& Message) const
{
	DataHelpers::AppendSubDocument(Message, "translation", ROSData());
	DataHelpers::AppendDouble(Message, "/translation/x", Tx);
	DataHelpers::AppendDouble(Message, "/translation/y", Ty);
	DataHelpers::AppendDouble(Message, "/translation/z", Tz);
	DataHelpers::AppendSubDocument(Message, "rotation", ROSData());
	DataHelpers::AppendDouble(Message, "/rotation/x", Rx);
	DataHelpers::AppendDouble(Message, "/rotation/y", Ry);
	DataHelpers::AppendDouble(Message, "/rotation/z", Rz);
	DataHelpers::AppendDouble(Message, "/rotation/w", Rw);
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