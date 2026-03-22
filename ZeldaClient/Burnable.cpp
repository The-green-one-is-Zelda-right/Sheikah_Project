#include "Burnable.h"

namespace Phyzzle
{
	void Burnable::OnEnable()
	{
		burnTimeElapsed = 0.0f;
		waitDestroy = false;
		destroyElapsed = 0.0f;

		if (burnableObject != nullptr)
		{
			burnableObject->SetEnable(true);
		}

		if (effect != nullptr)
		{
			effect->Reset();
		}
	}

	void Burnable::Update()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (fireList.size() > 0ull)
		{
			burnTimeElapsed += deltaTime;
		}
		else
		{
			burnTimeElapsed = 0.0f;
		}

		if (fireList.size() > 0ull && burnTimeElapsed >= burnTime)
		{
			burnableObject->SetEnable(false);
			effect->StopGenerate();
			waitDestroy = true;
		}

		if (waitDestroy)
		{
			destroyElapsed += deltaTime;

			if (destoryDelay <= destroyElapsed)
			{
				gameObject->SetEnable(false);
			}
		}
	}

	void Burnable::OnTriggerEnter(const PurahEngine::Collider* other)
	{
		if (other->GetGameObject()->tag.IsContain(L"Fire"))
		{
			fireList.push_back(other);
		}

		if (effect != nullptr)
		{
			if (fireList.size() > 0u)
			{
				effect->Play();
			}
			else
			{
				effect->StopGenerate();
			}
		}
	}

	void Burnable::OnTriggerExit(const PurahEngine::Collider* other)
	{
		if (other->GetGameObject()->tag.IsContain(L"Fire"))
		{
			auto iter = std::find(fireList.begin(), fireList.end(), other);
			if (iter != fireList.end())
			{
				fireList.erase(iter);
			}
		}

		if (effect != nullptr)
		{
			if (fireList.size() == 0u)
			{
				effect->StopGenerate();
			}
		}
	}

	void Burnable::PreSerialize(json& jsonData) const
	{
	}

	void Burnable::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(burnTime);
		PREDESERIALIZE_VALUE(destoryDelay);
	}

	void Burnable::PostSerialize(json& jsonData) const
	{
	}

	void Burnable::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(burnableObject);
		POSTDESERIALIZE_PTR(effect);
	}
}