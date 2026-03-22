#include "LauncherPlatform.h"

namespace Phyzzle
{
    void LauncherPlatform::PowerOn()
    {
        if (powerOnWithStart)
        {
            return;
        }

        powerCounter += 1;

        if (powerCounter >= 1)
        {
            state = State::Push;
            elapsedTime = 0.0f;
        }
    }
    void LauncherPlatform::PowerOff()
    {
        if (powerOnWithStart)
        {
            return;
        }

        powerCounter -= 1;

        if (powerCounter < 1)
        {
            state = State::Sleep;
        }
    }

    void LauncherPlatform::Awake()
    {
        rigid = GetGameObject()->GetComponent<PurahEngine::RigidBody>();
    }

    void LauncherPlatform::Start()
    {
        state = State::Sleep;
        if (powerOnWithStart)
        {
            state = State::Push;
        }
        elapsedTime = 0.0f;
    }

    void LauncherPlatform::FixedUpdate()
    {
        StateExecute();
    }

    void LauncherPlatform::Update()
    {
        if (state != State::Sleep)
        {
            elapsedTime += PurahEngine::TimeController::GetInstance().GetDeltaTime();
        }

        StateUpdate();
    }

    void LauncherPlatform::StateUpdate()
    {
        switch (state)
        {
            case State::Push:
            {
                if (elapsedTime >= pushTime)
                {
                    elapsedTime -= pushTime;
                    state = State::Pull;
                }
                break;
            }
            case State::Pull:
            {
                if (elapsedTime >= pullTime)
                {
                    elapsedTime -= pullTime;
                    state = State::Wait;
                }
                break;
            }
            case State::Wait:
            {
                if (elapsedTime >= waitTime)
                {
                    elapsedTime -= waitTime;
                    state = State::Push;
                }
                break;
            }
            case State::Sleep:
            {
                // Do Nothing
                break;
            }
            default:
            {
                assert(0);
                break;
            }
        }
    }

    void LauncherPlatform::StateExecute()
    {
        switch (state)
        {
            case State::Push:
            {
                rigid->SetLinearVelocity(moveDirection.normalized() * moveSpeed);
                break;
            }
            case State::Pull:
            {
                rigid->SetLinearVelocity(-moveDirection.normalized() * moveSpeed);
                break;
            }
            case State::Wait:
            {
                rigid->SetLinearVelocity(Eigen::Vector3f::Zero());
                break;
            }
            case State::Sleep:
            {
                rigid->SetLinearVelocity(Eigen::Vector3f::Zero());
                break;
            }
            default:
            {
                assert(0);
                break;
            }
        }
    }

    void LauncherPlatform::PreSerialize(json& jsonData) const
    {
    }

    void LauncherPlatform::PreDeserialize(const json& jsonData)
    {
	    PREDESERIALIZE_BASE();
        PREDESERIALIZE_VECTOR3F(moveDirection);
        PREDESERIALIZE_VALUE(moveSpeed);
        PREDESERIALIZE_VALUE(pushTime);
        PREDESERIALIZE_VALUE(pullTime);
        PREDESERIALIZE_VALUE(waitTime);
        PREDESERIALIZE_VALUE(powerOnWithStart);
    }

    void LauncherPlatform::PostSerialize(json& jsonData) const
    {
    }

    void LauncherPlatform::PostDeserialize(const json& jsonData)
    {
    }
}