#include <windows.h>
#include <iostream>
#include <cassert>

// dll import Ȯ�ο� (���߿� ���� �� ��)
#include "../PurahEngine/PurahEngine.h"

//#pragma comment(lib, "../x64/Debug/PurahEngine.lib")

//using namespace PurahEngine;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	std::wstring gameName = L"test";

	std::string path = "PurahEngine.dll";
	HMODULE PurahEngineDll = ::LoadLibraryA(path.c_str());

	assert(PurahEngineDll != nullptr);

	auto CreateInitialize = reinterpret_cast<void* (*)(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)>(GetProcAddress(PurahEngineDll, "Initialize"));
	auto CreateRun = reinterpret_cast<void* (*)(_In_ int nCmdShow)>(GetProcAddress(PurahEngineDll, "Run"));
	auto CreateFinalize = reinterpret_cast<void* (*)()>(GetProcAddress(PurahEngineDll, "Finalize"));

	// DLL �Լ��� ã�� �� �����ϴ�.
	assert(!(CreateInitialize == nullptr || CreateRun == nullptr || CreateFinalize == nullptr));


	//PurahEngine::Initialize(hInstance, gameName.c_str(), 1920, 1080); ����
	CreateInitialize(hInstance, gameName.c_str(), 1920, 1080);
	//PurahEngine::Run(); ����
	CreateRun(nCmdShow);
	//PurahEngine::Finalize(); ����
	CreateFinalize();

	::FreeLibrary(PurahEngineDll);

	return 0;
}

//int main()
//{
//	using namespace std;
//	cout << "Zelda Project Start!!" << endl;
//
//	return 0;
//}