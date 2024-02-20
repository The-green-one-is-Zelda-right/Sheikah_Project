#include <windows.h>
#include <iostream>
#include <cassert>
#include "PurahEngine.h"

#include "TestMovement.h"
#include "CameraMovement.h"
#include "TestSingleton.h"

/// 각자 만드는 것들
#include "NaDong.h"
#include "BeomJoon.h"
#include "KiHoon.h"
#include "YongHun.h"

namespace PurahEngine
{
	class BoxCollider;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	std::wstring gameName = L"test";

	std::string path = "PurahEngine.dll";
	HMODULE PurahEngineDll = ::LoadLibraryA(path.c_str());

	assert(PurahEngineDll != nullptr);

	auto CreateInitialize = reinterpret_cast<void (*)(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)>(GetProcAddress(PurahEngineDll, "Initialize"));
	auto CreateRun = reinterpret_cast<void (*)(_In_ int nCmdShow)>(GetProcAddress(PurahEngineDll, "Run"));
	auto CreateFinalize = reinterpret_cast<void (*)()>(GetProcAddress(PurahEngineDll, "Finalize"));

	// DLL 함수를 찾을 수 없습니다.
	assert(!(CreateInitialize == nullptr || CreateRun == nullptr || CreateFinalize == nullptr));

	//PurahEngine::Initialize(hInstance, gameName.c_str(), 1920, 1080); 역할
	CreateInitialize(hInstance, gameName.c_str(), 1920, 1080);

	int BJ = 0;
	int KH = 1;
	int DH = 2;
	int YH = 3;

	int play = DH;

	IMaker* iMaker = nullptr;

	if (play == DH)
	{
		iMaker = new NaDong();
	}
	else if (play == BJ)
	{
		iMaker = new BeomJoon();
	}
	else if (play == KH)
	{
		iMaker = new KiHoon();
	}
	else if (play == YH)
	{
		iMaker = new YongHun();
	}

	/// 건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마
	/// 건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마
	/// 건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마
	iMaker->Run();
	/// 건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마
	/// 건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마
	/// 건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마건들지마

	auto& test = PurahEngine::TestSingleton::Getinstance();



	//PurahEngine::Run(); 역할
	CreateRun(nCmdShow);
	//PurahEngine::Finalize(); 역할
	CreateFinalize();

	::FreeLibrary(PurahEngineDll);

	return 0;
}
