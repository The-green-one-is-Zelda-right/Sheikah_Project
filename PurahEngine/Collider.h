#pragma once
#include "PurahEngineAPI.h"
#include "Component.h"
#include "EngineSetting.h"
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
        void PostInitialize() override;

        void OnEnable() override;
        void OnDisable() override;

        void SetUserData();

    public:
        void SetPositionOffset(const Eigen::Vector3f& _pos);
        void SetRotationOffset(const Eigen::Quaternionf& _rot);

        void SetDynamic(bool _value);
        void SetTrigger(bool _value) const;
        void SetLayer(uint32_t _value) const;

        std::wstring GetMaterial();
        void SetMaterial(const std::wstring&);
        void SetMaterials(std::wstring*, int);

    public:
        virtual void PreStep();

    public:
        void PreSerialize(json& jsonData) const override {};
		void PreDeserialize(const json& jsonData) override;
        void PostSerialize(json& jsonData) const override {};
        void PostDeserialize(const json& jsonData) override {};

    protected:
        bool awake = true;
        Transform* transform = nullptr;
        ColliderType cType = ColliderType::STATIC;

    protected:
        std::wstring physicsMaterial;
        bool isTrigger = false;
        uint32_t layer = 0;
        bool run = false;
        ZonaiPhysics::ZnCollider* znCollider = nullptr;
        Eigen::Vector3f positionOffset{ Eigen::Vector3f::Zero() };
        Eigen::Quaternionf rotationOffset{ Eigen::Quaternionf::Identity() };
    };
}
