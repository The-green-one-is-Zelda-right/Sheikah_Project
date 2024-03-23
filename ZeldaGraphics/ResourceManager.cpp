#include "ResourceManager.h"

#include "ZeldaGraphicsDefine.h"
#include "ZeldaModel.h"
#include "ZeldaMesh.h"
#include "ZeldaTexture.h"
#include "ZeldaShader.h"
#include "ZeldaCamera.h"
#include "ZeldaLight.h"
#include "IDGenerator.h"

#include "FBXData.h"
#include "FBXLoader.h"

using namespace DirectX;

void ResourceManager::Initialize(ID3D11Device* device)
{
	this->device = device;
}

void ResourceManager::Finalize()
{
	for (auto iter = cameraTable.begin(); iter != cameraTable.end(); iter++)
	{
		delete iter->second;
	}
	cameraTable.clear();

	for (auto iter = textureTable.begin(); iter != textureTable.end(); iter++)
	{
		delete iter->second;
	}
	textureTable.clear();

	for (auto iter = modelTable.begin(); iter != modelTable.end(); iter++)
	{
		delete iter->second;
	}
	modelTable.clear();

	for (auto iter = meshTable.begin(); iter != meshTable.end(); iter++)
	{
		delete iter->second;
	}
	meshTable.clear();

	for (auto iter = shaderTable.begin(); iter != shaderTable.end(); iter++)
	{
		delete iter->second;
	}
	shaderTable.clear();

	for (auto iter = lightTable.begin(); iter != lightTable.end(); iter++)
	{
		delete iter->second;
	}
	lightTable.clear();
}

