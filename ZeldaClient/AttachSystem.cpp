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
		attachIsland.erase(_id);

		removedIndex.push(_id);						// ������ ID�� ť�� ����
	}

	IslandID AttachSystem::CreateIsland(const AttachIsland& _arr)
	{
		IslandID newID = CreateIslandID();

		for (auto& e : _arr)
			e->islandID = newID;

		attachIsland.insert(std::make_pair(newID, _arr));

		return newID;
	}

	void AttachSystem::RemoveIsland(const IslandID& _id)
	{
		if (_id == nullptr)
			return;

		if (!attachIsland.contains(_id))
			return;

		const AttachIsland& arr = attachIsland[_id];

		for (size_t i = 0; i < arr.size(); i++)
		{
			arr[i]->islandID = nullptr;				// ���� �ִ� ģ������ ID�� �ʱ�ȭ��.
		}

		RemoveIslandID(_id);
	}


	void AttachSystem::SelectBody(Attachable* _body)
	{
		const IslandID id = _body->GetIslandID();

		if (id == nullptr)		// ���� �̷�� �ִ°�?
		{
			_body->ValiantStore();
			_body->Selected();			// ȥ�ڸ� ��
		}
		else
		{
			if (attachIsland.contains(id))
			{
				const AttachIsland& arr = attachIsland[id];		// ���� ��θ� ��� �ø�

				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i]->ValiantStore();
					arr[i]->Selected();
				}
			}

			else
			{
				assert(0);
			}
		}
	}

	void AttachSystem::DeselectBody(Attachable* _body)
	{
		const IslandID id = _body->GetIslandID();

		if (id == nullptr)		// ���� �̷�� �ִ°�?
		{
			_body->ValiantRetrieve();			// ȥ�ڸ� ��
		}
		else
		{
			if (attachIsland.contains(id))
			{
				const AttachIsland& arr = attachIsland[id];		// ���� ��θ� ��� �ø�

				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i]->ValiantRetrieve();
				}
			}
			else
			{
				assert(0);
			}
		}
	}

	bool AttachSystem::Attach(Attachable* _object)
	{
		Attachable* _other = _object->attachable;

		if (!_other)
			return false;

		const IslandID obj0ID = _object->GetIslandID();
		const IslandID obj1ID = _other->GetIslandID();

		// ���� ������ �ִ��� �ƴ��� üũ
		AttachIsland island0;
		if (!HasAttachIsland(obj0ID, island0))
			island0.push_back(_other);

		AttachIsland island1;
		if (!HasAttachIsland(obj1ID, island1))
			island1.push_back(_object);

		// �������ְ�
		ConnectNode(_object, _other);
		ConnectJoint(_object, _other);

		// ���ο� ID�� �ο�
		island0.insert(island0.end(), island1.begin(), island1.end());
		CreateIsland(island0);

		return true;
	}

	bool AttachSystem::Dettach(Attachable* _object)
	{
		// ����Ȱ� ������ �ƹ��͵� ����.
		if (_object->connectedObjects.empty())
			return false;

		// ����ƾ��� ��ü�� �ϴ� �����ص�.
		const AttachIsland temp = _object->connectedObjects;

		// ����� ��ü ��ȸ�ϸ鼭 ������ ������
		for (auto& _other : temp)
		{
			const auto itr0 = std::ranges::find(_object->connectedObjects, _other);
			if (itr0 == _object->connectedObjects.end())
			{
				_object->connectedObjects.erase(itr0);
			}
			const auto itr1 = std::ranges::find(_other->connectedObjects, _object);
			if (itr1 == _other->connectedObjects.end())
			{
				_other->connectedObjects.erase(itr1);
			}
		}

		// ����ƾ��� ��ü�� ��ȸ�ϸ鼭 Island�� �������.
		for (auto& _other : temp)
		{
			// 
			std::queue<Attachable*> search;

			search.push(_other);

			while (!search.empty())
			{
				
			}
		}

		return true;
	}

	void AttachSystem::ConnectNode(Attachable* _base, Attachable* _other)
	{
		_base->connectedObjects.push_back(_other);
		_other->connectedObjects.push_back(_other);
	}

	void AttachSystem::ConnectJoint(Attachable* _base, Attachable* _other)
	{
		// ����Ʈ ������ְ�
		auto joint = _base->GetGameObject()->AddComponent<PurahEngine::FixedJoint>();
		joint->SetRigidbody(_other->GetGameObject()->GetComponent<PurahEngine::RigidBody>());

		const Eigen::Vector3f worldP = _base->worldAnchor;
		const Eigen::Quaternionf worldQ = Eigen::Quaternionf::Identity();

		// ������ ���� ��Ŀ ������ְ�
		Eigen::Vector3f baseP;
		Eigen::Quaternionf baseQ;
		CalculateLocalAnchor(worldP, worldQ, _base, baseP, baseQ);

		Eigen::Vector3f otherP;
		Eigen::Quaternionf otherQ;
		CalculateLocalAnchor(worldP, worldQ, _other, otherP, otherQ);

		// ��Ŀ ����
		joint->SetAnchor(baseP, baseQ, otherP, otherQ);
	}

	bool AttachSystem::HasAttachIsland(const IslandID& _id, AttachIsland& _outIsland)
	{
		// ���� ������ ���� ���
		if (_id != nullptr)
		{
			// �� ID�� ����
			_outIsland = attachIsland[_id];
			RemoveIslandID(_id);
			return true;
		}
		else
		{
			return false;
		}
	}

	void AttachSystem::CalculateLocalAnchor(
		const Eigen::Vector3f& _anchorP, const Eigen::Quaternionf& _anchorQ,
		const Attachable* _base,
		Eigen::Vector3f& _outP, Eigen::Quaternionf& _outQ)
	{
		const Eigen::Vector3f one = Eigen::Vector3f( 1.f, 1.f, 1.f );

		Eigen::Affine3f anchorMat = Eigen::Affine3f::Identity();
		anchorMat.fromPositionOrientationScale(_anchorP, _anchorQ, one);

		Eigen::Affine3f bodyMat = Eigen::Affine3f::Identity();
		bodyMat.fromPositionOrientationScale(
			_base->gameObject->GetTransform()->GetWorldPosition(), 
			_base->gameObject->GetTransform()->GetWorldRotation(), 
			one);

		Eigen::Transform localT = bodyMat.inverse() * anchorMat;

		_outP = localT.translation();
		_outQ = localT.rotation();
	}
}
