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
		// const size_t index(_id);
		const AttachIsland& arr = attachIsland[_id];

		for (size_t i = 0; i < arr.size(); i++)
		{
			arr[i]->islandID = nullptr;				// ���� �ִ� ģ������ ID�� �ʱ�ȭ��.
		}

		RemoveIslandID(_id);
	}


	void AttachSystem::SelectBody(Attachable* _body)
	{
		if (const IslandID id = _body->GetIslandID())		// ���� �̷�� �ִ°�?
		{
			_body->Selected();			// ȥ�ڸ� ��
		}
		else
		{
			// const size_t index(id);
			const AttachIsland& arr = attachIsland[id];		// ���� ��θ� ��� �ø�

			for (size_t i = 0; i < arr.size(); i++)
			{
				arr[i]->Selected();
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

		AttachIsland island0;
		AttachIsland island1;

		// �����ϴ� ������Ʈ�� ���� ������ ���� ���
		// �� ID�� ����
		if (obj0ID != nullptr)
		{
			island0 = attachIsland[obj0ID];
			RemoveIslandID(obj0ID);
		}
		if (obj1ID != nullptr)
		{
			island1 = attachIsland[obj1ID];
			RemoveIslandID(obj1ID);
		}

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

		Eigen::Vector3f baseP;
		Eigen::Quaternionf baseQ;
		CalculateLocalAnchor(worldP, worldQ, _base, baseP, baseQ);

		Eigen::Vector3f otherP;
		Eigen::Quaternionf otherQ;
		CalculateLocalAnchor(worldP, worldQ, _base, otherP, otherQ);

		joint->SetAnchor(baseP, baseQ, otherP, otherQ);
	}

	void AttachSystem::CalculateLocalAnchor(
		const Eigen::Vector3f& _anchorP,
		const Eigen::Quaternionf& _anchorQ,
		Attachable* _base,
		Eigen::Vector3f& _outP,
		Eigen::Quaternionf& _outQ)
	{
		const Eigen::Matrix4f anchorMat = 
			_anchorQ.matrix() * 
			_anchorP.matrix();

		const Eigen::Matrix4f bodyMat = 
			_base->gameObject->GetTransform()->GetWorldRotation().matrix() * 
			_base->gameObject->GetTransform()->GetWorldPosition().matrix();

		Eigen::Matrix4f local = bodyMat.inverse() * anchorMat;

		_outP = local.block<3, 1>(0, 3);

		const Eigen::Matrix3f rot = local.block<3, 3>(0, 0);
		_outQ = Eigen::Quaternionf(rot);
	}
}
