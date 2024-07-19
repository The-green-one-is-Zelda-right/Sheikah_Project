#include "AttachSelectState.h"
#include "PzObject.h"

#include "AttachSystem.h"

namespace Phyzzle
{
	AttachSelectState::~AttachSelectState()
		= default;

#pragma region StateEvent
	// ���� ������
	void AttachSelectState::StateEnter()
	{
		{
			using namespace Eigen;

			auto p = player->camData.coreDefaultPosition;
			p.x() += 0.5f;
			p.y() += 0.5f;

			player->SetCameraCoreLocalTargetPosition(p);
		}

		CrossHeadRender(true);
	}

	// ���� ������
	void AttachSelectState::StateExit()
	{
		{
			player->SetCameraCoreLocalTargetPosition(player->camData.coreDefaultPosition);
		}

		EnableOutline(false);
		AroundObjectEnableOutline(false);

		select = false;
		selectObject = nullptr;
		seleteBody = nullptr;
		aroundObject.clear();

		CrossHeadRender(false);
	}

	// Update
	void AttachSelectState::StateStay()
	{
		EnableOutline(false);
		AroundObjectEnableOutline(false);

		aroundObject.clear();
		seleteBody = nullptr;
		selectObject = nullptr;

		if (SearchAround())
		{
			AroundObjectEnableOutline(true);
		}

		select = Search();

		if (select)
		{
			EnableOutline(true);
		}
		
		if (player->data.debugMode)
		{
			SearchDebugDraw(select);
		}
	}

	void AttachSelectState::PostStateStay()
	{
		CameraUpdate();
	}

	void AttachSelectState::StateCancel()
	{
		Cancel();
	}

#pragma endregion StateEvent

#pragma region Input
	// �̵�
	void AttachSelectState::Stick_L()
	{
		// �̵� �������� ĳ���͸� ȸ�� ��Ŵ
		PlayerMove(player->data.moveSpeed);
		auto velocity = player->data.playerRigidbody->GetLinearVelocity();
		velocity.y() = 0.f;
		LookToWorldDirection(velocity);
	}

	// ī�޶� ȸ��
	void AttachSelectState::Stick_R()
	{

	}

	// ���
	void AttachSelectState::Click_A()
	{
		Cancel();
	}

	// ����
	void AttachSelectState::Click_B()
	{
		// ����ĳ����
		if (select)
		{ 
			// ���� �ٲ�
			player->data.holdObject = selectObject;
			player->data.holdObjectBody = seleteBody;
			player->ChangeAbilityState(Player::ATTACH_HOLD);
		}
	}

	// ���
	void AttachSelectState::Click_X()
	{
		Cancel();
	}

	// ���
	void AttachSelectState::Click_Y()
	{
		Cancel();
	}

	// ���
	void AttachSelectState::Click_LB()
	{
		Cancel();
	}
#pragma endregion Input

#pragma region Content
	void AttachSelectState::PlayerMove(float _speed) const
	{
		if (player->TryPlayerMove(_speed))
		{
			player->ChangePlayerAnimationState(Player::WALK);
		}
		else
		{
			player->ChangePlayerAnimationState(Player::IDLE);
		}
	}

	void AttachSelectState::CameraUpdate() const
	{
		player->UpdateSelectCamera();
	}

	void AttachSelectState::Cancel()
	{
		EnableOutline(false);
		CrossHeadRender(false);
		player->ChangeAbilityState(Player::AbilityState::DEFAULT);
	}

	void AttachSelectState::Jump() const
	{
		if (player->TryJump())
		{
			player->ChangePlayerAnimationState(Player::JUMP);
		}
	}

	void AttachSelectState::LookToWorldDirection(const Eigen::Vector3f& _to)
	{
		player->LookInWorldDirection(_to.normalized());
	}

	void AttachSelectState::LookToLocalDirection(const Eigen::Vector3f& _to)
	{
		player->LookInLocalDirection(_to.normalized());
	}

