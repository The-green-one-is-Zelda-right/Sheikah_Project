#include "PhysicsSystem.h"

#include "GameObject.h"
#include "ZnCollider.h"
#include "Transform.h"

#include "Collider.h"

namespace PurahEngine
{
	Collider::~Collider()
    {
        transform = nullptr;

        auto& physicsSystem = PhysicsSystem::GetInstance();

        switch (cType)
        {
        case ColliderType::DYNAMIC:
        {
            auto& colliders = physicsSystem.dynamicColliders;
            auto itr = std::ranges::find(colliders, this);
            if (itr != colliders.end())
            {
                colliders.erase(std::ranges::find(colliders, this));
            }
        }
        break;

        case ColliderType::STATIC:
        {
            auto& colliders = physicsSystem.staticColliders;
            auto itr = std::ranges::find(colliders, this);
            if (itr != colliders.end())
            {
				colliders.erase(std::ranges::find(colliders, this));
            }
        }
        break;

        default:
            assert(false);
            break;
        }
    }

	void Collider::PostInitialize()
	{
        this->transform = gameObject->GetTransform();

		auto& physicsSystem = PhysicsSystem::GetInstance();

        switch (cType)
        {
        case ColliderType::DYNAMIC:
        {
            auto& collisers = physicsSystem.dynamicColliders;
            collisers.push_back(this);
        }
        break;

        case ColliderType::STATIC:
        {
            auto& collisers = physicsSystem.staticColliders;
            collisers.push_back(this);
        }
        break;

        default:
            assert(false);
            break;
        }
	}

	void Collider::OnEnable()
	{
		if (!awake)
		{
            znCollider->Disable(false);
		}
	}

	void Collider::OnDisable()
	{
		if (!awake)
		{
			znCollider->Disable(true);
		}
	}

	void Collider::SetUserData()
	{
        znCollider->SetUserData(this);
	}

	void Collider::SetPositionOffset(const Eigen::Vector3f& _pos)
    {
        if (awake)
        {
            positionOffset = _pos;
        }
        else
        {
            if (_pos.isZero())
                return;

			Eigen::Matrix4f world = transform->GetWorldMatrix();
			Eigen::Vector4f positionOffset4D(_pos.x(), _pos.y(), _pos.z(), 1.0f);
			Eigen::Vector4f shapePos4D = world * positionOffset4D;

			Eigen::Vector3f pos = transform->GetWorldPosition();
			Eigen::Quaternionf rot = transform->GetWorldRotation();
            Eigen::Affine3f realWorld = Eigen::Affine3f::Identity();
            realWorld.translate(pos);
            realWorld.rotate(rot);
			Eigen::Vector4f result = realWorld.inverse() * shapePos4D;
            Eigen::Vector3f realOffset(result.x(), result.y(), result.z());

            znCollider->SetLocalPosition(realOffset);
        }
    }

    void Collider::SetRotationOffset(const Eigen::Quaternionf& _quat)
    {
        if (awake)
        {
            rotationOffset = _quat;
        }
        else
        {
            znCollider->SetLocalQuaternion(rotationOffset);
        }
    }

	void Collider::SetDynamic(bool _value)
    {
	    if (awake)
	    {
            OutputDebugStringW(L"실행 도중에 Collider Type 변경은 불가능 합니다.");
            return;
	    }
        
        cType = _value ? ColliderType::DYNAMIC : ColliderType::STATIC;
    }

	void Collider::SetTrigger(bool _value) const
	{
        // if (awake)
        // {
        //     isTrigger = _value;
        // }
        // else
        // {
            znCollider->SetTrigger(_value);
        // }
    }

	void Collider::SetLayer(uint32_t _value) const
	{
        // if (awake)
        // {
        //     layer = _value;
        // }
        // else
        // {
            znCollider->SetLayerData(_value);
        // }
    }

	std::wstring Collider::GetMaterial()
	{
        return physicsMaterial;
	}

	void Collider::SetMaterial(const std::wstring& _material)
	{
		physicsMaterial = _material;

        if (!awake)
        {
            std::vector<std::wstring> materials;
            materials.emplace_back(_material);
            PhysicsSystem::GetInstance().SetColliderMaterial(znCollider, &materials.front(), materials.size());
        }
	}
	
    void Collider::SetMaterials(std::wstring* _materials, int _size)
	{

	}

	void Collider::PreStep()
    {
        znCollider->SetPosition(transform->GetWorldPosition(), true);
        znCollider->SetQuaternion(transform->GetWorldRotation(), true);
    }

    void Collider::PreDeserialize(const json& jsonData)
    {
        PREDESERIALIZE_BASE();

		PREDESERIALIZE_VALUE(cType);
		PREDESERIALIZE_VALUE(isTrigger);
		PREDESERIALIZE_VALUE(layer);
		PREDESERIALIZE_VECTOR3F(positionOffset);
		PREDESERIALIZE_QUATERNIONF(rotationOffset);
		PREDESERIALIZE_WSTRING(physicsMaterial);
    }
}
