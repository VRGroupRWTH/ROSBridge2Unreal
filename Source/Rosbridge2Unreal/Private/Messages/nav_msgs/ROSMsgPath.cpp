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
	DataHelpers::Append<UROSMsgHeader*>(OutMessage, "header", Header);
	DataHelpers::Append<TArray<UROSMsgPoseStamped*>>(OutMessage, "poses", Poses);
}

bool UROSMsgPath::FromData(const ROSData& Message)
{
	return
		DataHelpers::Extract<UROSMsgHeader*>(Message, "header", Header) &&
		DataHelpers::Extract<TArray<UROSMsgPoseStamped*>>(Message, "poses", Poses);
}
