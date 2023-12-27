#include "AssimpLoader.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "../FloaterUtil/include/ConvString.h"
#include "../FloaterUtil/include/FloaterMacro.h"

#ifdef _DEBUG
#pragma comment(lib, "../External/lib/x64/debug/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "../External/lib/x64/release/assimp-vc143-mt.lib")
#endif

void flt::AssimpLoader::Load(const std::wstring& filePath)
{
	Assimp::Importer importer;

	std::string path = ConvertToString(filePath);

	const unsigned int flags = aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
		aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
		aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes |
		aiProcess_SortByPType | aiProcess_LimitBoneWeights;

	const aiScene* scene = importer.ReadFile(path, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		ASSERT(false, importer.GetErrorString());

		return;
	}

	if (scene->HasMaterials())
	{
		unsigned int materialCount = scene->mNumMaterials;

		for (unsigned int i = 0; i < materialCount; ++i)
		{
			aiMaterial* material = scene->mMaterials[i];

			aiString name;
			material->Get(AI_MATKEY_NAME, name);

			aiColor3D diffuse;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

			aiColor3D ambient;
			material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);

			aiColor3D specular;
			material->Get(AI_MATKEY_COLOR_SPECULAR, specular);

			float shininess;
			material->Get(AI_MATKEY_SHININESS, shininess);

			float opacity;
			material->Get(AI_MATKEY_OPACITY, opacity);

			aiString diffuseTexturePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexturePath);

			aiString normalTexturePath;
			material->GetTexture(aiTextureType_NORMALS, 0, &normalTexturePath);

			aiString specularTexturePath;
			material->GetTexture(aiTextureType_SPECULAR, 0, &specularTexturePath);

			aiString emissiveTexturePath;
			material->GetTexture(aiTextureType_EMISSIVE, 0, &emissiveTexturePath);

			aiString ambientTexturePath;
			material->GetTexture(aiTextureType_AMBIENT, 0, &ambientTexturePath);

			aiString heightTexturePath;
			material->GetTexture(aiTextureType_HEIGHT, 0, &heightTexturePath);

			aiString opacityTexturePath;
			material->GetTexture(aiTextureType_OPACITY, 0, &opacityTexturePath);

			aiString shininessTexturePath;
			material->GetTexture(aiTextureType_SHININESS, 0, &shininessTexturePath);

			aiString displacementTexturePath;
			material->GetTexture(aiTextureType_DISPLACEMENT, 0, &displacementTexturePath);

			aiString lightmapTexturePath;
			material->GetTexture(aiTextureType_LIGHTMAP, 0, &lightmapTexturePath);

			aiString reflectionTexturePath;
			material->GetTexture(aiTextureType_REFLECTION, 0, &reflectionTexturePath);

			aiString unknownTexturePath;
			material->GetTexture(aiTextureType_UNKNOWN, 0, &unknownTexturePath);

			// PBR용
			bool isPBR = false;
			material->Get(AI_MATKEY_USE_COLOR_MAP, isPBR);

			if (isPBR)
			{
				aiColor3D albedo;
				material->Get(AI_MATKEY_BASE_COLOR, albedo);

				float metalic;
				material->Get(AI_MATKEY_METALLIC_FACTOR, metalic);

				float roughness;
				material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);

				aiString albedoTexturePath;
				material->GetTexture(aiTextureType_BASE_COLOR, 0, &albedoTexturePath);

				aiString normalCameraTexturePath;
				material->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &normalCameraTexturePath);

				aiString emissionColorTexturePath;
				material->GetTexture(aiTextureType_EMISSION_COLOR, 0, &emissionColorTexturePath);

				aiString metalnessTexturePath;
				material->GetTexture(aiTextureType_METALNESS, 0, &metalnessTexturePath);

				aiString diffuseRoughnessTexturePath;
				material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &diffuseRoughnessTexturePath);

				aiString ambientOcclusionTexturePath;
				material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &ambientOcclusionTexturePath);
			}


		}
	}
}
