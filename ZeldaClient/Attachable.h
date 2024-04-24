#pragma once
#include "PurahEngine.h"
#include <vector>


namespace Phyzzle
{
	struct AttachIsland;
	
	struct IslandID
	{
		long long id;
	};

	class Attachable : public PurahEngine::Component
	{
	public:
		~Attachable() override;

		void Selected();

	private:
		IslandID islandID;
		bool isKinematic = false;
		bool hasGravity = false;
		float originMass = -1.f;
	};

	struct AttachIsland
	{
		IslandID islandID;
	};
}


/*
���� ������ ��� ������Ʈ���� IslandID�� ������ �� ����.
������ ������Ʈ�� ���� �����ϰ� �Ǹ� ����Ǹ鼭 ID�� �ο���.

������ ������ ��쿣 Island�� �ٽ� ����.
ex) ������� 1-2-3-4�� ���Ἲ�� ���� ������Ʈ�� �ִٰ� �������� ��
	2�� ������ ��, 1 �������� Ž���� �Ѵ�.

*/