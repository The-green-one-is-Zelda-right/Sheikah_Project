#pragma once
#include "PurahEngineAPI.h"
#include "Component.h"
#include "Eigen/Dense"

namespace ZonaiPhysics
{
    class ZnCollider;
}

namespace PurahEngine
{
    class Transform;

    enum class ColliderType
    {
        DYNAMIC,
        STATIC,
    };

    class PURAHENGINE_API Collider : public Component
    {
    public:
        Collider() = default;

        ~Collider() override;

    public:
        void Awake() override;

    public:
        void SetPositionOffset(const Eigen::Vector3f& _pos);

        void SetRotationOffset(const Eigen::Quaternionf& _quat);

        void SetDynamic(bool _value);

        void SetTrigger(bool _value);

        void SetLayer(uint32_t _value);

    public:
        virtual void PreStep();

    protected:
        bool awake = true;
        Transform* transform = nullptr;
        ColliderType type = ColliderType::STATIC;

    protected:
        bool isTrigger = false;
        uint32_t layer = 0;
        ZonaiPhysics::ZnCollider* znCollider = nullptr;
        Eigen::Vector3f positionOffset{ Eigen::Vector3f::Zero() };
        Eigen::Quaternionf rotationOffset{ Eigen::Quaternionf::Identity() };
    };
}
