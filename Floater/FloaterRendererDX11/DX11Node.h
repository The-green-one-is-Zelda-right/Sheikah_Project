﻿#pragma once
#include "../FloaterRendererCommon/include/Transform.h"
#include "../FloaterRendererCommon/include/Camera.h"
#include "DX11Mesh.h"
#include "../FloaterRendererCommon/include/RawNode.h"

#include <string>
#include <vector>
#include <map>

namespace flt
{
	struct DX11Node
	{
		DX11Node(Transform& transform, const bool& isDraw);
		//DX11Node(RawNode& rawNode, const bool& isDraw);

		std::wstring name;
		Transform& transform;
		const bool& isDraw;
		std::vector<Resource<DX11Mesh>> meshes;
		Camera* camera;

		int boneIndex;

		std::multimap<std::wstring, DX11Node*> children;
	};
}
