#include "ResourceManager.h"

#include "ZeldaGraphicsDefine.h"
#include "ZeldaModel.h"
#include "ZeldaMesh.h"
#include "ZeldaTexture.h"
#include "ZeldaShader.h"
#include "ZeldaCamera.h"
#include "IDGenerator.h"

#include "FBXData.h"
#include "FBXLoader.h"

using namespace DirectX;

void ResourceManager::Initialize(ID3D11Device* device)
{
	this->device = device;
}

ModelID ResourceManager::CreateModelFromModelingFile(const std::wstring& filePath)
{
	FBXLoader::FBXLoader fbxloader;
	FBXLoader::Model* fbxmodel = fbxloader.CreateModelFromFBX(filePath);

	ModelID resourceID = IDGenerator::CreateID<ResourceType::Model>();

	while (modelTable.count(resourceID) != 0 || resourceID == ModelID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Model>();
	}

	modelTable[resourceID] = new ZeldaModel(device, fbxmodel);

	fbxloader.ReleaseModel(fbxmodel);

	return resourceID;
}

bool ResourceManager::CreateCubeMesh()
{
	if (cubeMesh != nullptr)
	{
		return false;
	}

	std::vector<VertexType> vertexList;
	std::vector<unsigned int> indexList;

	vertexList.resize(24);
	indexList.resize(36);

	XMVECTOR worldUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	XMVECTOR worldBack = { 0.0f, 0.0f, 1.0f, 0.0f };

	// 순서 바꾸면 안됨(아래의 Cross계산 때문에)
	XMFLOAT3 dir[6] = {
		{0.0f, 0.0f, -1.0f},
		{0.0f, 0.0f, 1.0f},
		{-1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f}
	};

	for (int faceNum = 0; faceNum < 6; faceNum++)
	{
		XMVECTOR up = worldUp;
		if (faceNum == 4 || faceNum == 5)
		{
			up = worldBack;
		}

		XMVECTOR down = -up;
		XMVECTOR front = XMLoadFloat3(&dir[faceNum]);
		XMVECTOR left = XMVector3Cross(up, XMLoadFloat3(&dir[faceNum]));
		XMVECTOR right = -left;

		// 왼쪽 아래
		XMStoreFloat4(&vertexList[faceNum * 4 + 0].position, 0.5f * (front + left + down));
		vertexList[faceNum * 4 + 0].position.w = 1.0f;
		vertexList[faceNum * 4 + 0].texture = XMFLOAT2(0.0f, 1.0f);
		vertexList[faceNum * 4 + 0].normal = dir[faceNum];
		vertexList[faceNum * 4 + 0].boneIndices.x = 0xffffffffu;
		vertexList[faceNum * 4 + 0].boneIndices.y = 0xffffffffu;
		vertexList[faceNum * 4 + 0].boneIndices.z = 0xffffffffu;
		vertexList[faceNum * 4 + 0].boneIndices.w = 0xffffffffu;

		// 왼쪽 위
		XMStoreFloat4(&vertexList[faceNum * 4 + 1].position, 0.5f * (front + left + up));
		vertexList[faceNum * 4 + 1].position.w = 1.0f;
		vertexList[faceNum * 4 + 1].texture = XMFLOAT2(0.0f, 0.0f);
		vertexList[faceNum * 4 + 1].normal = dir[faceNum];
		vertexList[faceNum * 4 + 1].boneIndices.x = 0xffffffffu;
		vertexList[faceNum * 4 + 1].boneIndices.y = 0xffffffffu;
		vertexList[faceNum * 4 + 1].boneIndices.z = 0xffffffffu;
		vertexList[faceNum * 4 + 1].boneIndices.w = 0xffffffffu;

		// 오른쪽 위
		XMStoreFloat4(&vertexList[faceNum * 4 + 2].position, 0.5f * (front + right + up));
		vertexList[faceNum * 4 + 2].position.w = 1.0f;
		vertexList[faceNum * 4 + 2].texture = XMFLOAT2(1.0f, 0.0f);
		vertexList[faceNum * 4 + 2].normal = dir[faceNum];
		vertexList[faceNum * 4 + 2].boneIndices.x = 0xffffffffu;
		vertexList[faceNum * 4 + 2].boneIndices.y = 0xffffffffu;
		vertexList[faceNum * 4 + 2].boneIndices.z = 0xffffffffu;
		vertexList[faceNum * 4 + 2].boneIndices.w = 0xffffffffu;

		// 오른쪽 아래
		XMStoreFloat4(&vertexList[faceNum * 4 + 3].position, 0.5f * (front + right + down));
		vertexList[faceNum * 4 + 3].position.w = 1.0f;
		vertexList[faceNum * 4 + 3].texture = XMFLOAT2(1.0f, 1.0f);
		vertexList[faceNum * 4 + 3].normal = dir[faceNum];
		vertexList[faceNum * 4 + 3].boneIndices.x = 0xffffffffu;
		vertexList[faceNum * 4 + 3].boneIndices.y = 0xffffffffu;
		vertexList[faceNum * 4 + 3].boneIndices.z = 0xffffffffu;
		vertexList[faceNum * 4 + 3].boneIndices.w = 0xffffffffu;
	}

	// Load the index array with data.
	for (int faceNum = 0; faceNum < 6; faceNum++)
	{
		indexList[6 * faceNum + 0] = 4 * faceNum + 0;
		indexList[6 * faceNum + 1] = 4 * faceNum + 1;
		indexList[6 * faceNum + 2] = 4 * faceNum + 2;
		indexList[6 * faceNum + 3] = 4 * faceNum + 2;
		indexList[6 * faceNum + 4] = 4 * faceNum + 3;
		indexList[6 * faceNum + 5] = 4 * faceNum + 0;
	}

	cubeMesh = new ZeldaMesh(device, vertexList, indexList);

	return true;
}

