#pragma once

#include "../FloaterMath/include/Vector3f.h"
#include "fbxsdk.h"
#include <Vector>

namespace flt
{
	struct VertexNormalData final
	{
	public:
		VertexNormalData(const fbxsdk::FbxMesh& mesh);

	public:
		std::vector<std::vector<Vector3f>> vertexNormal;
		std::vector<std::vector<int>> index;
	};
}
