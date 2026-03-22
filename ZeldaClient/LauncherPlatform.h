#pragma once

#include "ElectricalDevice.h"

namespace Phyzzle
{
    class LauncherPlatform final : public ElectricalDevice
    {
    public:
        void PowerOn() override;
        void PowerOff() override;

        void Awake();
        void Start();
        void FixedUpdate();
        void Update();

    private:
        void StateUpdate();
        void StateExecute();

    private:
        Eigen::Vector3f moveDirection;
        float moveSpeed;

        float pushTime;
        float pullTime;
        float waitTime;

    private:
        enum class State
        {
            Push,
            Pull,
            Wait,
            Sleep
        };

        State state;
        bool powerOnWithStart;

        PurahEngine::RigidBody* rigid = nullptr;

    private:
        int powerCounter;
        float elapsedTime;

    public:
	    // Component을(를) 통해 상속됨
	    void PreSerialize(json& jsonData) const override;
	    void PreDeserialize(const json& jsonData) override;
	    void PostSerialize(json& jsonData) const override;
	    void PostDeserialize(const json& jsonData) override;
    };
}