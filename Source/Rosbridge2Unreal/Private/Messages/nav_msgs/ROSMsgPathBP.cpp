#include "Messages/nav_msgs/ROSMsgPathBP.h"
#include "DataHelpers.h"

UROSMsgPathBP* UROSMsgPathBP::CreateEmpty()
{
	UROSMsgPathBP* Message = NewObject<UROSMsgPathBP>();
	Message->Header = NewObject<UROSMsgHeader>(Message);
	Message->Poses = TArray<UROSMsgPoseStampedBP*>();
	return Message;
}

void UROSMsgPathBP::AddPoseInUnrealCoordinateFrame(const FTransform& Transform, UROSMsgHeader* HeaderIn)
{
	const FQuat Rotation = Transform.GetRotation();
	const FVector Position = Transform.GetTranslation();

	UROSMsgPoseBP* Pose = UROSMsgPoseBP::CreateFromPositionOrientation(
		FVector(Position.Y, Position.X, Position.Z) / FVector(100, 100, 100),
		FQuat(Rotation.Y, -Rotation.X, Rotation.Z, -Rotation.W)
	);
	
	Poses.Add(UROSMsgPoseStampedBP::Create(HeaderIn, Pose));
}

void UROSMsgPathBP::ToData(ROSData& Message) const
{
	ROSData SubElementHeader;
	Header->ToData(SubElementHeader);
	DataHelpers::AppendSubDocument(Message,  "header", SubElementHeader);

	DataHelpers::AppendTArray<UROSMsgPoseStampedBP*>(Message, "poses", Poses, [](ROSData& Result, const char* Key, const UROSMsgPoseStampedBP* TArrayElement)
	{
		ROSData SubElement;
		TArrayElement->ToData(SubElement);
		DataHelpers::AppendSubDocument(Result,  Key, SubElement);
	});
}

bool UROSMsgPathBP::FromData(const ROSData& Message)
{
	if(!Header) Header = NewObject<UROSMsgHeader>(this);
	Poses = TArray<UROSMsgPoseStampedBP*>();
	
	ROSData SubElementHeader;
	return DataHelpers::ExtractSubDocument(Message, "header", SubElementHeader) && Header->FromData(SubElementHeader)
	&& DataHelpers::ExtractTArrayOfUObjects<UROSMsgPoseStampedBP>(Message, "poses", Poses, this, [](const ROSData& Array, const char* Key, UROSMsgPoseStampedBP*& Result)
	{
		ROSData SubElement;
		return DataHelpers::ExtractSubDocument(Array, Key, SubElement) && Result->FromData(SubElement);
	});
}
