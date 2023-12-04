#pragma once
#pragma warning(push)
#pragma warning(disable: 26813 26495)
#include "fbxsdk.h"
#pragma warning (pop)

#include "CoordSystem.h"
#include <string>


namespace flt
{
	struct RawNode;
	struct RawMesh;
	class Transform;

	class FBXLoader final
	{
	public:
		FBXLoader();
		~FBXLoader();

		void Load(const std::wstring& filePath);

	private:
		CoordSystem CreateFBXCoodSystem(FbxScene* pScene);

		bool SetRawNode(fbxsdk::FbxNode& node, RawNode* outNode);
		bool SetMesh(fbxsdk::FbxMesh* pMesh, RawMesh** outMesh);
		bool SetTransform(fbxsdk::FbxNode& node, Transform* outTransform);
		
		void PrintNodeRecursive(fbxsdk::FbxNode* pNode, int depth);
		void PrintNodeVertex(fbxsdk::FbxNode* pNode);
		void PrintTransform(fbxsdk::FbxNode* pNode);

	private:
		fbxsdk::FbxManager* _pManager;
		fbxsdk::FbxIOSettings* _pIOSettings;
	};
}
