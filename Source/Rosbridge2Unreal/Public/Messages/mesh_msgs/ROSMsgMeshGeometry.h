#pragma once

#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/geometry_msgs/ROSMsgPoint.h"
#include "ProceduralMeshComponent.h"
#include "Messages/mesh_msgs/ROSMsgMeshTriangleIndices.h"
#include "ROSMsgMeshGeometry.generated.h"

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgMeshGeometry : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgMeshGeometry() = default;

	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override { return "mesh_msgs/MeshGeometry"; }

	UFUNCTION(BlueprintCallable) void CreateProceduralMeshSection(UProceduralMeshComponent* ProceduralMeshComponent);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UROSMsgPoint*> Vertices;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UROSMsgPoint*> VertexNormals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UROSMsgMeshTriangleIndices*> Faces;

	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
};
