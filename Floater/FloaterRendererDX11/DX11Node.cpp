#include "DX11Node.h"

flt::DX11Node::DX11Node(Transform& transform, const bool& isDraw) : 
	transform(transform),
	isDraw(isDraw),
	mesh(),
	camera(nullptr),
	boneIndex(-1),
	children()
{

}
