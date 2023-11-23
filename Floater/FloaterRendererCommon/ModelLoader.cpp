#include "./include/ModelLoader.h"
#include "FBXLoader.h"

flt::ModelLoader::ModelLoader() :
	_pFBXLoader(new(std::nothrow) FBXLoader())
{
}

flt::ModelLoader::~ModelLoader()
{
	delete _pFBXLoader;

}

void flt::ModelLoader::Load(std::wstring path)
{
	_pFBXLoader->Load(path);
}
