#include "./include/RawNode.h"

flt::RawNode::RawNode(const std::wstring& name) :
	name(name),
	transform(),
	meshes(),
	skin(nullptr),
	animation(nullptr),
	camera(nullptr),
	boneIndex(-1),
	parent(nullptr),
	children()
{

}

flt::RawNode::~RawNode()
{
	//delete skin;
	//delete animation;
	//delete camera;

	for (auto& child : children)
	{
		delete child;
	}
}