MeshID ResourceManager::CreateCubeMesh()
{
	if (cubeID != MeshID::ID_NULL)
	{
		return MeshID::ID_NULL;
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

	cubeID = IDGenerator::CreateID<ResourceType::Mesh>();

	while (meshTable.count(cubeID) != 0 || cubeID == MeshID::ID_NULL)
	{
		cubeID = IDGenerator::CreateID<ResourceType::Mesh>();
	}

	meshTable[cubeID] = new ZeldaMesh(device, vertexList, indexList);

	return cubeID;
}

MeshID ResourceManager::CreateSquareMesh()
{
	if (squareID != MeshID::ID_NULL)
	{
		return MeshID::ID_NULL;
	}

	std::vector<VertexType> vertexList(4);
	std::vector<unsigned int> indexList(6);

	vertexList[0] = { { -0.5f, -0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f} };
	vertexList[1] = { { -0.5f, +0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f} };
	vertexList[2] = { { +0.5f, +0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f} };
	vertexList[3] = { { +0.5f, -0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f} };

	indexList[0] = 0;
	indexList[1] = 1;
	indexList[2] = 2;
	indexList[3] = 0;
	indexList[4] = 2;
	indexList[5] = 3;

	squareID = IDGenerator::CreateID<ResourceType::Mesh>();

	while (meshTable.count(squareID) != 0 || squareID == MeshID::ID_NULL)
	{
		squareID = IDGenerator::CreateID<ResourceType::Mesh>();
	}

	meshTable[squareID] = new ZeldaMesh(device, vertexList, indexList);

	return squareID;
}

MeshID ResourceManager::CreateCircleMesh()
{
	if (circleID != MeshID::ID_NULL)
	{
		return MeshID::ID_NULL;
	}

	const unsigned int outerVertexCount = 16;
	const float radius = 0.5f;
	const float unitAngle = DirectX::XM_2PI / static_cast<float>(outerVertexCount);

	std::vector<VertexType> vertexList(outerVertexCount + 1);
	std::vector<unsigned int> indexList;
	
	for (unsigned int i = 0; i < outerVertexCount; i++)
	{
		float theta = unitAngle * (i + 1);
		float tanUnitAngleDiv2 = DirectX::XMScalarSin(unitAngle / 2.0f) / DirectX::XMScalarCos(unitAngle / 2.0f);

		VertexType v;
		v.position.x = radius * DirectX::XMScalarCos(theta) + radius * tanUnitAngleDiv2 * DirectX::XMScalarCos(theta - DirectX::XM_PIDIV2);
		v.position.y = radius * DirectX::XMScalarSin(theta) + radius * tanUnitAngleDiv2 * DirectX::XMScalarSin(theta - DirectX::XM_PIDIV2);
		v.position.z = 0.0f;
		v.position.w = 0.0f;
		v.normal = { 0.0f, 0.0f, -1.0f };
		v.texture.x = +(v.position.x + 1.0f) / 2.0f;
		v.texture.y = -(v.position.y + 1.0f) / 2.0f;

		vertexList[i] = v;
	}

	for (unsigned int i = 0; i < outerVertexCount; i++)
	{
		unsigned int j = (i + 1) % outerVertexCount;

		indexList.push_back(j);
		indexList.push_back(i);
		indexList.push_back(outerVertexCount);
	}

	circleID = IDGenerator::CreateID<ResourceType::Mesh>();

	while (meshTable.count(circleID) != 0 || circleID == MeshID::ID_NULL)
	{
		circleID = IDGenerator::CreateID<ResourceType::Mesh>();
	}

	meshTable[circleID] = new ZeldaMesh(device, vertexList, indexList);

	return circleID;
}

MeshID ResourceManager::CreateSphereMesh()
{
	if (sphereID != MeshID::ID_NULL)
	{
		return MeshID::ID_NULL;
	}

	const float radius = 0.5f; // 구의 반지름
	const unsigned int stackCount = 20; // 가로 분할
	const unsigned int sliceCount = 20; // 세로 분할

	std::vector<VertexType> vertexList;
	std::vector<unsigned int> indexList(36);

	// 북극
	// 일단 임시로 tangent는 (1, 0, 0)을 넣었다, 제대로 하려면 북극 버텍스를 면의 갯수만큼 여러개 만들고 면마다 다르게 설정해 줘야 한다.
	vertexList.push_back({ { 0.0f, radius, 0.0f, 0.0f }, { 0.0f , 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.5f, 0.0f } });

	float stackAngle = DirectX::XM_PI / static_cast<float>(stackCount);
	float sliceAngle = DirectX::XM_2PI / static_cast<float>(sliceCount);

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
	for (size_t y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리에 위치한 정점
		for (size_t x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			VertexType v;
			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);
			v.position.w = 0.0f;
			DirectX::XMStoreFloat3(&v.normal, DirectX::XMVector3Normalize({ v.position.x, v.position.y, v.position.z }));
			v.texture = { deltaU * x, deltaV * y };

			DirectX::XMStoreFloat3(&v.tangent, DirectX::XMVector3Normalize({ -radius * sinf(phi) * sinf(theta), 0.0f, radius * sinf(phi) * cosf(theta) }));

			vertexList.push_back(v);
		}
	}

	// 남극
	// 일단 임시로 tangent는 (1, 0, 0)을 넣었다, 제대로 하려면 남극 버텍스를 면의 갯수만큼 여러개 만들고 면마다 다르게 설정해 줘야 한다.
	vertexList.push_back({ { 0.0f, -radius, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.5f, 1.0f } });

	// 북극 인덱스
	for (unsigned int i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
			//  [i+1]-[i+2]
		indexList.push_back(0);
		indexList.push_back(i + 2);
		indexList.push_back(i + 1);
	}

	// 몸통 인덱스
	unsigned int ringVertexCount = sliceCount + 1;
	for (unsigned int y = 0; y < stackCount - 2; ++y)
	{
		for (unsigned int x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			indexList.push_back(1 + (y)*ringVertexCount + (x));
			indexList.push_back(1 + (y)*ringVertexCount + (x + 1));
			indexList.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			indexList.push_back(1 + (y + 1) * ringVertexCount + (x));
			indexList.push_back(1 + (y)*ringVertexCount + (x + 1));
			indexList.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// 남극 인덱스
	size_t bottomIndex = vertexList.size() - 1;
	size_t lastRingStartIndex = bottomIndex - ringVertexCount;
	for (size_t i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		indexList.push_back(static_cast<int>(bottomIndex));
		indexList.push_back(static_cast<int>(lastRingStartIndex + i));
		indexList.push_back(static_cast<int>(lastRingStartIndex + i + 1));
	}

	sphereID = IDGenerator::CreateID<ResourceType::Mesh>();

	while (meshTable.count(sphereID) != 0 || sphereID == MeshID::ID_NULL)
	{
		sphereID = IDGenerator::CreateID<ResourceType::Mesh>();
	}

	meshTable[sphereID] = new ZeldaMesh(device, vertexList, indexList);

	return sphereID;
}

MeshID ResourceManager::CreateCapsuleMesh()
{
	if (capsuleID != MeshID::ID_NULL)
	{
		return MeshID::ID_NULL;
	}

	std::vector<VertexType> vertexList;
	std::vector<unsigned int> indexList;

	const float radius = 0.5f; // 캡슐의 반지름
	const float height = 1.0f; // 캡슐의 높이
	const int stackCount = 5; // 수평 분할
	const int sliceCount = 20; // 수직 분할

	// 상단 반구 정점
	vertexList.push_back({ { 0.0f, radius + height * 0.5f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } });

	for (int i = 1; i <= stackCount; i++)
	{
		// 윗방향 벡터와의 각도
		float upTheta = XM_PI * 0.5f * (i / static_cast<float>(stackCount));

		float xzsize = radius * sinf(upTheta);
		float ysize = radius * cosf(upTheta);

		for (int j = 0; j < sliceCount; j++)
		{
			float zTheta = XM_PI * 2.0f * (j / static_cast<float>(sliceCount));

			float x = xzsize * sinf(zTheta);
			float y = ysize + height * 0.5f;
			float z = xzsize * cosf(zTheta);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize({ x, y - height * 0.5f, z }));

			vertexList.push_back({ { x, y, z, 0.0f }, normal });
		}
	}

	size_t middleIdx = vertexList.size();

	// 하단 반구 정점
	for (int i = stackCount; i >= 1; i--)
	{
		// 윗방향 벡터와의 각도
		float upTheta = XM_PI * 0.5f * (i / static_cast<float>(stackCount));

		float xzsize = radius * sinf(upTheta);
		float ysize = radius * cosf(upTheta);

		for (int j = 0; j < sliceCount; j++)
		{
			float zTheta = XM_PI * 2.0f * (j / static_cast<float>(sliceCount));

			float x = xzsize * sinf(zTheta);
			float y = ysize + height * 0.5f;
			float z = xzsize * cosf(zTheta);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize({ x, -(y - height * 0.5f), z }));

			vertexList.push_back({ { x, -y, z, 0.0f }, normal });
		}
	}

	vertexList.push_back({ { 0.0f, -(radius + height * 0.5f), 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } });

	// 상단 반구 인덱스
	for (int i = 0; i < sliceCount; i++) {
		int a = 0;
		int b = 1 + i;
		int c = 1 + ((i + 1) % sliceCount);

		indexList.push_back(a);
		indexList.push_back(b);
		indexList.push_back(c);
	}

	for (int i = 1; i < stackCount; i++) {
		for (int j = 0; j < sliceCount; j++) {
			int a = 1 + (i - 1) * sliceCount + j;
			int b = 1 + (i - 1) * sliceCount + ((j + 1) % sliceCount);
			int c = 1 + i * sliceCount + j;
			int d = 1 + i * sliceCount + ((j + 1) % sliceCount);

			indexList.push_back(a);
			indexList.push_back(c);
			indexList.push_back(d);

			indexList.push_back(a);
			indexList.push_back(d);
			indexList.push_back(b);
		}
	}

	// 실린더 부분 인덱스
	for (int i = 0; i < sliceCount; i++)
	{
		int a = middleIdx - sliceCount + i;
		int b = middleIdx - sliceCount + ((i + 1) % sliceCount);
		int c = middleIdx + i;
		int d = middleIdx + ((i + 1) % sliceCount);

		indexList.push_back(a);
		indexList.push_back(c);
		indexList.push_back(d);

		indexList.push_back(a);
		indexList.push_back(d);
		indexList.push_back(b);
	}

	// 하단 반구 인덱스
	for (int i = 1; i < stackCount; i++) {
		for (int j = 0; j < sliceCount; j++) {
			int a = middleIdx + (i - 1) * sliceCount + j;
			int b = middleIdx + (i - 1) * sliceCount + ((j + 1) % sliceCount);
			int c = middleIdx + i * sliceCount + j;
			int d = middleIdx + i * sliceCount + ((j + 1) % sliceCount);

			indexList.push_back(a);
			indexList.push_back(c);
			indexList.push_back(d);

			indexList.push_back(a);
			indexList.push_back(d);
			indexList.push_back(b);
		}
	}

	for (int i = 0; i < sliceCount; i++) {
		int a = vertexList.size() - 1;
		int b = vertexList.size() - 1 - sliceCount + i;
		int c = vertexList.size() - 1 - sliceCount + ((i + 1) % sliceCount);

		indexList.push_back(b);
		indexList.push_back(a);
		indexList.push_back(c);
	}

	capsuleID = IDGenerator::CreateID<ResourceType::Mesh>();

	while (meshTable.count(capsuleID) != 0 || capsuleID == MeshID::ID_NULL)
	{
		capsuleID = IDGenerator::CreateID<ResourceType::Mesh>();
	}

	meshTable[capsuleID] = new ZeldaMesh(device, vertexList, indexList);

	return capsuleID;
}

