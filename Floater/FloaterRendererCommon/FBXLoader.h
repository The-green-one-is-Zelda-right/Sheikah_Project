﻿#pragma once
#include <string>
#pragma warning(push)
#pragma warning(disable: 26813 26495)
#include "fbxsdk.h"
#pragma warning (pop)

namespace flt
{
	class FBXLoader final
	{
	public:
		FBXLoader();
		~FBXLoader();

		void Load(const std::wstring& filePath);

	private:
		void PrintNodeRecursive(FbxNode* pNode, int depth);

	private:
		FbxManager* _pManager;
		FbxIOSettings* _pIOSettings;
	};

}
