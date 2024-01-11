﻿#include "AssimpLoader.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "../FloaterUtil/include/ConvString.h"
#include "../FloaterUtil/include/FloaterMacro.h"

#include "./include/RawScene.h"
#include "./include/RawMesh.h"
#include "./include/RawMaterial.h"
#include "AssimpRawMeshBuilder.h"

#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "../External/lib/x64/debug/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "../External/lib/x64/release/assimp-vc143-mt.lib")
#endif

void flt::AssimpLoader::Load(const std::wstring& filePath, RawScene* outRawScene)
{
	ASSERT(outRawScene, "outRawScene is nullptr");

	Assimp::Importer importer;

	const unsigned int flags = aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
		aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
		aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes |
		aiProcess_SortByPType | aiProcess_LimitBoneWeights;

	//const unsigned int flags = aiProcess_Triangulate |
	//	aiProcess_ConvertToLeftHanded |	aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
	//	aiProcess_LimitBoneWeights;

	std::string path = ConvertToString(filePath);
	const aiScene* scene = importer.ReadFile(path, flags);

	// 데이터 로드 전 벡터 등 크기 조절
	// 노드와 메쉬 등의 데이터가 떨어져있어서 두번 다시 돌면서 로드하지 않기 위해
	// 미리 데이터들을 넣을 공간만 잡아놓기.
	// 머테리얼
	std::vector<RawMaterial*>& rawMaterials = outRawScene->materials;
	unsigned int materialCount = scene->mNumMaterials;
	rawMaterials.resize(materialCount);
	for (unsigned int i = 0; i < materialCount; ++i)
	{
		rawMaterials[i] = new RawMaterial();
	}

	// 메쉬
	unsigned int meshCount = scene->mNumMeshes;
	std::vector<Resource<RawMesh>>& rawMeshes = outRawScene->meshes;
	rawMeshes.resize(meshCount);
	//for (unsigned int i = 0; i < meshCount; ++i)
	//{
	//	rawMeshes[i] = new RawMesh();
	//}

	// 먼저 머티리얼 로드
	if (scene->HasMaterials())
	{
		for (unsigned int i = 0; i < materialCount; ++i)
		{
			aiMaterial* material = scene->mMaterials[i];

			aiString outStr;

			auto ret = material->Get(AI_MATKEY_NAME, outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->name = ConvertToWstring(outStr.C_Str());
			}

			aiColor3D diffuse;
			ret = material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->baseColor[0] = diffuse.r;
				rawMaterials[i]->baseColor[1] = diffuse.g;
				rawMaterials[i]->baseColor[2] = diffuse.b;
			}

			aiColor3D ambient;
			ret = material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->ambient[0] = ambient.r;
				rawMaterials[i]->ambient[1] = ambient.g;
				rawMaterials[i]->ambient[2] = ambient.b;
			}

			aiColor3D specular;
			ret = material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->specular[0] = specular.r;
				rawMaterials[i]->specular[1] = specular.g;
				rawMaterials[i]->specular[2] = specular.b;
			}

			float shininess;
			ret = material->Get(AI_MATKEY_SHININESS, shininess);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->roughness = shininess;
			}

			float opacity;
			ret = material->Get(AI_MATKEY_OPACITY, opacity);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->opacity = opacity;
			}

			ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->textures[RawMaterial::ALBEDO]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_NORMALS, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->textures[RawMaterial::NORMAL]->path = ConvertToWstring(outStr.C_Str());

			}

			//material->GetTexture(aiTextureType_SPECULAR, 0, &outStr);
			//rawMaterials[i].specularMap->path = ConvertToWstring(outStr.C_Str());

			ret = material->GetTexture(aiTextureType_EMISSIVE, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->textures[RawMaterial::EMISSIVE]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_AMBIENT, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->textures[RawMaterial::AO]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_HEIGHT, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->textures[RawMaterial::HEIGHT]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_OPACITY, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->textures[RawMaterial::OPACITY]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_SHININESS, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->textures[RawMaterial::ROUGHNESS]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_UNKNOWN, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i]->textures[RawMaterial::UNKNOWN]->path = ConvertToWstring(outStr.C_Str());
			}

			/*ASSERT(!(material->GetTextureCount(aiTextureType_DIFFUSE)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_SPECULAR)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_AMBIENT)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_EMISSIVE)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_HEIGHT)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_NORMALS)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_SHININESS)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_OPACITY)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_DISPLACEMENT)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_LIGHTMAP)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_REFLECTION)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_BASE_COLOR)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_NORMAL_CAMERA)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_EMISSION_COLOR)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_METALNESS)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_SHEEN)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_CLEARCOAT)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_TRANSMISSION)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_UNKNOWN)), "has Texture");*/

			ret = material->GetTexture(aiTextureType_BASE_COLOR, 0, &outStr);
			ret = material->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &outStr);
			ret = material->GetTexture(aiTextureType_EMISSION_COLOR, 0, &outStr);
			ret = material->GetTexture(aiTextureType_METALNESS, 0, &outStr);
			ret = material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &outStr);
			ret = material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &outStr);
		}
	}

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		auto errorStr = importer.GetErrorString();
		ASSERT(false, errorStr);

		return;
	}

	// 노드 트리 구조 구축 및 노드 이름 맵핑
	int nodeCount = scene->mRootNode->mNumChildren;
	outRawScene->nodes.reserve(nodeCount);
	for (int i = 0; i < nodeCount; ++i)
	{
		outRawScene->nodes.push_back(new RawNode());
		GetNodeRecursive(scene->mRootNode->mChildren[i], outRawScene->nodes.back(), outRawScene);
	}

	// 메쉬 로드
	if (scene->HasMeshes())
	{
		for (unsigned int i = 0; i < meshCount; ++i)
		{
			AssimpRawMeshBuilder meshBuilder(scene->mMeshes[i], _nodeMap);

			rawMeshes[i].Set(meshBuilder);
		}
	}
}

