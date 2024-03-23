#pragma once
/// 외부에는 이부분만 보여지게 하기 위하도록 설계
#include <Windows.h>
#include "PurahEngineAPI.h"

#include "Serializable.h"

// Object Base
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "AudioListener.h"
#include "AudioSource.h"

// Manager
#include "SceneManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "GamePadManager.h"
#include "TimeController.h"

// Physics
#include "RigidBody.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "FixedJoint.h"
#include "HingeJoint.h"
#include "BallJoint.h"
#include "DistanceJoint.h"
#include "SpringJoint.h"
#include "SlideJoint.h"

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
