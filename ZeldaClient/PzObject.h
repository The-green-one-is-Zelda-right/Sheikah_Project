#pragma once
#include "PurahEngine.h"
#include <vector>
#include <string>
#include "IslandID.h"

namespace Phyzzle
{
	class PzObject final : public PurahEngine::Component
	{
	public:
		~PzObject() override;

		void Start() override;
		void Update() override;
		void LateUpdate() override;

	public:
		bool isLocked();

	private:
		IslandID GetIslandID() const;
		void ValiantStore();		// 강체 변수 저장
		void Selected();			// 선택
		void ValiantRetrieve();		// 강체 변수 되돌림

	protected:
		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionExit(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;

		void PreSerialize(json& jsonData) const override {}
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override {}
		void PostDeserialize(const json& jsonData) override;

	private:
		friend class AttachSystem;

		struct SelectionPhysicsSnapshot
		{
			bool isKinematic = false;
			bool hasGravity = false;
			float mass = -1.f;
			Eigen::Vector3f inertiaTensor = Eigen::Vector3f::Zero();
			std::vector<std::wstring> materials;

			bool IsValid() const
			{
				return mass > 0.01f;
			}

			void Reset()
			{
				isKinematic = false;
				hasGravity = false;
				mass = -1.f;
				inertiaTensor = Eigen::Vector3f::Zero();
				materials.clear();
			}
		};

		struct SelectionPhysicsOverride
		{
			bool isKinematic = false;
			bool hasGravity = false;
			float mass = 0.1f;
			Eigen::Vector3f inertiaTensor = Eigen::Vector3f(100.f, 100.f, 100.f);
			std::wstring material = L"SelectedObject";
		};

		PzObject* attachable;
		PurahEngine::RigidBody* body;
		std::vector<PurahEngine::Collider*> colliders;
		Eigen::Vector3f worldAnchor = Eigen::Vector3f::Zero();

		IslandID islandID;							// IslandID
		std::vector<PzObject*> connectedObjects;	// 현 객체와 연결된 객체들

	private:
		bool preState = false;
		bool select = false;

		SelectionPhysicsSnapshot selectionSnapshot;
		SelectionPhysicsOverride selectionOverride;
	};
}


/*
부착 가능한 모든 객체들은 IslandID를 가지게 될 것임.
임의의 객체를 집고 다른 부착 가능한 객체에 부착(attach)이 가능함.
섬을 이루지 못하는 객체는 ID값이 -1임.
본인 포함해서 2개 이상의 연결성이 확인되는 객체들은 IslandID값이 부여됨

연결이 해제(dettach)될 경우엔 Island를 다시 만듬.

ex) 예를들어 1-2-3-4의 연결성을 가진 오브젝트가 있다고 가정했을 때
	2을 떼었을 때, 2는 연결성이 사라졌기 때문에 ID값이 -1로 초기화 된다.
	2와 연결성을 가진 오브젝트들은 2와의 연결성이 끊어지게 된다.

	기존 Island에서 2가 떨어져 나왔기 떄문에
	남아있는 객체들의 연결성을 확인하고 Island를 새롭게 만들어줘야한다.
	2와 연결되어 있었던 1과 3 방향으로 탐색을 한다.
	1 방향으로는 객체가 1개만 존재하기 때문에 Island를 이루지 못하고 ID값이 -1로 초기화된다.
	3 방향으로는 4라는 객체와 연결성이 확인되기 때문에
	3 방향의 Island들은 새로운 IslandID값을 부여받는다.
*/
