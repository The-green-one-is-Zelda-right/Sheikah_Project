#include "AttachSystem.h"

#include <numbers>

#include "Attachable.h"
#include "RigidBody.h"


namespace Phyzzle
{
	AttachSystem::~AttachSystem()
		= default;

	// ID ����
	IslandID AttachSystem::CreateIslandID()
	{
		while (!removedIndex.empty())			
		{
			auto result = removedIndex.front();
			removedIndex.pop();

			// ������ �ε����� �ִٸ�
			// �װ� ��Ȱ�� ��.
			return result;
		}

		static long long Identifier = 0;

		return { Identifier++ };
	}

	// ID ����
	void AttachSystem::RemoveIslandID(const IslandID& _id)
	{
		removedIndex.push(_id);						// ������ ID�� ť�� ����
	}

	IslandID AttachSystem::CreateIsland(const std::vector<Attachable*>& _arr)
	{
		IslandID newID = CreateIslandID();
		attachIsland.push_back({ newID, _arr });

		return newID;
	}

	void AttachSystem::RemoveIsland(const IslandID& _id)
	{
		const size_t index(_id);
		const auto& arr = attachIsland[index].elements;

		for (size_t i = 0; i < arr.size(); i++)
		{
			arr[i]->islandID = nullptr;				// ���� �ִ� ģ������ ID�� �ʱ�ȭ��.
		}

		RemoveIslandID(_id);
	}


	void AttachSystem::SelectBody(Attachable* _body) const
	{
		if (const IslandID id = _body->GetIslandID())		// ���� �̷�� �ִ°�?
		{
			_body->Selected();			// ȥ�ڸ� ��
		}
		else
		{
			const size_t index(id);
			const auto& arr = attachIsland[index].elements;		// ���� ��θ� ��� �ø�

			for (size_t i = 0; i < arr.size(); i++)
			{
				arr[i]->Selected();
			}
		}
	}

	void AttachSystem::Attatch(Attachable* _object)
	{
		// ���������� �����ϴ� ���

		// ������Ʈ�� �����ϱ� ��
		// �����ϴ� ������Ʈ�� ���� ������ ���� ���
		// �� ID�� ����

		// �������ְ� ���ο� ID�� �ο�
	}

	void AttachSystem::Dettatch(Attachable* _object)
	{

	}
}
