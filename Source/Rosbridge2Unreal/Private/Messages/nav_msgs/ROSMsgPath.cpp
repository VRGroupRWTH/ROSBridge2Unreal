#include "Messages/nav_msgs/ROSMsgPath.h"
#include "DataHelpers.h"

UROSMsgPath* UROSMsgPath::CreateEmpty()
{
	UROSMsgPath* Message = NewObject<UROSMsgPath>();
	Message->Header = NewObject<UROSMsgHeader>(Message);
	Message->Poses = TArray<UROSMsgPoseStamped*>();
	return Message;
}

void UROSMsgPath::AddPoseInUnrealCoordinateFrame(const FTransform& Transform, UROSMsgHeader* HeaderIn)
{
	const FQuat& Rotation = Transform.GetRotation();
	const FVector& Position = Transform.GetTranslation();

	UROSMsgPose* Pose = UROSMsgPose::CreateFromPositionOrientation(
		FVector(Position.Y, Position.X, Position.Z) / FVector(100, 100, 100),
		FQuat(Rotation.Y, -Rotation.X, Rotation.Z, -Rotation.W)
	);
	
	Poses.Add(UROSMsgPoseStamped::Create(HeaderIn, Pose));
}

void UROSMsgPath::ToData(ROSData& OutMessage) const
{
	ROSData SubElementHeader;
	Header->ToData(SubElementHeader);
	DataHelpers::AppendSubDocument(OutMessage,  "header", SubElementHeader);

	DataHelpers::AppendTArray<UROSMsgPoseStamped*>(OutMessage, "poses", Poses, [](ROSData& Result, const char* Key, const UROSMsgPoseStamped* TArrayElement)
	{
		ROSData SubElement;
		TArrayElement->ToData(SubElement);
		DataHelpers::AppendSubDocument(Result,  Key, SubElement);
	});
}

bool UROSMsgPath::FromData(const ROSData& Message)
{
	if(!Header) Header = NewObject<UROSMsgHeader>(this);
	Poses = TArray<UROSMsgPoseStamped*>();
	
	ROSData SubElementHeader;
	return DataHelpers::ExtractSubDocument(Message, "header", SubElementHeader) && Header->FromData(SubElementHeader)
	&& DataHelpers::ExtractTArrayOfUObjects<UROSMsgPoseStamped>(Message, "poses", Poses, this, [](const ROSData& Array, const char* Key, UROSMsgPoseStamped*& Result)
	{
		ROSData SubElement;
		return DataHelpers::ExtractSubDocument(Array, Key, SubElement) && Result->FromData(SubElement);
	});
}
