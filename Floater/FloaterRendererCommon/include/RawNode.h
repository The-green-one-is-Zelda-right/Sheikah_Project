#pragma once
#include "Transform.h"
#include "RawMesh.h"
#include "RawSkin.h"
#include "RawAnimation.h"
#include "Camera.h"

#include <string>
#include <vector>
//#include <map>


namespace flt
{
	struct RawNode
	{
		RawNode() : RawNode(L"") {}
		RawNode(const std::wstring& name);
		~RawNode();

		std::wstring name;
		Transform transform;
		RawMesh* mesh;
		RawSkin* skin;
		RawAnimation* animation;
		Camera* camera;

		int boneIndex;

		RawNode* parent;
		std::vector<RawNode*> children;
	};
}
