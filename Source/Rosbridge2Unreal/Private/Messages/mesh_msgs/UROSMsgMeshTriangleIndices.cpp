#include "Messages/mesh_msgs/ROSMsgMeshTriangleIndices.h"
#include "DataHelpers.h"


void UROSMsgMeshTriangleIndices::ToData(ROSData& OutMessage) const {

}

bool UROSMsgMeshTriangleIndices::FromData(const ROSData& Message) {
	return
		DataHelpers::Extract<TArray<uint32>>(Message, "vertex_indices", VertexIndices);
}