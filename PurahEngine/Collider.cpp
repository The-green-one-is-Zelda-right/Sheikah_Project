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
            colliders.erase(std::ranges::find(colliders, this));
        }
        break;

        case ColliderType::STATIC:
        {
            auto& colliders = physicsSystem.staticColliders;
            colliders.erase(std::ranges::find(colliders, this));
        }
        break;

        default:
            assert(false);
            break;
        }
           
        physicsSystem.FreeObject(znCollider, GetGameObject());
    }

	void Collider::OnDataLoadComplete()
	{
        this->transform = gameObject->GetTransform();

        switch (cType)
        {
        case ColliderType::DYNAMIC:
            PhysicsSystem::GetInstance().dynamicColliders.push_back(this);
            break;

        case ColliderType::STATIC:
            PhysicsSystem::GetInstance().staticColliders.push_back(this);
            break;

        default:
            assert(false);
            break;
        }
	}

	void Collider::SetPositionOffset(const Eigen::Vector3f& _pos)
    {
        if (awake)
        {
            positionOffset = _pos;
        }
        else
        {
            znCollider->SetLocalPosition(positionOffset);
        }
    }

	//void Collider::SetRotationOffset(const Eigen::Quaternionf& _quat)
 //   {
 //       if (awake)
 //       {
 //           rotationOffset = _quat;
 //       }
 //       else
 //       {
 //           znCollider->SetLocalQuaternion(rotationOffset);
 //       }
 //   }

	void Collider::SetDynamic(bool _value)
    {
	    if (awake)
	    {
            OutputDebugStringW(L"���� ���߿� Collider Type ������ �Ұ��� �մϴ�.");
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

	void Collider::PreStep()
    {
        znCollider->SetPosition(transform->GetWorldPosition());
        znCollider->SetQuaternion(transform->GetWorldRotation());
    }
}
