#pragma once
#include <string>

namespace flt
{
	class FBXLoader;
	class GLTFLoader;
	class AssimpLoader;

	class ModelLoader
	{
	public:
		ModelLoader();
		~ModelLoader();

		bool Load(std::wstring path);

	private:
		FBXLoader* _pFBXLoader;
		GLTFLoader* _pGLTFLoader;
		AssimpLoader* _pAssimpLoader;
	};
}
