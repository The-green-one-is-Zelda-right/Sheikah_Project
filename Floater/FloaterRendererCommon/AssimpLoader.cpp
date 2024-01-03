#include "AssimpLoader.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "../FloaterUtil/include/ConvString.h"
#include "../FloaterUtil/include/FloaterMacro.h"

#include "./include/RawMesh.h"
#include "./include/RawMaterial.h"

#ifdef _DEBUG
#pragma comment(lib, "../External/lib/x64/debug/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "../External/lib/x64/release/assimp-vc143-mt.lib")
#endif

void flt::AssimpLoader::Load(const std::wstring& filePath)
{
	Assimp::Importer importer;

	const unsigned int flags = aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
		aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
		aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes |
		aiProcess_SortByPType | aiProcess_LimitBoneWeights;

	std::string path = ConvertToString(filePath);

	const aiScene* scene = importer.ReadFile(path, flags);

	std::vector<RawMaterial> rawMaterials;

	// 먼저 머티리얼 로드
	if (scene->HasMaterials())
	{
		unsigned int materialCount = scene->mNumMaterials;
		rawMaterials.resize(materialCount);

		for (unsigned int i = 0; i < materialCount; ++i)
		{
			aiMaterial* material = scene->mMaterials[i];

			aiString outStr;

			auto ret = material->Get(AI_MATKEY_NAME, outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].name = ConvertToWstring(outStr.C_Str());
			}

			aiColor3D diffuse;
			ret = material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].baseColor[0] = diffuse.r;
				rawMaterials[i].baseColor[1] = diffuse.g;
				rawMaterials[i].baseColor[2] = diffuse.b;
			}


			aiColor3D ambient;
			ret = material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].ambient[0] = ambient.r;
				rawMaterials[i].ambient[1] = ambient.g;
				rawMaterials[i].ambient[2] = ambient.b;
			}

			aiColor3D specular;
			ret = material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].specular[0] = specular.r;
				rawMaterials[i].specular[1] = specular.g;
				rawMaterials[i].specular[2] = specular.b;
			}

			float shininess;
			ret = material->Get(AI_MATKEY_SHININESS, shininess);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].roughness = shininess;
			}


			float opacity;
			ret = material->Get(AI_MATKEY_OPACITY, opacity);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].opacity = opacity;
			}

			ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].textures[RawMaterial::ALBEDO]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_NORMALS, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].textures[RawMaterial::NORMAL]->path = ConvertToWstring(outStr.C_Str());

			}

			//material->GetTexture(aiTextureType_SPECULAR, 0, &outStr);
			//rawMaterials[i].specularMap->path = ConvertToWstring(outStr.C_Str());

			ret = material->GetTexture(aiTextureType_EMISSIVE, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].textures[RawMaterial::EMISSIVE]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_AMBIENT, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].textures[RawMaterial::AO]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_HEIGHT, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].textures[RawMaterial::HEIGHT]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_OPACITY, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].textures[RawMaterial::OPACITY]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_SHININESS, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].textures[RawMaterial::ROUGHNESS]->path = ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_UNKNOWN, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				rawMaterials[i].textures[RawMaterial::UNKNOWN]->path = ConvertToWstring(outStr.C_Str());
			}

			//// PBR용
			//bool isPBR = false;
			//material->Get(AI_MATKEY_USE_COLOR_MAP, isPBR);

			//if (isPBR)
			//{
			//	aiColor3D albedo;
			//	material->Get(AI_MATKEY_BASE_COLOR, albedo);

			//	float metalic;
			//	material->Get(AI_MATKEY_METALLIC_FACTOR, metalic);

			//	float roughness;
			//	material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);

			//	material->GetTexture(aiTextureType_BASE_COLOR, 0, &outStr);
			//	std::wstring albedoTexturePath = ConvertToWstring(outStr.C_Str());

			//	material->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &outStr);
			//	std::wstring normalCameraTexturePath = ConvertToWstring(outStr.C_Str());

			//	material->GetTexture(aiTextureType_EMISSION_COLOR, 0, &outStr);
			//	std::wstring emissionColorTexturePath = ConvertToWstring(outStr.C_Str());

			//	material->GetTexture(aiTextureType_METALNESS, 0, &outStr);
			//	std::wstring metalnessTexturePath = ConvertToWstring(outStr.C_Str());

			//	material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &outStr);
			//	std::wstring diffuseRoughnessTexturePath = ConvertToWstring(outStr.C_Str());

			//	material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &outStr);
			//	std::wstring ambientOcclusionTexturePath = ConvertToWstring(outStr.C_Str());
			//}
		}
	}

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		auto errorStr = importer.GetErrorString();
		ASSERT(false, errorStr);

		return;
	}

	if (scene->HasMeshes())
	{
		unsigned int meshCount = scene->mNumMeshes;
		std::vector<RawMesh> rawMeshes(meshCount);

		for (unsigned int i = 0; i < meshCount; ++i)
		{
			aiMesh* mesh = scene->mMeshes[i];
			int vertexCount = mesh->mNumVertices;
			rawMeshes[i].vertices.resize(vertexCount);

			if (mesh->HasPositions())
			{
				for (int j = 0; j < vertexCount; ++j)
				{
					rawMeshes[i].vertices[j].pos.x = mesh->mVertices[j].x;
					rawMeshes[i].vertices[j].pos.y = mesh->mVertices[j].y;
					rawMeshes[i].vertices[j].pos.z = mesh->mVertices[j].z;
				}

			}

			if (mesh->HasNormals())
			{
				for (int j = 0; j < vertexCount; ++j)
				{
					rawMeshes[i].vertices[j].normal.x = mesh->mNormals[j].x;
					rawMeshes[i].vertices[j].normal.y = mesh->mNormals[j].y;
					rawMeshes[i].vertices[j].normal.z = mesh->mNormals[j].z;
				}
			}

			if (mesh->HasTangentsAndBitangents())
			{
				for (int j = 0; j < vertexCount; ++j)
				{
					rawMeshes[i].vertices[j].tangent.x = mesh->mTangents[j].x;
					rawMeshes[i].vertices[j].tangent.y = mesh->mTangents[j].y;
					rawMeshes[i].vertices[j].tangent.z = mesh->mTangents[j].z;

					rawMeshes[i].vertices[j].binormal.x = mesh->mBitangents[j].x;
					rawMeshes[i].vertices[j].binormal.y = mesh->mBitangents[j].y;
					rawMeshes[i].vertices[j].binormal.z = mesh->mBitangents[j].z;
				}
			}

			for (int j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++j)
			{
				if (!mesh->HasTextureCoords(j))
				{
					continue;
				}

				int texCount = mesh->mNumUVComponents[j];
				ASSERT(texCount == 2, "텍스쳐 좌표가 2개가 아닙니다.");

				for (int k = 0; k < vertexCount; ++k)
				{
					auto test = mesh->mTextureCoords[j][k];
					rawMeshes[i].vertices[j].uvs[k].x = mesh->mTextureCoords[j][k].x;
					rawMeshes[i].vertices[j].uvs[k].y = mesh->mTextureCoords[j][k].y;
				}
			}

			/*aiString outStr;
			mesh->Get(AI_MATKEY_NAME, outStr);
			std::wstring testName = ConvertToWstring(outStr.C_Str());

			unsigned int vertexCount = mesh->mNumVertices;
			aiVector3D* vertices = mesh->mVertices;
			aiVector3D* normals = mesh->mNormals;
			aiVector3D* tangents = mesh->mTangents;
			aiVector3D* bitangents = mesh->mBitangents;
			aiVector3D* textureCoords = mesh->mTextureCoords[0];

			unsigned int faceCount = mesh->mNumFaces;
			aiFace* faces = mesh->mFaces;

			unsigned int boneCount = mesh->mNumBones;
			aiBone** bones = mesh->mBones;

			for (unsigned int j = 0; j < boneCount; ++j)
			{
				aiBone* bone = bones[j];

				aiString outStr;
				bone->Get(AI_MATKEY_NAME, outStr);
				std::wstring testName = ConvertToWstring(outStr.C_Str());

				aiMatrix4x4 offsetMatrix = bone->mOffsetMatrix;
				auto position = offsetMatrix.a4;
				aiVector3D position = bone->mOffsetMatrix.a4;
				aiQuaternion rotation = bone->mOffsetMatrix.a1;
				aiVector3D scale = bone->mOffsetMatrix.a3;

				unsigned int weightCount = bone->mNumWeights;
				aiVertexWeight* weights = bone->mWeights;
			}*/
		}
	}
}
