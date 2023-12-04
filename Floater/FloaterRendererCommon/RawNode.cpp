#include "./include/RawNode.h"

flt::RawNode::RawNode(const std::wstring& name) :
	name(name),
	transform(),
	mesh(nullptr),
	skin(nullptr),
	animation(nullptr),
	camera(nullptr),
	boneIndex(-1),
	children()
{

}

flt::RawNode::~RawNode()
{
	if (mesh != nullptr)
		delete mesh;
	if (skin != nullptr)
		delete skin;
	if (animation != nullptr)
		delete animation;
	if (camera != nullptr)
		delete camera;

	for (auto& child : children)
	{
		delete child.second;
	}
}

