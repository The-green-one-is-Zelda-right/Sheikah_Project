#pragma once
/// �ܺο��� �̺κи� �������� �ϱ� ���ϵ��� ����
#include <Windows.h>
#include "PurahEngineAPI.h"
#include "GraphicsSystem.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "BoxCollider.h"

namespace PurahEngine
{
	extern "C" PURAHENGINE_API void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
	extern "C" PURAHENGINE_API void Run(_In_ int nCmdShow);
	extern "C" PURAHENGINE_API void Finalize();
};
