#pragma once
/// �ܺο��� �̺κи� �������� �ϱ� ���ϵ��� ����
#include <Windows.h>
#include "PurahEngineAPI.h"
#include "Component.h"
#include "Transform.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "BoxCollider.h"
#include "SoundManager.h"
#include "TimeController.h"

// Graphics
#include "Camera.h"
#include "DirectionalLight.h"
#include "ModelRenderer.h"
#include "MeshRenderer.h"
#include "Animator.h"

namespace PurahEngine
{
	extern "C" PURAHENGINE_API void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
	extern "C" PURAHENGINE_API void Run(_In_ int nCmdShow);
	extern "C" PURAHENGINE_API void Finalize();
};
