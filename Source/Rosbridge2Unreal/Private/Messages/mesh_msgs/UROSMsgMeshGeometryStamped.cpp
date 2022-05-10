#include "Messages/mesh_msgs/ROSMsgMeshGeometryStamped.h"
#include "DataHelpers.h"

void UROSMsgMeshGeometryStamped::ToData(ROSData& OutMessage) const {

}

bool UROSMsgMeshGeometryStamped::FromData(const ROSData& Message) {
	return
		DataHelpers::Extract(Message, "header", Header) &&
		DataHelpers::Extract(Message, "uuid", UUID) &&
		DataHelpers::Extract(Message, "mesh_geometry", MeshGeometry);
}
