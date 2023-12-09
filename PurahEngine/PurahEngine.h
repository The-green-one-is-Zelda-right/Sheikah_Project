#pragma once
/// 외부에는 이부분만 보여지게 하기 위하도록 설계
#include "PurahEngineAPI.h"
#include <Windows.h>

namespace PurahEngine
{
	PURAHENGINE_API void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
	PURAHENGINE_API void Run(_In_ int nCmdShow);
	PURAHENGINE_API void Finalize();
};

	