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
	_pIOSettings(nullptr),
	_importer(nullptr),
	_scene(nullptr)
{
	_pManager = fbxsdk::FbxManager::Create();
	_importer = fbxsdk::FbxImporter::Create(_pManager, "");
	_pIOSettings = fbxsdk::FbxIOSettings::Create(_pManager, IOSROOT);
	_pManager->SetIOSettings(_pIOSettings);
	_scene = fbxsdk::FbxScene::Create(_pManager, "");
}

flt::FBXLoader::~FBXLoader()
{
	_scene->Destroy();
	_importer->Destroy();
	_pIOSettings->Destroy();
	_pManager->Destroy();
}

void flt::FBXLoader::Load(const std::wstring& filePath)
{
	std::string filePathStr = flt::ConvertToString(filePath);
	if (!_importer->Initialize(filePathStr.c_str(), -1, _pManager->GetIOSettings()))
	{
		auto var = _importer->GetStatus().GetErrorString();
		ASSERT(false, "importer Init Fail");
	}
	std::cout << "-------------------------------------" << std::endl;
	std::wcout << filePath << std::endl;
	int major;
	int minor;
	int revision;
	_importer->GetFileVersion(major, minor, revision);
	std::cout << major << "." << minor << "." << revision << std::endl;


	bool ret = _importer->Import(_scene);
	ASSERT(ret, "import Scene fail");

	// 일단 fbx 자체기능으로 좌표계를 바꾸자.
	fbxsdk::FbxAxisSystem dx11Axis(fbxsdk::FbxAxisSystem::eDirectX);
	dx11Axis.ConvertScene(_scene);

	// 삼각형 폴리곤으로 변경.
	fbxsdk::FbxGeometryConverter meshConverter(_pManager);
	meshConverter.Triangulate(_scene, true);


	auto fileCoord = CreateFBXCoodSystem(_scene);
	fileCoord.PrintfCoord();

	//auto animStack = _scene->GetSrcObject<FbxAnimStack>(0);
	//int animStackCount = _scene->GetSrcObjectCount<fbxsdk::FbxAnimStack>();
	//
	//FbxAxisSystem{ FbxAxisSystem::eMax }.ConvertScene(scene);
	//dxCoordSystem = CreateFBXCoodSystem(scene);
	//
	// animation
	//auto animStackCount = _scene->GetSrcObjectCount<fbxsdk::FbxAnimStack>();
	//for (int i = 0; i < animStackCount; ++i)
	//{
	//	auto animStack = _scene->GetSrcObject<fbxsdk::FbxAnimStack>(i);
	//	auto animLayerCount = animStack->GetMemberCount<fbxsdk::FbxAnimLayer>();
	//	for (int j = 0; j < animLayerCount; ++j)
	//	{
	//		auto animLayer = animStack->GetMember<fbxsdk::FbxAnimLayer>(j);
	//		auto animCurveNodeCount = node.GetSrcObjectCount<fbxsdk::FbxAnimCurveNode>();
	//		for (int k = 0; k < animCurveNodeCount; ++k)
	//		{
	//			auto animCurveNode = node.GetSrcObject<fbxsdk::FbxAnimCurveNode>(k);
	//			auto animCurveCount = animCurveNode->GetChannelsCount();
	//			for (int l = 0; l < animCurveCount; ++l)
	//			{
	//				auto animCurve = animCurveNode->GetCurve(l);
	//				auto animCurveKeyCount = animCurve->KeyGetCount();
	//				for (int m = 0; m < animCurveKeyCount; ++m)
	//				{
	//					auto animCurveKey = animCurve->KeyGet(m);
	//					auto animCurveKeyValue = animCurveKey.GetValue();
	//					auto animCurveKeyTime = animCurveKey.GetTime();
	//				}
	//			}
	//		}
	//	}
	//}

	fbxsdk::FbxNode* rootNode = _scene->GetRootNode();

	if (rootNode)
	{
		// 일단 루트 노드의 경우 identity가 아닌 경우는 고려하지 말자.
		Transform transform;
		LoadToTransform(rootNode->EvaluateLocalTransform(), &transform);
		auto matrix = transform.GetLocalMatrix4f();
		ASSERT((matrix.Determinant() - 1.0f) < FLT_EPSILON || (matrix.Determinant() - 1.0f) > -FLT_EPSILON, "root Transform is not identity");

		int childCount = rootNode->GetChildCount();
		for (int i = 0; i < childCount; ++i)
		{
			fbxsdk::FbxNode* childNode = rootNode->GetChild(i);
			//if (childNode->GetNodeAttribute() == nullptr)
			//	continue;

			RawNode* childRawNode = new RawNode();
			LoadToRawNodeRecursive(childNode, childRawNode);
		}
	}

	return;
}

