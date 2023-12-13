#pragma once
#include "../../FloaterMath/include/Vector2f.h"
#include "../../FloaterMath/include/Vector3f.h"
#include "../../FloaterMath/include/Vector4f.h"
#include <vector>

namespace flt
{
	struct RawVertex
	{
		RawVertex() = default;
		RawVertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
			: Pos(pos), Tex(tex), Normal(normal) {}

		Vector3f Pos;
		Vector2f Tex;
		Vector3f Normal;
	};

	struct RawMesh
	{
		std::vector<RawVertex> vertices;
		std::vector<int> indices;
	};
}
