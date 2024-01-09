#pragma once
#include <string>
#include <unordered_map>
//#include "../FloaterUtil/include/RBTree.h"

struct aiNode;

namespace flt
{
	struct RawScene;
	struct RawNode;

	class AssimpLoader
	{
	public:
		void Load(const std::wstring& filePath, RawScene* outRawScene);

	private:
		void GetNodeRecursive(aiNode* pNode, RawNode* pRawNode);

		void PrintNodeNameRecursive(aiNode* pNode, int depth = 0);

	private:
		std::unordered_map<std::wstring, RawNode*> _nodeMap;
	};
}
