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

#include "ZnWorld.h"


namespace ZonaiPhysics
{
	ZnPhysicsX* ZnPhysicsX::instance = nullptr;

	void ZnPhysicsX::Initialize(ZnSimulationCallback* _instance) noexcept
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

		defaultMaterial = ZnFactoryX::CreateMaterial(0.5f, 0.5f, 0.6f);
	}

	void ZnPhysicsX::Simulation(float _dt) noexcept
	{
		ZnWorld::Run(_dt);
	}

	void ZnPhysicsX::Finalize() noexcept
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

	// ������ Scene �����͸� key�� PxScene�� �����.
	void ZnPhysicsX::CreateScene(void* _userScene, const Eigen::Vector3f& _gravity) noexcept
	{
		assert(_userScene != nullptr);

		ZnWorld::AddScene(_userScene, ZnFactoryX::CreateScene(_userScene, _gravity));
	}

	void ZnPhysicsX::LoadScene(void* _userScene) noexcept
	{
		assert(_userScene != nullptr);

		ZnWorld::LoadScene(_userScene);
	}

	void ZnPhysicsX::UnloadScene(void* _userScene) noexcept
	{
		assert(_userScene != nullptr);

		ZnWorld::UnloadScene(_userScene);
	}

	void ZnPhysicsX::SetGravity(const Vector3f& _gravity, void* _userScene) noexcept
	{
		ZnWorld::SetGravity(_gravity, _userScene);
	}

	void ZnPhysicsX::SetCollisionLayerData(uint32_t _layer, const std::initializer_list<uint32_t>& _data) noexcept
	{
		ZnLayer::SetCollisionData(_layer, _data);
	}

	/// <summary>
	/// ��ü�� ���� ��ȯ
	/// </summary>
	ZnRigidBody* ZnPhysicsX::CreateRigidBody(void* _userData, void* _userScene) noexcept
	{
		// ������ (������Ʈ) �����͸� Ű������ ���� ������ �Ǿ��ִ��� Ȯ���ϰ�
		// ������ �Ǿ� ������ ��ȯ
		// ������ �Ǿ����� ������ �����ϰ� ��ȯ
		// ������ ���� World�� �߰��� �ϴµ�
		// userScene�� nullptr�̸� ���� Scene�� �߰��ϰ�
		// nullptr�� �ƴϸ� userScene�� �ش��ϴ� Scene�� �߰���.
		const auto znBody = ZnFactoryX::CreateRigidBody(_userData);
		ZnWorld::AddBody(_userScene, znBody);

		return znBody;
	}

	/// <summary>
	/// ��ü�� ã�Ƽ� �ű⿡ �ݶ��̴��� ����.
	/// </summary>
	ZnCollider* ZnPhysicsX::CreateBoxCollider(void* _userData, float _x, float _y, float _z) noexcept
	{
		// userData�� Ű������ rigidbody�� ������ �Ǿ��ִ��� üũ�ϰ�
		// ������ �Ǿ� ������ rigidbody�� collider�� �߰�
		// ������ �Ǿ� ���� ������ rigidbody�� �����ϰ� collider�� �߰�.

		auto collider = ZnFactoryX::CreateBoxCollider(_userData, _x, _y, _z, nullptr);
	}

	ZnCollider* ZnPhysicsX::CreateSphereCollider(void* _userData, float _radius) noexcept
	{
		auto collider = ZnFactoryX::CreateSphereCollider(_userData, _radius, nullptr);
	}

	ZnCollider* ZnPhysicsX::CreateCapsuleCollider(void* _userData, float _radius, float _height) noexcept
	{
		auto collider = ZnFactoryX::CreateCapsuleCollider(_userData, _radius, _height, nullptr);
	}

	// 	ZnCollider* ZnPhysicsX::CreateCustomCollider(const std::wstring&) noexcept
	// 	{
	// 
	// 	}

	ZnFixedJoint* ZnPhysicsX::CreateFixedJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                           ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{


		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateFixedJoint(nullptr, _transform0, nullptr, _transform1);
		return joint;
	}

	ZnDistanceJoint* ZnPhysicsX::CreateDistanceJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                 ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateDistanceJoint(nullptr, _transform0, nullptr, _transform1);

		return joint;
	}

	ZnSphericalJoint* ZnPhysicsX::CreateSphericalJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                   ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateSphericalJoint(nullptr, _transform0, nullptr, _transform1);
		return joint;
	}

	ZnHingeJoint* ZnPhysicsX::CreateHingeJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                           ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateHingeJoint(nullptr, _transform0, nullptr, _transform1);
		return joint;
	}

	ZnPrismaticJoint* ZnPhysicsX::CreatePrismaticJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
	                                                   ZnRigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		auto ob0 = dynamic_cast<RigidBody*>(_object0);
		auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreatePrismaticJoint(nullptr, _transform0, nullptr, _transform1);
		return joint;
	}

	bool ZnPhysicsX::Raycast(const Vector3f& _from, const Vector3f& _to, float _distance, ZnRaycastInfo& _out) noexcept
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
