#pragma once
/// �ܺο��� �̺κи� �������� �ϱ� ���ϵ��� ����
#include "PurahEngineAPI.h"
#include <Windows.h>

namespace PurahEngine
{
	PURAHENGINE_API void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
	PURAHENGINE_API void Run(_In_ int nCmdShow);
	PURAHENGINE_API void Finalize();
};

	