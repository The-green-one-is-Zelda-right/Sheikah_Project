#include <windows.h>
#include <iostream>
#include <cassert>
#include "PurahEngine.h"


#include "TestMovement.h"
#include "CameraMovement.h"
#include "TestSingleton.h"
#include "AudioTest.h"

#include "Controller.h"
#include "LauncherPlatform.h"

#include "Button.h"
#include "DisconnectedWire.h"
#include "Door.h"
#include "DoorParts.h"
#include "ElectricWire.h"

/// ���� ����� �͵�
#include "NaDong.h"
#include "BeomJoon.h"
#include "KiHoon.h"
#include "YongHun.h"

namespace PurahEngine
{
	class BoxCollider;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
// int APIENTRY main(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	std::wstring gameName = L"test";

	std::string path = "PurahEngine.dll";
	HMODULE PurahEngineDll = ::LoadLibraryA(path.c_str());

	assert(PurahEngineDll != nullptr);

	auto CreateInitialize = reinterpret_cast<void (*)(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)>(GetProcAddress(PurahEngineDll, "Initialize"));
	auto CreateRun = reinterpret_cast<void (*)(_In_ int nCmdShow)>(GetProcAddress(PurahEngineDll, "Run"));
	auto CreateFinalize = reinterpret_cast<void (*)()>(GetProcAddress(PurahEngineDll, "Finalize"));

	// DLL �Լ��� ã�� �� �����ϴ�.
	assert(!(CreateInitialize == nullptr || CreateRun == nullptr || CreateFinalize == nullptr));

	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::Test>("Test");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::Test2>("Test2");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::Test3>("Test3");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::TestMovement>("TestMovement");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::CameraMovement>("CameraMovement");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<AnimationTestController>("AnimationTestController");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::AudioTest>("AudioTest");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<LauncherPlatform>("LauncherPlatform");

	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::Controller>("Controller");

	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::Button>("Button");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::DisconnectedWire>("DisconnectedWire");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::Door>("Door");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::DoorParts>("DoorParts");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::ElectricWire>("ElectricWire");

	//PurahEngine::Initialize(hInstance, gameName.c_str(), 1920, 1080); ����
	CreateInitialize(hInstance, gameName.c_str(), 1920, 1080);

	int BJ = 0;
	int KH = 1;
	int DH = 2;
	int YH = 3;
	int loadScene = 4;

	int play = loadScene;

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
	else if (play == loadScene)
	{
		PurahEngine::SceneManager::GetInstance().LoadScene(L"DataExportTestWorldObjectInfo.json");
	}

	/// �ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ�����
	/// �ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ�����
	/// �ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ�����

	if (play != loadScene)
	{
		iMaker->Run();
	}
	/// �ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ�����
	/// �ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ�����
	/// �ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ������ǵ�����

	auto& test = PurahEngine::TestSingleton::Getinstance();



	//PurahEngine::Run(); ����
	CreateRun(nCmdShow);
	//PurahEngine::Finalize(); ����
	CreateFinalize();

	::FreeLibrary(PurahEngineDll);

	return 0;
}
