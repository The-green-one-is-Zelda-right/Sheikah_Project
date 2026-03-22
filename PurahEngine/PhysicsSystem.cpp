#include <cassert>
#include "../ZonaiPhysicsBase/ZnPhysicsBase.h"

#include "EventCallbackSystem.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Joint.h"
#include "ZnJoint.h"

#include "Transform.h"
#include "ZnTransform.h"

#include "EngineSetting.h"

#include "PhysicsSystem.h"

#include "GameObject.h"
#include "ZnQueryDesc.h"

namespace PurahEngine
{
	void PhysicsSystem::Initialize()
	{
		ZonaiPhysicsXDLL = LoadLibrary(L"ZonaiPhysicsX.dll");

		if (ZonaiPhysicsXDLL == nullptr)
		{
			// DLL 로드 실패
			PHYSCIS_CAUTUON(PhysicsSystem, DLL not found!)
			assert(0);
		}

		using ImportFunction = ZonaiPhysics::ZnPhysicsBase* (*) ();
		ImportFunction createZonaiPhysics{ reinterpret_cast<ImportFunction>(GetProcAddress(ZonaiPhysicsXDLL, "CreatePhysics")) };


		if (createZonaiPhysics == nullptr)
		{
			// DLL 함수를 찾을 수 없습니다.
			PHYSCIS_CAUTUON(PhysicsSystem, Physcis DLL Initialize Function Load Failed!)
			assert(0);
		}

		releaseFuntion = { reinterpret_cast<void (*) ()>(GetProcAddress(ZonaiPhysicsXDLL, "ReleasePhysics")) };

		if (releaseFuntion == nullptr)
		{
			PHYSCIS_CAUTUON(PhysicsSystem, Physcis DLL Finalize Function Load Failed!)
			assert(0);
		}
		// DLL 함수를 찾을 수 없습니다.

		physics = createZonaiPhysics();

		if (physics == nullptr)
		{
			PHYSCIS_CAUTUON(PhysicsSystem, Physcis Initialize Failed!)
			assert(0);
		}

		callbackSystem = new EventCallbackSystem;

		physics->Initialize(callbackSystem);
		physics->CreateScene(this, {0, -9.81f, 0});
		physics->LoadScene(this);

		const Eigen::Vector3f gravity (0.f, EngineSetting::GetInstance().GetGravity(), 0.f);
		physics->SetGravity(gravity);

		auto LayerData(EngineSetting::GetInstance().GetCollsionSetting());
		for (size_t i = 0; i < LayerData.size(); i++)
		{
			for (size_t j = i; j < LayerData[i].size(); j++)
			{
				physics->SetCollisionLayer(i, j, LayerData[i][j]);
			}
		}

		auto materialData = EngineSetting::GetInstance().GetPhysicsMaterials();
		for (size_t i = 0; i < materialData.size(); i++)
		{
			using namespace ZonaiPhysics;
			auto& [name, sFriction, dFriction, restitution, eFriction, eRestitution] = materialData[i];

			auto id = physics->CreateMaterial({ sFriction, dFriction, restitution, (eCombineMode)eFriction,(eCombineMode)eRestitution});

			if (id == ZnMaterialID::None)
				PHYSCIS_CAUTUON(PhysicsSystem, Material Create Error)

			if (BindMaterial(name, id) == false)
			{
				PHYSCIS_CAUTUON(PhysicsSystem, Material Bind Error)
			}
		}

		//auto preLoadData = EngineSetting::GetInstance().GetPhysicsMaterials();
		//for (size_t i = 0; i < materialData.size(); i++)
		//{
		//	using namespace ZonaiPhysics;
		//	auto& name = std::get<0>(materialData[i]);
		//
		//	auto id = physics->ConvexMeshLoadFromPath(name);
		//
		//	if (id == ZnConvexID::None)
		//		PHYSCIS_CAUTUON(Convex Create Error)
		//
		//		if (BindConvex(name, id) == false)
		//		{
		//			PHYSCIS_CAUTUON(Convex Bind Error)
		//		}
		//}

		//auto preLoadData = EngineSetting::GetInstance().GetPhysicsMaterials();
		//for (size_t i = 0; i < materialData.size(); i++)
		//{
		//	using namespace ZonaiPhysics;
		//	auto& name = std::get<0>(materialData[i]);
		//
		//	auto id = physics->TriangleMeshLoadFromPath(name);
		//
		//	if (id == ZnMeshID::None)
		//		PHYSCIS_CAUTUON(Mesh Create Error)
		//
		//		if (BindConvex(name, id) == false)
		//		{
		//			PHYSCIS_CAUTUON(Mesh Bind Error)
		//		}
		//}
	}

