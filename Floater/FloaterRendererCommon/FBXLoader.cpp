#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/ConvString.h"
#include "FBXLoader.h"
#include <iostream>


#ifdef _DEBUG
#pragma comment(lib, "../External/lib/x64/debug/libfbxsdk.lib")
#else
#pragma comment(lib, "../External/lib/x64/release/libfbxsdk.lib")
#endif


flt::FBXLoader::FBXLoader() :
	_pManager(nullptr),
	_pIOSettings(nullptr)
{
	_pManager = FbxManager::Create();
	_pIOSettings = FbxIOSettings::Create(_pManager, IOSROOT);
	_pManager->SetIOSettings(_pIOSettings);
}

flt::FBXLoader::~FBXLoader()
{
}

void flt::FBXLoader::Load(const std::wstring& filePath)
{
	FbxImporter* importer = FbxImporter::Create(_pManager, "");

	std::string filePathStr = flt::convToString(filePath);
	if (!importer->Initialize(filePathStr.c_str(), -1, _pManager->GetIOSettings()))
	{
		auto var = importer->GetStatus().GetErrorString();
		ASSERT(false, "importer Init Fail");
	}

	FbxScene* scene = FbxScene::Create(_pManager, "");
	importer->Import(scene);
	importer->Destroy();

	FbxAxisSystem dx11Axis(FbxAxisSystem::eDirectX);
	dx11Axis.ConvertScene(scene);

	auto dxCoordSystem = CreateFBXCoodSystem(scene);
	FbxAxisSystem{ FbxAxisSystem::eMax }.ConvertScene(scene);
	//dxCoordSystem = CreateFBXCoodSystem(scene);

	FbxNode* rootNode = scene->GetRootNode();

	PrintNodeRecursive(rootNode, 0);

	if (rootNode)
	{
		int childCount = rootNode->GetChildCount();
		for (int i = 0; i < childCount; ++i)
		{
			FbxNode* childNode = rootNode->GetChild(i);
			if (childNode->GetNodeAttribute() == nullptr)
				continue;

			FbxNodeAttribute::EType attributeType = childNode->GetNodeAttribute()->GetAttributeType();
			if (attributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxTransform transform = childNode->GetTransform();
			FbxDouble3 translation = childNode->LclTranslation;
			FbxDouble3 rotation = childNode->LclRotation;
			FbxDouble3 scaling = childNode->LclScaling;

			FbxMesh* mesh = childNode->GetMesh();
			if (mesh == nullptr)
				continue;

			FbxVector4* vertices = mesh->GetControlPoints();
			for (int j = 0; j < mesh->GetControlPointsCount(); ++j)
			{
				FbxVector4 vertex = vertices[j];
				std::cout << vertex.mData[0] << " " << vertex.mData[1] << " " << vertex.mData[2] << std::endl;
			}

			for (int j = 0; j < mesh->GetPolygonCount(); ++j)
			{
				int polygonSize = mesh->GetPolygonSize(j);
				for (int k = 0; k < polygonSize; ++k)
				{
					int controlPointIndex = mesh->GetPolygonVertex(j, k);
					std::cout << controlPointIndex << std::endl;
				}
			}
		}
	}


}

flt::CoordSystem flt::FBXLoader::CreateFBXCoodSystem(FbxScene* pScene)
{
	int upSign = 0;
	auto upVector = pScene->GetGlobalSettings().GetAxisSystem().GetUpVector(upSign);

	// up벡터가 x 라면 나머지 두 축은 y, z이기 때문에 ParityEven이면 y, ParityOdd이면 z
	// up벡터가 y 라면 나머지 두 축은 x, z이기 때문에 ParityEven이면 x, ParityOdd이면 z
	// up벡터가 z 라면 나머지 두 축은 x, y이기 때문에 ParityEven이면 x, ParityOdd이면 y
	int frontSign = 0;
	auto frontVector = pScene->GetGlobalSettings().GetAxisSystem().GetFrontVector(frontSign);

	auto coordSystem = pScene->GetGlobalSettings().GetAxisSystem().GetCoorSystem();
	bool rightHanded = false;

	if (coordSystem == fbxsdk::FbxAxisSystem::eRightHanded)
	{
		rightHanded = true;
	}
	else
	{
		rightHanded = false;
	}

	CoordSystem::Axis upAxis = CoordSystem::Axis::X;
	CoordSystem::Axis frontAxis = CoordSystem::Axis::Y;

	switch (upVector)
	{
		case FbxAxisSystem::eXAxis:
		{
			upAxis = CoordSystem::Axis::X;
			if (frontVector == FbxAxisSystem::eParityEven)
				frontAxis = CoordSystem::Axis::Y;
			else
				frontAxis = CoordSystem::Axis::Z;
		}

		break;
		case FbxAxisSystem::eYAxis:
		{
			upAxis = CoordSystem::Axis::Y;
			if (frontVector == FbxAxisSystem::eParityEven)
				frontAxis = CoordSystem::Axis::X;
			else
				frontAxis = CoordSystem::Axis::Z;
		}
		break;
		case FbxAxisSystem::eZAxis:
		{
			upAxis = CoordSystem::Axis::Z;
			if (frontVector == FbxAxisSystem::eParityEven)
				frontAxis = CoordSystem::Axis::X;
			else
				frontAxis = CoordSystem::Axis::Y;
		}
		break;
	}


	return CoordSystem{ upSign, frontSign, upAxis, frontAxis, rightHanded };
}

void flt::FBXLoader::PrintNodeRecursive(FbxNode* pNode, int depth)
{
	if (pNode == nullptr)
		return;
	for (int i = 0; i < depth; ++i)
	{
		std::cout << "-" << depth << "-";
	}
	std::cout << pNode->GetName() << std::endl;

	for (int i = 0; i < pNode->GetChildCount(); ++i)
		PrintNodeRecursive(pNode->GetChild(i), depth + 1);
}
