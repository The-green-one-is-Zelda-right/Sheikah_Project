#include "RigidBody.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

#include "FixedJoint.h"
#include "DistanceJoint.h"
#include "HingeJoint.h"
#include "PrismaticJoint.h"
#include "SphericalJoint.h"

#include "ZnRaycastInfo.h"
#include "FilterCallback.h"

#include "ZnPhysicsX.h"
#include <ranges>

#include "ZnFactoryX.h"
#include "ZnWorld.h"


namespace ZonaiPhysics
{
	ZnPhysicsX* ZnPhysicsX::instance = nullptr;

	void ZnPhysicsX::Initialize(ZnSimulationCallback* _instance)
	{
		assert(_instance);

		// SDK 생성
		{
			ZnFactoryX::CreatePhysxFactory();
		}

		{
			ZnFactoryX::SetSimulationCallback(_instance);
		}

		// 레이어 설정
		{
			ZnLayer::Clear();
			ZnLayer::SetCollisionData(0, {0, 1, 2, 3});
		}

		defaultMaterial = ZnFactoryX::CreateMaterial(0.5f, 0.5f, 0.6f);
	}

	void ZnPhysicsX::Simulation(float _dt)
	{
		ZnWorld::Run(_dt);
	}

	void ZnPhysicsX::Finalize()
	{
		ZnWorld::Release();
		ZnFactoryX::Release();
	}

	ZnPhysicsX* ZnPhysicsX::Instance()
	{
		assert(!instance);

		instance = new ZnPhysicsX();

		return instance;
	}

	void ZnPhysicsX::AddMaterial(uint32_t _id, float staticFriction, float dynamicFriction, float _restitution)
	{
		const auto material = ZnFactoryX::CreateMaterial(staticFriction, dynamicFriction, _restitution);

		ZnWorld::AddMaterial(_id, material);
	}

	// 유저의 Scene 포인터를 key로 PxScene을 만든다.
	void ZnPhysicsX::CreateScene(void* _userScene, const Eigen::Vector3f& _gravity)
	{
		assert(_userScene != nullptr);

		ZnWorld::AddScene(_userScene, ZnFactoryX::CreateScene(_userScene, _gravity));
	}

	void ZnPhysicsX::LoadScene(void* _userScene)
	{
		assert(_userScene != nullptr);

		ZnWorld::LoadScene(_userScene);
	}

	void ZnPhysicsX::UnloadScene(void* _userScene)
	{
		assert(_userScene != nullptr);

		ZnWorld::UnloadScene(_userScene);
	}

	void ZnPhysicsX::SetGravity(const Eigen::Vector3f& _gravity, void* _userScene)
	{
		ZnWorld::SetGravity(_gravity, _userScene);
	}

	void ZnPhysicsX::SetCollisionLayerData(uint32_t _layer, const std::initializer_list<uint32_t>& _data)
	{
		ZnLayer::SetCollisionData(_layer, _data);
	}

	/// <summary>
	/// 강체를 만들어서 반환
	/// </summary>
	ZnRigidBody* ZnPhysicsX::CreateRigidBody(void* _userData, void* _userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, _userScene);

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userScene);
		}
		znBody->UseGravity(true);
		znBody->SetKinematic(false);

		return znBody;
	}

	/// <summary>
	/// 강체를 찾아서 거기에 콜라이더를 붙임.
	/// </summary>
	ZnCollider* ZnPhysicsX::CreateBoxCollider(void* _userData, const Eigen::Vector3f& _extend, uint32_t _material, void* userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, userScene);

		auto material = ZnWorld::GetMaterial(_material);

		if (!material)
			material = defaultMaterial;

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateRigidBody(_userData);
			ZnWorld::AddBody(znBody, userScene);
		}

		const auto collider = ZnFactoryX::CreateBoxCollider(znBody, _extend, material);
		znBody->UseGravity(false);
		znBody->SetKinematic(true);

		return collider;
	}

	ZnCollider* ZnPhysicsX::CreateSphereCollider(void* _userData, float _radius, uint32_t _material, void* userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, userScene);
		auto material = ZnWorld::GetMaterial(_material);

		if (!material)
			material = defaultMaterial;

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateRigidBody(_userData);
			ZnWorld::AddBody(znBody, userScene);
		}

		const auto collider = ZnFactoryX::CreateSphereCollider(znBody, _radius, material);
		znBody->UseGravity(false);
		znBody->SetKinematic(true);

		return collider;
	}

	ZnCollider* ZnPhysicsX::CreateCapsuleCollider(void* _userData, float _radius, float _height, uint32_t _material, void* userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, userScene);
		auto material = ZnWorld::GetMaterial(_material);

		if (!material)
			material = defaultMaterial;

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateRigidBody(_userData);
			ZnWorld::AddBody(znBody, userScene);
		}

		const auto collider = ZnFactoryX::CreateCapsuleCollider(znBody, _radius, _height, material);
		znBody->UseGravity(false);
		znBody->SetKinematic(true);

		return collider;
	}

	// 	ZnCollider* ZnPhysicsX::CreateCustomCollider(const std::wstring&)
	// 	{
	// 
	// 	}

	ZnFixedJoint* ZnPhysicsX::CreateFixedJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                           ZnRigidBody* _object1, const ZnTransform& _transform1)
	{
		const auto ob0 = dynamic_cast<RigidBody*>(_object0);
		const auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateFixedJoint(ob0, _transform0, ob1, _transform1);
		return joint;
	}

	ZnDistanceJoint* ZnPhysicsX::CreateDistanceJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                 ZnRigidBody* _object1, const ZnTransform& _transform1)
	{
		const auto ob0 = dynamic_cast<RigidBody*>(_object0);
		const auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateDistanceJoint(ob0, _transform0, ob1, _transform1);
		return joint;
	}

	ZnSphericalJoint* ZnPhysicsX::CreateSphericalJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                   ZnRigidBody* _object1, const ZnTransform& _transform1)
	{
		const auto ob0 = dynamic_cast<RigidBody*>(_object0);
		const auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateSphericalJoint(ob0, _transform0, ob1, _transform1);
		return joint;
	}

	ZnHingeJoint* ZnPhysicsX::CreateHingeJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                           ZnRigidBody* _object1, const ZnTransform& _transform1)
	{
		const auto ob0 = dynamic_cast<RigidBody*>(_object0);
		const auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateHingeJoint(ob0, _transform0, ob1, _transform1);
		return joint;
	}

	ZnPrismaticJoint* ZnPhysicsX::CreatePrismaticJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                   ZnRigidBody* _object1, const ZnTransform& _transform1)
	{
		const auto ob0 = dynamic_cast<RigidBody*>(_object0);
		const auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreatePrismaticJoint(ob0, _transform0, ob1, _transform1);
		return joint;
	}

	bool ZnPhysicsX::Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance, ZnRaycastInfo& _out)
	{
		return ZnWorld::Raycast(_from, _to, _distance, _out);
	}

	extern "C" {
	ZnPhysicsBase* CreatePhysics()
	{
		return ZnPhysicsX::Instance();
	}
	}
} // namespace ZonaiPhysics
