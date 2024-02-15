#pragma once
#include "PurahEngineAPI.h"
#include "GameObject.h"
#include "Transform.h"

#include "ZnCollider.h"

#include "ColliderBase.h"

#include "PhysicsSystem.h"

namespace PurahEngine
{
    class Transform;

    class PURAHENGINE_API Collider : public ColliderBase
    {
    public:
        Collider() = default;

        ~Collider() override
        {
            transform = nullptr;

            auto& physicsSystem = PhysicsSystem::GetInstance();

            switch (type)
            {
            case ColliderType::DYNAMIC:
            {
				auto& colliders = physicsSystem.dynamicColliders;
				colliders.erase(std::remove(colliders.begin(), colliders.end(), this));
                physicsSystem.FreeObject(znCollider);
            }
            break;

            case ColliderType::STATIC:
            {
                auto& colliders = physicsSystem.staticColliders;
                colliders.erase(std::remove(colliders.begin(), colliders.end(), this));
                physicsSystem.FreeObject(znCollider);
            }
            break;

            default:
                throw"";
            }
        }

        void Awake() override
        {
            this->transform = gameObject->GetComponent<Transform>();

            switch (type)
            {
            case ColliderType::DYNAMIC:
                PhysicsSystem::GetInstance().dynamicColliders.push_back(this);
                break;

            case ColliderType::STATIC:
                PhysicsSystem::GetInstance().staticColliders.push_back(this);
                break;

            default:
                throw"";
            }
        }

        void SetDynamic(bool _value)
        {
            type = _value ? ColliderType::DYNAMIC : ColliderType::STATIC;
        }

    public:
        void PreStep() override
        {
            znCollider->SetPosition(transform->GetWorldPosition());
            znCollider->SetQuaternion(transform->GetWorldRotation());
        }

    protected:
        Transform* transform = nullptr;
        ColliderType type = ColliderType::STATIC;
    };
}
