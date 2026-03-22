#include "RewindState.h"

#include <cmath>

#include "AttachSystem.h"
#include "PzObject.h"
#include "RewindSystem.h"
#include "Rewindable.h"
#include "RigidBody.h"
#include "TimeController.h"

namespace Phyzzle
{
	RewindState::~RewindState()
	= default;

	void RewindState::StateEnter()
	{
		player->SetCameraCoreLocalTargetPosition(player->camData.coreSelectPosition);

		if (!paused)
		{
			pauseLevel = PurahEngine::TimeController::GetInstance().PauseAll();
			paused = true;
		}

		CrossHeadRender(true);
		SearchUIRender(true);
	}

	void RewindState::StateExit()
	{
		if (paused)
		{
			PurahEngine::TimeController::GetInstance().ResumeAll();
			paused = false;
			pauseLevel = 0;
		}

		player->SetCameraCoreLocalTargetPosition(player->camData.coreDefaultPosition);

		EnableOutline(false);
		AroundObjectEnableOutline(false);

		select = false;
		around = false;
		selectObject = nullptr;
		selectBody = nullptr;
		aroundObject.clear();

		CrossHeadRender(false);
		CrossHeadSelectRender(false);
		SearchUIRender(false);
		SearchCatchUIRender(false);
	}

	void RewindState::PostStateStay()
	{
		if (around)
		{
			AroundObjectEnableOutline(true);
		}

		if (select)
		{
			CrossHeadSelectRender(true);
			SearchCatchUIRender(true);
			EnableOutline(true);
		}
		else
		{
			CrossHeadSelectRender(false);
			SearchCatchUIRender(false);
		}

		CameraUpdate();
	}

	void RewindState::StateStay()
	{
		EnableOutline(false);
		AroundObjectEnableOutline(false);

		aroundObject.clear();
		selectBody = nullptr;
		selectObject = nullptr;

		around = SearchAround();
		select = Search();
	}

	void RewindState::Stick_L()
	{
	}

	void RewindState::Stick_R()
	{
	}

	void RewindState::Trigger_L()
	{
	}

	void RewindState::Trigger_R()
	{
	}

	void RewindState::Click_DUp()
	{
	}

	void RewindState::Click_DDown()
	{
	}

	void RewindState::Click_DLeft()
	{
	}

	void RewindState::Click_DRight()
	{
	}

	void RewindState::Click_A()
	{
		Cancel();
	}

	void RewindState::Click_B()
	{
		Select();
	}

	void RewindState::Click_X()
	{
		Cancel();
	}

	void RewindState::Click_Y()
	{
		Cancel();
	}

	void RewindState::Click_LB()
	{
		Cancel();
	}

	void RewindState::Click_RB()
	{
	}

	void RewindState::Aim()
	{
	}

	void RewindState::Cancel()
	{
		player->ChangeAbilityState(Player::AbilityState::DEFAULT);
	}

	void RewindState::Select()
	{
		if (!select || selectObject == nullptr)
		{
			return;
		}

		RewindSystem::Instance().StartRewind(selectObject);
		player->ChangeAbilityState(Player::AbilityState::DEFAULT);
	}

	void RewindState::CameraUpdate() const
	{
		player->UpdateSelectCamera();
	}

	bool RewindState::Search()
	{
		Eigen::Vector3f from = player->data.cameraCore->GetWorldPosition();
		Eigen::Matrix3f rotate = player->data.cameraCore->GetWorldRotation().toRotationMatrix();
		Eigen::Vector3f to = rotate * Eigen::Vector3f{ 0.f, 0.f, 1.f };
		float distance = player->abilData.attachRaycastDistance + std::fabs(player->data.cameraCore->GetLocalPosition().z());
		unsigned int layers = player->abilData.searchAroundLayers;
		ZonaiPhysics::ZnQueryInfo info;

		const bool hit = PurahEngine::Physics::Raycast(from, to, distance, layers, info);
		if (!hit)
		{
			return false;
		}

		const auto* shape = static_cast<PurahEngine::Collider*>(info.colliderData);
		if (shape == nullptr)
		{
			return false;
		}

		const PurahEngine::GameObject* obj = shape->GetGameObject();
		PurahEngine::RigidBody* body = obj->GetComponent<PurahEngine::RigidBody>();
		if (body == nullptr || body->IsKinematic())
		{
			return false;
		}

		PzObject* object = obj->GetComponent<PzObject>();
		Rewindable* rewindable = obj->GetComponent<Rewindable>();
		if (object == nullptr || rewindable == nullptr || !RewindSystem::Instance().CanUseRewind(object))
		{
			return false;
		}

		selectBody = body;
		selectObject = object;
		return true;
	}

	bool RewindState::SearchAround()
	{
		using namespace Eigen;

		float radius = player->abilData.searchAroundDistance;
		Affine3f parentMatrix(player->data.cameraArm->GetWorldMatrix());
		Eigen::Vector3f corePosition = player->data.cameraCore->GetLocalPosition();
		corePosition.z() = 0.f;
		Eigen::Vector3f position = parentMatrix * corePosition;
		Quaternionf rotation = Quaternionf::Identity();
		int layer = player->abilData.searchAroundLayers;
		ZonaiPhysics::ZnQueryInfo info;
		info.actors.resize(player->abilData.searchAroundbufferSize);
		info.shapes.resize(player->abilData.searchAroundbufferSize);

		bool hit = PurahEngine::Physics::SphereOverlap(radius, position, rotation, layer, info);
		if (!hit)
		{
			return false;
		}

		for (int i = 0; i < info.shapes.size(); ++i)
		{
			auto* collider = static_cast<PurahEngine::Collider*>(info.shapes[i]);
			if (collider == nullptr)
			{
				continue;
			}

			const PurahEngine::GameObject* obj = collider->GetGameObject();
			PurahEngine::RigidBody* body = obj->GetComponent<PurahEngine::RigidBody>();
			if (body == nullptr || body->IsKinematic())
			{
				continue;
			}

			PzObject* object = obj->GetComponent<PzObject>();
			Rewindable* rewindable = obj->GetComponent<Rewindable>();
			if (object == nullptr || rewindable == nullptr || !RewindSystem::Instance().CanUseRewind(object))
			{
				continue;
			}

			aroundObject.emplace_back(object);
		}

		return !aroundObject.empty();
	}

	void RewindState::EnableOutline(bool value) const
	{
		if (selectObject == nullptr)
		{
			return;
		}

		if (value)
		{
			AttachSystem::Instance()->EnableOutline(selectObject, &player->color0, &player->color1);
		}
		else
		{
			AttachSystem::Instance()->DisableOutline(selectObject);
		}
	}

	void RewindState::AroundObjectEnableOutline(bool value)
	{
		for (auto* object : aroundObject)
		{
			if (value)
			{
				AttachSystem::Instance()->EnableOutline(object, &player->color3);
			}
			else
			{
				AttachSystem::Instance()->DisableOutline(object);
			}
		}
	}

	void RewindState::CrossHeadRender(bool value) const
	{
		player->data.crossHead01->SetEnable(value);
	}

	void RewindState::CrossHeadSelectRender(bool value) const
	{
		player->data.crossHead02->SetEnable(value);
	}

	void RewindState::SearchUIRender(bool value) const
	{
		player->uiData.Attach_Default->SetEnable(value);
	}

	void RewindState::SearchCatchUIRender(bool value) const
	{
		player->uiData.Catch_B->SetEnable(value);
	}

	void RewindState::StateCancel()
	{
		Cancel();
	}
}