TextureID ResourceManager::CreateTexture(const std::wstring& filePath)
{
	TextureID resourceID = IDGenerator::CreateID<ResourceType::Texture>();

	while (textureTable.count(resourceID) != 0 || resourceID == TextureID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Texture>();
	}

	textureTable[resourceID] = new ZeldaTexture(device, filePath);

	return resourceID;
}

bool ResourceManager::CreateDefaultShader()
{
	if (defaultShader != nullptr)
	{
		return false;
	}

	defaultShader = new ZeldaShader(device, L"CompiledShader\\VertexShader.cso", L"CompiledShader\\PixelShader.cso");
	return true;
}

ShaderID ResourceManager::CreateShader(const std::wstring& vsFilePath, const std::wstring& psFilePath)
{
	ShaderID resourceID = IDGenerator::CreateID<ResourceType::Shader>();

	while (shaderTable.count(resourceID) != 0 || resourceID == ShaderID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Shader>();
	}

	shaderTable[resourceID] = new ZeldaShader(device, vsFilePath, psFilePath);

	return resourceID;
}

CameraID ResourceManager::CreateCamera(unsigned int screenWidth, unsigned int screenHeight)
{
	ResourceID cameraID = IDGenerator::CreateID<ResourceType::Camera>();

	while (cameraTable.count(cameraID) != 0 || cameraID == CameraID::ID_NULL)
	{
		cameraID = IDGenerator::CreateID<ResourceType::Camera>();
	}

	cameraTable[cameraID] = new ZeldaCamera(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

	return cameraID;
}

ZeldaModel* ResourceManager::GetModel(ModelID key)
{
	auto iter = modelTable.find(key);

	if (iter != modelTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaMesh* ResourceManager::GetCubeMesh()
{
	return cubeMesh;
}

ZeldaMesh* ResourceManager::GetMesh(MeshID key)
{
	auto iter = meshTable.find(key);

	if (iter != meshTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaTexture* ResourceManager::GetTexture(TextureID key)
{
	auto iter = textureTable.find(key);

	if (iter != textureTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaShader* ResourceManager::GetDefaultShader()
{
	return defaultShader;
}

ZeldaShader* ResourceManager::GetShader(ShaderID key)
{
	auto iter = shaderTable.find(key);

	if (iter != shaderTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaCamera* ResourceManager::GetCamera(CameraID cameraID)
{
	auto iter = cameraTable.find(cameraID);

	if (iter != cameraTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

bool ResourceManager::CheckCameraID(CameraID cameraID)
{
	return cameraTable.count(cameraID) != 0;
}

bool ResourceManager::ReleaseCamera(CameraID cameraID)
{
	if (cameraTable.count(cameraID) != 0)
	{
		ZeldaCamera* targetCamera = cameraTable[cameraID];
		if (targetCamera != nullptr)
		{
			delete targetCamera;
		}
		cameraTable.erase(cameraID);

		return true;
	}

	return false;
}

ResourceManager& ResourceManager::GetInstance()
{
	static ResourceManager instance;
	return instance;
}

ResourceManager::ResourceManager() :
	device(nullptr),
	cubeMesh(nullptr),
	defaultShader(nullptr)
{

}

ResourceManager::~ResourceManager()
{
	if (cubeMesh != nullptr)
	{
		delete cubeMesh;
		cubeMesh = nullptr;
	}

	if (defaultShader != nullptr)
	{
		delete defaultShader;
		defaultShader = nullptr;
	}
}