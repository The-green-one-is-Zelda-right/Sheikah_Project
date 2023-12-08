#pragma once
#include <string>

namespace flt
{
	class FBXLoader;

	class ModelLoader
	{
	public:
		ModelLoader();
		~ModelLoader();

		bool Load(std::wstring path);

	private:
		FBXLoader* _pFBXLoader;
	};
}
