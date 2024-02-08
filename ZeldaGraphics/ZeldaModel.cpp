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

constexpr unsigned int ID_NO_ANIMATION = 0;

ZeldaModel::~ZeldaModel()
{
	if (animationResourceView)
	{
		animationResourceView->Release();
		animationResourceView = nullptr;
	}

	// root ���� Ʈ��Ÿ�� ���� ����
	std::queue<Node*> q;
	if (root != nullptr)
	{
		q.push(root);
	}

	while (!q.empty())
	{
		Node* current = q.front();
		q.pop();

		for (int i = 0; i < current->children.size(); i++)
		{
			q.push(current->children[i]);
		}

		delete current;
	}

	root = nullptr;

	// meshes ���� (ZeldaMesh)
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
	meshes.clear();

	// material ���� (ZeldaMaterial)
	for (int i = 0; i < materials.size(); i++)
	{
		delete materials[i];
	}
	materials.clear();

	// Animation ����
	for (auto iter = animationTable.begin(); iter != animationTable.end(); iter++)
	{
		Animation* animation = iter->second;
		delete animation;
	}
	animationTable.clear();

	if (animationResourceView != nullptr)
	{
		animationResourceView->Release();
		animationResourceView = nullptr;
	}
}

void ZeldaModel::Render(
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
	ConstantBuffer<AnimationHierarchyBufferType, ShaderType::VertexShader>* animationHierarchyVsConstBuffer,
	ConstantBuffer<AnimationBufferType, ShaderType::VertexShader>* animationConstBuffer,
	ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
	DirectX::XMMATRIX worldMatrix,
	ZeldaShader* shader,
	const std::wstring& firstAnimationName,
	const std::wstring& secondAnimationName,
	float firstAnimationTime,
	float secondAnimationTime,
	float ratio)
{
	assert(firstAnimationTime >= 0.0f);
	assert(secondAnimationTime >= 0.0f);

	SetAnimationTexture(deviceContext);

	animationHierarchyVsConstBuffer->SetData(boneHierarchyData);

	AnimationBufferType animationData;

	// ���������� �ִϸ��̼� ������ ���� ��� �⺻���¸� ����Ѵ�.
	if (animationTable.count(firstAnimationName) == 0 || animationTable.count(secondAnimationName) == 0)
	{
		animationData.animationInfo.firstAnimationID = ID_NO_ANIMATION; // ID 0�� �ִϸ��̼��� ���� ����
		animationData.animationInfo.secondAnimationID = ID_NO_ANIMATION; // ID 0�� �ִϸ��̼��� ���� ����
		animationData.animationInfo.firstAnimationFrame = 0.0f;
		animationData.animationInfo.secondAnimationFrame = 0.0f;
		animationData.animationInfo.ratio = 0.0f;
	}
	else
	{
		animationData.animationInfo.firstAnimationID = animationIDTable[firstAnimationName];
		animationData.animationInfo.secondAnimationID = animationIDTable[secondAnimationName];

		float firstFrameTime = firstAnimationTime * animationTable[firstAnimationName]->tickPerSecond;
		if (firstFrameTime < 0.0f) firstFrameTime = 0.0f;
		if (firstFrameTime > animationTable[firstAnimationName]->duration) firstFrameTime = animationTable[firstAnimationName]->duration;

		float secondFrameTime = secondAnimationTime * animationTable[secondAnimationName]->tickPerSecond;
		if (secondFrameTime < 0.0f) secondFrameTime = 0.0f;
		if (secondFrameTime > animationTable[secondAnimationName]->duration) secondFrameTime = animationTable[secondAnimationName]->duration;

		animationData.animationInfo.firstAnimationFrame = firstFrameTime;
		animationData.animationInfo.secondAnimationFrame = secondFrameTime;
		animationData.animationInfo.ratio = ratio;
	}

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// ���̴��� �ѱ�� ����� ��ġ�� �� �� �Ѱܾ� �Ѵ�.
	matrixConstBuffer->SetData({ XMMatrixTranspose(worldMatrix), XMMatrixTranspose(currentcamera->GetViewMatrix()), XMMatrixTranspose(currentcamera->GetProjMatrix()) });
	animationConstBuffer->SetData(animationData);

	// ��� �޽� �׸���
	for (int i = 0; i < meshes.size(); i++)
	{
		ZeldaMesh* currentMesh = meshes[i];
		currentMesh->Render(deviceContext);
		int indexCount = currentMesh->GetIndexCount();

		materialConstBuffer->SetData({
			materials[materialIndex[i]]->baseColor,
			!materials[materialIndex[i]]->useDiffuseMap,
			materials[materialIndex[i]]->UseSRGB(ZeldaMaterial::DIFFUSE_MAP) ,
			materials[materialIndex[i]]->useDiffuseMap
			});

		ConstantBufferManager::GetInstance().SetBuffer();

		materials[materialIndex[i]]->SetShaderResource(deviceContext);

		shader->Render(deviceContext, indexCount);
	}
}

