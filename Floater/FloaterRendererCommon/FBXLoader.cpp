#include "FBXLoader.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/ConvString.h"
#include "./include/RawNode.h"
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
	fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(_pManager, "");

	std::string filePathStr = flt::convToString(filePath);
	if (!importer->Initialize(filePathStr.c_str(), -1, _pManager->GetIOSettings()))
	{
		auto var = importer->GetStatus().GetErrorString();
		ASSERT(false, "importer Init Fail");
	}
	std::cout << "-------------------------------------" << std::endl;
	std::wcout << filePath << std::endl;
	int major;
	int minor;
	int revision;
	importer->GetFileVersion(major, minor, revision);
	std::cout << major << "." << minor << "." << revision << std::endl;

	fbxsdk::FbxScene* scene = fbxsdk::FbxScene::Create(_pManager, "");
	importer->Import(scene);
	importer->Destroy();

	fbxsdk::FbxAxisSystem dx11Axis(FbxAxisSystem::eDirectX);
	dx11Axis.ConvertScene(scene);

	auto fileCoord = CreateFBXCoodSystem(scene);
	fileCoord.PrintfCoord();

	auto dxCoord = CoordSystem::DX11();

	//FbxAxisSystem{ FbxAxisSystem::eMax }.ConvertScene(scene);
	//dxCoordSystem = CreateFBXCoodSystem(scene);

	FbxNode* rootNode = scene->GetRootNode();

	//PrintNodeRecursive(rootNode, 0);

	if (rootNode)
	{
		int childCount = rootNode->GetChildCount();
		for (int i = 0; i < childCount; ++i)
		{
			fbxsdk::FbxNode* childNode = rootNode->GetChild(i);
			if (childNode->GetNodeAttribute() == nullptr)
				continue;

			fbxsdk::FbxNodeAttribute::EType attributeType = childNode->GetNodeAttribute()->GetAttributeType();
			if (attributeType != fbxsdk::FbxNodeAttribute::eMesh)
				continue;

			std::cout << "name : " << childNode->GetName() << std::endl;

			PrintTransform(childNode);

			fbxsdk::FbxMesh* mesh = childNode->GetMesh();
			if (mesh == nullptr)
				continue;

			PrintNodeVertex(childNode);
			//FbxVector4* vertices = mesh->GetControlPoints();
			//for (int j = 0; j < mesh->GetControlPointsCount(); ++j)
			//{
			//	FbxVector4 vertex = vertices[j];
			//	//std::cout << vertex.mData[0] << " " << vertex.mData[1] << " " << vertex.mData[2] << std::endl;
			//}

			//for (int j = 0; j < mesh->GetPolygonCount(); ++j)
			//{
			//	int polygonSize = mesh->GetPolygonSize(j);
			//	for (int k = 0; k < polygonSize; ++k)
			//	{
			//		int controlPointIndex = mesh->GetPolygonVertex(j, k);
			//		//std::cout << controlPointIndex << std::endl;
			//	}
			//}
		}
	}

	scene->Destroy();
	return;
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
		case fbxsdk::FbxAxisSystem::eXAxis:
		{
			upAxis = CoordSystem::Axis::X;
			if (frontVector == fbxsdk::FbxAxisSystem::eParityEven)
				frontAxis = CoordSystem::Axis::Y;
			else
				frontAxis = CoordSystem::Axis::Z;
		}

		break;
		case fbxsdk::FbxAxisSystem::eYAxis:
		{
			upAxis = CoordSystem::Axis::Y;
			if (frontVector == fbxsdk::FbxAxisSystem::eParityEven)
				frontAxis = CoordSystem::Axis::X;
			else
				frontAxis = CoordSystem::Axis::Z;
		}
		break;
		case fbxsdk::FbxAxisSystem::eZAxis:
		{
			upAxis = CoordSystem::Axis::Z;
			if (frontVector == fbxsdk::FbxAxisSystem::eParityEven)
				frontAxis = CoordSystem::Axis::X;
			else
				frontAxis = CoordSystem::Axis::Y;
		}
		break;
	}


	return CoordSystem{ upSign, frontSign, upAxis, frontAxis, rightHanded };
}

bool flt::FBXLoader::SetRawNode(fbxsdk::FbxNode& node, RawNode* outNode)
{
	ASSERT(outNode != nullptr, "outNode is nullptr");

	outNode->name = convToWstring(node.GetName());
	SetTransform(node, &outNode->transform);
	SetMesh(node.GetMesh(), &outNode->mesh);

	return true;
}

