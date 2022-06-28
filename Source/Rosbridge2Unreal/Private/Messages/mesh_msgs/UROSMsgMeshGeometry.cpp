#include "Messages/mesh_msgs/ROSMsgMeshGeometry.h"
#include "DataHelpers.h"

void UROSMsgMeshGeometry::CreateProceduralMeshSection(UProceduralMeshComponent* ProceduralMeshComponent)
{
	TArray<FVector> MeshVertices;
	MeshVertices.SetNum(Vertices.Num());
	for (int i = 0; i < Vertices.Num(); ++i) {
		// MeshVertices[i] = UROSTransform::ConvertFromROSToUnrealCoordinateSystem(FTransform{ .Translation = { Vertices[i]->X, Vertices[i]->Y, Vertices[i]->Z });
		MeshVertices[i].X = Vertices[i]->X * 100;
		MeshVertices[i].Y = -Vertices[i]->Y * 100;
		MeshVertices[i].Z = Vertices[i]->Z * 100;
	}

	TArray<int32> MeshTriangleIndices;
	MeshTriangleIndices.SetNum(Faces.Num() * 3);
	for (int i = 0; i < Faces.Num(); ++i) {
		MeshTriangleIndices[i * 3 + 0] = Faces[i]->VertexIndices[0];
		MeshTriangleIndices[i * 3 + 1] = Faces[i]->VertexIndices[1];
		MeshTriangleIndices[i * 3 + 2] = Faces[i]->VertexIndices[2];
	}

	TArray<FVector> MeshNormals;
	MeshNormals.SetNum(VertexNormals.Num());
	for (int i = 0; i < VertexNormals.Num(); ++i) {
		MeshNormals[i].X = VertexNormals[i]->X;
		MeshNormals[i].Y = VertexNormals[i]->Y;
		MeshNormals[i].Z = VertexNormals[i]->Z;
	}

	ProceduralMeshComponent->CreateMeshSection(
		0,
		MeshVertices,
		MeshTriangleIndices,
		MeshNormals,
		{},
		{},
		{},
		{},
		{},
		{},
		false
	);
}

void UROSMsgMeshGeometry::ToData(ROSData& OutMessage) const {

}

bool UROSMsgMeshGeometry::FromData(const ROSData& Message) {
	return
		DataHelpers::Extract<TArray<UROSMsgPoint*>>(Message, "vertices", Vertices) &&
		DataHelpers::Extract<TArray<UROSMsgPoint*>>(Message, "vertex_normals", VertexNormals) &&
		DataHelpers::Extract(Message, "faces", Faces);
}