
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

#include <chrono>
#include <iostream>
#include <foundation/PxMath.h>

#include "ZnCollision.h"
#include "ZnSimulationCallback.h"

class Timer {
private:
	std::chrono::steady_clock::time_point lastTime;

public:
	Timer() {
		lastTime = std::chrono::steady_clock::now();
	}

	// ��ȯ��: �� ������ ��Ÿ Ÿ��
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
	void OnConstraintBreak(ZonaiPhysics::ZnJoint*) override
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
		const ZonaiPhysics::ZnCollision& _collision0, const ZonaiPhysics::ZnCollision& _collision) override
	{
		std::cout << "OnCollisionEnter " << _collision.contactCount << std::endl;
	}
	void OnCollisionStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision& _collision0, const ZonaiPhysics::ZnCollision& _collision) override
	{
		std::cout << "OnCollisionStay " << _collision.contactCount << std::endl;
	}
	void OnCollisionExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision& _collision0, const ZonaiPhysics::ZnCollision& _collision) override
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
			// MessageBox(_hwnd, L"�ش� ��ο� Physics DLL ������ �������� �ʽ��ϴ�.", L"DLL ����", MB_OK | MB_ICONWARNING);
			return false;
		}

		using ImportFunction = ZonaiPhysics::ZnPhysicsBase* (*) ();
		ImportFunction CreateInstance{ reinterpret_cast<ImportFunction>(GetProcAddress(physicsDLL, "CreatePhysics")) };

		if (CreateInstance == nullptr)
		{
			// MessageBox(_hwnd, L"Physics DLL���� �Լ� �����͸� �޾ƿ��� ���߽��ϴ�.", L"DLL ����", MB_OK | MB_ICONWARNING);
			return false;
		}

		physicsEngine = CreateInstance();

		if (physicsEngine == nullptr)
		{
			// MessageBox(_hwnd, L"Graphics Engine ��ü ���� ����", L"DLL ����", NULL);
			return false;
		}
	}

	PhysicsEvent event;

	physicsEngine->Initialize(&event);
	physicsEngine->CreateScene(dscene, { 0.f, -9.81f, 0.f });
	physicsEngine->LoadScene(dscene);

	auto material0 = physicsEngine->CreateMaterial({ 0.4f, 0.5f, 0.1f });
	auto material1 = physicsEngine->CreateMaterial({ 0.7f, 0.7f, 0.2f });
	auto material2 = physicsEngine->CreateMaterial({ 0.7f, 0.7f, 0.9f });

	physicsEngine->SetCollisionLayerData(1, {0, 1, 4, 6, 7, 31});
	physicsEngine->SetCollisionLayerData(0, {1, 4, 6, 7, 31});

	const auto collider = physicsEngine->CreateBoxCollider(drigid, { 2.f, 0.5f, 6.f }, material0, dscene);
	collider->SetLayerData(1);
	const auto rigid = physicsEngine->CreateRigidBody(drigid);
	rigid->SetMaxLinearVelocity(10.f);
	rigid->SetPosition({0.f, 3.f, 0.f});
	// rigid->UseGravity(true);
	rigid->UseGravity(false);

	const auto collider2 = physicsEngine->CreateSphereCollider(drigid2, 2.f, material1, dscene);
	collider2->SetLayerData(1);
	const auto rigid2 = physicsEngine->CreateRigidBody(drigid2);
	rigid2->SetMaxLinearVelocity(10.f);
	rigid2->SetPosition({ 0.f, 15.f, 0.f });
	rigid2->UseGravity(false);

	Eigen::Vector3f axis(1.0f, 1.0f, 1.0f);
	axis.normalize(); // Normalize the axis
	const float angle = physx::PxPi / 4.f; // Angle in radians
	Eigen::Quaternionf q(Eigen::AngleAxisf(angle, axis));

	const auto joint = physicsEngine->CreateFixedJoint(
		rigid2, { {0.f, -0.f, 0.f} },
		rigid, { {0.f, 0.5f, 0.f}, q } );
	// joint->LimitEnable(true);
	// joint->SetLimitCone(physx::PxPi / 4.f, physx::PxPi / 8.f);

	auto groundCollider = physicsEngine->CreateBoxCollider(drigid3, { 1000, 1, 1000 }, material2, dscene);
	const auto ground = physicsEngine->CreateRigidBody(drigid3);
	ground->SetPosition({ 0, 0, 0 });
	ground->SetKinematic(true);

	Timer timer;

	while (true)
	{
		float dt = timer.GetDelta();

		physicsEngine->Simulation(1.f/5000.f);

		if(GetAsyncKeyState(VK_RIGHT))
		{
			rigid->AddForce({ 10.f, 0.f, 0.f });
		}
		if (GetAsyncKeyState(VK_UP))
		{
			rigid->AddForce({ 0.f, 0.f, 10.f });
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			rigid->AddForce({ -10.f, 0.f, 0.f });
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			rigid->AddForce({ 0.f, 0.f, -10.f });
		}
	}

	return 0;
}
