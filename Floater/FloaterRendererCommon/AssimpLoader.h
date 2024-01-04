#pragma once
#include <string>

namespace flt
{
	struct RawScene;

	class AssimpLoader
	{
	public:
		void Load(const std::wstring& filePath, RawScene* outRawScene);

	private:

	};
}
