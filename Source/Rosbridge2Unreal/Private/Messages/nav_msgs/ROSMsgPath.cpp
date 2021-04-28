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
	DataHelpers::AppendSubMessage(OutMessage, "header", Header);

	DataHelpers::AppendTArray<UROSMsgPoseStamped*>(OutMessage, "poses", Poses, [](ROSData& Result, const char* Key, const UROSMsgPoseStamped* TArrayElement)
	{
		DataHelpers::AppendSubMessage(Result, Key, TArrayElement);
	});
}

bool UROSMsgPath::FromData(const ROSData& Message)
{
	return DataHelpers::ExtractSubMessage(Message, "header", Header)
	&& DataHelpers::ExtractTArrayOfUObjects<UROSMsgPoseStamped>(Message, "poses", Poses, this, [](const ROSData& Array, const char* Key, UROSMsgPoseStamped*& Result)
	{
		return DataHelpers::ExtractSubMessage(Array, Key, Result);
	});
}
