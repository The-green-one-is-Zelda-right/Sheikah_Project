#pragma once
#include "../../FloaterMath/include/Vector2f.h"
#include "../../FloaterMath/include/Vector3f.h"
#include "../../FloaterMath/include/Vector4f.h"
#include <vector>
#include <string>

namespace flt
{
	struct RawVertex
	{
		RawVertex() = default;
		RawVertex(const Vector3f& pos, const Vector2f uvs[10], const Vector3f& normal)
			: pos(pos), uvs(), normal(normal), binormal(), tangent(), boneIndice(), boneWeights()
		{
			for (int i = 0; i < 10; ++i)
			{
				this->uvs[i] = uvs[i];
			}
		}

		std::partial_ordering operator<=>(const RawVertex& other) const
		{
			std::partial_ordering result = pos <=> other.pos;
			if (result == std::partial_ordering::less || result == std::partial_ordering::greater)
			{
				return result;
			}

			for (int i = 0; i < 10; ++i)
			{
				result = uvs[i] <=> other.uvs[i];
				if (result == std::partial_ordering::less || result == std::partial_ordering::greater)
				{
					return result;
				}
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
		Vector2f uvs[10];
		Vector3f normal;
		Vector3f binormal;
		Vector3f tangent;
		std::vector<int> boneIndice;
		std::vector<float> boneWeights;
	};

	struct RawMesh
	{
		std::vector<RawVertex> vertices;
		std::vector<int> indices;

		//std::wstring albedo;
		//std::wstring normal;
		//std::wstring metallic;
		//std::wstring roughness;
		//std::wstring specular;
		//std::wstring height;
		//std::wstring opacity;
		//std::wstring emissive;
		//std::wstring ao;
	};
}
