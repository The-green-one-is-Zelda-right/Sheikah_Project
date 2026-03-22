#include <PxShape.h>

#include "ZnLayer.h"
#include "ZnUtil.h"
#include "ZnBound3.h"
#include "ZnTransform.h"

#include "ResourceManager.h"

#include "ColliderHelper.h"

namespace ZonaiPhysics
{
	void ColliderHelper::Release(void* _shape)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape**>(_shape);
		(*_pxShape)->release();
		*_pxShape = nullptr;
	}

	void ColliderHelper::DisableQuery(void* _shape, bool _value)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);

		_pxShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, !_value);
	}

	void ColliderHelper::DisableSimulation(void* _shape, bool _value)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);

		if (_value)
			_pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, !_value);
		
		_pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, _value);
	}

	void ColliderHelper::SetTrigger(void* _shape, bool _value)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);

		if (_value)
			_pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !_value);

		_pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, _value);
	}

	void ColliderHelper::SetLayer(void* _shape, uint32_t _layer)
	{
		assert(_shape != nullptr);
		assert(ZnLayer::MIN_LAYER <= _layer && _layer <= ZnLayer::MAX_LAYER);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		_pxShape->setSimulationFilterData({ _layer, _layer, _layer, _layer });
		physx::PxU32 layerData = 1 << _layer;
		_pxShape->setQueryFilterData({ layerData, layerData, layerData, layerData });
	}

	Eigen::Vector3f ColliderHelper::GetLocalPosition(void* _shape)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		return PhysxToEigen(_pxShape->getLocalPose().p);
	}

	void ColliderHelper::SetLocalPosition(void* _shape, const Eigen::Vector3f& _pos)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		physx::PxTransform t = _pxShape->getLocalPose();
		t.p = EigenToPhysx(_pos);
		_pxShape->setLocalPose(t);
	}

	Eigen::Quaternionf ColliderHelper::GetLocalQuaternion(void* _shape)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		return PhysxToEigen(_pxShape->getLocalPose().q);
	}

	void ColliderHelper::SetLocalQuaternion(void* _shape, const Eigen::Quaternionf& _quat)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		physx::PxTransform t = _pxShape->getLocalPose();
		t.q = EigenToPhysx(_quat);
		_pxShape->setLocalPose(t);
	}

	Eigen::Vector3f ColliderHelper::GetGlobalPosition(void* _shape)
	{
		using namespace physx;

		assert(_shape != nullptr);

		const auto pxShape = static_cast<physx::PxShape*>(_shape);
		const auto pxBody = pxShape->getActor();
		PxVec3 parentPos = pxBody->getGlobalPose().p;
		PxQuat parentRot = pxBody->getGlobalPose().q;
		PxVec3 localPos = pxShape->getLocalPose().p;

		return PhysxToEigen(parentPos + parentRot.rotate(localPos));
	}

	void ColliderHelper::SetGlobalPosition(void* _shape, const Eigen::Vector3f& _pos)
	{
		assert(_shape != nullptr);

		//const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		//physx::PxTransform t = _pxShape->getActor()->getGlobalPose();
		//t.p = EigenToPhysx(_pos);
		//_pxShape->getActor()->setGlobalPose(t);
	}

	Eigen::Quaternionf ColliderHelper::GetGlobalQuaternion(void* _shape)
	{
		using namespace physx;

		assert(_shape != nullptr);

		const auto pxShape = static_cast<physx::PxShape*>(_shape);
		const auto pxBody = pxShape->getActor();
		PxQuat parentRot = pxBody->getGlobalPose().q;
		PxQuat localRot = pxShape->getLocalPose().q;

		return PhysxToEigen(parentRot * localRot);
	}

	void ColliderHelper::SetGlobalQuaternion(void* _shape, const Eigen::Quaternionf& _quat)
	{
		assert(_shape != nullptr);

		//const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		//physx::PxTransform t = _pxShape->getActor()->getGlobalPose();
		//t.q = EigenToPhysx(_quat);
		//_pxShape->getActor()->setGlobalPose(t);
	}

	ZonaiPhysics::ZnBound3 ColliderHelper::GetBoundingBox(void* _shape, const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot)
	{
		// 입력된 void* 포인터를 physx::PxShape*로 변환합니다.
		const auto pxShape = static_cast<physx::PxShape*>(_shape);

		// PxShape에서 형상을 가져옵니다.
		const physx::PxGeometry& geom = pxShape->getGeometry();

		// Eigen 형식의 위치와 회전을 PhysX 형식으로 변환합니다.
		physx::PxTransform pose(EigenToPhysx(_pos), EigenToPhysx(_rot));

		// 경계 상자를 저장할 객체를 생성합니다.
		physx::PxBounds3 aabb;

		// 주어진 형상과 변환(위치 및 회전)에 대해 경계 상자를 계산합니다.
		physx::PxGeometryQuery::computeGeomBounds(aabb, geom, pose, 0.1f);

		// PhysX 형식의 경계 상자를 Eigen 형식으로 변환하여 반환합니다.
		return ZnBound3(PhysxToEigen(aabb.minimum), PhysxToEigen(aabb.maximum));
	}

	void ColliderHelper::SetMaterial(void* _shape, const ZnMaterialID& _id)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		
		physx::PxMaterial* material = ResourceManager::GetPxMaterial(_id);

		_pxShape->setMaterials(&material, 1);
	}

	void ColliderHelper::SetMaterials(void* _shape, ZnMaterialID* _pointer, int size)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		std::vector<physx::PxMaterial*> materials;

		for (int i = 0; i < size; i++)
		{
			physx::PxMaterial* material = ResourceManager::GetPxMaterial(_pointer[i]);
			materials.emplace_back(material);
		}

		_pxShape->setMaterials(&materials.front(), materials.size());
	}
}
