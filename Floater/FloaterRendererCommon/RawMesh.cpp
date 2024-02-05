#include "./include/RawMesh.h"

flt::RawMesh* flt::RawMeshBuilder::build() const
{
	RawMesh* pMesh = new RawMesh();
	pMesh->vertices = std::move(vertices);
	pMesh->indices = std::move(indices);
	pMesh->pRootBone = pRootBone;
	return pMesh;
}
