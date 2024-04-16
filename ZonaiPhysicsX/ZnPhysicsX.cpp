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

		// SDK ����
		{
			ZnFactoryX::CreatePhysxFactory();
		}

		{
			ZnFactoryX::SetSimulationCallback(_instance);
		}

		// ���̾� ����
		{
			ZnLayer::Clear();
			ZnLayer::SetCollisionData(0, {0, 1, 2, 3});
		}

		defaultMaterial = ZnFactoryX::CreateMaterial(0.8f, 0.8f, 0.f, eAVERAGE, eAVERAGE);
	}

	void ZnPhysicsX::Simulation(float _dt)
	{
		ZnWorld::Run(_dt);
	}

	void ZnPhysicsX::Finalize()
	{
		ZnLayer::Clear();
		ZnWorld::Release();
		ZnFactoryX::Release();
	}

	ZnPhysicsX*& ZnPhysicsX::Instance()
	{
		assert(!instance);

		instance = new ZnPhysicsX();

		return instance;
	}

	void ZnPhysicsX::Release()
	{
		assert(instance);

		delete instance;
	}

	void ZnPhysicsX::AddMaterial(uint32_t _id, 
		float staticFriction, float dynamicFriction, float _restitution, 
		eCombineMode _eFriction, eCombineMode _eRestitution)
	{
		const auto material = ZnFactoryX::CreateMaterial(staticFriction, dynamicFriction, _restitution, _eFriction, _eRestitution);

		ZnWorld::AddMaterial(_id, material);
	}

	// ������ Scene �����͸� key�� PxScene�� �����.
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

	Eigen::Vector3f ZnPhysicsX::GetGravity(void* _userScene)
	{
		return ZnWorld::GetGravity(_userScene);
	}

	void ZnPhysicsX::SetGravity(const Eigen::Vector3f& _gravity, void* _userScene)
	{
		ZnWorld::SetGravity(_gravity, _userScene);
	}

	void ZnPhysicsX::SetCollisionLayer(uint32_t _layer, uint32_t _collision, bool _value)
	{
		ZnLayer::SetCollisionLayer(_layer, _collision, _value);
	}

	void ZnPhysicsX::SetCollisionLayerData(uint32_t _layer, const std::initializer_list<uint32_t>& _data)
	{
		ZnLayer::SetCollisionData(_layer, _data);
	}

	/// <summary>
	/// ��ü�� ���� ��ȯ
	/// </summary>
	ZnRigidBody* ZnPhysicsX::CreateRigidBody(void* _userData, void* _userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, _userScene);

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateDynamicRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userData, _userScene);
		}

		ZnWorld::SetHasBody(_userData, true, _userScene);
		znBody->UseGravity(true);
		znBody->SetKinematic(false);

		return znBody;
	}

	/// <summary>
	/// ��ü�� ã�Ƽ� �ű⿡ �ݶ��̴��� ����.
	/// </summary>
	ZnCollider* ZnPhysicsX::CreateBoxCollider(void* _userData, const Eigen::Vector3f& _extend, uint32_t _material, void* _userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, _userScene);

		auto material = ZnWorld::GetMaterial(_material);

		if (!material)
			material = defaultMaterial;

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateDynamicRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userData, _userScene);
			ZnWorld::SetHasBody(_userData, false, _userScene);
			znBody->UseGravity(false);
			znBody->SetKinematic(true);
		}

		const auto collider = ZnFactoryX::CreateBoxCollider(znBody, _extend, material);
		ZnWorld::AddCollider(collider, _userData, _userScene);

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
			znBody = ZnFactoryX::CreateDynamicRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userData, userScene);
			ZnWorld::SetHasBody(_userData, false, userScene);
			znBody->UseGravity(false);
			znBody->SetKinematic(true);
		}

		const auto collider = ZnFactoryX::CreateSphereCollider(znBody, _radius, material);
		ZnWorld::AddCollider(collider, _userData, userScene);

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
			znBody = ZnFactoryX::CreateDynamicRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userData, userScene);
			ZnWorld::SetHasBody(_userData, false, userScene);
			znBody->UseGravity(false);
			znBody->SetKinematic(true);
		}

		const auto collider = ZnFactoryX::CreateCapsuleCollider(znBody, _radius, _height, material);
		ZnWorld::AddCollider(collider, _userData, userScene);

		return collider;
	}

	ZnCollider* ZnPhysicsX::CreateMeshCollider(void* _userData, const std::wstring& _path, uint32_t _material, void* userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, userScene);
		auto material = ZnWorld::GetMaterial(_material);

		if (!material)
			material = defaultMaterial;

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateDynamicRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userData, userScene);
			ZnWorld::SetHasBody(_userData, false, userScene);
			znBody->UseGravity(false);
			znBody->SetKinematic(true);
		}

		// const auto collider = ZnFactoryX::CreateMeshCollider(znBody, material);
		// ZnWorld::AddCollider(collider, _userData, userScene);

		return nullptr;
	}

	ZnCollider* ZnPhysicsX::CreateConvexCollider(void* _userData, const std::wstring& _path, uint32_t _material, void* userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, userScene);
		auto material = ZnWorld::GetMaterial(_material);

		if (!material)
			material = defaultMaterial;

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateDynamicRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userData, userScene);
			ZnWorld::SetHasBody(_userData, false, userScene);
			znBody->UseGravity(false);
			znBody->SetKinematic(true);
		}

		// const auto collider = ZnFactoryX::CreateConvexCollider(znBody, material);

		return nullptr;
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

		ZnWorld::AddJoint(joint);
		return joint;
	}

	ZnDistanceJoint* ZnPhysicsX::CreateDistanceJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                 ZnRigidBody* _object1, const ZnTransform& _transform1)
	{
		const auto ob0 = dynamic_cast<RigidBody*>(_object0);
		const auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateDistanceJoint(ob0, _transform0, ob1, _transform1);

		ZnWorld::AddJoint(joint);
		return joint;
	}

	ZnSphericalJoint* ZnPhysicsX::CreateSphericalJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                   ZnRigidBody* _object1, const ZnTransform& _transform1)
	{
		const auto ob0 = dynamic_cast<RigidBody*>(_object0);
		const auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateSphericalJoint(ob0, _transform0, ob1, _transform1);

		ZnWorld::AddJoint(joint);
		return joint;
	}

	ZnHingeJoint* ZnPhysicsX::CreateHingeJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                           ZnRigidBody* _object1, const ZnTransform& _transform1)
	{
		const auto ob0 = dynamic_cast<RigidBody*>(_object0);
		const auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateHingeJoint(ob0, _transform0, ob1, _transform1);

		ZnWorld::AddJoint(joint);

		return joint;
	}

	ZnPrismaticJoint* ZnPhysicsX::CreatePrismaticJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                   ZnRigidBody* _object1, const ZnTransform& _transform1)
	{
		const auto ob0 = dynamic_cast<RigidBody*>(_object0);
		const auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreatePrismaticJoint(ob0, _transform0, ob1, _transform1);

		ZnWorld::AddJoint(joint);
		return joint;
	}

	bool ZnPhysicsX::Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance, ZnRaycastInfo& _out)
	{
		return ZnWorld::Raycast(_from, _to, _distance, _out);
	}

	void ZnPhysicsX::ReleaseRigidBody(ZnRigidBody* _body, void* _userData, void* _userScene)
	{
		assert(_body != nullptr);
		assert(_userData != nullptr);

		auto rigidbody = static_cast<RigidBody*>(_body);

		ZnWorld::RemoveBody(rigidbody, _userData, _userScene);
	}

	void ZnPhysicsX::ReleaseCollider(ZnCollider* _shape, void* _userData, void* _userScene)
	{
		assert(_shape != nullptr);
		assert(_userData != nullptr);

		auto collider = static_cast<Collider*>(_shape);

		ZnWorld::RemoveCollider(collider, _userData, _userScene);
	}

	void ZnPhysicsX::ReleaseJoint(ZnJoint* _joint, void* _userData, void* _userScene)
	{
		assert(_joint != nullptr);

		ZnWorld::RemoveJoint(_joint, _userData, _userScene);
	}

	extern "C"
	{
		ZnPhysicsBase* CreatePhysics()
		{
			return ZnPhysicsX::Instance();
		}

		void ReleasePhysics()
		{
			ZnPhysicsX::Release();
		}
	}
} // namespace ZonaiPhysics
