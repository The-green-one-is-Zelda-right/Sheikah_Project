#include "FBXLoader.h"

#include <filesystem>

#include <fstream>
#include <queue>

#include "FBXData.h"

namespace FBXLoader
{
	const unsigned int FBXLoader::BONE_MAX = 256;

	Model* FBXLoader::CreateModelFromFBX(const std::wstring& filePath)
	{
		Model* model = nullptr;

		std::filesystem::path absolutePath = std::filesystem::absolute(filePath);
		std::wstring fileDirectory = absolutePath.parent_path().wstring();

		Assimp::Importer importer;

		std::string multibyteFilePath;
		multibyteFilePath.assign(filePath.begin(), filePath.end());

		const unsigned int DEFAULT_LOAD_FLAG// = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded | aiProcess_FlipUVs;
			//*
			= aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
			aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
			aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes |
			aiProcess_SortByPType | aiProcess_LimitBoneWeights;
		//*/

		// aiProcess_FlipWindingOrder -> aiProcess_ConvertToLeftHanded에 포함됨
		// aiProcess_PreTransformVertices

		const aiScene* scene = importer.ReadFile(multibyteFilePath, DEFAULT_LOAD_FLAG);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::string errorMessage = importer.GetErrorString();
			std::wstring wErrorMessage;
			wErrorMessage.assign(errorMessage.begin(), errorMessage.end());

			return model;
		}

		model = new Model();

		if (scene->HasMaterials())
		{
			for (int i = 0; i < scene->mNumMaterials; i++)
			{
				aiMaterial* assimpMaterial = scene->mMaterials[i];

				Material* materialData = new Material();

				CopyMaterialData(materialData, assimpMaterial, fileDirectory);

				model->materialList.push_back(materialData);
			}
		}

		model->boneList.assign(BONE_MAX, nullptr);

