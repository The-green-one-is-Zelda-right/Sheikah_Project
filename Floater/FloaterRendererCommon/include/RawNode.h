#pragma once
#include "Transform.h"
#include "RawMesh.h"
#include "RawSkin.h"
#include "RawAnimation.h"
#include "Camera.h"

#include <string>
#include <map>


namespace flt
{
	struct RawNode
	{
		RawNode(const std::wstring& name);

		std::wstring name;
		Transform transform;
		RawMesh* mesh;
		RawSkin* skin;
		RawAnimation* animation;
		Camera* camera;

		int boneIndex;

		std::multimap<std::wstring, RawNode*> children;
	};
}
