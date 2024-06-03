#pragma once
#include "PurahEngine.h"
#include <vector>
#include "IslandID.h"

namespace Phyzzle
{
	class PzObject final : public PurahEngine::Component
	{
	public:
		~PzObject() override;

		void Start() override;
		void Update() override;

	public:
		bool isLocked();

	private:
		IslandID GetIslandID() const;
		void ValiantStore();		// ��ü ���� ����
		void Selected();			// ����
		void ValiantRetrieve();		// ��ü ���� �ǵ���

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
		PzObject* attachable;
		PurahEngine::RigidBody* body;
		Eigen::Vector3f worldAnchor = Eigen::Vector3f::Zero();

		IslandID islandID;							// IslandID
		std::vector<PzObject*> connectedObjects;	// �� ��ü�� ����� ��ü��

	private:
		bool preState = false;
		bool select = false;

		// Select�Ǹ� ��ü�� ���¸� �����ϱ� ���� ����
		bool isKinematic = false;					
		bool hasGravity = false;
		float originMass = -1.f;
	};
}


/*
���� ������ ��� ��ü���� IslandID�� ������ �� ����.
������ ��ü�� ���� �ٸ� ���� ������ ��ü�� ����(attach)�� ������.
���� �̷��� ���ϴ� ��ü�� ID���� -1��.
���� �����ؼ� 2�� �̻��� ���Ἲ�� Ȯ�εǴ� ��ü���� IslandID���� �ο���

������ ����(dettach)�� ��쿣 Island�� �ٽ� ����.

ex) ������� 1-2-3-4�� ���Ἲ�� ���� ������Ʈ�� �ִٰ� �������� ��
	2�� ������ ��, 2�� ���Ἲ�� ������� ������ ID���� -1�� �ʱ�ȭ �ȴ�.
	2�� ���Ἲ�� ���� ������Ʈ���� 2���� ���Ἲ�� �������� �ȴ�.

	���� Island���� 2�� ������ ���Ա� ������
	�����ִ� ��ü���� ���Ἲ�� Ȯ���ϰ� Island�� ���Ӱ� ���������Ѵ�.
	2�� ����Ǿ� �־��� 1�� 3 �������� Ž���� �Ѵ�.
	1 �������δ� ��ü�� 1���� �����ϱ� ������ Island�� �̷��� ���ϰ� ID���� -1�� �ʱ�ȭ�ȴ�.
	3 �������δ� 4��� ��ü�� ���Ἲ�� Ȯ�εǱ� ������
	3 ������ Island���� ���ο� IslandID���� �ο��޴´�.
*/