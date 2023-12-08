#pragma once
#pragma warning(push)
#pragma warning(disable: 26813 26495)
#define FBXSDK_SHARED
#define FBXSDK_NAMESPACE_USING 0
#include "fbxsdk.h"
#pragma warning (pop)

#include "CoordSystem.h"
#include <string>


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
