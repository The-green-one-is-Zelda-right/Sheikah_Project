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

	// Shader Resource View ����
	for (auto iter = animationResourceViewTable.begin(); iter != animationResourceViewTable.end(); iter++)
	{
		ID3D11ShaderResourceView* srv = iter->second;
		if (srv != nullptr)
		{
			srv->Release();
		}
	}
	animationResourceViewTable.clear();
}

void ZeldaModel::Render(
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
	ConstantBuffer<AnimationBufferType, ShaderType::VertexShader>* animationConstBuffer,
	ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
	DirectX::XMMATRIX worldMatrix,
	ZeldaShader* shader,
	const std::wstring& animationName,
	float animationTime)
{
	assert(animationIDTable.count(animationName) > 0);
	unsigned int animationID = animationIDTable[animationName];

	assert(animationTPSTable.count(animationID) > 0 || animationID);
	float tps = animationTPSTable[animationIDTable[animationName]];

	SetAnimationTexture(deviceContext, animationID);

	AnimationBufferType animationData;
	animationData.animationInfo.animationTime = animationTime * tps;

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
	ConstantBuffer<InstancingAnimationBufferType, ShaderType::VertexShader>* instancingAnimationConstBuffer,
	ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
	const std::vector<ModelInstancingInfo>& instancingInfo,
	ZeldaShader* shader,
	const std::wstring& animationName)
{
	assert(animationIDTable.count(animationName) > 0);
	unsigned int animationID = animationIDTable[animationName];

	assert(animationTPSTable.count(animationID) > 0);
	float tps = animationTPSTable[animationIDTable[animationName]];

	SetAnimationTexture(deviceContext, animationID);

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
		// ���̴��� �ѱ�� ����� ��ġ�� �� �� �Ѱܾ� �Ѵ�.
		instacingMatrix->instancingWorldMatrix[i % INSTANCING_MAX] = XMMatrixTranspose(instancingInfo[i].worldMatrix);
		instacingData->animationInfo[i % INSTANCING_MAX].animationTime = instancingInfo[i].time * tps;

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

void ZeldaModel::RenderBlendingAnimation(
	ID3D11DeviceContext* deviceContext, ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
	ConstantBuffer<BlendingAnimationBufferType, ShaderType::VertexShader>* blendingAnimationVsConstBuffer,
	ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer,
	DirectX::XMMATRIX worldMatrix,
	ZeldaShader* shader,
	const std::wstring& firstAnimationName,
	const std::wstring& secondAnimationName,
	float firstAnimationTime,
	float secondAnimationTime,
	float ratio)
{
	auto iter1 = animationTable.find(firstAnimationName);
	auto iter2 = animationTable.find(secondAnimationName);

	if (iter1 != animationTable.end() && iter2 != animationTable.end())
	{
		Animation* animation1 = iter1->second;
		Animation* animation2 = iter2->second;

		float tickTime1 = firstAnimationTime * animation1->tickPerSecond;
		float tickTime2 = secondAnimationTime * animation2->tickPerSecond;

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
				
				XMVECTOR s1, r1, t1;
				XMVECTOR s2, r2, t2;

				XMMatrixDecompose(&s1, &r1, &t1, nextNode->transformMatrix);
				s2 = s1;
				r2 = r1;
				t2 = t1;

				if (animation1->animationKey.count(nextNode->name) != 0)
				{
					auto lowerIter = animation1->animationKey[nextNode->name].lower_bound(static_cast<double>(tickTime1));

					auto upperIter = animation1->animationKey[nextNode->name].upper_bound(static_cast<double>(tickTime1));

					if (lowerIter != animation1->animationKey[nextNode->name].begin())
					{
						lowerIter--;
					}

					if (upperIter == animation1->animationKey[nextNode->name].end())
					{
						upperIter--;
					}

					if (lowerIter != upperIter)
					{
						float beginTime = static_cast<float>(lowerIter->first);
						float endTime = static_cast<float>(upperIter->first);
						float currentTime = static_cast<float>(static_cast<double>(tickTime1));

						// �ð��� ���� ���� ��� ���
						float t = (currentTime - beginTime) / (endTime - beginTime);

						// ���� ������ ����Ͽ� ��� ����
						AnimationKeyInfo lowerKey = lowerIter->second;
						AnimationKeyInfo upperKey = upperIter->second;

						XMVECTOR scale = XMVectorLerp(lowerKey.scale, upperKey.scale, t);
						XMVECTOR rotation = XMQuaternionSlerp(lowerKey.rotation, upperKey.rotation, t);
						XMVECTOR position = XMVectorLerp(lowerKey.position, upperKey.position, t);

						s1 = scale;
						r1 = rotation;
						t1 = position;
					}
					else
					{
						AnimationKeyInfo keyinfo = lowerIter->second;
						s1 = keyinfo.scale;
						r1 = keyinfo.rotation;
						t1 = keyinfo.position;
					}
				}

				if (animation2->animationKey.count(nextNode->name) != 0)
				{
					auto lowerIter = animation2->animationKey[nextNode->name].lower_bound(static_cast<double>(tickTime2));

					auto upperIter = animation2->animationKey[nextNode->name].upper_bound(static_cast<double>(tickTime2));

					if (lowerIter != animation2->animationKey[nextNode->name].begin())
					{
						lowerIter--;
					}

					if (upperIter == animation2->animationKey[nextNode->name].end())
					{
						upperIter--;
					}

					if (lowerIter != upperIter)
					{
						float beginTime = static_cast<float>(lowerIter->first);
						float endTime = static_cast<float>(upperIter->first);
						float currentTime = static_cast<float>(static_cast<double>(tickTime2));

						// �ð��� ���� ���� ��� ���
						float t = (currentTime - beginTime) / (endTime - beginTime);

						// ���� ������ ����Ͽ� ��� ����
						AnimationKeyInfo lowerKey = lowerIter->second;
						AnimationKeyInfo upperKey = upperIter->second;

						XMVECTOR scale = XMVectorLerp(lowerKey.scale, upperKey.scale, t);
						XMVECTOR rotation = XMQuaternionSlerp(lowerKey.rotation, upperKey.rotation, t);
						XMVECTOR position = XMVectorLerp(lowerKey.position, upperKey.position, t);

						s2 = scale;
						r2 = rotation;
						t2 = position;
					}
					else
					{
						AnimationKeyInfo keyinfo = lowerIter->second;
						s2 = keyinfo.scale;
						r2 = keyinfo.rotation;
						t2 = keyinfo.position;
					}
				}

				XMMATRIX interpolatedMatrix;
				XMVECTOR scale = XMVectorLerp(s1, s2, ratio);
				XMVECTOR rotation = XMQuaternionSlerp(r1, r2, ratio);
				XMVECTOR position = XMVectorLerp(t1, t2, ratio);

				DirectX::XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
				DirectX::XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotation);
				DirectX::XMMATRIX translationMatrix = XMMatrixTranslationFromVector(position);
				interpolatedMatrix = scaleMatrix * rotationMatrix * translationMatrix;

				DirectX::XMMATRIX nextMatrix = interpolatedMatrix * currentMatrix;

				nodeQueue.push({ nextNode, nextMatrix });
			}
		}
	}

	BlendingAnimationBufferType* finalTMData = new BlendingAnimationBufferType();
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i] == nullptr) continue;

		finalTMData->finalTM[i] = XMMatrixTranspose(bones[i]->finalTM);
	}

	blendingAnimationVsConstBuffer->SetData(*finalTMData);

	delete finalTMData;

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// ���̴��� �ѱ�� ����� ��ġ�� �� �� �Ѱܾ� �Ѵ�.
	matrixConstBuffer->SetData({ XMMatrixTranspose(worldMatrix), XMMatrixTranspose(currentcamera->GetViewMatrix()), XMMatrixTranspose(currentcamera->GetProjMatrix()) });

	// ��� �޽� �׸���
	for (int i = 0; i < meshes.size(); i++)
	{
		ZeldaMesh* currentMesh = meshes[i];
		currentMesh->Render(deviceContext);
		int indexCount = currentMesh->GetIndexCount();

		materialConstBuffer->SetData({
			materials[materialIndex[i]]->baseColor,
			!materials[materialIndex[i]]->useDiffuseMap,
			materials[materialIndex[i]]->UseSRGB(ZeldaMaterial::DIFFUSE_MAP),
			materials[materialIndex[i]]->useDiffuseMap
			});

		ConstantBufferManager::GetInstance().SetBuffer();

		materials[materialIndex[i]]->SetShaderResource(deviceContext);

		shader->Render(deviceContext, indexCount);
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
	animationResourceViewTable()
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
	constexpr unsigned int animationTickMax = Model::Animation::Tick::Max;
	constexpr unsigned int boneCountMax = Model::Animation::Bone::Max;
	constexpr float targetTPS = Model::Animation::Tick::TargetTickPerSecond;

	const size_t animationCount = animationTable.size() + 1;

	std::vector<std::vector<XMMATRIX>> animationData(animationCount, std::vector<XMMATRIX>(boneCountMax * animationTickMax, XMMatrixIdentity()));

	// �⺻ ���¿� ���� ������ ����
	CalculateIdleBoneTM();

	for (size_t i = 0ull; i < bones.size(); i++)
	{
		if (bones[i] == nullptr) continue;

		for (unsigned int j = 0u; j < animationTickMax; j++)
		{
			animationData[0][boneCountMax * j + i] = bones[i]->worldMatrix;
		}
	}

	animationIDTable.clear();
	animationIDTable[L""] = 0u; // �⺻������ AnimationID

	animationTPSTable.clear();
	animationTPSTable[0u] = 0.0f; // �⺻������ TPS

	// �����̳ʵ� ����
	int cnt = 0;
	for (auto animIter = animationTable.begin(); animIter != animationTable.end(); animIter++)
	{
		unsigned int currentAnimationID = cnt + 1;

		Animation* currentAnimation = animIter->second;

		// animationIDTable�� ä���.
		animationIDTable[animIter->first] = currentAnimationID;

		cnt += 1;
	}

	// ���� �ִϸ��̼ǿ� ���� ������ ����
	for (auto animIter = animationTable.begin(); animIter != animationTable.end(); animIter++)
	{
		Animation* currentAnimation = animIter->second;
		unsigned int currentAnimationID = animationIDTable[animIter->first];

		double animationDuration = round(currentAnimation->duration);
		double currentTPS = currentAnimation->tickPerSecond;
		auto& currentAnimationKey = currentAnimation->animationKey;

		float textureTPS = targetTPS;
		unsigned int textureDuration = static_cast<unsigned int>(animationDuration * (static_cast<double>(targetTPS) / currentTPS));
		if (!(animationDuration * (static_cast<double>(targetTPS) / currentTPS) <= static_cast<double>(animationTickMax - 1u)))
		{
			textureTPS = currentTPS * (static_cast<double>(animationTickMax - 1) / animationDuration);
			textureDuration = animationTickMax - 1u;
		}

		animationTPSTable[currentAnimationID] = textureTPS;

		float samplingRate = static_cast<float>(currentTPS) / textureTPS;

		for (unsigned int tick = 0; tick <= animationTickMax - 1; tick++)
		{
			float fTick = static_cast<float>(tick);

			if (tick <= textureDuration)
			{
				CalculateAnimationBoneTM(currentAnimation, fTick * samplingRate);

				for (size_t i = 0ull; i < bones.size(); i++)
				{
					if (bones[i] == nullptr) continue;

					animationData[currentAnimationID][boneCountMax * tick + i] = bones[i]->finalTM;
				}
			}
			else
			{
				assert(tick != 0);

				// ���� ƽ�� ������ �ֱ�
				for (size_t i = 0ull; i < bones.size(); i++)
				{
					if (bones[i] == nullptr) continue;

					animationData[currentAnimationID][boneCountMax * tick + i] = animationData[currentAnimationID][boneCountMax * (tick - 1) + i];
				}
			}
		}
	}


	for (size_t i = 0; i < animationCount; i++)
	{
		// ���� �ִϸ��̼� �����ͷ� �ؽ��� �����Ͽ� ���̴� ���ҽ� �� ����
		ID3D11Texture2D* tex2d = nullptr;

		// �ؽ�ó ����
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = 4 * boneCountMax;		// �ؽ�ó ���� ũ�� (Matrix�� �̷�� Vector�� ��) x (�� �ִ�ġ)
		textureDesc.Height = animationTickMax;	// �ؽ�ó ���� ũ�� (������ �ִ�ġ)
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		//textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = animationData[i].data();
		initData.SysMemPitch = 16 * 4 * boneCountMax; // ����Ʈ �����ΰ� ����...
		initData.SysMemSlicePitch = 16 * 4 * boneCountMax * animationTickMax;

		HRESULT hr = device->CreateTexture2D(&textureDesc, &initData, &tex2d);
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
		srvDesc.Texture2DArray.ArraySize = 1;

		ID3D11ShaderResourceView* animationResourceView;
		hr = device->CreateShaderResourceView(tex2d, &srvDesc, &animationResourceView);
		if (FAILED(hr))
		{
			MessageBox(0, L"Failed to Create Animation Resource View", L"Model Error", MB_OK);
			assert(0);
		}

		animationResourceViewTable[i] = animationResourceView;

		if (tex2d != nullptr)
		{
			tex2d->Release();
			tex2d = nullptr;
		}
	}
}