flt::CoordSystem flt::FBXLoader::CreateFBXCoodSystem(fbxsdk::FbxScene* pScene)
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

bool flt::FBXLoader::LoadToRawNodeRecursive(fbxsdk::FbxNode* pNode, RawNode* outNode)
{
	ASSERT(outNode, "outNode is nullptr");

	if (!LoadToRawNode(*pNode, outNode))
	{
		return false;
	}

	int childCount = pNode->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		auto child = pNode->GetChild(i);
		RawNode* childRawNode = new RawNode();
		outNode->children.push_back(childRawNode);
		childRawNode->parent = outNode;
		if (!LoadToRawNodeRecursive(child, childRawNode))
		{
			return false;
		}
	}

	return true;
}

bool flt::FBXLoader::LoadToTransform(fbxsdk::FbxAMatrix& fbxMatrix, Transform* outTransform)
{
	auto localTranslation = fbxMatrix.GetT();
	auto localRotation = fbxMatrix.GetR();
	auto localScale = fbxMatrix.GetS();

	outTransform->SetPosition(localTranslation.mData[0], localTranslation.mData[1], localTranslation.mData[2]);
	outTransform->SetRotation(localRotation.mData[0], localRotation.mData[1], localRotation.mData[2]);
	outTransform->SetScale(localScale.mData[0], localScale.mData[1], localScale.mData[2]);

	return true;
}

bool flt::FBXLoader::LoadToRawNode(fbxsdk::FbxNode& node, RawNode* outNode)
{
	ASSERT(outNode != nullptr, "outNode is nullptr");

	outNode->name = ConvertToWstring(node.GetName());

	// Transform 세팅
	LoadToTransform(node.EvaluateLocalTransform(), &outNode->transform);

	fbxsdk::FbxVector4 tl = node.GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 ro = node.GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 sc = node.GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);



	// Mesh 세팅
	fbxsdk::FbxMesh* pMesh = node.GetMesh();
	if (pMesh != nullptr)
	{
		CreateMesh(*pMesh, &outNode->mesh);
	}

	// animation 세팅
	CreateAnimation(node, &outNode->animation);

	return true;
}

