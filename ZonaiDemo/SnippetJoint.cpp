
#include <ctype.h>
#include <windows.h>

#include "Eigen/Dense"
#include "ZnCollider.h"
#include "ZnPhysicsBase.h"
#include "ZnRigidBody.h"


#include "ZnDistanceJoint.h"
#include "ZnFixedJoint.h"
#include "ZnHingeJoint.h"
#include "ZnJoint.h"
#include "ZnPrismaticJoint.h"
#include "ZnSphericalJoint.h"
#include "ZnTransform.h"
#include "../ZonaiMath/ZonaiMath.h"


#include <chrono>
#include <iostream>

#include "ZnCollision.h"
#include "ZnRaycastInfo.h"
#include "ZnSimulationCallback.h"

class Timer {
private:
	std::chrono::steady_clock::time_point lastTime;

public:
	Timer() {
		lastTime = std::chrono::steady_clock::now();
	}

	// 반환값: 초 단위의 델타 타임
	double GetDelta() {
		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> delta = currentTime - lastTime;
		lastTime = currentTime;
		return delta.count();
	}
};

class PhysicsEvent : public ZonaiPhysics::ZnSimulationCallback
{
public:
	void OnWake(const ZonaiPhysics::ZnRigidBody*) override
	{
		std::cout << "OnWake" << std::endl;
	}
	void OnSleep(const ZonaiPhysics::ZnRigidBody*) override
	{
		std::cout << "OnSleep" << std::endl;
	}
	void OnConstraintBreak(const ZonaiPhysics::ZnJoint*) override
	{
		std::cout << "OnConstraintBreak" << std::endl;
	}
	void OnTriggerEnter(const ZonaiPhysics::ZnCollider* _collider0, const ZonaiPhysics::ZnCollider* _collider1) override
	{
		std::cout << "OnTriggerEnter" << std::endl;
	}
	void OnTriggerStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*) override
	{
		std::cout << "OnTriggerStay" << std::endl;
	}
	void OnTriggerExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*) override
	{
		std::cout << "OnTriggerExit" << std::endl;
	}
	void OnCollisionEnter(const ZonaiPhysics::ZnCollider* _collider0, const ZonaiPhysics::ZnCollider* _collider1,
		const ZonaiPhysics::ZnCollision& _collision) override
	{
		std::cout << "OnCollisionEnter " << _collision.contactCount << std::endl;
	}
	void OnCollisionStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision& _collision) override
	{
		std::cout << "OnCollisionStay " << _collision.contactCount << std::endl;
	}
	void OnCollisionExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision& _collision) override
	{
		std::cout << "OnCollisionExit " << _collision.contactCount << std::endl;
	}
};

class DummyObject
{
public:

};

class DummyRigidBody
{
public:
	float c = 3.f;
};

class DummyCollider
{
public:
	float b = 3.f;
};

class DummyScene
{
public:
	int a = 3;
};

