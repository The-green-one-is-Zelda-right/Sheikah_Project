#define FLT_WINDOWS
#define _CRT_SECURE_NO_WARNINGS

#include "../FloaterPlatform/include/Platform.h"
#include "../FloaterRendererCommon/include/Transform.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/Timer.h"
#include <iostream>

#include "../FloaterUtil/include/ConvString.h"

#pragma region testInclude
#include "../FloaterRendererCommon/include/IBuilder.h"
#include "../FloaterRendererCommon/include/Resource.h"
#include "../FloaterRendererCommon/include/ResourceMgr.h"
#include "../FloaterUtil/TesterRBTree.h"

#include "../FloaterUtil/TesterFixedSizeMemoryPool.h"
#include "../FloaterUtil/include/Hash.h"

#include "../FloaterRendererCommon/include/Camera.h"

#include "../FloaterRendererCommon/include/ModelLoader.h"
#include "../FloaterRendererCommon/CoordSystem.h"
#include <unordered_map>
#include <filesystem>

#include "../FloaterMath/include/Common.h"
#include "../FloaterMath/include/Vector2f.h"
#include "../FloaterMath/include/Vector3f.h"

#pragma endregion

#include "../ZeldaGraphicsAdapter/include/IZeldaRendererAdapter.h"

#pragma warning(push)
#pragma warning(disable: 4717)
void StackOverflow()
{
	int i = 10;
	StackOverflow();
	std::cout << i << std::endl;
}
#pragma warning (pop)

char TestGetAxis(const flt::Vector3f& vec)
{
	if (vec.x == 1.0f || vec.x == -1.0f)
	{
		return 'x';
	}
	else if (vec.y == 1.0f || vec.y == -1.0f)
	{
		return 'y';
	}
	else if (vec.z == 1.0f || vec.z == -1.0f)
	{
		return 'z';
	}
	else
	{
		return 'n';
	}
}

void TestVectorPrint(const flt::Vector3f& up, const flt::Vector3f& front, const flt::Vector3f& right)
{
	std::cout << "up : " << TestGetAxis(up)
		<< " front: " << TestGetAxis(front)
		<< " rignt: " << TestGetAxis(right) << std::endl;

	std::cout << "up : " << up.x + up.y + up.z
		<< " front: " << front.x + front.y + front.z
		<< " rignt: " << right.x + right.y + right.z << std::endl;

	std::cout << "--------------------------------" << std::endl;
}

int main()
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;

	flt::RawScene rawScene;

#pragma region 테스트
	{
		using namespace flt;
		using namespace flt::test;

		//TesterRBTree tester;
		//tester.Test();

		Quaternion q1{ 0.0f, 0.0f, 0.0f, 1.0f };
		q1.SetEuler(1.f, 2.f, 3.f);
		//Vector3f up{ 0.0f, 0.0f, 1.0f };
		//Vector3f right{ 0.0f, 1.0f, 0.0f };
		//Vector3f forward{ 1.0f, 0.0f, 0.0f };

		//CoordSystem coord = CoordSystem::DX11();
		//std::cout << "CoordSystem::GetDX11()" << std::endl;
		//coord.PrintfCoord();
		////coord.ConvertFrom(CoordSystem::Unreal(), up);
		////coord.ConvertFrom(CoordSystem::Unreal(), right);
		////coord.ConvertFrom(CoordSystem::Unreal(), forward);
		//CoordSystem::Convert(CoordSystem::Unreal(), CoordSystem::DX11(), up);
		//CoordSystem::Convert(CoordSystem::Unreal(), CoordSystem::DX11(), right);
		//CoordSystem::Convert(CoordSystem::Unreal(), CoordSystem::DX11(), forward);
		//TestVectorPrint(up, forward, right);

		//coord = CoordSystem::Unreal();
		//std::cout << "CoordSystem::GetUnreal()" << std::endl;
		//coord.PrintfCoord();
		////coord.ConvertFrom(CoordSystem::DX11(), up);
		////coord.ConvertFrom(CoordSystem::DX11(), right);
		////coord.ConvertFrom(CoordSystem::DX11(), forward);
		//CoordSystem::Convert(CoordSystem::DX11(), CoordSystem::Unreal(), up);
		//CoordSystem::Convert(CoordSystem::DX11(), CoordSystem::Unreal(), right);
		//CoordSystem::Convert(CoordSystem::DX11(), CoordSystem::Unreal(), forward);
		//TestVectorPrint(up, forward, right);

		//coord = CoordSystem::OpenGL();
		//std::cout << "CoordSystem::GetOpenGL()" << std::endl;
		//coord.PrintfCoord();
		//coord.ConvertFrom(CoordSystem::DX11(), up);
		//coord.ConvertFrom(CoordSystem::DX11(), right);
		//coord.ConvertFrom(CoordSystem::DX11(), forward);
		//TestVectorPrint(up, forward, right);

		//coord = CoordSystem::Max3D();
		//std::cout << "CoordSystem::Get3DsMax()" << std::endl;
		//coord.PrintfCoord();
		//coord.ConvertFrom(CoordSystem::OpenGL(), up);
		//coord.ConvertFrom(CoordSystem::OpenGL(), right);
		//coord.ConvertFrom(CoordSystem::OpenGL(), forward);
		//TestVectorPrint(up, forward, right);

		//coord = CoordSystem::Unreal();
		//std::cout << "CoordSystem::GetUnreal()" << std::endl;
		//coord.PrintfCoord();
		//coord.ConvertFrom(CoordSystem::DX11(), up);
		//coord.ConvertFrom(CoordSystem::DX11(), right);
		//coord.ConvertFrom(CoordSystem::DX11(), forward);
		//TestVectorPrint(up, forward, right);



		ModelLoader loader;
		std::wstring filePath = L"..\\x64\\fbx\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Ganondorf (TotK).fbx";
		std::wstring zUpYForward = L"..\\x64\\fbx\\Test\\ZY.fbx";
		std::wstring yUpZForward = L"..\\x64\\fbx\\Test\\YZ.fbx";
		std::wstring xUpYForward = L"..\\x64\\fbx\\Test\\XY.fbx";

		char buffer[1024];
		auto str = ConvertToString(L"..\\x64\\fbx\\Test\\*.*");
		
		WIN32_FIND_DATAA findData;
		HANDLE handle = FindFirstFileA(str.c_str(), &findData);

		if (handle != INVALID_HANDLE_VALUE)
		{
			do
			{
				std::cout << findData.cFileName << std::endl;
			} while(FindNextFileA(handle, &findData));
		}
		
		GetFullPathNameA(str.c_str(), 1024, buffer, NULL);

		//std::filesystem::path currPath = std::filesystem::current_path();

		//loader.Load(L"..\\x64\\fbx\\PBR_BasicShader.blend");


		loader.Load(filePath, &rawScene);
		//loader.Load(zUpYForward);
		//loader.Load(yUpZForward);
		//loader.Load(xUpYForward);

	}