void flt::AssimpLoader::GetNodeRecursive(aiNode* pNode, RawNode* pRawNode, RawScene* pRawScene)
{
	ASSERT(pNode, "pNode is nullptr");
	ASSERT(pRawNode, "pRawNode is nullptr");

	pRawNode->name = ConvertToWstring(pNode->mName.C_Str());
	_nodeMap.insert({ pRawNode->name , pRawNode });

	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	pNode->mTransformation.Decompose(scale, rotation, position);

	pRawNode->transform.SetPosition(position.x, position.y, position.z);
	pRawNode->transform.SetRotation(rotation.x, rotation.y, rotation.z, rotation.w);
	pRawNode->transform.SetScale(scale.x, scale.y, scale.z);

	auto test = pNode->mTransformation;
	auto test2 = pRawNode->transform.GetLocalMatrix4f();

	{
		float epsilon = 0.0001f;
		float sub = test.a1 - test2.e[0][0];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.a2 - test2.e[1][0];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.a3 - test2.e[2][0];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.a4 - test2.e[3][0];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.b1 - test2.e[0][1];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.b2 - test2.e[1][1];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.b3 - test2.e[2][1];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.b4 - test2.e[3][1];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.c1 - test2.e[0][2];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.c2 - test2.e[1][2];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.c3 - test2.e[2][2];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.c4 - test2.e[3][2];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.d1 - test2.e[0][3];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.d2 - test2.e[1][3];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.d3 - test2.e[2][3];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.d4 - test2.e[3][3];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
	}

	int meshCount = pNode->mNumMeshes;
	//ASSERT(meshCount == 1 || meshCount == 0, "meshCount more then 1");

	pRawNode->meshes.reserve(meshCount);
	for (int i = 0; i < meshCount; ++i)
	{
		pRawNode->meshes.push_back(pRawScene->meshes[pNode->mMeshes[i]]);
	}
	
	int childCount = pNode->mNumChildren;
	pRawNode->children.reserve(childCount);

	for (int i = 0; i < childCount; ++i)
	{
		RawNode* childNode = new RawNode();
		pRawNode->children.push_back(childNode);
		childNode->parent = pRawNode;

		childNode->transform.SetParent(&pRawNode->transform);

		GetNodeRecursive(pNode->mChildren[i], childNode, pRawScene);
	}
}

void flt::AssimpLoader::PrintNodeNameRecursive(aiNode* pNode, int depth /*= 0*/)
{
	for (int i = 0; i < depth; ++i)
	{
		std::wcout << L"| ";
	}
	//std::wcout << L" ";
	std::wstring testName = ConvertToWstring(pNode->mName.C_Str());
	std::wcout << testName << std::endl;

	if (pNode->mNumChildren == 0)
	{
		return;
	}

	//for (int i = 0; i < depth; ++i)
	//{
	//	std::wcout << L" |";
	//}
	//std::wcout << L" ( " << std::endl;

	for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
	{
		PrintNodeNameRecursive(pNode->mChildren[i], depth + 1);
	}

	//for (int i = 0; i < depth; ++i)
	//{
	//	std::wcout << L" |";
	//}
	//std::wcout << L" ) " << std::endl;
}
