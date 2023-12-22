#include "ModelingData.h"

ZNode::ZNode() :
	transformMatrix(DirectX::XMMatrixIdentity()),
	parent(nullptr)
{
}

ZNode::~ZNode()
{
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

DirectX::XMMATRIX ZNode::GetWorldTransformMatrix()
{
	if (parent != nullptr)
	{
		return transformMatrix * parent->GetWorldTransformMatrix();
	}

	return transformMatrix;
}

ZMesh::ZMesh() :
	hasPosition(false),
	hasUV(false),
	hasNormal(false),
	material(nullptr)
{
}

ZMaterial::ZMaterial() :
	hasTexture(false)
{
}