void ZeldaModel::SetAnimationTexture(ID3D11DeviceContext* deviceContext, unsigned int animationID)
{
	assert(animationResourceViewTable.count(animationID) > 0);

	auto iter = animationResourceViewTable.find(animationID);
	if (iter != animationResourceViewTable.end())
	{
		ID3D11ShaderResourceView* srv = iter->second;
		deviceContext->VSSetShaderResources(TEXTURE_SLOT_ANIMATION, 1, &srv);
	}
}

void ZeldaModel::CalculateIdleBoneTM()
{
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

void ZeldaModel::CalculateAnimationBoneTM(Animation* animation, float tickTime)
{
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

			if (animation->animationKey.count(nextNode->name) != 0)
			{
				auto lowerIter = animation->animationKey[nextNode->name].lower_bound(static_cast<double>(tickTime));

				auto upperIter = animation->animationKey[nextNode->name].upper_bound(static_cast<double>(tickTime));

				if (lowerIter != animation->animationKey[nextNode->name].begin())
				{
					lowerIter--;
				}

				if (upperIter == animation->animationKey[nextNode->name].end())
				{
					upperIter--;
				}

				XMMATRIX interpolatedMatrix;
				if (lowerIter != upperIter)
				{
					float beginTime = static_cast<float>(lowerIter->first);
					float endTime = static_cast<float>(upperIter->first);
					float currentTime = static_cast<float>(static_cast<double>(tickTime));

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

			nodeQueue.push({ nextNode, nextMatrix });
		}
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