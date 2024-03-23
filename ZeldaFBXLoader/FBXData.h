#pragma once

#include <string>
#include <vector>
#include <map>

#include <Eigen\Dense>

namespace FBXLoader
{
	struct Vertex;
	struct Mesh;
	struct Material;
	struct Bone;
	struct Model;
	struct Animation;

	struct Float2
	{
		union
		{
			struct
			{
				float x, y;
			};

			struct
			{
				float u, v;
			};
		};
	};

	struct Float3
	{
		union
		{
			struct
			{
				float x, y, z;
			};
		};
	};

	struct Float4
	{
		union
		{
			struct
			{
				float x, y, z, w;
			};

			struct
			{
				float r, g, b, a;
			};
		};
	};

	using Color = Float4;
	using Quaternion = Float4;

	struct Vertex
	{
		Float3 position;
		Float3 normal;
		Float3 tangent;
		Float3 biTangent;
		Float2 textureCoordinate;
		unsigned int boneIndices[4];
		float weight[4];
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		unsigned int materialIndex;
	};

	struct Material
	{
		bool useDiffuseMap;
		bool useNormalMap;

		Color baseColor;
		std::wstring diffuseMap;
		std::wstring normalMap;
	};

	struct Bone
	{
		Bone* parent;
		std::vector<Bone*> children;

		std::wstring name;

		std::vector<unsigned int> meshes;

		Eigen::Matrix4f transformMatrix;
		Eigen::Matrix4f offsetMatrix;
	};

	struct Model
	{
		Bone* root;

		std::vector<Bone*> boneList;
		std::vector<Mesh*> meshList;
		std::vector<Material*> materialList;

		std::vector<Animation*> animationList;
	};

	struct AnimationKeyInfo
	{
		Float3 scaleKey;
		Quaternion rotationKey;
		Float3 positionKey;
	};

	struct Animation
	{
		std::wstring name;

		double duration; // 틱당 시간
		double tickPerSecond; // 시간당 틱

		// key: boneIndex
		// value: map<time, Matrix>
		// 값을 채우는 과정에서
		// 전부 identity로 초기화한 후에 time값을 index로 대입한다
		// 읽어본 샘플에서는 발견하지 못했지만 건너뛰는 index값이 있다면
		// identity가 아닌 그 이전의 값으로 바꿔야할지도 모른다.
		std::map<std::wstring, std::map<double , AnimationKeyInfo>> animationKey;
	};
}