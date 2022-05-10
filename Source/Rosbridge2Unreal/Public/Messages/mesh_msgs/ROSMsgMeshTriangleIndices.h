#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/geometry_msgs/ROSMsgPoint.h"
#include "ProceduralMeshComponent.h"
#include "ROSMsgMeshTriangleIndices.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgMeshTriangleIndices : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgMeshTriangleIndices() = default;

	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override { return "mesh_msgs/MeshTriangleIndices"; }
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<int> VertexIndicesBP;
	TArray<uint32> VertexIndices;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
