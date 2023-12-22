#include "ZeldaModel.h"

#include "MathConverter.h"
#include "ZeldaMesh.h"
#include "ZeldaMaterial.h"
#include "ZeldaCamera.h"
#include "GraphicsResourceID.h"
#include "ZeldaShader.h"
#include "ZeldaLight.h"
#include "ZeldaMaterial.h"
#include "ZeldaTexture.h"
#include "ResourceManager.h"
#include "ConstantBufferManager.h"


#include <fstream>

using namespace DirectX;

ZeldaModel::~ZeldaModel()
{
	// mesh
	

	// material




	// 채워야함
}

void ZeldaModel::Render(
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
	ConstantBuffer<BoneBufferType, ShaderType::VertexShader>* boneConstBuffer,
	ConstantBuffer<LightBufferType, ShaderType::PixelShader>* lightConstBuffer,
	ConstantBuffer<UseBufferType, ShaderType::PixelShader>* useConstBuffer,
	ConstantBuffer<ColorBufferType, ShaderType::PixelShader>* colorConstBuffer,
	DirectX::XMMATRIX worldMatrix,
	ZeldaShader* shader,
	ZeldaLight* light,
	const std::wstring& animationName,
	float animationTime)
{
	// 정상적인 애니메이션 정보가 들어온 경우 RenderAnimation을 호출한다.
	if (animationTime > 0.0f && animationTable.count(animationName) > 0)
	{
		RenderAnimation(deviceContext, matrixConstBuffer, boneConstBuffer, lightConstBuffer, useConstBuffer, colorConstBuffer, worldMatrix, shader, light, animationName, animationTime);
		return;
	}

	// 애니메이션 정보가 완전하지 않은 경우, 정적인 모델을 그린다.

	// 첫 번째 Render에 한하여 월드행렬을 계산하고 그 후에는 저장된 것을 사용한다.
	if (updatedWorldMatrix == false)
	{
		updatedWorldMatrix = true;

		std::queue<std::pair<Node*, DirectX::XMMATRIX>> nodeQueue;

		nodeQueue.push({ root, root->transformMatrix });

		while (!nodeQueue.empty())
		{
			auto current = nodeQueue.front();
			nodeQueue.pop();
			Node* currentNode = current.first;
			DirectX::XMMATRIX currentMatrix = current.second;

			currentNode->worldMatrix = currentNode->offsetMatrix * currentMatrix;

			for (int i = 0; i < currentNode->children.size(); i++)
			{
				Node* nextNode = currentNode->children[i];
				DirectX::XMMATRIX nextMatrix = nextNode->transformMatrix * currentMatrix;

				nodeQueue.push({ nextNode, nextMatrix });
			}
		}
	}

	BoneBufferType* boneBuffer = new BoneBufferType();
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i] == nullptr) continue;
		// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
		boneBuffer->boneTM[i] = XMMatrixTranspose(bones[i]->worldMatrix);
	}

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
	matrixConstBuffer->SetData({ XMMatrixTranspose(worldMatrix), XMMatrixTranspose(currentcamera->GetViewMatrix()), XMMatrixTranspose(currentcamera->GetProjMatrix()) });
	boneConstBuffer->SetData(*boneBuffer);
	lightConstBuffer->SetData({ light->GetAmbient(), light->GetDiffuseColor(), light->GetSpecular(), light->GetDirection() });
	colorConstBuffer->SetData({ { 1, 1, 1, 1 } });

	delete boneBuffer;

	// 모든 메쉬 그리기
	for (int i = 0; i < meshes.size(); i++)
	{
		ZeldaMesh* currentMesh = meshes[i];
		currentMesh->Render(deviceContext);
		int indexCount = currentMesh->GetIndexCount();

		useConstBuffer->SetData({ false, (materials[materialIndex[i]]->useDiffuseMap ), true, (materials[materialIndex[i]]->diffuseMap != nullptr) && materials[materialIndex[i]]->diffuseMap->UseSRGB() });

		ConstantBufferManager::GetInstance().SetBuffer();

		shader->Render(deviceContext, indexCount, materials[materialIndex[i]]->diffuseMap);
	}
}

