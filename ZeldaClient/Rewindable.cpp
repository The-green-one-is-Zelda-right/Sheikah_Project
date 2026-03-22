#include "TimeController.h"

#include "RigidBody.h"
#include "GameObject.h"

#include "Snapshot.h"
#include "PzObject.h"

#include "Rewindable.h"

#include "RewindSystem.h"

namespace Phyzzle
{
	Rewindable::Rewindable(): prev(nullptr), curr(nullptr), next(nullptr), step(0), body(nullptr), container(nullptr)
	{
	}

	void Rewindable::Awake()
	{
		body = gameObject->GetComponent<PurahEngine::RigidBody>();

		if (!body)
		{
			gameObject->DeleteComponent(this);
			assert(body != nullptr);
		}
	}

	void Rewindable::Update()
	{
		assert(body != nullptr);

		if (rewinding)
		{
			Rewind();
		}
		else
		{
			Store();
		}
	}

	void Rewindable::Rewind()
	{
		if (container->empty())
		{
			rewinding = false;
			return;
		}

		Snapshot* snap = container->back();
	}

	void Rewindable::Rewind(std::list<Snapshot*>* _history)
	{
		container = _history;
		rewinding = true;
	}

	void Rewindable::Store()
	{
		assert(body != nullptr);

		this->step = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		auto* object = gameObject->GetComponent<PzObject>();
		if (object == nullptr)
		{
			return;
		}

		RewindSystem::Instance().SaveState(object);
	}

	void Rewindable::Restore(Snapshot* _data)
	{
		// 시간 간격

		// dt를 
		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		// 물리 정보를 꺼냄.
		_data->linearVelocity;
		_data->angularVelocity;
	}

	bool Rewindable::Complete()
	{
		return true;
	}

	void Rewindable::Cancel()
	{
		rewinding = false;
		container = nullptr;
	}
}