	void PhysicsSystem::PreStep() const
	{
		for (const auto& e : dynamicColliders)
		{
			if (e->GetGameObject()->IsRootEnable())
			{
				e->PreStep();
			}
		}
	}

	void PhysicsSystem::Simulation(float _dt) const
	{
		//static float acc = 0.f;

		//acc += _dt;

		//if (acc >= 1.f / 60.f)
		//{
		//	acc -= 1.f / 60.f;
		//	physics->Simulation(1.f / 60.f);
		//}
		
		physics->Simulation(_dt);
	}

	void PhysicsSystem::SimulateResult() const
	{
		for (const auto& e : bodies)
		{
			if (e->GetGameObject()->IsRootEnable())
			{
				e->SimulateResult();
			}
		}
	}

	void PhysicsSystem::Finalize()
	{
		// 강제 종료 되는 경우 아니면
		// 여기서 컴포넌트를 삭제하는 일은 없을 것임.
		//for (auto& joint : joints)
		//{
		//		delete joint;
		//}

		//joints.clear();

		//for (auto& collider : dynamicColliders)
		//{
		//	delete collider;
		//}

		//dynamicColliders.clear();

		//for (auto& collider : staticColliders)
		//{
		//	delete collider;
		//}

		//staticColliders.clear();

		//for (auto& body : bodies)
		//{
		//	delete body;
		//}

		//bodies.clear();

		physics->Finalize();

		/// Release 함수
		releaseFuntion();

		FreeLibrary(ZonaiPhysicsXDLL);
	}

	Eigen::Vector3f PhysicsSystem::GetGravity() const
	{
		return physics->GetGravity();
	}

	void PhysicsSystem::SetGravity(const Eigen::Vector3f& _gravity) const
	{
		physics->SetGravity(_gravity);
	}

	void PhysicsSystem::FreeObject(ZonaiPhysics::ZnRigidBody* _object, void* _gameObject) const
	{
		assert(_object != nullptr);

		physics->ReleaseRigidBody(_object, _gameObject);
	}

	void PhysicsSystem::FreeObject(ZonaiPhysics::ZnCollider* _object, void* _gameObject) const
	{
		assert(_object != nullptr);

		physics->ReleaseCollider(_object, _gameObject);
	}

	void PhysicsSystem::FreeObject(ZonaiPhysics::ZnJoint* _object, void* _gameObject) const
	{
		assert(_object != nullptr);

		physics->ReleaseJoint(_object, _gameObject);
	}

	ZonaiPhysics::ZnConvexID PhysicsSystem::CreateConvexMeshFromPath(const std::wstring& _path, bool _make)
	{
		ZonaiPhysics::ZnConvexID result = ZonaiPhysics::ZnConvexID::None;

		if (_path.empty())
			return result;

		bool hasSame = convexNameTable.contains(_path);

		// 중복 아님
		if (!hasSame)
		{
			result = physics->ConvexMeshLoadFromPath(_path);

			BindConvex(_path, result, _make);
		}
		// 중복이면 새로 만듬
		else if (hasSame && _make)
		{
			// 기존게 지워짐?
			if (ReleaseConvexMesh(convexNameTable[_path]))
			{
				result = physics->ConvexMeshLoadFromPath(_path);

				BindConvex(_path, result, _make);
			}
			// 못지우면 새로 만드는건 실패한 거임
			else
			{
				result = ZonaiPhysics::ZnConvexID::None;
			}
		}
		// 중복이면 기존 것을 반환함
		else if (hasSame && !_make)
		{
			result = convexNameTable[_path];
		}

		return result;
	}

	ZonaiPhysics::ZnMeshID PhysicsSystem::CreateTriangleMeshFromPath(const std::wstring& _path, bool _make)
	{
		ZonaiPhysics::ZnMeshID result = ZonaiPhysics::ZnMeshID::None;

		if (_path.empty())
			return result;

		bool hasSame = meshNameTable.contains(_path);

		// 중복 아님
		if (!hasSame)
		{
			result = physics->TriangleMeshLoadFromPath(_path);

			BindMesh(_path, result, _make);
		}
		// 중복이면 새로 만듬
		else if (hasSame && _make)
		{
			// 기존게 지워짐?
			if (ReleaseTriangleMesh(meshNameTable[_path]))
			{
				result = physics->TriangleMeshLoadFromPath(_path);

				BindMesh(_path, result, _make);
			}
			// 못지우면 새로 만드는건 실패한 거임
			else
			{
				result = ZonaiPhysics::ZnMeshID::None;
			}
		}
		// 중복이면 기존 것을 반환함
		else if (hasSame && !_make)
		{
			result = meshNameTable[_path];
		}

		return result;
	}

