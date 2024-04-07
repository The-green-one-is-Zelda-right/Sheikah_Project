#include "TimeController.h"
#include "Snapshot.h"

#include "RigidBody.h"
#include "GameObject.h"

#include "Rewindable.h"

namespace Phyzzle
{
	Rewindable::Rewindable()
	{
		static long long ID = 0;

		REWIDABLE_ID = ID;
		ID++;
	}

	void Rewindable::Awake()
	{
		body = gameObject->GetComponent<PurahEngine::RigidBody>();

		assert(body != nullptr);
	}

	void Rewindable::Update()
	{
		if (!body)
			return;

		if (body->IsSleeping())
			return;

		auto direction = body->GetLinearVelocity();


	}

	Snapshot* Rewindable::Store() const
	{
		assert(body != nullptr);

		// ���� ������ ����.
		Snapshot* snapshot			= new Snapshot;
		snapshot->time				= std::chrono::system_clock::now();
		snapshot->position			= body->GetPosition();
		snapshot->rotation			= body->GetRotation();
		snapshot->linearVelocity	= body->GetLinearVelocity();
		snapshot->angularVelocity	= body->GetAngularVelocity();

		return snapshot;
	}

	void Rewindable::Restore(Snapshot* _data)
	{
		// �ð� ����
		std::chrono::duration<float> d = next->time - curr->time;
		float difference = d.count();

		// dt�� 
		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();
		auto a = dt / difference;

		// ���� ������ ����.
		_data->time;
		_data->linearVelocity;
		_data->angularVelocity;
	}

	bool Rewindable::Complete()
	{
		return true;
	}

	void Rewindable::Cancel()
	{

	}
}
