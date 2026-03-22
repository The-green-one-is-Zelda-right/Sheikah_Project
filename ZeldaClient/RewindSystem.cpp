#include "RewindSystem.h"

#include "AttachSystem.h"
#include "PzObject.h"
#include "RigidBody.h"

namespace Phyzzle
{
	void RewindSystem::SaveState(PzObject* inObject)
	{
		if (inObject == nullptr)
		{
			return;
		}

		if (_isRewinding && _currentRewindObject == inObject)
		{
			return;
		}

		auto* body = inObject->GetGameObject()->GetComponent<PurahEngine::RigidBody>();
		if (body == nullptr)
		{
			return;
		}

		RewindSnapshot snapshot;
		snapshot.position = body->GetPosition();
		snapshot.rotation = body->GetRotation();
		snapshot.velocity = body->GetLinearVelocity();
		snapshot.angularVelocity = body->GetAngularVelocity();

		auto& history = _objectHistories[inObject];
		history.push_back(snapshot);

		if (history.size() > MAX_HISTORY_SIZE)
		{
			history.erase(history.begin());
		}
	}

	void RewindSystem::StartRewind(PzObject* inObject)
	{
		if (inObject == nullptr)
		{
			return;
		}

		auto historyIter = _objectHistories.find(inObject);
		if (historyIter == _objectHistories.end() || historyIter->second.size() < 2)
		{
			return;
		}

		if (_isRewinding)
		{
			EndRewind();
		}

		auto* body = inObject->GetGameObject()->GetComponent<PurahEngine::RigidBody>();
		if (body == nullptr)
		{
			return;
		}

		_currentRewindObject = inObject;
		_currentHistoryIndex = historyIter->second.size() - 1;
		_rewindTimer = 0.0f;
		_rewindStepAccumulator = 0.0f;
		_isRewinding = true;

		body->SetKinematic(true);
		AttachSystem::Instance()->EnableOutline(inObject, &AttachSystem::Instance()->color0, &AttachSystem::Instance()->color2);
	}

	void RewindSystem::UpdateRewind(float inDeltaTime)
	{
		if (!_isRewinding || _currentRewindObject == nullptr)
		{
			return;
		}

		auto historyIter = _objectHistories.find(_currentRewindObject);
		if (historyIter == _objectHistories.end() || historyIter->second.empty())
		{
			EndRewind();
			return;
		}

		auto* body = _currentRewindObject->GetGameObject()->GetComponent<PurahEngine::RigidBody>();
		if (body == nullptr)
		{
			EndRewind();
			return;
		}

		_rewindTimer += inDeltaTime;
		_rewindStepAccumulator += inDeltaTime;

		while (_rewindStepAccumulator >= REWIND_SAMPLE_INTERVAL && _currentHistoryIndex > 0)
		{
			--_currentHistoryIndex;
			_rewindStepAccumulator -= REWIND_SAMPLE_INTERVAL;
		}

		const RewindSnapshot& snapshot = historyIter->second[_currentHistoryIndex];
		body->SetPosition(snapshot.position);
		body->SetRotation(snapshot.rotation);
		body->SetLinearVelocity(Eigen::Vector3f::Zero());
		body->SetAngularVelocity(Eigen::Vector3f::Zero());

		if (_currentHistoryIndex == 0 || _rewindTimer >= REWIND_DURATION)
		{
			EndRewind();
		}
	}

	void RewindSystem::EndRewind()
	{
		if (!_isRewinding || _currentRewindObject == nullptr)
		{
			return;
		}

		auto* body = _currentRewindObject->GetGameObject()->GetComponent<PurahEngine::RigidBody>();
		auto historyIter = _objectHistories.find(_currentRewindObject);
		if (body != nullptr)
		{
			body->SetKinematic(false);

			if (historyIter != _objectHistories.end() && _currentHistoryIndex < historyIter->second.size())
			{
				const RewindSnapshot& snapshot = historyIter->second[_currentHistoryIndex];
				body->SetLinearVelocity(snapshot.velocity);
				body->SetAngularVelocity(snapshot.angularVelocity);
			}
		}

		AttachSystem::Instance()->DisableOutline(_currentRewindObject);

		_isRewinding = false;
		_currentRewindObject = nullptr;
		_currentHistoryIndex = 0;
		_rewindTimer = 0.0f;
		_rewindStepAccumulator = 0.0f;
	}

	bool RewindSystem::CanUseRewind(PzObject* inObject) const
	{
		if (inObject == nullptr || _isRewinding)
		{
			return false;
		}

		auto historyIter = _objectHistories.find(inObject);
		return historyIter != _objectHistories.end() && historyIter->second.size() > 1;
	}

	void RewindSystem::ClearHistory(PzObject* inObject)
	{
		if (inObject == nullptr)
		{
			return;
		}

		_objectHistories.erase(inObject);
	}
}
