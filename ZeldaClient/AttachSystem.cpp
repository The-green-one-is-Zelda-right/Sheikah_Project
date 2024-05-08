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

	// �� ����
	IslandID AttachSystem::CreateIsland(const AttachIsland& _arr)
	{
		IslandID newID = CreateIslandID();

		for (auto& e : _arr)
			e->islandID = newID;

		attachIsland.insert(std::make_pair(newID, _arr));

		return newID;
	}

	// �� ����
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

	// ��ü ����
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

	// ��ü ���� ����
	void AttachSystem::DeselectBody(Attachable* _body)
	{
		const IslandID id = _body->GetIslandID();

		if (id == nullptr)		// ���� �̷�� �ִ°�?
		{
			_body->ValiantRetrieve();							// ȥ�ڸ�
		}
		else
		{
			if (attachIsland.contains(id))
			{
				const AttachIsland& arr = attachIsland[id];		// ���� ��θ�

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

	// ��ü ����
	bool AttachSystem::TryAttach(Attachable* _object)
	{
		// ������ ������Ʈ�� ���� ������ �ִ��� �ƴ��� üũ

		const IslandID obj0ID = _object->GetIslandID();

		AttachIsland island0;
		if (!HasAttachIsland(obj0ID, island0))
			island0.push_back(_object);

		Attachable* _other = nullptr;

		for (const auto& e : island0)
		{
			if (e->attachable && (_object != e->attachable))
			{
				_other = e->attachable;
				break;
			}
		}

		if (!_other)
			return false;

		const IslandID obj1ID = _other->GetIslandID();

		AttachIsland island1;
		if (!HasAttachIsland(obj1ID, island1))
			island1.push_back(_other);

		// �������ְ�
		const auto joint = CreateJoint(_object, _other);
		ConnectNode(_object, _other, joint);

		// ���ο� ID�� �ο�
		island0.insert(island0.end(), island1.begin(), island1.end());
		CreateIsland(island0);

		_object->attachable = nullptr;
		_other->attachable = nullptr;

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
			DisconnectNode(_object, _other);
			BreakJoint(_object, _other);
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

	void AttachSystem::ConnectNode(Attachable* _base, Attachable* _other, PurahEngine::FixedJoint* _joint)
	{
		_base->connectedObjects.push_back(_other);
		_other->connectedObjects.push_back(_other);
	}

	void AttachSystem::DisconnectNode(Attachable* _base, Attachable* _other)
	{
		const auto itr0 = std::ranges::find(_base->connectedObjects, _other);
		if (itr0 != _base->connectedObjects.end())
		{
			_base->connectedObjects.erase(itr0);
		}
		const auto itr1 = std::ranges::find(_other->connectedObjects, _base);
		if (itr1 != _other->connectedObjects.end())
		{
			_other->connectedObjects.erase(itr1);
		}
	}

	PurahEngine::FixedJoint* AttachSystem::CreateJoint(Attachable* _base, Attachable* _other)
	{
		// ����Ʈ ������ְ�
		const auto joint = _base->GetGameObject()->AddComponent<PurahEngine::FixedJoint>();
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

		return joint;
	}

	void AttachSystem::BreakJoint(Attachable* _base, Attachable* _other)
	{
		// _base->GetGameObject()->GetComponents<PurahEngine::FixedJoint>();
	}

	bool AttachSystem::HasAttachIsland(const IslandID& _id, AttachIsland& _outIsland)
	{
		// ���� ������ ���� ������
		if (_id == nullptr)
		{
			return false;
		}
		else
		{
			// �� ID�� ����
			_outIsland = attachIsland[_id];
			RemoveIslandID(_id);
			return true;
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