MeshID ResourceManager::CreateCylinderMesh()
{
	if (cylinderID != MeshID::ID_NULL)
	{
		return MeshID::ID_NULL;
	}

	const float radius = 0.5f; // 실린더의 반지름
	const float height = 1.0f; // 실린더의 높이
	const int sliceCount = 20; // 수직 분할

	std::vector<VertexType> vertexList;
	std::vector<unsigned int> indexList;

	// 정상 정점
	vertexList.push_back({ { 0.0f, height * 0.5f, 0.0f, 0.0f }, { 0.0f , 1.0f, 0.0f } });

	// 바닥 정점
	vertexList.push_back({ { 0.0f, -height * 0.5f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } });

	// 윗면
	for (int i = 0; i < sliceCount; ++i)
	{
		float zTheta = DirectX::XM_2PI * (i / static_cast<float>(sliceCount));

		float x = radius * sinf(zTheta);
		float y = height * 0.5f;
		float z = radius * cosf(zTheta);

		vertexList.push_back({ { x, y, z, 0.0f }, { 0.0f, 1.0f, 0.0f } });
	}

	// 밑면
	for (int i = 0; i < sliceCount; ++i)
	{
		float zTheta = DirectX::XM_2PI * (i / static_cast<float>(sliceCount));

		float x = radius * sinf(zTheta);
		float y = height * 0.5f;
		float z = radius * cosf(zTheta);

		vertexList.push_back({ { x, -y, z, 0.0f }, { 0.0f, -1.0f, 0.0f } });
	}

	// 옆면 위
	for (int i = 0; i < sliceCount; ++i)
	{
		float zTheta = DirectX::XM_2PI * (i / static_cast<float>(sliceCount));

		float x = radius * sinf(zTheta);
		float y = height * 0.5f;
		float z = radius * cosf(zTheta);
		DirectX::XMFLOAT3 normal;
		DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize({ x, 0.0f, z }));

		vertexList.push_back({ { x, y, z, 0.0f }, normal });
	}

	// 옆면 아래
	for (int i = 0; i < sliceCount; ++i)
	{
		float zTheta = DirectX::XM_2PI * (i / static_cast<float>(sliceCount));

		float x = radius * sinf(zTheta);
		float y = height * 0.5f;
		float z = radius * cosf(zTheta);
		DirectX::XMFLOAT3 normal;
		DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize({ x, 0.0f, z }));

		vertexList.push_back({ { x, -y, z, 0.0f }, normal });
	}


	// 윗면 인덱스
	for (int i = 0; i < sliceCount; ++i)
	{
		int a = 0;
		int b = 2 + i;
		int c = 2 + ((i + 1) % sliceCount);

		indexList.push_back(a);
		indexList.push_back(b);
		indexList.push_back(c);
	}

	// 옆면 인덱스
	for (int i = 0; i < sliceCount; ++i)
	{
		int a = 2 + (2 * sliceCount) + i;
		int b = 2 + (2 * sliceCount) + ((i + 1) % sliceCount);
		int c = 2 + (3 * sliceCount) + i;
		int d = 2 + (3 * sliceCount) + ((i + 1) % sliceCount);

		indexList.push_back(a);
		indexList.push_back(c);
		indexList.push_back(d);

		indexList.push_back(a);
		indexList.push_back(d);
		indexList.push_back(b);
	}

	// 밑면 인덱스
	for (int i = 0; i < sliceCount; ++i)
	{
		int a = 1;
		int b = 2 + sliceCount + i;
		int c = 2 + sliceCount + ((i + 1) % sliceCount);

		indexList.push_back(b);
		indexList.push_back(a);
		indexList.push_back(c);
	}

	cylinderID = IDGenerator::CreateID<ResourceType::Mesh>();

	while (meshTable.count(cylinderID) != 0 || cylinderID == MeshID::ID_NULL)
	{
		cylinderID = IDGenerator::CreateID<ResourceType::Mesh>();
	}

	meshTable[cylinderID] = new ZeldaMesh(device, vertexList, indexList);

	return cylinderID;
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

