#include "RigidBody.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

#include "FixedJoint.h"
#include "DistanceJoint.h"
#include "HingeJoint.h"
#include "PrismaticJoint.h"
#include "SphericalJoint.h"
#include "SpringFlexJoint.h"

#include "ZnQueryInfo.h"
#include "FilterCallback.h"

#include "CapsuleController.h"
#include "BoxController.h"

#include "ZnPhysicsX.h"
#include <ranges>

#include "ResourceManager.h"
#include "ZnFactoryX.h"
#include "ZnWorld.h"
#include "ZnMaterial.h"
#include "ZnUtil.h"


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

		defaultMaterial = CreateMaterial({ 0.5f, 0.5f, 0.2f, eAVERAGE, eAVERAGE });
	}

	void ZnPhysicsX::Simulation(float _dt)
	{
		ZnWorld::Run(_dt);
		// 콜벡 호출하는 위치
		EventCallback::SimulationEventCallback();
	}

	void ZnPhysicsX::Finalize()
	{
		ZnLayer::Clear();
		ZnWorld::Release();
		ResourceManager::Release();
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

	ZnMaterialID ZnPhysicsX::CreateMaterial(const MaterialDesc& _desc)
	{
		const auto material = ZnFactoryX::CreateMaterial(_desc);
		ZnMaterialID result = ZnMaterialID::None;

		if (material)
			result = ResourceManager::RegistMaterial(material);

		return result;
	}

	ZnConvexID ZnPhysicsX::ConvexMeshLoadFromPath(const std::wstring& _path)
	{
		auto model = ResourceManager::LoadConvex(_path);
		
		if (model == nullptr)
		{
			ZONAI_CAUTUON(Create Zonai Convex Failed, Zonai Physics);
			return ZnConvexID::None;
		}

		auto pxMesh = ZnFactoryX::CookConvexMesh(model);
		ResourceManager::UnloadModel(model);

		return ResourceManager::AddConvex(pxMesh);
	}

	ZnMeshID ZnPhysicsX::TriangleMeshLoadFromPath(const std::wstring& _path)
	{
		auto model = ResourceManager::LoadMesh(_path);

		if (model == nullptr)
		{
			ZONAI_CAUTUON(Create Zonai TriangleMesh Failed, Zonai Physics);
			return ZnMeshID::None;
		}

		auto pxMesh = ZnFactoryX::CookTriagleMesh(model);
		ResourceManager::UnloadModel(model);

		return 	ResourceManager::AddMesh(pxMesh);
	}

	bool ZnPhysicsX::ReleaseMaterial(const ZnMaterialID& _id)
	{
		return ResourceManager::ReleasePxMaterial(_id);
	}

	bool ZnPhysicsX::ReleaseConvexMesh(const ZnConvexID& _id)
	{
		return ResourceManager::ReleaseConvexMesh(_id);
	}

	bool ZnPhysicsX::ReleaseTriangleMesh(const ZnMeshID& _id)
	{
		return ResourceManager::ReleaseTriangleMesh(_id);
	}

	// 유저의 Scene 포인터를 key로 PxScene을 만든다.
	void ZnPhysicsX::CreateScene(void* _userScene, const Eigen::Vector3f& _gravity)
	{
		NULL_POINTER_REFERENCE(_userScene, Load Scene Error!);
		assert(_userScene != nullptr);

		ZnWorld::AddScene(_userScene, ZnFactoryX::CreateScene(_userScene, _gravity));
	}

	void ZnPhysicsX::LoadScene(void* _userScene)
	{
		NULL_POINTER_REFERENCE(_userScene, Load Scene Error!);
		assert(_userScene != nullptr);

		ZnWorld::LoadScene(_userScene);
	}

	void ZnPhysicsX::UnloadScene(void* _userScene)
	{
		NULL_POINTER_REFERENCE(_userScene, Unload Scene Error!);
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

	void ZnPhysicsX::SetColliderMaterial(ZonaiPhysics::ZnCollider* _col, const ZonaiPhysics::ZnMaterialID& _id)
	{
		Collider* col = static_cast<Collider*>(_col);

		col->SetMaterial(_id);
	}

	void ZnPhysicsX::SetColliderMaterials(ZonaiPhysics::ZnCollider* _col, ZonaiPhysics::ZnMaterialID* _idList, int _size)
	{
		Collider* col = static_cast<Collider*>(_col);

		col->SetMaterials(_idList, _size);
	}

	ZonaiPhysics::ZnCapsuleController* ZnPhysicsX::CreateCapsuleController(
		void* _userScene,
		float _radius, float _height, const ZnControllerDecs& _desc, float _density, const ZnMaterialID& _material)
	{
		physx::PxScene* scene = ZnWorld::GetScene(_userScene);

		if (scene != nullptr)
		{
			scene = ZnWorld::GetCurrentScene();
		}
		
		physx::PxControllerManager* manager = ZnWorld::GetManager(scene);

		if (manager != nullptr)
		{
			manager = ZnFactoryX::CreateControllerManager(scene);
		}

		physx::PxMaterial* material = nullptr;

		if (_material == ZnMaterialID::None)
		{
			material = ResourceManager::GetPxMaterial(defaultMaterial);
		}
		else
		{
			material = ResourceManager::GetPxMaterial(_material);

			if (!material)
				material = ResourceManager::GetPxMaterial(defaultMaterial);
		}

		ZnFactoryX::CreateCapsuleController(manager, _radius, _height, _desc, _density, material);
	
		// ZnCapsuleController* result = new CapsuleController;
		return nullptr;
	}

	ZonaiPhysics::ZnBoxController* ZnPhysicsX::CreateBoxController(
		void* _userScene,
		const Eigen::Vector3f& _extend, const ZnControllerDecs& _desc, float _density, const ZnMaterialID& _material)
	{
		return nullptr;
	}

	/// <summary>
	/// 강체를 만들어서 반환
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
	/// 강체를 찾아서 거기에 콜라이더를 붙임.
	/// </summary>
	ZnCollider* ZnPhysicsX::CreateBoxCollider(
		void* _userData, 
		const Eigen::Vector3f& _extend, 
		const ZnMaterialID& _material,
		void* _userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, _userScene);

		physx::PxMaterial* material = nullptr;

		if (_material == ZnMaterialID::None)
		{
			ZONAI_CAUTUON(Material is Null, Zonai Physics Error);
			material = ResourceManager::GetPxMaterial(defaultMaterial);
		}
		else
		{
			material = ResourceManager::GetPxMaterial(_material);

			if (!material)
				material = ResourceManager::GetPxMaterial(defaultMaterial);
		}

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateDynamicRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userData, _userScene);
			ZnWorld::SetHasBody(_userData, false, _userScene);
			znBody->UseGravity(false);
			znBody->SetKinematic(true);
		}

		const auto shape = ZnFactoryX::CreateBoxShape(_extend, material);
		const auto collider = ZnFactoryX::CreateZnCollider<BoxCollider>(znBody, shape);

		if (collider)
		{
			ZnWorld::AddCollider(collider, _userData, _userScene);
		}
		else
		{
			OutputDebugStringW(L"Zonai Collider Initialize Error");
		}

		return collider;
	}

	ZnCollider* ZnPhysicsX::CreateSphereCollider(
		void* _userData, 
		float _radius, 
		const ZnMaterialID& _material,
		void* userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, userScene);

		physx::PxMaterial* material = nullptr;

		if (_material == ZnMaterialID::None)
		{
			ZONAI_CAUTUON(Material is Null, Zonai Physics Error);
			material = ResourceManager::GetPxMaterial(defaultMaterial);
		}
		else
		{
			material = ResourceManager::GetPxMaterial(_material);

			if (!material)
				material = ResourceManager::GetPxMaterial(defaultMaterial);
		}

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateDynamicRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userData, userScene);
			ZnWorld::SetHasBody(_userData, false, userScene);
			znBody->UseGravity(false);
			znBody->SetKinematic(true);
		}

		const auto shape = ZnFactoryX::CreateSphereShape(_radius, material);
		const auto collider = ZnFactoryX::CreateZnCollider<SphereCollider>(znBody, shape);
		if (collider)
		{
			ZnWorld::AddCollider(collider, _userData, userScene);
		}
		else
		{
			OutputDebugStringW(L"Zonai Collider Initialize Error");
		}

		return collider;
	}

	ZnCollider* ZnPhysicsX::CreateCapsuleCollider(
		void* _userData, 
		float _radius, float _height, 
		const ZnMaterialID& _material, 
		void* userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, userScene);

		physx::PxMaterial* material = nullptr;

		if (_material == ZnMaterialID::None)
		{
			ZONAI_CAUTUON(Material is Null, Zonai Physics Error);
			material = ResourceManager::GetPxMaterial(defaultMaterial);
		}
		else
		{
			material = ResourceManager::GetPxMaterial(_material);

			if (!material)
				material = ResourceManager::GetPxMaterial(defaultMaterial);
		}

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateDynamicRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userData, userScene);
			ZnWorld::SetHasBody(_userData, false, userScene);
			znBody->UseGravity(false);
			znBody->SetKinematic(true);
		}

		const auto shape = ZnFactoryX::CreateCapsuleShape(_radius, _height, material);
		const auto collider = ZnFactoryX::CreateZnCollider<CapsuleCollider>(znBody, shape);

		if (collider)
		{
			ZnWorld::AddCollider(collider, _userData, userScene);
		}
		else
		{
			OutputDebugStringW(L"Zonai Collider Initialize Error");
		}

		return collider;
	}

	ZnCollider* ZnPhysicsX::CreateMeshCollider(
		void* _userData, 
		const ZnMeshID& _id,
		const Eigen::Vector3f& _scale,
		const ZnMaterialID& _material, 
		void* userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, userScene);

		physx::PxMaterial* material = nullptr;

		if (_material == ZnMaterialID::None)
		{
			ZONAI_CAUTUON(Material is Null, Zonai Physics Error);
			material = ResourceManager::GetPxMaterial(defaultMaterial);
		}
		else
		{
			material = ResourceManager::GetPxMaterial(_material);

			if (!material)
				material = ResourceManager::GetPxMaterial(defaultMaterial);
		}

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateDynamicRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userData, userScene);
			ZnWorld::SetHasBody(_userData, false, userScene);
			znBody->UseGravity(false);
			znBody->SetKinematic(true);
		}

		const auto rawShape = ResourceManager::GetPxMeshShape(_id);

		const auto shape = ZnFactoryX::CreateTriagleMeshShape(rawShape, _scale, material);
		const auto collider = ZnFactoryX::CreateZnCollider<MeshCollider>(znBody, shape);

		if (collider)
		{
			ZnWorld::AddCollider(collider, _userData, userScene);
		}
		else
		{
			OutputDebugStringW(L"Zonai Collider Initialize Error");
		}

		return collider;
	}

	ZnCollider* ZnPhysicsX::CreateConvexCollider(
		void* _userData, 
		const ZnConvexID& _id,
		const Eigen::Vector3f& _scale,
		const ZnMaterialID& _material,
		void* userScene)
	{
		auto znBody = ZnWorld::GetBody(_userData, userScene);

		physx::PxMaterial* material = nullptr;

		if (_material == ZnMaterialID::None)
		{
			ZONAI_CAUTUON(Material is Null, Zonai Physics Error);
			material = ResourceManager::GetPxMaterial(defaultMaterial);
		}
		else
		{
			material = ResourceManager::GetPxMaterial(_material);

			if (!material)
				material = ResourceManager::GetPxMaterial(defaultMaterial);
		}

		if (!znBody)
		{
			znBody = ZnFactoryX::CreateDynamicRigidBody(_userData);
			ZnWorld::AddBody(znBody, _userData, userScene);
			ZnWorld::SetHasBody(_userData, false, userScene);
			znBody->UseGravity(false);
			znBody->SetKinematic(true);
		}

		const auto rawShape = ResourceManager::GetPxConvexShape(_id);

		const auto shape = ZnFactoryX::CreateConvexMeshShape(rawShape, _scale, material);
		const auto collider = ZnFactoryX::CreateZnCollider<ConvexCollider>(znBody, shape);
		
		if (collider)
		{
			ZnWorld::AddCollider(collider, _userData, userScene);
		}
		else
		{
			OutputDebugStringW(L"Zonai Collider Initialize Error");
		}

		return collider;
	}

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

	ZnSpringFlexJoint* ZnPhysicsX::CreateSpringFlexJoint(ZnRigidBody* _object0, const ZnTransform& _transform0,
														ZnRigidBody* _object1, const ZnTransform& _transform1)
	{
		const auto ob0 = dynamic_cast<RigidBody*>(_object0);
		const auto ob1 = dynamic_cast<RigidBody*>(_object1);

		const auto joint = ZnFactoryX::CreateSpringFlexJoint(ob0, _transform0, ob1, _transform1);

		ZnWorld::AddJoint(joint);
		return joint;
	}

	bool ZnPhysicsX::Raycast(const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		return ZnWorld::Raycast(_desc, _out);
	}

	bool ZnPhysicsX::Boxcast(const Eigen::Vector3f& _extend, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		return ZnWorld::Boxcast(_extend, _desc, _out);
	}

	bool ZnPhysicsX::Spherecast(float _radius, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		return ZnWorld::Spherecast(_radius, _desc, _out);
	}

	bool ZnPhysicsX::Capsulecast(float _radius, float _height, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		return ZnWorld::Capsulecast(_radius, _height, _desc, _out);
	}

	bool ZnPhysicsX::SphereOverlap(float _radius, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		return ZnWorld::SphereOverLap(_radius, _desc, _out);
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
