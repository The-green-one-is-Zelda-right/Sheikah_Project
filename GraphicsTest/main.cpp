#include <Windows.h>

#include "CoreSystem.h"

using namespace std;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	CoreSystem::GetInstance().Initialize(hInstance, L"sample", 1920, 1080, false, false, false, false);

	CoreSystem::GetInstance().Run(nCmdShow);

	CoreSystem::GetInstance().Finalize();

	return 0;
}