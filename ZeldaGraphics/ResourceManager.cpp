#include "ResourceManager.h"

#include "ZeldaGraphicsDefine.h"
#include "ZeldaModel.h"
#include "ZeldaMesh.h"
#include "ZeldaTexture.h"
#include "ZeldaShader.h"
#include "ZeldaCamera.h"
#include "IDGenerator.h"
#include "ModelingData.h"

using namespace DirectX;

void ResourceManager::Initialize(ID3D11Device* device)
{
	this->device = device;
}

ModelID ResourceManager::CreateModelFromModelingFile(const std::wstring& filePath)
{
	ZNode* root;
	std::vector<ZMesh*> meshes;
	std::vector<ZMaterial*> materials;

	std::vector<ZeldaMesh*> zeldaMeshes;
	std::vector<ZeldaTexture*> zeldaTextures;


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
		aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes |
		aiProcess_SortByPType | aiProcess_LimitBoneWeights;
	//*/

	// aiProcess_PreTransformVertices

	const aiScene* scene = importer.ReadFile(multibyteFilePath, DEFAULT_LOAD_FLAG);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string errorMessage = importer.GetErrorString();
		std::wstring wErrorMessage;
		wErrorMessage.assign(errorMessage.begin(), errorMessage.end());

		MessageBox(0, wErrorMessage.c_str(), L"ZeldaModel Error", MB_OK);

		return ModelID::ID_NULL;
	}

	if (scene->HasMaterials())
	{
		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* assimpMaterial = scene->mMaterials[i];

			ZMaterial* materialData = new ZMaterial();

			if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				materialData->hasTexture = true;

				aiString texturePath;
				if (assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
				{
					std::string str = texturePath.C_Str();
					std::wstring wstr;
					wstr.assign(str.begin(), str.end());
					wstr = fileDirectory + L"\\" + wstr;
					materialData->textureFilePath = wstr;
				}
			}
			else
			{
				materialData->hasTexture = false;
			}

			materials.push_back(materialData);
		}
	}

	if (scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* assimpMesh = scene->mMeshes[i];

			ZMesh* meshData = new ZMesh();
			meshData->hasPosition = assimpMesh->HasPositions();
			meshData->hasUV = assimpMesh->HasTextureCoords(0);
			meshData->hasNormal = assimpMesh->HasNormals();

			for (int j = 0; j < assimpMesh->mNumVertices; j++)
			{
				ZVertex vtx;
				if (assimpMesh->HasPositions())
				{
					vtx.posX = assimpMesh->mVertices[j].x;
					vtx.posY = assimpMesh->mVertices[j].y;
					vtx.posZ = assimpMesh->mVertices[j].z;
				}

				if (assimpMesh->HasTextureCoords(0))
				{
					vtx.u = assimpMesh->mTextureCoords[0][j].x;
					vtx.v = assimpMesh->mTextureCoords[0][j].y;
				}

				if (assimpMesh->HasNormals())
				{
					vtx.norX = assimpMesh->mNormals[j].x;
					vtx.norY = -assimpMesh->mNormals[j].z;
					vtx.norZ = assimpMesh->mNormals[j].y;
				}

				meshData->vertices.push_back(vtx);
			}

			for (int j = 0; j < assimpMesh->mNumFaces; j++)
			{
				aiFace face = assimpMesh->mFaces[j];
				for (int k = 0; k < face.mNumIndices; k++)
				{
					meshData->indices.push_back(face.mIndices[k]);
				}
			}

			auto bufferPair = CreateBufferFromZMesh(meshData);
			unsigned int materialNum = assimpMesh->mMaterialIndex;

			meshData->material = materials[materialNum];

			meshes.push_back(meshData);

			zeldaMeshes.push_back(new ZeldaMesh(bufferPair.first, bufferPair.second, meshData->vertices.size(), meshData->indices.size()));
			if (materials[materialNum]->hasTexture)
			{
				zeldaTextures.push_back(new ZeldaTexture(device, materials[materialNum]->textureFilePath));
			}
			else
			{
				zeldaTextures.push_back(nullptr);
			}
		}
	}

	root = new ZNode();
	CopyNodeData(root, scene->mRootNode, scene, meshes, zeldaMeshes, zeldaTextures);

	importer.FreeScene();

	ModelID resourceID = IDGenerator::CreateID<ResourceType::Model>();

	while (modelTable.count(resourceID) != 0 || resourceID == ModelID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Model>();
	}

	modelTable[resourceID] = new ZeldaModel(root, meshes, materials, zeldaMeshes, zeldaTextures);

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

	// ���� �ٲٸ� �ȵ�(�Ʒ��� Cross��� ������)
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

		// ���� �Ʒ�
		XMStoreFloat4(&vertexList[faceNum * 4 + 0].position, 0.5f * (front + left + down));
		vertexList[faceNum * 4 + 0].position.w = 1.0f;
		vertexList[faceNum * 4 + 0].texture = XMFLOAT2(0.0f, 1.0f);
		vertexList[faceNum * 4 + 0].normal = dir[faceNum];

		// ���� ��
		XMStoreFloat4(&vertexList[faceNum * 4 + 1].position, 0.5f * (front + left + up));
		vertexList[faceNum * 4 + 1].position.w = 1.0f;
		vertexList[faceNum * 4 + 1].texture = XMFLOAT2(0.0f, 0.0f);
		vertexList[faceNum * 4 + 1].normal = dir[faceNum];

		// ������ ��
		XMStoreFloat4(&vertexList[faceNum * 4 + 2].position, 0.5f * (front + right + up));
		vertexList[faceNum * 4 + 2].position.w = 1.0f;
		vertexList[faceNum * 4 + 2].texture = XMFLOAT2(1.0f, 0.0f);
		vertexList[faceNum * 4 + 2].normal = dir[faceNum];

		// ������ �Ʒ�
		XMStoreFloat4(&vertexList[faceNum * 4 + 3].position, 0.5f * (front + right + down));
		vertexList[faceNum * 4 + 3].position.w = 1.0f;
		vertexList[faceNum * 4 + 3].texture = XMFLOAT2(1.0f, 1.0f);
		vertexList[faceNum * 4 + 3].normal = dir[faceNum];
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