LightID ResourceManager::CreateDirectionalLight()
{
	LightID resourceID = IDGenerator::CreateID<ResourceType::Light>();

	while (lightTable.count(resourceID) != 0 || resourceID == LightID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Light>();
	}

	lightTable[resourceID] = new ZeldaLight(LightType::Directional);

	return resourceID;
}

LightID ResourceManager::CreatePointLight()
{
	LightID resourceID = IDGenerator::CreateID<ResourceType::Light>();

	while (lightTable.count(resourceID) != 0 || resourceID == LightID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Light>();
	}

	lightTable[resourceID] = new ZeldaLight(LightType::Point);

	return resourceID;
}

LightID ResourceManager::CreateSpotLight()
{
	LightID resourceID = IDGenerator::CreateID<ResourceType::Light>();

	while (lightTable.count(resourceID) != 0 || resourceID == LightID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Light>();
	}

	lightTable[resourceID] = new ZeldaLight(LightType::Spot);

	return resourceID;
}

void ResourceManager::ReleaseCubeMesh()
{
	ReleaseMesh(cubeID);
	cubeID = MeshID::ID_NULL;
}

void ResourceManager::ReleaseSquareMesh()
{
	ReleaseMesh(squareID);
	squareID = MeshID::ID_NULL;
}

void ResourceManager::ReleaseCircleMesh()
{
	ReleaseMesh(circleID);
	circleID = MeshID::ID_NULL;
}

