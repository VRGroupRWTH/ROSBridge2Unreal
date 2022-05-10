#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgHeader.h"
#include "Messages/mesh_msgs/ROSMsgMeshGeometry.h"
#include "ROSMsgMeshGeometryStamped.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgMeshGeometryStamped : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgMeshGeometryStamped() = default;

	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override { return "mesh_msgs/MeshGeometryStamped"; }
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString UUID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgMeshGeometry* MeshGeometry;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
