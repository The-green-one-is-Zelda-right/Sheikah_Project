#include "../FloaterUtil/include/FloaterMacro.h"
#include "./include/ModelLoader.h"
#include "FBXLoader.h"
#include <filesystem>

flt::ModelLoader::ModelLoader() :
	_pFBXLoader(new(std::nothrow) FBXLoader())
{

}

flt::ModelLoader::~ModelLoader()
{
	delete _pFBXLoader;
}

bool flt::ModelLoader::Load(std::wstring path)
{
	if (!std::filesystem::exists(path))
	{
		ASSERT(false, "File Not Exist");
		return false;
	}

	_pFBXLoader->Load(path);
	return true;
}
