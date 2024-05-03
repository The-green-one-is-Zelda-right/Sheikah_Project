#pragma once
#include <queue>

#include "AttachIsland.h"
#include "Singleton.h"

namespace Phyzzle
{
	class AttachIsland;
	class Attachable;
}

namespace PurahEngine
{
	class RigidBody;
}

namespace Phyzzle
{
	class AttachSystem final : public Singleton<AttachSystem>
	{
	public:
		~AttachSystem() override;

	private:
		// ID ���� ����
		IslandID CreateIslandID();
		void RemoveIslandID(const IslandID& _id);

		// �� ���� ����
		IslandID CreateIsland(const std::vector<Attachable*>& _arr);
		void RemoveIsland(const IslandID& _id);

	public:
		void SelectBody(Attachable* _body) const;
		void Attatch(Attachable* _base);
		void Dettatch(Attachable* _base);

	private:
		std::vector<AttachIsland> attachIsland;
		std::queue<IslandID> removedIndex;

		IslandID islandID;
		bool gravity = true;
		float mass = 1.f;
	};
}

/*
 * 1.	�÷��̾ ��Ʈ���ڵ�� ������Ʈ �ϳ��� ����
 *
 * 2.	������Ʈ�� ������ �� �ִ� ������Ʈ�� Ž��
 *
 * 3.	���� �Ÿ��� �ٴٸ��� Attatch�� ����.
 *
 * 4.	Attatch�� �ϸ� �� ������Ʈ�� ������ �ӷ����� ���ο��� �������
 *
 * 5.	�� ��ü�� �浹�� �����ϸ� �浹�� ��ġ�� Anchor ��ġ�� �ϰ� FixedJoint�� ������.
 *
 * 6.	Anchor�� ��ǥ�� ����� ������ ������ ���÷� ����ؼ� ����Ʈ�� ���� �ʿ䰡 ����.
*/