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




	// ä������
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
	ZeldaLight* light)
{
	// �ӽ� �ִϸ��̼� �׽�Ʈ �ڵ�
	static double animationFrame = 0.0;
	animationFrame += 1.0;
	/////////////////////////////////////////////////////////

	static int animationNumber = 18;

	if (animationFrame > 300.0)
	{
		animationFrame = 0.0;
	}

	// �ִϸ��̼� �ð��� ���� Bone���� ����
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

			// �ӽ� �ִϸ��̼� �׽�Ʈ �ڵ�
			if (animationList.size() != 0 && animationList[animationNumber]->animationKey.count(nextNode->name) != 0)
			{
				// lower_bound�� targetKey �̻��� ù ��° iterator�� ��ȯ�մϴ�.
				auto upperIter = animationList[animationNumber]->animationKey[nextNode->name].lower_bound(animationFrame);

				// upper_bound�� targetKey�� �ʰ��ϴ� ù ��° iterator�� ��ȯ�մϴ�.
				auto lowerIter = animationList[animationNumber]->animationKey[nextNode->name].upper_bound(animationFrame);

				// ���� targetKey�� map ���� �ִ� ���, lowerIter�� upperIter�� ���� ��ġ�� ����ŵ�ϴ�.
				if (lowerIter != animationList[animationNumber]->animationKey[nextNode->name].begin()) {
					// lowerIter�� �ٷ� ���� iterator�� targetKey���� �����鼭 ���� ����� ���Դϴ�.
					--lowerIter;
				}
				if (upperIter == animationList[animationNumber]->animationKey[nextNode->name].end())
				{
					upperIter--;
				}

				XMMATRIX interpolatedMatrix;
				if (lowerIter != upperIter)
				{
					float beginTime = static_cast<float>(lowerIter->first);
					float endTime = static_cast<float>(upperIter->first);
					float currentTime = static_cast<float>(animationFrame);

					// �ð��� ���� ���� ��� ���
					float t = (currentTime - beginTime) / (endTime - beginTime);

					// ���� ������ ����Ͽ� ��� ����
					AnimationKeyInfo lowerKey = lowerIter->second;
					AnimationKeyInfo upperKey = upperIter->second;

					XMVECTOR scale = XMVectorLerp(lowerKey.scale, upperKey.scale, t);
					XMVECTOR rotation = XMQuaternionSlerp(lowerKey.rotation, upperKey.rotation, t);
					XMVECTOR position = XMVectorLerp(lowerKey.position, upperKey.position, t);

					// ������ ��� ����
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
			/////////////////////////////////////////////////////////


			nodeQueue.push({ nextNode, nextMatrix });
		}
	}

	BoneBufferType* boneBuffer = new BoneBufferType();
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i] == nullptr) continue;
		// ���̴��� �ѱ�� ����� ��ġ�� �� �� �Ѱܾ� �Ѵ�.
		boneBuffer->boneTM[i] = XMMatrixTranspose(bones[i]->finalTM);
	}

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// ���̴��� �ѱ�� ����� ��ġ�� �� �� �Ѱܾ� �Ѵ�.
	matrixConstBuffer->SetData({ XMMatrixTranspose(worldMatrix), XMMatrixTranspose(currentcamera->GetViewMatrix()), XMMatrixTranspose(currentcamera->GetProjMatrix()) });
	boneConstBuffer->SetData(*boneBuffer);
	lightConstBuffer->SetData({ light->GetAmbient(), light->GetDiffuseColor(), light->GetSpecular(), light->GetDirection() });
	colorConstBuffer->SetData({ { 1, 1, 1, 1 } });

	delete boneBuffer;

	// ��� �޽� �׸���
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

ZeldaModel::ZeldaModel(ID3D11Device* device, FBXLoader::Model* fbxModel) :
	root(nullptr)
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
		animation->name = fbxAnimation->name;
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

		animationList.push_back(animation);
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