int snippetMain(int, const char* const*)
{

	DummyScene* dscene = new DummyScene;
	DummyRigidBody* drigid = new DummyRigidBody;
	DummyRigidBody* drigid2 = new DummyRigidBody;
	DummyRigidBody* drigid3 = new DummyRigidBody;
	DummyCollider* dcoll = new DummyCollider;

	ZonaiPhysics::ZnPhysicsBase* physicsEngine = nullptr;
	{
		std::wstring path = L"../x64/Debug/ZonaiPhysicsX.dll";

		HMODULE physicsDLL = LoadLibraryW(path.c_str());
		if (!physicsDLL)
		{
			// MessageBox(_hwnd, L"해당 경로에 Physics DLL 파일이 존재하지 않습니다.", L"DLL 오류", MB_OK | MB_ICONWARNING);
			return false;
		}

		using ImportFunction = ZonaiPhysics::ZnPhysicsBase* (*) ();
		ImportFunction CreateInstance{ reinterpret_cast<ImportFunction>(GetProcAddress(physicsDLL, "CreatePhysics")) };

		if (CreateInstance == nullptr)
		{
			// MessageBox(_hwnd, L"Physics DLL에서 함수 포인터를 받아오지 못했습니다.", L"DLL 오류", MB_OK | MB_ICONWARNING);
			return false;
		}

		physicsEngine = CreateInstance();

		if (physicsEngine == nullptr)
		{
			// MessageBox(_hwnd, L"Graphics Engine 객체 생성 실패", L"DLL 오류", NULL);
			return false;
		}
	}

	PhysicsEvent event;

	physicsEngine->Initialize(&event);
	
	physicsEngine->CreateScene(dscene, { 0.f, -9.81f, 0.f });
	physicsEngine->LoadScene(dscene);

	physicsEngine->AddMaterial(0, 0.4f, 0.5f, 0.1f);
	physicsEngine->AddMaterial(1, 0.7f, 0.7f, 0.2f);
	physicsEngine->AddMaterial(2, 0.7f, 0.7f, 0.9f);

	physicsEngine->SetCollisionLayerData(1, {0, 1, 4, 6, 7, 31});
	physicsEngine->SetCollisionLayerData(0, {1, 4, 6, 7, 31});

	auto collider = physicsEngine->CreateBoxCollider(drigid, { 2.f, 0.5f, 6.f }, 0, dscene);
	collider->SetLayerData(1);
	const auto rigid = physicsEngine->CreateRigidBody(drigid);
	rigid->SetMaxLinearVelocity(10.f);
	rigid->SetPosition({0.f, 15.f, 0.f});
	rigid->UseGravity(true);

	auto collider2 = physicsEngine->CreateSphereCollider(drigid2, 2.f, 1, dscene);
	// const auto rigid2 = physicsEngine->CreateRigidBody(drigid2);
	collider2->SetPosition({ 0.f, 3.f, 0.f });
	collider2->SetTrigger(true);

	auto groundCollider = physicsEngine->CreateBoxCollider(drigid3, { 1000, 1, 1000 }, 2, dscene);
	const auto ground = physicsEngine->CreateRigidBody(drigid3);
	ground->SetPosition({ 0, 0, 0 });
	ground->SetKinematic(true);

	Timer timer;

	while (true)
	{
		float dt = timer.GetDelta();

		physicsEngine->Simulation(1.f/5000.f);

		bool clickFront = GetAsyncKeyState('I');
		bool clickBack = GetAsyncKeyState('K');
		bool clickLeft = GetAsyncKeyState('J');
		bool clickRight = GetAsyncKeyState('L');
		bool clickUp = GetAsyncKeyState('U');
		bool clickDown = GetAsyncKeyState('O');

		bool KeySpace = GetAsyncKeyState(VK_SPACE);

		if (clickUp || clickRight || clickLeft || clickDown || clickFront || clickBack)
		{
			Eigen::Vector3f nowpos = rigid->GetPosition();
			Eigen::Vector3f movement{ 0.f, 0.f, 0.f };

			if (clickFront)
			{
				movement += Eigen::Vector3f{ 0.f, 0.f, -0.01f };
			}
			if (clickBack)
			{
				movement += Eigen::Vector3f{ 0.f, 0.f, 0.01f };
			}
			if (clickUp)
			{
				movement += Eigen::Vector3f{ 0.f, 0.01f, 0.f };
			}
			if (clickDown)
			{
				movement += Eigen::Vector3f{ 0.f, -0.01f, 0.f };
			}
			if (clickLeft)
			{
				movement += Eigen::Vector3f{ -0.01f, 0.f, 0.f };
			}
			if (clickRight)
			{
				movement += Eigen::Vector3f{ 0.01f, 0.f, 0.f };
			}

			rigid->SetPosition(nowpos + movement);
		}
	}

	return 0;
}