		std::map<std::wstring, unsigned int> boneIndexMap;
		if (scene->HasMeshes())
		{
			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				aiMesh* assimpMesh = scene->mMeshes[i];
				for (int j = 0; j < assimpMesh->mNumBones; j++)
				{
					std::string str = assimpMesh->mBones[j]->mName.C_Str();
					std::wstring boneName;
					boneName.assign(str.begin(), str.end());

					boneIndexMap[boneName] = 0;
				}
			}
		}

		int boneCount = 0;
		for (auto iter = boneIndexMap.begin(); iter != boneIndexMap.end(); iter++)
		{
			(*iter).second = boneCount;
			boneCount++;
		}

		model->root = new Bone();
		CopyNodeData(model->root, scene->mRootNode, boneIndexMap, model->boneList, boneCount);

		if (scene->HasMeshes())
		{
			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				aiMesh* assimpMesh = scene->mMeshes[i];

				Mesh* meshData = new Mesh();

				for (int j = 0; j < assimpMesh->mNumVertices; j++)
				{
					Vertex vtx;
					if (assimpMesh->HasPositions())
					{
						vtx.position.x = assimpMesh->mVertices[j].x;
						vtx.position.y = assimpMesh->mVertices[j].y;
						vtx.position.z = assimpMesh->mVertices[j].z;
					}

					if (assimpMesh->HasTextureCoords(0))
					{
						vtx.textureCoordinate.u = assimpMesh->mTextureCoords[0][j].x;
						vtx.textureCoordinate.v = assimpMesh->mTextureCoords[0][j].y;
					}

					if (assimpMesh->HasNormals())
					{
						vtx.normal.x = assimpMesh->mNormals[j].x;
						vtx.normal.y = assimpMesh->mNormals[j].y;
						vtx.normal.z = assimpMesh->mNormals[j].z;
					}

					if (assimpMesh->HasTangentsAndBitangents())
					{
						vtx.tangent.x = assimpMesh->mTangents[j].x;
						vtx.tangent.y = assimpMesh->mTangents[j].y;
						vtx.tangent.z = assimpMesh->mTangents[j].z;

						vtx.biTangent.x = assimpMesh->mBitangents[j].x;
						vtx.biTangent.y = assimpMesh->mBitangents[j].y;
						vtx.biTangent.z = assimpMesh->mBitangents[j].z;
					}

					vtx.boneIndices[0] = 0xffffffffu;
					vtx.boneIndices[1] = 0xffffffffu;
					vtx.boneIndices[2] = 0xffffffffu;
					vtx.boneIndices[3] = 0xffffffffu;

					vtx.weight[0] = 0.0f;
					vtx.weight[1] = 0.0f;
					vtx.weight[2] = 0.0f;
					vtx.weight[3] = 0.0f;

					meshData->vertices.push_back(vtx);
				}

				for (int j = 0; j < assimpMesh->mNumBones; j++)
				{
					std::string str = assimpMesh->mBones[j]->mName.C_Str();
					std::wstring boneName;
					boneName.assign(str.begin(), str.end());

					if (boneIndexMap.count(boneName) == 0)
					{
						continue;
					}

					auto offset = assimpMesh->mBones[j]->mOffsetMatrix;

					model->boneList[boneIndexMap[boneName]]->offsetMatrix <<
						offset.a1, offset.a2, offset.a3, offset.a4,
						offset.b1, offset.b2, offset.b3, offset.b4,
						offset.c1, offset.c2, offset.c3, offset.c4,
						offset.d1, offset.d2, offset.d3, offset.d4;

					for (int k = 0; k < assimpMesh->mBones[j]->mNumWeights; k++)
					{
						unsigned int vid = assimpMesh->mBones[j]->mWeights[k].mVertexId;
						float weight = assimpMesh->mBones[j]->mWeights[k].mWeight;

						for (int l = 0; l < 4; l++)
						{
							if (meshData->vertices[vid].weight[l] == 0.0f)
							{
								meshData->vertices[vid].weight[l] = weight;
								meshData->vertices[vid].boneIndices[l] = boneIndexMap[boneName];
								break;
							}
							else
							{
								// 하나의 버텍스가 4개 이상의 본에 영향을 받음
								assert(l < 3);
								continue;
							}
						}
					}
				}

				for (int j = 0; j < assimpMesh->mNumFaces; j++)
				{
					aiFace face = assimpMesh->mFaces[j];
					for (int k = 0; k < face.mNumIndices; k++)
					{
						meshData->indices.push_back(face.mIndices[k]);
					}
				}

				meshData->materialIndex = assimpMesh->mMaterialIndex;
				model->meshList.push_back(meshData);
			}
		}

		// Animation
		for (int i = 0; i < scene->mNumAnimations; i++)
		{
			aiAnimation* aianimation = scene->mAnimations[i];

			Animation* animationData = new Animation();
			model->animationList.push_back(animationData);

			std::string str = aianimation->mName.C_Str();
			animationData->name.assign(str.begin(), str.end());

			animationData->duration = aianimation->mDuration;
			animationData->tickPerSecond = aianimation->mTicksPerSecond;


			for (int j = 0; j < aianimation->mNumChannels; j++)
			{
				aiNodeAnim* aibone = aianimation->mChannels[j];

				std::string boneName = aibone->mNodeName.C_Str();

				std::wstring wboneName;
				wboneName.assign(boneName.begin(), boneName.end());

				std::map<double, AnimationKeyInfo> keys;

				// 키 개수는 모두 같아야한다.
				assert(aibone->mNumScalingKeys == aibone->mNumRotationKeys && aibone->mNumScalingKeys == aibone->mNumPositionKeys);

				for (int k = 0; k < aibone->mNumScalingKeys; k++)
				{
					aiVectorKey scalingKey = aibone->mScalingKeys[k];
					aiQuatKey rotationKey = aibone->mRotationKeys[k];
					aiVectorKey positionKey = aibone->mPositionKeys[k];

					// time이 일치하지 않는 데이터가 있다면 중단
					assert(scalingKey.mTime == rotationKey.mTime && scalingKey.mTime == positionKey.mTime);
					double time = scalingKey.mTime;

					aiVector3D scalingValue = scalingKey.mValue;
					aiQuaternion rotationValue = rotationKey.mValue;
					aiVector3D positionValue = positionKey.mValue;

					Float3 scale;
					scale.x = scalingValue.x;
					scale.y = scalingValue.y;
					scale.z = scalingValue.z;

					Quaternion rotation;
					rotation.x = rotationValue.x;
					rotation.y = rotationValue.y;
					rotation.z = rotationValue.z;
					rotation.w = rotationValue.w;

					Float3 position;
					position.x = positionValue.x;
					position.y = positionValue.y;
					position.z = positionValue.z;

					AnimationKeyInfo keyinfo;
					keyinfo.scaleKey = scale;
					keyinfo.rotationKey = rotation;
					keyinfo.positionKey = position;

					keys[time] = keyinfo;
				}

				animationData->animationKey[wboneName] = keys;
			}
		}

		importer.FreeScene();

		return model;
	}

	void FBXLoader::ReleaseModel(Model* model)
	{
		// root에서 부터 모든 노드를 타고가며 해제
		std::queue<Bone*> q;
		if (model->root != nullptr)
		{
			q.push(model->root);
		}

		while (!q.empty())
		{
			Bone* current = q.front();
			q.pop();

			for (int i = 0; i < current->children.size(); i++)
			{
				q.push(current->children[i]);
			}

			delete current;
		}

		model->root = nullptr;

		// Mesh
		for (int i = 0; i < model->meshList.size(); i++)
		{
			delete model->meshList[i];
		}
		model->meshList.clear();

		// Material
		for (int i = 0; i < model->materialList.size(); i++)
		{
			delete model->materialList[i];
		}
		model->materialList.clear();

		// Animation
		for (int i = 0; i < model->animationList.size(); i++)
		{
			delete model->animationList[i];
		}
		model->animationList.clear();
	}

	void FBXLoader::CopyNodeData(Bone* bone, aiNode* ainode, std::map<std::wstring, unsigned int>& boneIndexMap, std::vector<Bone*>& boneList, int& boneCount)
	{
		std::string str = ainode->mName.C_Str();
		std::wstring wstr;
		wstr.assign(str.begin(), str.end());

		bone->name = wstr;

		for (int i = 0; i < ainode->mNumMeshes; i++)
		{
			bone->meshes.push_back(ainode->mMeshes[i]);
		}

		// 오프셋 초기화
		bone->offsetMatrix = Eigen::Matrix4f::Identity();

		bone->transformMatrix <<
			ainode->mTransformation.a1, ainode->mTransformation.a2, ainode->mTransformation.a3, ainode->mTransformation.a4,
			ainode->mTransformation.b1, ainode->mTransformation.b2, ainode->mTransformation.b3, ainode->mTransformation.b4,
			ainode->mTransformation.c1, ainode->mTransformation.c2, ainode->mTransformation.c3, ainode->mTransformation.c4,
			ainode->mTransformation.d1, ainode->mTransformation.d2, ainode->mTransformation.d3, ainode->mTransformation.d4;

		if (boneIndexMap.count(wstr) != 0)
		{
			boneList[boneIndexMap[wstr]] = bone;
		}
		else
		{
			assert(boneCount < BONE_MAX);
			boneList[boneCount] = bone;
			boneIndexMap[wstr] = boneCount;
			boneCount++;
		}

		for (int i = 0; i < ainode->mNumChildren; i++)
		{
			aiNode* aichild = ainode->mChildren[i];
			Bone* childBone = new Bone();
			childBone->parent = bone;
			bone->children.push_back(childBone);
			CopyNodeData(childBone, aichild, boneIndexMap, boneList, boneCount);
		}
	}

	void FBXLoader::CopyMaterialData(Material* materialData, aiMaterial* assimpMaterial, const std::wstring& fileDirectory)
	{
		if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			materialData->useDiffuseMap = true;

			aiString texturePath;
			if (assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
			{
				std::string str = texturePath.C_Str();
				std::wstring wstr(str.begin(), str.end());

				wstr = fileDirectory + L"\\" + wstr;

				materialData->diffuseMap = wstr;
			}
		}
		else
		{
			materialData->useDiffuseMap = false;
		}

		if (assimpMaterial->GetTextureCount(aiTextureType_NORMALS) > 0)
		{
			materialData->useNormalMap = true;

			aiString texturePath;
			if (assimpMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS)
			{
				std::string str = texturePath.C_Str();
				std::wstring wstr(str.begin(), str.end());

				wstr = fileDirectory + L"\\" + wstr;

				materialData->normalMap = wstr;
			}
		}
		else
		{
			materialData->useNormalMap = false;
		}

		aiColor3D baseColor(1.0f, 1.0f, 1.0f);

		if (assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor) == AI_SUCCESS)
		{
			// Base Color를 가진경우
		}

		materialData->baseColor.r = baseColor.r;
		materialData->baseColor.g = baseColor.g;
		materialData->baseColor.b = baseColor.b;
		materialData->baseColor.a = 1.0f;
	}
}