#pragma endregion

	//auto adpaterTest = new IZeldaRendererAdapter();
	//adpaterTest->Initialize(1280, 720, false, NULL, false, 0.0f, 0.0f);

	flt::Platform platform{ true };
	platform.Initialize(1280, 720, L"title", L".\\path");

	auto renderer = platform.CreateRenderer(flt::RendererType::DX11);

	bool isDraw = true;
	flt::RawNode node(L"testNode");
	node.transform.SetPosition(0.0f, 0.0f, 0.7f);
	node.transform.SetScale(0.3f, 0.3f, 0.3f);

	flt::RendererObject rendererObejct1(*rawScene.nodes[1], isDraw, L"test1");
	auto objectID0 = renderer->RegisterObject(rendererObejct1);
	rawScene.nodes[1]->transform.SetScale(1.f, 1.f, 1.f);
	rawScene.nodes[1]->transform.SetPosition(0.f, 0.f, -30.f);

	//flt::RendererObject renderable(node, isDraw, L"testObject");
	//renderable.name = L"test";
	//auto objectID1 = renderer->RegisterObject(renderable);

	//flt::RawNode childNode(L"testChildNode");
	//childNode.transform.SetPosition(1.0f, 0.0f, 0.0f);
	//childNode.transform.SetScale(0.5f, 0.5f, 0.5f);
	//childNode.transform.SetParent(&node.transform);

	//flt::RendererObject childRenderable(childNode, isDraw, L"testChildObject");
	//renderable.name = L"testChild";
	//auto objectID2 = renderer->RegisterObject(childRenderable);

	//ASSERT(renderer, "렌더러 생성 실패");

	while (true)
	{
		if (!platform.Update())
		{
			break;
		}

		renderer->Render(1.0f);
		rawScene.nodes[1]->transform.AddRotation({ 0.0f, 1.0f, 0.0f }, 0.01f);
		//transform.AddRotation({ 1.0f, 0.0f, 0.0f }, 0.01f);
		//node.transform.AddRotation({ 0.0f, 1.0f, 0.0f }, -0.1f);
		////transform.AddRotation({ 0.0f, 0.0f, 1.0f }, 0.01f);

		//childNode.transform.AddRotation({ 0.0f, 1.0f, 0.0f }, 0.80f);

		//if (!renderer->Render(deltaTime))
		//{
		//	break;
		//}
		{
			auto keyData = platform.GetKey(flt::KeyCode::z);
			if (keyData)
			{
				std::cout << "z" << keyData.keyTime << std::endl;
			}
			keyData = platform.GetKey(flt::KeyCode::x);
			if (keyData)
			{
				std::cout << "x" << keyData.keyTime << std::endl;
			}
			keyData = platform.GetKey(flt::KeyCode::mouseLButton);
			if (keyData)
			{
				std::cout << "LL " << keyData.keyTime << " " << keyData.x << " " << keyData.y << std::endl;
			}
			keyData = platform.GetKey(flt::KeyCode::mouseRButton);
			if (keyData)
			{
				std::cout << "RR " << keyData.keyTime << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseRelativePos);
			if (keyData)
			{
				std::cout << "diff Pos " << keyData.x << " " << keyData.y << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseWheelUp);
			if (keyData)
			{
				std::cout << "wheel up " << keyData.x << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseWheelDown);
			if (keyData)
			{
				std::cout << "wheel down " << keyData.x << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseButton3);
			if (keyData)
			{
				std::cout << "button3 " << keyData.keyTime << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseButton4);
			if (keyData)
			{
				std::cout << "button4 " << keyData.keyTime << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseMButton);
			if (keyData)
			{
				std::cout << "WheelDown " << keyData.keyTime << std::endl;
			}

			keyData = platform.GetKey(flt::KeyCode::mouseAbsolutePos);
			if (keyData)
			{
				std::cout << "abs Pos " << keyData.x << " " << keyData.y << std::endl;
			}
		}

		Sleep(10);
	}

	renderer->DeregisterObject(objectID0);
	//renderer->DeregisterObject(objectID1);
	//renderer->DeregisterObject(objectID2);
	platform.DestroyRenderer(renderer);
	platform.Finalize();
}
