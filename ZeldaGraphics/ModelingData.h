#pragma once

#include <vector>
#include <string>

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

	// �߰��� �� �ִ� �͵�
	// diffuseColor ����
	// specularReflection �ݻ絵
	// Glossiness ����
	// Transparency ����
	// Normal Maps �븻��
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
	ZNode* parent;
	std::vector<ZNode*> children;

	std::vector<ZMesh*> meshes;
};