bool flt::FBXLoader::CreateMesh(fbxsdk::FbxMesh& mesh, RawMesh** outMesh)
{
	*outMesh = new RawMesh();

	// 아래 레이어들 특히 UV레이어는 여러개 있을 수 있음.
	int normalLayerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eNormal);
	for (int i = 0; i < normalLayerCount; ++i)
	{
		std::vector<Vector3f> testNormalVector;
		fbxsdk::FbxLayerElementNormal* normalLayer = mesh.GetElementNormal(i);
		GetNormal(normalLayer, &testNormalVector);
	}

	int uvLayerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eUV);
	for (int i = 0; i < uvLayerCount; ++i)
	{
		std::vector<Vector2f> testUvVector;
		fbxsdk::FbxLayerElementUV* uvLayer = mesh.GetElementUV(i);
		GetUV(uvLayer, &testUvVector);
	}

	int colorLayerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eVertexColor);
	for (int i = 0; i < colorLayerCount; ++i)
	{
		std::vector<Vector4f> testColorVector;
		fbxsdk::FbxLayerElementVertexColor* colorLayer = mesh.GetElementVertexColor(i);
		GetColor(colorLayer, &testColorVector);
	}

	int tangentLayerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eTangent);
	for (int i = 0; i < tangentLayerCount; ++i)
	{
		std::vector<Vector3f> testTangentVector;
		fbxsdk::FbxLayerElementTangent* tangentLayer = mesh.GetElementTangent(i);
		GetTangent(tangentLayer, &testTangentVector);
	}

	int binormalLayerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eBiNormal);
	for (int i = 0; i < binormalLayerCount; ++i)
	{
		std::vector<Vector3f> testBinormalVector;
		fbxsdk::FbxLayerElementBinormal* binormalLayer = mesh.GetElementBinormal(i);
		GetBinormal(binormalLayer, &testBinormalVector);
	}



	//auto uvLayer = mesh.GetLayer(0);
	//int testBinormalCount = mesh.GetElementBinormalCount();
	//int testTangentCount = mesh.GetElementTangentCount();
	//int testVertexColorCount = mesh.GetElementVertexColorCount();
	//int testVertexCount = mesh.GetControlPointsCount();


	int vertexCount = mesh.GetControlPointsCount();
	(*outMesh)->vertices.reserve(vertexCount);
	for (int i = 0; i < vertexCount; ++i)
	{
		auto vertex = mesh.GetControlPointAt(i);
		Vector3f pos{ vertex.mData[0], vertex.mData[1], vertex.mData[2] };
		Vector2f Tex{};
		Vector3f Normal{};
		(*outMesh)->vertices.emplace_back(pos, Tex, Normal);
	}

	int polygonCount = mesh.GetPolygonCount();
	//int polygonVertexCount = mesh.GetPolygonVertexCount();

	(*outMesh)->indices.reserve(polygonCount * 3);
	for (int i = 0; i < polygonCount; ++i)
	{
		int polygonSize = mesh.GetPolygonSize(i);
		ASSERT(polygonSize == 3, "polygonSize is not 3");

		if (polygonSize != 3)
		{
			return false;
		}

		for (int j = 0; j < polygonSize; ++j)
		{
			int index = mesh.GetPolygonVertex(i, j); // 인덱스 버퍼에 들어갈 것.
			(*outMesh)->indices.push_back(index);
		}
	}

	if (mesh.GetElementNormalCount() == 0)
	{
		fbxsdk::FbxGeometryElementNormal* normal = mesh.GetElementNormal();
		int normalIndex = 0;

		fbxsdk::FbxGeometryElement::EMappingMode mappingMode = normal->GetMappingMode();
		switch (mappingMode)
		{
			case fbxsdk::FbxGeometryElement::eByPolygonVertex:
			{
				if (normal->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
				{
					normalIndex = vertexCount;
				}
				else
				{
					normalIndex = normal->GetIndexArray().GetAt(vertexCount);
				}
			}
			break;
			case fbxsdk::FbxGeometryElement::eByControlPoint:
			{
				if (normal->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
				{
					normalIndex = vertexCount;
				}
				else
				{
					normalIndex = normal->GetIndexArray().GetAt(vertexCount);
				}
			}
			break;
			default:
			{
				ASSERT(false, "can`t load Normal");
			}
			break;
		}
	}

	auto normalCount = mesh.GetElementNormalCount();
	auto normalElement = mesh.GetElementNormal();
	auto normalMappingMode = normalElement->GetMappingMode();
	auto normalReferenceMode = normalElement->GetReferenceMode();

	auto& normalDirectArray = normalElement->GetDirectArray();
	auto uvCount = mesh.GetElementUVCount();

	auto uvElement = mesh.GetElementUV();
	auto uvMappingMode = uvElement->GetMappingMode();
	auto uvReferenceMode = uvElement->GetReferenceMode();
	auto& uvDirectArray = uvElement->GetDirectArray();
	auto materialCount = mesh.GetElementMaterialCount();
	auto materialElement = mesh.GetElementMaterial();
	auto materialMappingMode = materialElement->GetMappingMode();
	auto materialReferenceMode = materialElement->GetReferenceMode();

	return true;
}

bool flt::FBXLoader::CreateAnimation(fbxsdk::FbxNode& node, RawAnimation** outAnimation)
{
	int animCurveNodeCount = node.GetSrcObjectCount<fbxsdk::FbxAnimCurveNode>();

	if (animCurveNodeCount == 0)
		return true;

	*outAnimation = new RawAnimation();

	for (int i = 0; i < animCurveNodeCount; ++i)
	{
		auto animCurveNode = node.GetSrcObject<fbxsdk::FbxAnimCurveNode>(i);


		int animCurveCount = animCurveNode->GetChannelsCount();
		for (int j = 0; j < animCurveCount; ++j)
		{
			auto animCurve = animCurveNode->GetCurve(j);
			int animCurveKeyCount = animCurve->KeyGetCount();
			(*outAnimation)->keyDatas.reserve(animCurveKeyCount);
			for (int k = 0; k < animCurveKeyCount; ++k)
			{
				auto animCurveKey = animCurve->KeyGet(k);
				auto animCurveKeyValue = animCurveKey.GetValue();
				auto animCurveKeyTime = animCurveKey.GetTime();
				float animCurveKeyTimeSecond = static_cast<float>(animCurveKeyTime.GetSecondDouble());

				Transform transform;
				LoadToTransform(node.EvaluateLocalTransform(animCurveKeyTime), &transform);
				(*outAnimation)->keyDatas.emplace_back(animCurveKeyTimeSecond, transform);
			}
		}
	}

	return true;
}

void flt::FBXLoader::GetNormal(fbxsdk::FbxLayerElementNormal* pNormalLayer, std::vector<Vector3f>* outVector)
{
	ASSERT(pNormalLayer, "pNormalLayer is nullptr");

	fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4>& elementNormal = pNormalLayer->GetDirectArray();
	fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;

	auto referenceMode = pNormalLayer->GetReferenceMode();
	if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
	{
		pIndices = &pNormalLayer->GetIndexArray();
	}

	auto mappingMode = pNormalLayer->GetMappingMode();
	// ByPolygonVertex : 폴리곤의 각 정점 별로 매핑
	// ByControlPoint : 컨트롤 포인트 별로 매핑
	// ByPolygon : 폴리곤 별로 매핑
	// TODO : 매핑 방법별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
	ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");

	int elementCount = elementNormal.GetCount();

	outVector->reserve(elementCount);

	for (int i = 0; i < elementCount; ++i)
	{
		int index = i;
		if (pIndices != nullptr)
		{
			index = pIndices->GetAt(i);
		}

		fbxsdk::FbxVector4 normal = elementNormal.GetAt(index);
		outVector->emplace_back(normal.mData[0], normal.mData[1], normal.mData[2]);
	}
}

void flt::FBXLoader::GetUV(fbxsdk::FbxLayerElementUV* pUVLayer, std::vector<Vector2f>* outVector)
{
	ASSERT(pUVLayer, "pUVLayer is nullptr");

	fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector2>& elementUV = pUVLayer->GetDirectArray();
	fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;

	auto referenceMode = pUVLayer->GetReferenceMode();
	if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
	{
		pIndices = &pUVLayer->GetIndexArray();
	}

	auto mappingMode = pUVLayer->GetMappingMode();
	// TODO : 매핑 방법 별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
	ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");

	int elementCount = elementUV.GetCount();

	outVector->reserve(elementCount);

	for (int i = 0; i < elementCount; ++i)
	{
		int index = i;
		if (pIndices != nullptr)
		{
			index = pIndices->GetAt(i);
		}

		fbxsdk::FbxVector2 uv = elementUV.GetAt(index);
		outVector->emplace_back(uv.mData[0], uv.mData[1]);
	}

}

void flt::FBXLoader::GetColor(fbxsdk::FbxLayerElementVertexColor* pColorLayer, std::vector<Vector4f>* outVector)
{
	ASSERT(pColorLayer, "pColorLayer is nullptr");

	fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxColor>& elementColor = pColorLayer->GetDirectArray();
	fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;

	auto referenceMode = pColorLayer->GetReferenceMode();
	if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
	{
		pIndices = &pColorLayer->GetIndexArray();
	}

	auto mappingMode = pColorLayer->GetMappingMode();
	// ByPolygonVertex : 폴리곤의 각 정점 별로 매핑
	// ByControlPoint : 컨트롤 포인트 별로 매핑
	// ByPolygon : 폴리곤 별로 매핑
	// TODO : 매핑 방법별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
	ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");

	int elementCount = elementColor.GetCount();

	outVector->reserve(elementCount);

	for (int i = 0; i < elementCount; ++i)
	{
		int index = i;
		if (pIndices != nullptr)
		{
			index = pIndices->GetAt(i);
		}

		fbxsdk::FbxColor color = elementColor.GetAt(index);
		
		outVector->emplace_back(color.mRed, color.mGreen, color.mBlue, color.mAlpha);
	}
}

void flt::FBXLoader::GetTangent(fbxsdk::FbxLayerElementTangent* pTangentLayer, std::vector<Vector3f>* outVector)
{
	ASSERT(pTangentLayer, "pTangentLayer is nullptr");

	fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4>& elementTangent = pTangentLayer->GetDirectArray();
	fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;

	auto referenceMode = pTangentLayer->GetReferenceMode();
	if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
	{
		pIndices = &pTangentLayer->GetIndexArray();
	}

	auto mappingMode = pTangentLayer->GetMappingMode();
	// ByPolygonVertex : 폴리곤의 각 정점 별로 매핑
	// ByControlPoint : 컨트롤 포인트 별로 매핑
	// ByPolygon : 폴리곤 별로 매핑
	// TODO : 매핑 방법별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
	ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");

	int elementCount = elementTangent.GetCount();

	outVector->reserve(elementCount);

	for (int i = 0; i < elementCount; ++i)
	{
		int index = i;
		if (pIndices != nullptr)
		{
			index = pIndices->GetAt(i);
		}

		fbxsdk::FbxVector4 tangent = elementTangent.GetAt(index);
		outVector->emplace_back(tangent.mData[0], tangent.mData[1], tangent.mData[2]);
	}
}

void flt::FBXLoader::GetBinormal(fbxsdk::FbxLayerElementBinormal* pBinormalLayer, std::vector<Vector3f>* outVector)
{
	ASSERT(pBinormalLayer, "pBinormalLayer is nullptr");

	fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4>& elementBinormal = pBinormalLayer->GetDirectArray();
	fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;

	auto referenceMode = pBinormalLayer->GetReferenceMode();
	if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
	{
		pIndices = &pBinormalLayer->GetIndexArray();
	}

	auto mappingMode = pBinormalLayer->GetMappingMode();
	// ByPolygonVertex : 폴리곤의 각 정점 별로 매핑
	// ByControlPoint : 컨트롤 포인트 별로 매핑
	// ByPolygon : 폴리곤 별로 매핑
	// TODO : 매핑 방법별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
	ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");

	int elementCount = elementBinormal.GetCount();

	outVector->reserve(elementCount);

	for (int i = 0; i < elementCount; ++i)
	{
		int index = i;
		if (pIndices != nullptr)
		{
			index = pIndices->GetAt(i);
		}

		fbxsdk::FbxVector4 binormal = elementBinormal.GetAt(index);
		outVector->emplace_back(binormal.mData[0], binormal.mData[1], binormal.mData[2]);
	}
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

	fbxsdk::FbxVector4* vertices = mesh->GetControlPoints();
	for (int j = 0; j < mesh->GetControlPointsCount(); ++j)
	{
		fbxsdk::FbxVector4 vertex = vertices[j];
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