	bool PhysicsSystem::ReleaseConvexMesh(const ZonaiPhysics::ZnConvexID& _id)
	{
		return physics->ReleaseConvexMesh(_id);
	}

	bool PhysicsSystem::ReleaseTriangleMesh(const ZonaiPhysics::ZnMeshID& _id)
	{
		return physics->ReleaseTriangleMesh(_id);
	}

	bool PhysicsSystem::BindMaterial(const std::wstring& _name, const ZonaiPhysics::ZnMaterialID& _id, bool _make)
	{
		bool hasSame = materialNameTable.contains(_name);

		// 중복을 허용하지 않음.
		if (hasSame && !_make)
			return false;

		materialNameTable.insert({ _name, _id });
		return true;
	}

	bool PhysicsSystem::BindConvex(const std::wstring& _name, const ZonaiPhysics::ZnConvexID& _id, bool _make)
	{
		bool hasSame = convexNameTable.contains(_name);

		// 중복을 허용하지 않음.
		if (hasSame && !_make)
			return false;

		convexNameTable.insert({ _name, _id });
		return true;
	}

	bool PhysicsSystem::BindMesh(const std::wstring& _name, const ZonaiPhysics::ZnMeshID& _id, bool _make)
	{
		bool hasSame = meshNameTable.contains(_name);
		
		// 중복을 허용하지 않음.
		if (hasSame && !_make)
			return false;

		// 중복을 허락함.
		meshNameTable.insert({ _name, _id });
		return true;
	}

	ZonaiPhysics::ZnMaterialID PhysicsSystem::GetMaterialID(const std::wstring& _name)
	{
		if (_name.empty() || !materialNameTable.contains(_name))
			return ZonaiPhysics::ZnMaterialID::None;

		return materialNameTable[_name];
	}

	ZonaiPhysics::ZnConvexID PhysicsSystem::GetConvexID(const std::wstring& _name)
	{
		if (_name.empty() || !convexNameTable.contains(_name))
			return ZonaiPhysics::ZnConvexID::None;

		return convexNameTable[_name];
	}

	ZonaiPhysics::ZnMeshID PhysicsSystem::GetMeshID(const std::wstring& _name)
	{
		if (_name.empty() || !meshNameTable.contains(_name))
			return ZonaiPhysics::ZnMeshID::None;

		return meshNameTable[_name];
	}

	void PhysicsSystem::SetColliderMaterial(ZonaiPhysics::ZnCollider* _collider, std::wstring* _materials, int _size)
	{
		std::vector<ZonaiPhysics::ZnMaterialID> materials;

		for (int i = 0; i < _size; i++)
		{
			std::wstring materialName = _materials[i];

			if (materialName.empty() || !materialNameTable.contains(materialName))
				continue;

			ZonaiPhysics::ZnMaterialID id = materialNameTable[materialName];
			materials.emplace_back(id);
		}

		physics->SetColliderMaterial(_collider, materials.front());
	}

	ZonaiPhysics::ZnBoxController* PhysicsSystem::CreateBoxController(void* _gameObject) const
	{
		return nullptr;
	}

	ZonaiPhysics::ZnCapsuleController* PhysicsSystem::CreateCapsuleController(void* _gameObject) const
	{
		return nullptr;
	}