void ZeldaModel::RenderInstanced(
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
	ConstantBuffer<InstancingMatrixBufferType, ShaderType::VertexShader>* instancingMatrixConstBuffer,
	ConstantBuffer<AnimationHierarchyBufferType, ShaderType::VertexShader>* animationHierarchyVsConstBuffer,
	ConstantBuffer<InstancingAnimationBufferType, ShaderType::VertexShader>* instancingAnimationConstBuffer,
	ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
	const std::vector<ModelInstancingInfo>& instancingInfo,
	ZeldaShader* shader)
{
	SetAnimationTexture(deviceContext);

	animationHierarchyVsConstBuffer->SetData(boneHierarchyData);

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// ���̴��� �ѱ�� ����� ��ġ�� �� �� �Ѱܾ� �Ѵ�.
	MatrixBufferType matrixBuffer;
	matrixBuffer.view = XMMatrixTranspose(currentcamera->GetViewMatrix());
	matrixBuffer.projection = XMMatrixTranspose(currentcamera->GetProjMatrix());
	matrixBuffer.cameraFar = currentcamera->GetFar();
	matrixConstBuffer->SetData(matrixBuffer);

	InstancingMatrixBufferType* instacingMatrix = new InstancingMatrixBufferType();
	InstancingAnimationBufferType* instacingData = new InstancingAnimationBufferType();

	for (size_t i = 0; i < instancingInfo.size(); i++)
	{
		double firstFrameTime = 0.0;
		if (instancingInfo[i].firstAnimationName != L"" && animationTable.count(instancingInfo[i].firstAnimationName) != 0)
		{
			float duration = animationTable[instancingInfo[i].firstAnimationName]->duration;
			firstFrameTime = instancingInfo[i].firstAnimationTime * animationTable[instancingInfo[i].firstAnimationName]->tickPerSecond;
			if (firstFrameTime < 0.0f) firstFrameTime = 0.0f;
			if (firstFrameTime > duration) firstFrameTime = duration;
		}

		double secondFrameTime = 0.0;
		if (instancingInfo[i].secondAnimationName != L"" && animationTable.count(instancingInfo[i].secondAnimationName) != 0)
		{
			float duration = animationTable[instancingInfo[i].secondAnimationName]->duration;
			secondFrameTime = instancingInfo[i].secondAnimationTime * animationTable[instancingInfo[i].secondAnimationName]->tickPerSecond;
			if (secondFrameTime < 0.0f) secondFrameTime = 0.0f;
			if (secondFrameTime > duration) secondFrameTime = duration;
		}

		unsigned int firstAnimationID = 0;
		if (instancingInfo[i].firstAnimationName != L"" && animationIDTable.count(instancingInfo[i].firstAnimationName) != 0)
		{
			firstAnimationID = animationIDTable[instancingInfo[i].firstAnimationName];
		}

		unsigned int secondAnimationID = 0;
		if (instancingInfo[i].secondAnimationName != L"" && animationIDTable.count(instancingInfo[i].secondAnimationName) != 0)
		{
			secondAnimationID = animationIDTable[instancingInfo[i].secondAnimationName];
		}

		// ���̴��� �ѱ�� ����� ��ġ�� �� �� �Ѱܾ� �Ѵ�.
		instacingMatrix->instancingWorldMatrix[i % INSTANCING_MAX] = XMMatrixTranspose(instancingInfo[i].worldMatrix);
		instacingData->animationInfo[i % INSTANCING_MAX].firstAnimationFrame = firstFrameTime;
		instacingData->animationInfo[i % INSTANCING_MAX].secondAnimationFrame = secondFrameTime;
		instacingData->animationInfo[i % INSTANCING_MAX].firstAnimationID = firstAnimationID;
		instacingData->animationInfo[i % INSTANCING_MAX].secondAnimationID = secondAnimationID;
		instacingData->animationInfo[i % INSTANCING_MAX].ratio = instancingInfo[i].ratio;

		// �ν��Ͻ� ������ �ִ� ������ ��� �׸���
		if (((i % INSTANCING_MAX) + 1 == INSTANCING_MAX) || (i == instancingInfo.size() - 1))
		{
			instancingMatrixConstBuffer->SetData(*instacingMatrix);
			instancingAnimationConstBuffer->SetData(*instacingData);

			// ��� �޽� �׸���
			for (size_t meshNum = 0; meshNum < meshes.size(); meshNum++)
			{
				ZeldaMesh* currentMesh = meshes[meshNum];
				currentMesh->RenderInstanced(deviceContext);
				int indexCount = currentMesh->GetIndexCount();

				materialConstBuffer->SetData({
					materials[materialIndex[meshNum]]->baseColor,
					!materials[materialIndex[meshNum]]->useDiffuseMap,
					materials[materialIndex[meshNum]]->UseSRGB(ZeldaMaterial::DIFFUSE_MAP),
					materials[materialIndex[meshNum]]->useDiffuseMap
					});

				ConstantBufferManager::GetInstance().SetBuffer();

				materials[materialIndex[meshNum]]->SetShaderResource(deviceContext);

				shader->RenderInstanced(deviceContext, indexCount, (i % INSTANCING_MAX) + 1, 0);
			}
		}
	}

	delete instacingMatrix;
	delete instacingData;
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
	updatedWorldMatrix(false),
	animationResourceView(nullptr)
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

	CreateAnimationResourceView(device);
}

