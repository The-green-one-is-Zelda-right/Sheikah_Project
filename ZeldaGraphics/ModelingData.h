#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>
#include <string>

class ZeldaMesh;
class ZeldaTexture;

struct ZVertex
{
	float posX, posY, posZ;
	float u, v;
	float norX, norY, norZ;
};

struct ZMaterial
{
	ZMaterial();

	bool hasTexture;
	std::wstring textureFilePath;

	// 추가될 수 있는 것들
	// diffuseColor 색상
	// specularReflection 반사도
	// Glossiness 광택
	// Transparency 투명도
	// Normal Maps 노말맵
};

struct ZMesh
{
	ZMesh();
	bool hasPosition;
	bool hasUV;
	bool hasNormal;
	std::vector<ZVertex> vertices;
	std::vector<unsigned int> indices;

	ZMaterial* material;
};

struct ZNode
{
	ZNode();
	~ZNode();

	DirectX::XMMATRIX GetWorldTransformMatrix();

	DirectX::XMMATRIX transformMatrix;

	ZNode* parent;
	std::vector<ZNode*> children;

	std::vector<ZMesh*> meshes;
	std::vector<ZeldaMesh*> zeldaMeshes;
	std::vector<ZeldaTexture*> zeldaTextures;
};