void ResourceManager::ReleaseSphereMesh()
{
	ReleaseMesh(sphereID);
	sphereID = MeshID::ID_NULL;
}

void ResourceManager::ReleaseCapsuleMesh()
{
	ReleaseMesh(capsuleID);
	capsuleID = MeshID::ID_NULL;
}

void ResourceManager::ReleaseCylinderMesh()
{
	ReleaseMesh(cylinderID);
	cylinderID = MeshID::ID_NULL;
}

void ResourceManager::ReleaseCamera(CameraID cameraID)
{
	auto& table = cameraTable;
	auto& id = cameraID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

void ResourceManager::ReleaseTexture(TextureID textureID)
{
	auto& table = textureTable;
	auto& id = textureID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

void ResourceManager::ReleaseModel(ModelID modelID)
{
	auto& table = modelTable;
	auto& id = modelID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

void ResourceManager::ReleaseMesh(MeshID meshID)
{
	auto& table = meshTable;
	auto& id = meshID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

void ResourceManager::ReleaseShader(ShaderID shaderID)
{
	auto& table = shaderTable;
	auto& id = shaderID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

void ResourceManager::ReleaseLight(LightID lightID)
{
	auto& table = lightTable;
	auto& id = lightID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

MeshID ResourceManager::GetCubeID()
{
	return cubeID;
}

MeshID ResourceManager::GetSquareID()
{
	return squareID;
}

MeshID ResourceManager::GetCircleID()
{
	return circleID;
}

MeshID ResourceManager::GetSphereID()
{
	return sphereID;
}

MeshID ResourceManager::GetCapsuleID()
{
	return capsuleID;
}

MeshID ResourceManager::GetCylinderID()
{
	return cylinderID;
}

ZeldaMesh* ResourceManager::GetCubeMesh()
{
	return meshTable[cubeID];
}

ZeldaMesh* ResourceManager::GetSquareMesh()
{
	return meshTable[squareID];
}

ZeldaMesh* ResourceManager::GetCircleMesh()
{
	return meshTable[circleID];
}

ZeldaMesh* ResourceManager::GetSphereMesh()
{
	return meshTable[sphereID];
}

ZeldaMesh* ResourceManager::GetCapsuleMesh()
{
	return meshTable[capsuleID];
}

ZeldaMesh* ResourceManager::GetCylinderMesh()
{
	return meshTable[cylinderID];
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

ZeldaTexture* ResourceManager::GetTexture(TextureID key)
{
	auto iter = textureTable.find(key);

	if (iter != textureTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
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

ZeldaMesh* ResourceManager::GetMesh(MeshID key)
{
	auto iter = meshTable.find(key);

	if (iter != meshTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
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

ZeldaLight* ResourceManager::GetLight(LightID key)
{
	auto iter = lightTable.find(key);

	if (iter != lightTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

bool ResourceManager::CheckCameraID(CameraID cameraID)
{
	return cameraTable.count(cameraID) != 0;
}

ResourceManager& ResourceManager::GetInstance()
{
	static ResourceManager instance;
	return instance;
}

ResourceManager::ResourceManager() :
	device(nullptr),
	cubeID(MeshID::ID_NULL),
	squareID(MeshID::ID_NULL),
	circleID(MeshID::ID_NULL),
	sphereID(MeshID::ID_NULL),
	capsuleID(MeshID::ID_NULL),
	cylinderID(MeshID::ID_NULL)
{

}

ResourceManager::~ResourceManager()
{
	
}