#pragma once
#include "PurahEngine.h"
#include <vector>


namespace Phyzzle
{
	struct AttachIsland;
	
	struct IslandID
	{
	private:
		long long id;
	};

	class Attachable : public PurahEngine::Component
	{
	public:
		~Attachable() override;

		void Start() override;

	public:
		IslandID GetIslandID() const;
		void ValiantStore();
		void Selected();
		void ValiantRetrieve();

	private:
		PurahEngine::RigidBody* body;
		IslandID islandID;							// IslandID
		std::vector<Attachable*> connectedObjects;	// �� ��ü�� ����� ��ü��

	private:
		// Select�Ǹ� ��ü�� ���¸� �����ϱ� ���� ����
		bool isKinematic = false;					
		bool hasGravity = false;
		float originMass = -1.f;
	};

	struct AttachIsland
	{
		IslandID islandID;
		std::vector<Attachable*> elements;			// Island�� �̷�� �ִ� ��ü��
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