std::vector<std::wstring> ZeldaModel::GetAnimationList()
{
	std::vector<std::wstring> result;

	for (auto iter = animationTable.begin(); iter != animationTable.end(); iter++)
	{
		std::wstring animationName = iter->first;
		result.push_back(animationName);
	}

	sort(result.begin(), result.end());

	return result;
}

std::vector<float> ZeldaModel::GetAnimationPlayTime()
{
	std::vector<std::pair<std::wstring, float>> result;

	for (auto iter = animationTable.begin(); iter != animationTable.end(); iter++)
	{
		std::wstring animationName = iter->first;
		float duration = iter->second->duration / iter->second->tickPerSecond;

		result.push_back({ animationName, duration });
	}

	sort(result.begin(), result.end());

	std::vector<float> durationResult;

	for (auto iter = result.begin(); iter != result.end(); iter++)
	{
		durationResult.push_back(iter->second);
	}

	return durationResult;
}

ZeldaModel::ZeldaModel(ID3D11Device* device, FBXLoader::Model* fbxModel) :
	root(nullptr),
	updatedWorldMatrix(false)
{
	root = new Node();

	std::map<std::wstring, Node*> nodeTable;
	CopyNode(root, fbxModel->root, nodeTable);

	bones.assign(256, nullptr);
	for (int i = 0; i < fbxModel->boneList.size(); i++)
	{
		if (fbxModel->boneList[i] != nullptr)
		{
			bones[i] = nodeTable[fbxModel->boneList[i]->name];
		}
	}

	for (int i = 0; i < fbxModel->meshList.size(); i++)
	{
		FBXLoader::Mesh* fbxMesh = fbxModel->meshList[i];

		std::vector<VertexType> vertices;
		std::vector<unsigned int> indices;

		for (int j = 0; j < fbxMesh->vertices.size(); j++)
		{
			VertexType vertexType;
			vertexType.position.x = fbxMesh->vertices[j].position.x;
			vertexType.position.y = fbxMesh->vertices[j].position.y;
			vertexType.position.z = fbxMesh->vertices[j].position.z;

			vertexType.normal.x = fbxMesh->vertices[j].normal.x;
			vertexType.normal.y = fbxMesh->vertices[j].normal.y;
			vertexType.normal.z = fbxMesh->vertices[j].normal.z;

			vertexType.texture.x = fbxMesh->vertices[j].textureCoordinate.u;
			vertexType.texture.y = fbxMesh->vertices[j].textureCoordinate.v;

			vertexType.boneIndices.x = fbxMesh->vertices[j].boneIndices[0];
			vertexType.boneIndices.y = fbxMesh->vertices[j].boneIndices[1];
			vertexType.boneIndices.z = fbxMesh->vertices[j].boneIndices[2];
			vertexType.boneIndices.w = fbxMesh->vertices[j].boneIndices[3];

			vertexType.weight.x = fbxMesh->vertices[j].weight[0];
			vertexType.weight.y = fbxMesh->vertices[j].weight[1];
			vertexType.weight.z = fbxMesh->vertices[j].weight[2];
			vertexType.weight.w = fbxMesh->vertices[j].weight[3];

			vertices.push_back(vertexType);
		}

		for (int j = 0; j < fbxMesh->indices.size(); j++)
		{
			indices.push_back(fbxMesh->indices[j]);
		}

		meshes.push_back(new ZeldaMesh(device, vertices, indices));
		materialIndex.push_back(fbxMesh->materialIndex);
	}

	for (int i = 0; i < fbxModel->materialList.size(); i++)
	{
		FBXLoader::Material* fbxMaterial = fbxModel->materialList[i];
		ZeldaMaterial* material = new ZeldaMaterial(device, DirectX::XMFLOAT4(fbxMaterial->baseColor.r, fbxMaterial->baseColor.g, fbxMaterial->baseColor.b, fbxMaterial->baseColor.a), fbxMaterial->diffuseMap);
		materials.push_back(material);
	}

	for (int i = 0; i < fbxModel->animationList.size(); i++)
	{
		FBXLoader::Animation* fbxAnimation = fbxModel->animationList[i];

		Animation* animation = new Animation();
		animation->duration = fbxAnimation->duration;
		animation->tickPerSecond = fbxAnimation->tickPerSecond;
		
		for (auto iter = fbxAnimation->animationKey.begin(); iter != fbxAnimation->animationKey.end(); iter++)
		{
			std::wstring nodeName = iter->first;

			std::map<double, AnimationKeyInfo> keys;

			for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			{
				double time = iter2->first;
				DirectX::XMVECTOR scale;
				scale.m128_f32[0] = iter2->second.scaleKey.x;
				scale.m128_f32[1] = iter2->second.scaleKey.y;
				scale.m128_f32[2] = iter2->second.scaleKey.z;
				scale.m128_f32[3] = 1.0f;

				DirectX::XMVECTOR rotation;
				rotation.m128_f32[0] = iter2->second.rotationKey.x;
				rotation.m128_f32[1] = iter2->second.rotationKey.y;
				rotation.m128_f32[2] = iter2->second.rotationKey.z;
				rotation.m128_f32[3] = iter2->second.rotationKey.w;

				DirectX::XMVECTOR position;
				position.m128_f32[0] = iter2->second.positionKey.x;
				position.m128_f32[1] = iter2->second.positionKey.y;
				position.m128_f32[2] = iter2->second.positionKey.z;
				position.m128_f32[3] = 1.0f;

				keys[time] = AnimationKeyInfo(scale, rotation, position);
			}
			animation->animationKey[nodeName] = keys;
		}

		animationTable[fbxAnimation->name] = animation;
	}
}

