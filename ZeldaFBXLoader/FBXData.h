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

		double duration; // ƽ�� �ð�
		double tickPerSecond; // �ð��� ƽ

		// key: boneIndex
		// value: map<time, Matrix>
		// ���� ä��� ��������
		// ���� identity�� �ʱ�ȭ�� �Ŀ� time���� index�� �����Ѵ�
		// �о ���ÿ����� �߰����� �������� �ǳʶٴ� index���� �ִٸ�
		// identity�� �ƴ� �� ������ ������ �ٲ�������� �𸥴�.
		std::map<std::wstring, std::map<double , AnimationKeyInfo>> animationKey;
	};
}