void ZeldaModel::CreateAnimationResourceView(ID3D11Device* device)
{
	struct AnimationData
	{
		XMMATRIX matrix;
	};

	// �⺻���¸� �߰��Ͽ� �ִϸ��̼� ���� + 1
	// 0�� �ִϸ��̼ǿ��� Animation Node�� Transform�� Offset�� �����Ѵ�.
	unsigned long long animationSize = static_cast<unsigned long long>(animationTable.size()) + 1;
	
	// (�ִϸ��̼� ����) x (�ִϸ��̼� ������ ũ��)
	AnimationData identityData;
	identityData.matrix = XMMatrixIdentity();
	std::vector<std::vector<AnimationData>> textureData(animationSize, std::vector<AnimationData>(BONE_COUNT_MAX* ANIMATION_FRAME_MAX, identityData));

	std::unordered_map<std::wstring, unsigned int> boneIDMap;

	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i] == nullptr) continue;

		boneIDMap[bones[i]->name] = i;
	}

	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i] == nullptr) continue;

		textureData[0][BONE_COUNT_MAX * 0 + i].matrix = bones[i]->transformMatrix;
		textureData[0][BONE_COUNT_MAX * 1 + i].matrix = bones[i]->offsetMatrix;

		// �θ� ���� �� ����ϱ� ���� �ڱ� �ڽ��� ID
		unsigned int parentID = i;

		if (bones[i]->parent != nullptr)
		{
			// boneIDMap���� �θ� ã�� ���ߴٸ� ã�ų� root�� ���� ������ �ö󰣴�.
			if (boneIDMap.count(bones[i]->parent->name) == 0)
			{
				Node* tmpNode = bones[i]->parent;

				XMMATRIX parentTM = XMMatrixIdentity();

				while (true)
				{
					parentTM = parentTM * tmpNode->transformMatrix;

					if (tmpNode == root)
					{
						break;
					}

					if (boneIDMap.count(tmpNode->name) != 0)
					{
						break;
					}

					tmpNode = tmpNode->parent;
				}

				// root�� �θ�
				if (tmpNode == root)
				{
					// �θ� �ڱ� �ڽ����� ����
					parentID = i;

					// �ڱ� �ڽ��� ��� �����ӿ� parentTM�� �̸� ���صд�.
					int animationCount = 0;
					for (auto animIter = animationTable.begin(); animIter != animationTable.end(); animIter++)
					{
						for (int currentFrame = 0; currentFrame < ANIMATION_FRAME_MAX - 1; currentFrame++)
						{
							textureData[animationCount + 1][BONE_COUNT_MAX * currentFrame + i].matrix = textureData[animationCount + 1][BONE_COUNT_MAX * currentFrame + i].matrix * parentTM;
						}
						animationCount += 1;
					}

					// transformMatrix���� ���صд�
					textureData[0][BONE_COUNT_MAX * 0 + i].matrix = textureData[0][BONE_COUNT_MAX * 0 + i].matrix * parentTM;
				}
				else if (boneIDMap.count(tmpNode->name) != 0)
				{
					// �ϴ� boneIDMap���� �θ� �ٽ� ã�� ���� ���ٰ� �����ϰ���
					// ��������� �׷� �����Ͱ� ����
					assert(0);
				}
			}
			else
			{
				parentID = boneIDMap[bones[i]->parent->name];
			}
		}

		boneHierarchyData.parentBone[i] = parentID;
	}

	animationIDTable.clear();

	int animationCount = 0;
	for (auto animIter = animationTable.begin(); animIter != animationTable.end(); animIter++)
	{
		// animationIDTable�� ä���. TransformMatrix�� Offset�� ���������� ����ϱ� ���� 0���� ������� ����, ������� frame 0, 1, 2 ���
		animationIDTable[animIter->first] = animationCount + 1;

		Animation* currentAnimation = animIter->second;

		double currentDuration = currentAnimation->duration;
		double currentTickPerSecond = currentAnimation->tickPerSecond;
		auto& currentAnimationKey = currentAnimation->animationKey;

		int intDuration = static_cast<int>(currentDuration);

		// assert�� �ɸ��ٸ� �ܼ��� static_cast�� �ƴ϶� �ݿø� �� ĳ�����ϴ°����� ������ ��
		assert(abs(currentDuration - intDuration) < 0.1);

		// ��� �ִϸ��̼��� �� ������
		for (int currentFrame = 0; currentFrame < ANIMATION_FRAME_MAX - 1; currentFrame++)
		{
			// �ִϸ��̼� �����Ͱ� �����ϴ� �κ� (Duration���� �ִϸ��̼��� �����Ͽ� ��ȣ ����)
			if (currentFrame <= intDuration)
			{
				double frameTime = static_cast<double>(currentFrame);

				// ���� finalTM�� Texture�� �ִ´�.
				for (int i = 0; i < bones.size(); i++)
				{
					if (bones[i] == nullptr) continue;

					DirectX::XMMATRIX data = bones[i]->transformMatrix;

					if (currentAnimation->animationKey.count(bones[i]->name) != 0)
					{
						auto rightIter = currentAnimation->animationKey[bones[i]->name].lower_bound(frameTime);
						auto endIter = currentAnimation->animationKey[bones[i]->name].end();

						if (rightIter == endIter)
						{
							rightIter--;
						}

						XMMATRIX animationMatrix;

						AnimationKeyInfo keyinfo = rightIter->second;
						DirectX::XMMATRIX scaleMatrix = XMMatrixScalingFromVector(keyinfo.scale);
						DirectX::XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(keyinfo.rotation);
						DirectX::XMMATRIX translationMatrix = XMMatrixTranslationFromVector(keyinfo.position);
						animationMatrix = scaleMatrix * rotationMatrix * translationMatrix;

						data = animationMatrix;
					}

					// identity�� �ʱ�ȭ �س��� ������ �߰��� ���ϴ� ������� �����Ѵ�.
					// parentTM�� �̸� ������ ������ ������ �� �ֱ� ����
					textureData[animationCount + 1][BONE_COUNT_MAX * currentFrame + i].matrix = data * textureData[animationCount + 1][BONE_COUNT_MAX * currentFrame + i].matrix;
				}
			}
			// Duration������ �������� ������ ���������� ���� ä���.
			else
			{
				// �߸��� �����͸� �����ͼ� ����, �����Ͱ� ���� �ִϸ��̼�
				assert(currentFrame > 0);

				// ���� �������� finalTM�� Texture�� �ִ´�.
				for (int i = 0; i < bones.size(); i++)
				{
					if (bones[i] == nullptr) continue;

					textureData[animationCount + 1][BONE_COUNT_MAX * currentFrame + i] = textureData[animationCount + 1][BONE_COUNT_MAX * (currentFrame - 1) + i];
				}
			}
		}

		animationCount += 1;
	}
	

	// ���� �ִϸ��̼� �����ͷ� �ؽ��� �����Ͽ� ���̴� ���ҽ� �� ����
	ID3D11Texture2D* tex2d = nullptr;

	// �ؽ�ó ����
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 4 * BONE_COUNT_MAX;		// �ؽ�ó ���� ũ�� (Matrix�� �̷�� Vector�� ��) x (�� �ִ�ġ)
	textureDesc.Height = ANIMATION_FRAME_MAX;	// �ؽ�ó ���� ũ�� (������ �ִ�ġ)
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = animationSize;	// ArraySize �ִϸ��̼� ����
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	std::vector<D3D11_SUBRESOURCE_DATA> initData(animationSize);
	for (int i = 0; i < animationSize; i++)
	{
		initData[i].pSysMem = textureData[i].data();
		initData[i].SysMemPitch = 16 * 4 * BONE_COUNT_MAX; // ����Ʈ �����ΰ� ����...
		initData[i].SysMemSlicePitch = 16 * 4 * BONE_COUNT_MAX * ANIMATION_FRAME_MAX;
	}

	HRESULT hr = device->CreateTexture2D(&textureDesc, initData.data(), &tex2d);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to Create Animation Resource View", L"Model Error", MB_OK);
		assert(0);
	}

	// ���̴� ���ҽ� �� ����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = animationSize;

	hr = device->CreateShaderResourceView(tex2d, &srvDesc, &animationResourceView);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to Create Animation Resource View", L"Model Error", MB_OK);
		assert(0);
	}
}

void ZeldaModel::SetAnimationTexture(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetShaderResources(TEXTURE_SLOT_ANIMATION, 1, &animationResourceView);
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