	bool AttachSelectState::Search()
	{
		Eigen::Vector3f from = player->data.cameraCore->GetWorldPosition();
		Eigen::Matrix3f rotate = player->data.cameraCore->GetWorldRotation().toRotationMatrix();
		Eigen::Vector3f to = rotate * Eigen::Vector3f{ 0.f, 0.f, 1.f };
		float distance = player->abilData.attachRaycastDistance + std::fabs(player->data.cameraCore->GetLocalPosition().z());
		unsigned int layers = player->abilData.attachRaycastLayers;
		ZonaiPhysics::ZnQueryInfo info;

		const bool block = PurahEngine::Physics::Raycast(from, to, distance, layers, info);

		if (!block)
			return false;

		const PurahEngine::Collider* shape = static_cast<PurahEngine::Collider*>(info.colliderData);

		if (!shape)
			return false;

		const PurahEngine::GameObject* obj = shape->GetGameObject();
		PurahEngine::RigidBody* body = obj->GetComponent<PurahEngine::RigidBody>();

		if (!body)
			return false;

		seleteBody = body;
		selectObject = obj->GetComponent<PzObject>();

		if (player->data.debugMode)
		{
			PurahEngine::GraphicsManager::GetInstance().DrawString(
				shape->GetGameObject()->GetName(), 800, 600, 100, 100, 30, 255, 255, 255, 255);
		}

		if (!selectObject || !seleteBody)
			return false;

		if (seleteBody->IsKinematic())
			return false;

		return true;
	}

	bool AttachSelectState::SearchAround()
	{
		using namespace Eigen;

		float radius = player->abilData.attachRaycastDistance;
		Eigen::Affine3f parentMatrix(player->data.cameraArm->GetWorldMatrix());
		Eigen::Vector3f corePosition = player->data.cameraCore->GetLocalPosition();
		corePosition.z() = 0.f;
		Eigen::Vector3f position = parentMatrix * corePosition;
		Quaternionf rotation = Quaternionf::Identity();
		int layer = player->abilData.attachRaycastLayers;
		ZonaiPhysics::ZnQueryInfo info;
		info.actors.resize(player->abilData.searchAroundbufferSize);
		info.shapes.resize(player->abilData.searchAroundbufferSize);

		bool hit = PurahEngine::Physics::SphereOverlap(radius, position, rotation, layer, info);

		if (hit)
		{
			for (int i = 0; i < info.shapes.size(); i++)
			{
				PurahEngine::Collider* collider = static_cast<PurahEngine::Collider*>(info.shapes[i]);
				
				if (!collider)
					continue;

				const PurahEngine::GameObject* obj = collider->GetGameObject();

				PzObject* pzObject = obj->GetComponent<PzObject>();

				if (pzObject)
					aroundObject.emplace_back(pzObject);
			}
		}

		return hit;
	}

	void AttachSelectState::AroundObjectEnableOutline(bool _value)
	{
		for (auto& obj : aroundObject)
		{
			if (_value)
			{
				AttachSystem::Instance()->EnableOutline(obj, &player->color3);
			}
			else
			{
				AttachSystem::Instance()->DisableOutline(obj);
			}
		}
	}

	void AttachSelectState::EnableOutline(bool _value) const
	{
		if (!seleteBody || !selectObject)
			return;

		if (_value)
		{
			AttachSystem::Instance()->EnableOutline(selectObject, &player->color0, &player->color1);
		}
		else
		{
			AttachSystem::Instance()->DisableOutline(selectObject);
		}
	}
	
	void AttachSelectState::CrossHeadRender(bool _value)
	{
		player->data.crossHead->SetEnable(_value);
	}
#pragma endregion Content

#pragma region Debug
	void AttachSelectState::SearchDebugDraw(bool _value)
	{
		if (_value)
		{
			PurahEngine::GraphicsManager::GetInstance().DrawString(
				L"O",
				945, 510,
				200, 100, 50,
				255, 255, 255, 255);
		}
		else
		{
			PurahEngine::GraphicsManager::GetInstance().DrawString(
				L"X",
				945, 510,
				200, 100, 50,
				255, 255, 255, 255);
		}
	}
#pragma endregion Debug
}