	ZonaiPhysics::ZnRigidBody* PhysicsSystem::CreateRigidBody(void* _gameObject) const
	{
		return physics->CreateRigidBody(_gameObject);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateBoxCollider(
		void* _gameObject, 
		float x, float y, float z, 
		const std::wstring& _materialName)
	{
		auto materialID = GetMaterialID(_materialName);

		return physics->CreateBoxCollider(_gameObject, { x, y, z }, materialID);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateSphereCollider(
		void* _gameObject, 
		float radius, 
		const std::wstring& _materialName)
	{
		auto materialID = GetMaterialID(_materialName);

		return physics->CreateSphereCollider(_gameObject, radius, materialID);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateCapsuleCollider(
		void* _gameObject, 
		float radius, float height, 
		const std::wstring& _materialName)
	{
		auto materialID = GetMaterialID(_materialName);

		return physics->CreateCapsuleCollider(_gameObject, radius, height, materialID);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateConvexCollider(
		void* _gameObject, 
		const std::wstring& _path, 
		const Eigen::Vector3f& _scale, 
		const std::wstring& _materialName)
	{
		auto materialID = GetMaterialID(_materialName);
		auto convexID = GetConvexID(_path);

		if (convexID == nullptr)
		{
			convexID = physics->ConvexMeshLoadFromPath(_path);
			NULL_POINTER_REFERENCE(convexID, PhysicsSystem, Convex Collider Load Error)
		}

		return physics->CreateConvexCollider(_gameObject, convexID, _scale, materialID);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateMeshCollider(
		void* _gameObject, 
		const std::wstring& _path, 
		const Eigen::Vector3f& _scale, 
		const std::wstring& _materialName)
	{
		auto materialID = GetMaterialID(_materialName);
		auto meshid = GetMeshID(_path);

		if (meshid == nullptr)
		{
			meshid = physics->TriangleMeshLoadFromPath(_path);
			NULL_POINTER_REFERENCE(meshid, PhysicsSystem, MeshCollider Load Error)
		}

		return physics->CreateMeshCollider(_gameObject, meshid, _scale, materialID);
	}

	ZonaiPhysics::ZnFixedJoint* PhysicsSystem::CreateFixedJoint(
		ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0,
		ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const
	{
		assert(_body0 != nullptr);

		return physics->CreateFixedJoint(_body0, _localTm0, _body1, _localTm1);
	}

	ZonaiPhysics::ZnPrismaticJoint* PhysicsSystem::CreateSlideJoint(
		ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0,
		ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const
	{
		assert(_body0 != nullptr);

		return physics->CreatePrismaticJoint(_body0, _localTm0, _body1, _localTm1);
	}

	ZonaiPhysics::ZnHingeJoint* PhysicsSystem::CreateHingeJoint(
		ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0,
		ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const
	{
		assert(_body0 != nullptr);

		return physics->CreateHingeJoint(_body0, _localTm0, _body1, _localTm1);
	}

	ZonaiPhysics::ZnSphericalJoint* PhysicsSystem::CreateBallJoint(
		ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0,
		ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const
	{
		assert(_body0 != nullptr);

		return physics->CreateSphericalJoint(_body0, _localTm0, _body1, _localTm1);
	}

	ZonaiPhysics::ZnDistanceJoint* PhysicsSystem::CreateDistanceJoint(
		ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0, 
		ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const
	{
		assert(_body0 != nullptr);

		return physics->CreateDistanceJoint(_body0, _localTm0, _body1, _localTm1);
	}

	ZonaiPhysics::ZnDistanceJoint* PhysicsSystem::CreateSpringJoint(
		ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0,
		ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const
	{
		assert(_body0 != nullptr);

		return physics->CreateDistanceJoint(_body0, _localTm0, _body1, _localTm1);
	}

	bool PhysicsSystem::Raycast(
		const ZonaiPhysics::ZnQueryDesc& _desc, ZonaiPhysics::ZnQueryInfo& _out)
	{
		return physics->Raycast(_desc, _out);
	}

	bool PhysicsSystem::Boxcast(const Eigen::Vector3f _extend, const ZonaiPhysics::ZnQueryDesc& _desc,
		ZonaiPhysics::ZnQueryInfo& _out)
	{
		return physics->Boxcast(_extend, _desc, _out);
	}

	bool PhysicsSystem::Spherecast(float _radius, const ZonaiPhysics::ZnQueryDesc& _desc,
		ZonaiPhysics::ZnQueryInfo& _out)
	{
		return physics->Spherecast(_radius, _desc, _out);
	}

	bool PhysicsSystem::Capsulecast(float _radius, float _height, const ZonaiPhysics::ZnQueryDesc& _desc,
		ZonaiPhysics::ZnQueryInfo& _out)
	{
		return physics->Capsulecast(_radius, _height, _desc, _out);
	}

	bool PhysicsSystem::SphereOverlap(float _radius, const ZonaiPhysics::ZnQueryDesc& _desc, ZonaiPhysics::ZnQueryInfo& _out)
	{
		return physics->SphereOverlap(_radius, _desc, _out);
	}

	PurahEngine::PhysicsSystem& PurahEngine::PhysicsSystem::GetInstance()
	{
		static PhysicsSystem instance;
		return instance;
	}
}
