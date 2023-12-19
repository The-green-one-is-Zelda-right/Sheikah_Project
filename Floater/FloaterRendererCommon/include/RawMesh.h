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
			: pos(pos), tex(tex), normal(normal) {}

		std::partial_ordering operator<=>(const RawVertex& other) const
		{
			std::partial_ordering result = pos <=> other.pos;
			if (result == std::partial_ordering::less || result == std::partial_ordering::greater)
			{
				return result;
			}

			result = tex <=> other.tex;
			if (result == std::partial_ordering::less || result == std::partial_ordering::greater)
			{
				return result;
			}
			return normal <=> other.normal;
			return result;
		}
		bool operator==(const RawVertex& other) const
		{
			if (*this <=> other == std::partial_ordering::equivalent)
			{
				return true;
			}
			return false;
		}

		Vector3f pos;
		Vector2f tex;
		Vector3f normal;
		Vector3f binormal;
		Vector3f tangent;
	};

	struct RawMesh
	{
		std::vector<RawVertex> vertices;
		std::vector<int> indices;
	};
}