void ResourceManager::CopyNodeData(ZNode* node, const aiNode* assimpNode, const aiScene* assimpScene, const std::vector<ZMesh*>& meshes, const std::vector<ZeldaMesh*>& zeldaMeshes, const std::vector<ZeldaTexture*>& zeldaTextures)
{
	assert(node->children.size() == 0);

	for (int i = 0; i < assimpNode->mNumMeshes; i++)
	{
		assert(meshes.size() > assimpNode->mMeshes[i]);
		node->meshes.push_back(meshes[assimpNode->mMeshes[i]]);
		node->zeldaMeshes.push_back(zeldaMeshes[assimpNode->mMeshes[i]]);
		node->zeldaTextures.push_back(zeldaTextures[assimpNode->mMeshes[i]]);

		node->transformMatrix.r[0].m128_f32[0] = assimpNode->mTransformation.a1;
		node->transformMatrix.r[0].m128_f32[1] = assimpNode->mTransformation.b1;
		node->transformMatrix.r[0].m128_f32[2] = assimpNode->mTransformation.c1;
		node->transformMatrix.r[0].m128_f32[3] = assimpNode->mTransformation.d1;

		node->transformMatrix.r[1].m128_f32[0] = assimpNode->mTransformation.a2;
		node->transformMatrix.r[1].m128_f32[1] = assimpNode->mTransformation.b2;
		node->transformMatrix.r[1].m128_f32[2] = assimpNode->mTransformation.c2;
		node->transformMatrix.r[1].m128_f32[3] = assimpNode->mTransformation.d2;

		node->transformMatrix.r[2].m128_f32[0] = assimpNode->mTransformation.a3;
		node->transformMatrix.r[2].m128_f32[1] = assimpNode->mTransformation.b3;
		node->transformMatrix.r[2].m128_f32[2] = assimpNode->mTransformation.c3;
		node->transformMatrix.r[2].m128_f32[3] = assimpNode->mTransformation.d3;

		node->transformMatrix.r[3].m128_f32[0] = assimpNode->mTransformation.a4;
		node->transformMatrix.r[3].m128_f32[1] = assimpNode->mTransformation.b4;
		node->transformMatrix.r[3].m128_f32[2] = assimpNode->mTransformation.c4;
		node->transformMatrix.r[3].m128_f32[3] = assimpNode->mTransformation.d4;
	}

	for (int i = 0; i < assimpNode->mNumChildren; i++)
	{
		ZNode* child = new ZNode();
		aiNode* childAssimpNode = assimpNode->mChildren[i];

		child->parent = node;
		node->children.push_back(child);

		CopyNodeData(child, childAssimpNode, assimpScene, meshes, zeldaMeshes, zeldaTextures);
	}
}

std::pair<ID3D11Buffer*, ID3D11Buffer*> ResourceManager::CreateBufferFromZMesh(ZMesh* mesh)
{
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	int vertexCount = 0;
	int indexCount = 0;

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	vertexCount = mesh->vertices.size();
	// Set the number of indices in the index array.
	indexCount = mesh->indices.size();
	// Create the vertex array.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		assert(0);
	}
	// Create the index array.
	indices = new unsigned long[indexCount];
	if (!indices)
	{
		assert(0);
	}

	for (int i = 0; i < vertexCount; i++)
	{
		XMStoreFloat4(&vertices[i].position, { mesh->vertices[i].posX, mesh->vertices[i].posY, mesh->vertices[i].posZ });
		vertices[i].texture = XMFLOAT2(mesh->vertices[i].u, mesh->vertices[i].v);
		vertices[i].normal = XMFLOAT3(mesh->vertices[i].norX, mesh->vertices[i].norY, mesh->vertices[i].norZ);
	}

	for (int i = 0; i < indexCount; i++)
	{
		indices[i] = mesh->indices[i];
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		assert(0);
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		assert(0);
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return std::make_pair(vertexBuffer, indexBuffer);
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