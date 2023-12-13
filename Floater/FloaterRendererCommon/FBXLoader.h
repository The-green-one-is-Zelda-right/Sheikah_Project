#pragma once
#pragma warning(push)
#pragma warning(disable: 26813 26495)
#define FBXSDK_SHARED
#define FBXSDK_NAMESPACE_USING 0
#include "fbxsdk.h"
#pragma warning (pop)

#include "CoordSystem.h"
#include "../FloaterMath/include/Vector2f.h"
#include "../FloaterMath/include/Vector3f.h"
#include "../FloaterMath/include/Vector4f.h"
#include <string>
#include <vector>


namespace flt
{
	struct RawNode;
	struct RawMesh;
	struct RawAnimation;
	class Transform;

	class FBXLoader final
	{
	public:
		FBXLoader();
		~FBXLoader();

		void Load(const std::wstring& filePath);

	private:
		CoordSystem CreateFBXCoodSystem(fbxsdk::FbxScene* pScene);

		bool LoadToRawNodeRecursive(fbxsdk::FbxNode* pNode, RawNode* outNode);

		bool LoadToTransform(fbxsdk::FbxAMatrix& fbxMatrix, Transform* outTransform);
		bool LoadToRawNode(fbxsdk::FbxNode& node, RawNode* outNode);
		bool CreateMesh(fbxsdk::FbxMesh& mesh, RawMesh** outMesh);
		bool CreateAnimation(fbxsdk::FbxNode& node, RawAnimation** outNode);

		void GetNormal(fbxsdk::FbxLayerElementNormal* pNormalLayer, std::vector<Vector3f>* outVector);
		void GetUV(fbxsdk::FbxLayerElementUV* pUVLayer, std::vector<Vector2f>* outVector);
		void GetColor(fbxsdk::FbxLayerElementVertexColor* pColorLayer, std::vector<Vector4f>* outVector);
		void GetTangent(fbxsdk::FbxLayerElementTangent* pTangentLayer, std::vector<Vector3f>* outVector);
		void GetBinormal(fbxsdk::FbxLayerElementBinormal* pBinormalLayer, std::vector<Vector3f>* outVector);

		void PrintNodeRecursive(fbxsdk::FbxNode* pNode, int depth);
		void PrintNodeVertex(fbxsdk::FbxNode* pNode);
		void PrintTransform(fbxsdk::FbxNode* pNode);

	private:
		fbxsdk::FbxManager* _pManager;
		fbxsdk::FbxIOSettings* _pIOSettings;
		fbxsdk::FbxImporter* _importer;
		fbxsdk::FbxScene* _scene;
	};
}