void ZeldaModel::CopyNode(Node* node, FBXLoader::Bone* bone, std::map<std::wstring, Node*>& nodeTable)
{
	node->name = bone->name;
	node->meshes = bone->meshes;

	node->transformMatrix = MathConverter::EigenMatrixToXMMatrix(bone->transformMatrix);
	node->offsetMatrix = MathConverter::EigenMatrixToXMMatrix(bone->offsetMatrix);

	nodeTable[node->name] = node;

	for (int i = 0; i < bone->children.size(); i++)
	{
		Node* child = new Node();
		child->parent = node;
		node->children.push_back(child);
		CopyNode(child, bone->children[i], nodeTable);
	}
}

void ZeldaModel::RenderAnimation(
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<MatrixBufferType,
	ShaderType::VertexShader>* matrixConstBuffer,
	ConstantBuffer<BoneBufferType, ShaderType::VertexShader>* boneConstBuffer,
	ConstantBuffer<LightBufferType, ShaderType::PixelShader>* lightConstBuffer,
	ConstantBuffer<UseBufferType, ShaderType::PixelShader>* useConstBuffer,
	ConstantBuffer<ColorBufferType, ShaderType::PixelShader>* colorConstBuffer,
	DirectX::XMMATRIX worldMatrix,
	ZeldaShader* shader,
	ZeldaLight* light,
	const std::wstring& animationName,
	float animationTime)
{
	assert(animationTable.count(animationName) != 0);
	
	float frameTime = animationTime * animationTable[animationName]->tickPerSecond;

	// 애니메이션 시간에 따라 Bone정보 변경
	std::queue<std::pair<Node*, DirectX::XMMATRIX>> nodeQueue;

	nodeQueue.push({ root, root->transformMatrix });

	while (!nodeQueue.empty())
	{
		auto current = nodeQueue.front();
		nodeQueue.pop();
		Node* currentNode = current.first;
		DirectX::XMMATRIX currentMatrix = current.second;

		currentNode->finalTM = currentNode->offsetMatrix * currentMatrix;

		for (int i = 0; i < currentNode->children.size(); i++)
		{
			Node* nextNode = currentNode->children[i];
			DirectX::XMMATRIX nextMatrix = nextNode->transformMatrix * currentMatrix;

			if (animationTable[animationName]->animationKey.count(nextNode->name) != 0)
			{
				auto lowerIter = animationTable[animationName]->animationKey[nextNode->name].lower_bound(static_cast<double>(frameTime));

				auto upperIter = animationTable[animationName]->animationKey[nextNode->name].upper_bound(static_cast<double>(frameTime));

				if (lowerIter != animationTable[animationName]->animationKey[nextNode->name].begin())
				{
					lowerIter--;
				}

				if (upperIter == animationTable[animationName]->animationKey[nextNode->name].end())
				{
					upperIter--;
				}

				XMMATRIX interpolatedMatrix;
				if (lowerIter != upperIter)
				{
					float beginTime = static_cast<float>(lowerIter->first);
					float endTime = static_cast<float>(upperIter->first);
					float currentTime = static_cast<float>(static_cast<double>(frameTime));

					// 시간에 대한 보간 계수 계산
					float t = (currentTime - beginTime) / (endTime - beginTime);

					// 선형 보간을 사용하여 행렬 보간
					AnimationKeyInfo lowerKey = lowerIter->second;
					AnimationKeyInfo upperKey = upperIter->second;

					XMVECTOR scale = XMVectorLerp(lowerKey.scale, upperKey.scale, t);
					XMVECTOR rotation = XMQuaternionSlerp(lowerKey.rotation, upperKey.rotation, t);
					XMVECTOR position = XMVectorLerp(lowerKey.position, upperKey.position, t);

					// 보간된 행렬 구성
					XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
					XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotation);
					XMMATRIX translationMatrix = XMMatrixTranslationFromVector(position);

					interpolatedMatrix = scaleMatrix * rotationMatrix * translationMatrix;
				}
				else
				{
					AnimationKeyInfo keyinfo = lowerIter->second;
					DirectX::XMMATRIX scaleMatrix = XMMatrixScalingFromVector(keyinfo.scale);
					DirectX::XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(keyinfo.rotation);
					DirectX::XMMATRIX translationMatrix = XMMatrixTranslationFromVector(keyinfo.position);
					interpolatedMatrix = scaleMatrix * rotationMatrix * translationMatrix;
				}

				nextMatrix = interpolatedMatrix * currentMatrix;
			}

			nodeQueue.push({ nextNode, nextMatrix });
		}
	}

	BoneBufferType* boneBuffer = new BoneBufferType();
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i] == nullptr) continue;
		// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
		boneBuffer->boneTM[i] = XMMatrixTranspose(bones[i]->finalTM);
	}

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
	matrixConstBuffer->SetData({ XMMatrixTranspose(worldMatrix), XMMatrixTranspose(currentcamera->GetViewMatrix()), XMMatrixTranspose(currentcamera->GetProjMatrix()) });
	boneConstBuffer->SetData(*boneBuffer);
	lightConstBuffer->SetData({ light->GetAmbient(), light->GetDiffuseColor(), light->GetSpecular(), light->GetDirection() });
	colorConstBuffer->SetData({ { 1, 1, 1, 1 } });

	delete boneBuffer;

	// 모든 메쉬 그리기
	for (int i = 0; i < meshes.size(); i++)
	{
		ZeldaMesh* currentMesh = meshes[i];
		currentMesh->Render(deviceContext);
		int indexCount = currentMesh->GetIndexCount();

		useConstBuffer->SetData({ false, (materials[materialIndex[i]]->useDiffuseMap), true, (materials[materialIndex[i]]->diffuseMap != nullptr) && materials[materialIndex[i]]->diffuseMap->UseSRGB() });

		ConstantBufferManager::GetInstance().SetBuffer();

		shader->Render(deviceContext, indexCount, materials[materialIndex[i]]->diffuseMap);
	}
}