bool flt::FBXLoader::SetMesh(fbxsdk::FbxMesh* pMesh, RawMesh** outMesh)
{
	if (pMesh == nullptr)
	{
		*outMesh = nullptr;
		return false;
	}

	*outMesh = new RawMesh();

	auto controlPoints = pMesh->GetControlPoints();
	auto controlPointCount = pMesh->GetControlPointsCount();
	auto polygonCount = pMesh->GetPolygonCount();
	auto polygonVertexCount = pMesh->GetPolygonVertexCount();
	auto polygonVertices = pMesh->GetPolygonVertices();
	auto normalCount = pMesh->GetElementNormalCount();
	auto normalElement = pMesh->GetElementNormal();
	auto normalMappingMode = normalElement->GetMappingMode();
	auto normalReferenceMode = normalElement->GetReferenceMode();
	auto normalDirectArray = normalElement->GetDirectArray();
	auto uvCount = pMesh->GetElementUVCount();
	auto uvElement = pMesh->GetElementUV();
	auto uvMappingMode = uvElement->GetMappingMode();
	auto uvReferenceMode = uvElement->GetReferenceMode();
	auto uvDirectArray = uvElement->GetDirectArray();
	auto materialCount = pMesh->GetElementMaterialCount();
	auto materialElement = pMesh->GetElementMaterial();
	auto materialMappingMode = materialElement->GetMappingMode();
	auto materialReferenceMode = materialElement->GetReferenceMode();
}

bool flt::FBXLoader::SetTransform(fbxsdk::FbxNode& node, Transform* outTransform)
{
	ASSERT(outTransform != nullptr, "outTransform is nullptr");
	auto localTransform = node.EvaluateLocalTransform();
	auto globalTransform = node.EvaluateGlobalTransform();
	auto localTranslation = localTransform.GetT();
	auto localRotation = localTransform.GetR();
	auto localScale = localTransform.GetS();
	auto globalTranslation = globalTransform.GetT();
	auto globalRotation = globalTransform.GetR();
	auto globalScale = globalTransform.GetS();
	outTransform->SetPosition(localTranslation.mData[0], localTranslation.mData[1], localTranslation.mData[2]);
	outTransform->SetRotation( localRotation.mData[0], localRotation.mData[1], localRotation.mData[2] );
	outTransform->SetScale( localScale.mData[0], localScale.mData[1], localScale.mData[2] );
	//outTransform->globalTranslation = { globalTranslation.mData[0], globalTranslation.mData[1], globalTranslation.mData[2] };
	//outTransform->globalRotation = { globalRotation.mData[0], globalRotation.mData[1], globalRotation.mData[2] };
	//outTransform->globalScale = { globalScale.mData[0], globalScale.mData[1], globalScale.mData[2] };
	return true;

}

void flt::FBXLoader::PrintNodeRecursive(fbxsdk::FbxNode* pNode, int depth)
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

void flt::FBXLoader::PrintNodeVertex(fbxsdk::FbxNode* pNode)
{
	fbxsdk::FbxMesh* mesh = pNode->GetMesh();
	if (mesh == nullptr)
		return;

	FbxVector4* vertices = mesh->GetControlPoints();
	for (int j = 0; j < mesh->GetControlPointsCount(); ++j)
	{
		FbxVector4 vertex = vertices[j];
		printf("v : %3.2f %3.2f %3.2f\n", vertex.mData[0], vertex.mData[1], vertex.mData[2]);
	}
}

void flt::FBXLoader::PrintTransform(fbxsdk::FbxNode* pNode)
{
	fbxsdk::FbxDouble3 translation = pNode->LclTranslation;
	fbxsdk::FbxDouble3 rotation = pNode->LclRotation;
	fbxsdk::FbxDouble3 scaling = pNode->LclScaling;
	fbxsdk::FbxEuler::EOrder rotationOrder = pNode->RotationOrder.Get();

	std::string rotationOrderString;
	switch (rotationOrder)
	{
		case fbxsdk::FbxEuler::eOrderXYZ:
			rotationOrderString = "XYZ";
			break;
		case fbxsdk::FbxEuler::eOrderXZY:
			rotationOrderString = "XZY";
			break;
		case fbxsdk::FbxEuler::eOrderYZX:
			rotationOrderString = "YZX";
			break;
		case fbxsdk::FbxEuler::eOrderYXZ:
			rotationOrderString = "YXZ";
			break;
		case fbxsdk::FbxEuler::eOrderZXY:
			rotationOrderString = "ZXY";
			break;
		case fbxsdk::FbxEuler::eOrderZYX:
			rotationOrderString = "ZYX";
			break;
		case fbxsdk::FbxEuler::eOrderSphericXYZ:
			rotationOrderString = "SphericXYZ";
			break;
		default:
			break;
	}

	printf("translation : %3.2f %3.2f %3.2f\n", translation.mData[0], translation.mData[1], translation.mData[2]);
	printf("  rotation  : %3.2f %3.2f %3.2f\n", rotation.mData[0], rotation.mData[1], rotation.mData[2]);
	printf("  scaling   : %3.2f %3.2f %3.2f\n", scaling.mData[0], scaling.mData[1], scaling.mData[2]);
	printf("  rotOrder  : %s\n", rotationOrderString.c_str());
}
