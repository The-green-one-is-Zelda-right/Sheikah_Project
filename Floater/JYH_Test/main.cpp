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

#include <unordered_map>
#include <filesystem>
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

int main()
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;

#pragma region 테스트
	{
		using namespace flt;
		using namespace flt::test;

		ModelLoader loader;
		std::wstring filePath = L"..\\x64\\fbx\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Ganondorf (TotK).fbx";

		std::filesystem::path currPath = std::filesystem::current_path();
		if(std::filesystem::exists(filePath))
			loader.Load(filePath);
	}
#pragma endregion

	auto adpaterTest = new IZeldaRendererAdapter();
	adpaterTest->Initialize(1280, 720, false, NULL, false, 0.0f, 0.0f);

	flt::Platform platform{ true };
	platform.Initialize(1280, 720, L"title", L".\\path");

	auto renderer = platform.CreateRenderer(flt::RendererType::DX11);

	bool isDraw = true;
	flt::RawNode node(L"testNode");
	node.transform.SetPosition(0.0f, 0.0f, 0.7f);
	node.transform.SetScale(0.3f, 0.3f, 0.3f);

	flt::RendererObject renderable(node, isDraw, L"testObject");
	renderable.name = L"test";
	renderer->RegisterObject(renderable);

	flt::RawNode childNode(L"testChildNode");
	childNode.transform.SetPosition(1.0f, 0.0f, 0.0f);
	childNode.transform.SetScale(0.5f, 0.5f, 0.5f);
	childNode.transform.SetParent(&node.transform);

	flt::RendererObject childRenderable(childNode, isDraw, L"testChildObject");
	renderable.name = L"testChild";
	renderer->RegisterObject(childRenderable);

	//ASSERT(renderer, "렌더러 생성 실패");

	while (true)
	{
		if (!platform.Update())
		{
			break;
		}

		renderer->Render(1.0f);
		//transform.AddRotation({ 1.0f, 0.0f, 0.0f }, 0.01f);
		node.transform.AddRotation({ 0.0f, 1.0f, 0.0f }, -0.1f);
		//transform.AddRotation({ 0.0f, 0.0f, 1.0f }, 0.01f);

		childNode.transform.AddRotation({ 0.0f, 1.0f, 0.0f }, 0.80f);

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

	renderer->DeregisterObject(renderable);
	platform.Finalize();
}
