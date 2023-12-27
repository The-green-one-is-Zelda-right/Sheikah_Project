#pragma once
#include <string>

namespace flt
{
	class GLTFLoader
	{
	public:
		void Load(const std::wstring& filePath);

	private:
		void LoadGLTF(const std::wstring& filePath);
		void LoadGLB(const std::wstring& filePath);
	};
}


