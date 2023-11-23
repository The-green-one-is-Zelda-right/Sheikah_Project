#include "./include/FBXLoader.h"


#pragma warning(push)
#pragma warning(disable: 26813 26495)
#include "fbxsdk.h"
#pragma warning (pop)

#ifdef _DEBUG
#pragma comment(lib, "../External/lib/x64/debug/libfbxsdk-md.lib")
#else
#pragma comment(lib, "../External/lib/x64/release/libfbxsdk-md.lib")
#endif


#pragma warning(push)
#pragma warning(disable: 4717)
#pragma warning (pop)
