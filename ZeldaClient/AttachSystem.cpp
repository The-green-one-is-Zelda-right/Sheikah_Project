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
	void AttachSystem::RemoveIslandID(IslandID _id)
	{
		if (!attachIsland.contains(_id))
			return;

		attachIsland.erase(_id);
		removedIndex.push(_id);						// ������ ID�� ť�� ����
	}

	// �� ����
	IslandID AttachSystem::CreateIsland(const AttachIsland& _arr)
	{
		IslandID newID;;
		
		if (_arr.size() < 2)
		{
			newID = nullptr;
		}
		else
		{
			newID = CreateIslandID();

			attachIsland.insert(std::make_pair(newID, _arr));
		}

		for (auto& e : _arr)
			e->islandID = newID;

		return newID;
	}

	// �� ����
	void AttachSystem::RemoveIsland(IslandID _id)
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

	void AttachSystem::Clear()
	{
		attachIsland.clear();

		while (!removedIndex.empty())
		{
			removedIndex.pop();
		}
	}

	// ��ü ����
	void AttachSystem::SelectBody(Attachable* _body)
	{
		const IslandID id = _body->GetIslandID();

		if (id == nullptr)		// ���� �̷�� �ִ°�?
		{
			_body->Selected();			// ȥ�ڸ� ��
		}
		else
		{
			if (attachIsland.contains(id))
			{
				const AttachIsland& arr = attachIsland[id];		// ���� ��θ� ��� �ø�

				for (size_t i = 0; i < arr.size(); i++)
				{
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

	void AttachSystem::EnableOutline(
		Attachable* _obj, 
		bool _value, 
		const Eigen::Vector4f& _targetColor, 
		const Eigen::Vector4f& _subColor)
	{
		const IslandID objID = _obj->GetIslandID();
		AttachIsland island;

		if (!HasAttachIsland(objID, island))
			island.push_back(_obj);

		for (auto& obj : island)
		{
			auto model = obj->GetGameObject()->GetComponent<PurahEngine::ModelRenderer>();
			if (model)
			{
				model->SetOutLineColor(_subColor);
				model->SetOutLine(_value);
			}

			auto mesh = obj->GetGameObject()->GetComponent<PurahEngine::MeshRenderer>();
			if (mesh)
			{
				mesh->SetOutLineColor(_subColor);
				mesh->SetOutLine(_value);
			}
		}

		auto model = _obj->GetGameObject()->GetComponent<PurahEngine::ModelRenderer>();
		if (model)
		{
			model->SetOutLineColor(_targetColor);
			model->SetOutLine(_value);
		}

		auto mesh = _obj->GetGameObject()->GetComponent<PurahEngine::MeshRenderer>();
		if (mesh)
		{
			mesh->SetOutLineColor(_targetColor);
			mesh->SetOutLine(_value);
		}
	}

	// ��ü ����
	bool AttachSystem::TryAttach(Attachable* _object)
	{
		// ������ ������Ʈ�� ���� ������ �ִ��� �ƴ��� üũ

		const IslandID obj0ID = _object->GetIslandID();

		AttachIsland island0;

		// ���� ������ �ӽ� �迭�� ����
		if (!HasAttachIsland(obj0ID, island0))
			island0.push_back(_object);

		Attachable* base = nullptr;
		Attachable* other = nullptr;

		for (const auto& e : island0)
		{
			if (e->attachable && (_object != e->attachable))
			{
				base = e;
				other = e->attachable;
				break;
			}
		}

		if (!other)
			return false;

		const IslandID obj1ID = other->GetIslandID();

		AttachIsland island1;

		// ���� ������ �ӽ� �迭�� ����
		if (!HasAttachIsland(obj1ID, island1))
			island1.push_back(other);

		// ����Ʈ�� �������ְ�
		const auto joint = CreateJoint(base, other);
		
		// ��嵵 ��������
		ConnectNode(base, other, joint);

		// ���ο� ID�� �ο�
		island0.insert(island0.end(), island1.begin(), island1.end());
		RemoveIslandID(obj0ID);
		RemoveIslandID(obj1ID);
		CreateIsland(island0);

		base->attachable = nullptr;
		other->attachable = nullptr;

		return true;
	}

	bool AttachSystem::Attach(Attachable* _base, Attachable* _other)
	{
		return false;
	}

	bool AttachSystem::Dettach(Attachable* _object)
	{
		// ����Ȱ� ������ �ƹ��͵� ����.
		if (_object->connectedObjects.empty())
			return false;

		// ���� �����
		RemoveIsland(_object->islandID);

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
			AttachIsland island;
			
			std::queue<Attachable*> search;
			search.push(_other);

			while (!search.empty())
			{
				auto obj = search.front();
				search.pop();

				// ���� ����
				obj->ValiantRetrieve();

				island.push_back(obj);

				for (auto& e : obj->connectedObjects)
				{
					// ����� �ֵ��߿� ���� �Ǿ��ִ� ģ������ ť�� �ְ� Ž��
					if (e->select)
						search.push(e);
				}
			}

			CreateIsland(island);

			EnableOutline(island.front(), false);
		}

		return true;
	}

	void AttachSystem::ConnectNode(Attachable* _base, Attachable* _other, PurahEngine::FixedJoint* _joint)
	{
		_base->connectedObjects.push_back(_other);
		_other->connectedObjects.push_back(_base);
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

		const auto baseBody = _base->GetGameObject()->GetComponent<PurahEngine::RigidBody>();
		const auto otherBody = _other->GetGameObject()->GetComponent<PurahEngine::RigidBody>();
		joint->SetRigidbody(baseBody, otherBody);

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
		using BodySet = std::pair<PurahEngine::RigidBody*, PurahEngine::RigidBody*>;
		BodySet duo = std::minmax(_base->body, _other->body);

		auto joints0 = _base->GetGameObject()->GetComponents<PurahEngine::FixedJoint>();
		auto joints1 = _other->GetGameObject()->GetComponents<PurahEngine::FixedJoint>();

		for (auto& joint : joints0)
		{
			PurahEngine::RigidBody* body0 = nullptr;
			PurahEngine::RigidBody* body1 = nullptr;
			joint->GetRigidbody(body0, body1);
			BodySet test = std::minmax(body0, body1);

			if (duo == test)
			{
				auto obj = joint->GetGameObject();
				obj->DeleteComponent(joint);
			}
		}

		for (auto& joint : joints1)
		{
			PurahEngine::RigidBody* body0 = nullptr;
			PurahEngine::RigidBody* body1 = nullptr;
			joint->GetRigidbody(body0, body1);
			BodySet test = std::minmax(body0, body1);

			if (duo == test)
			{
				auto obj = joint->GetGameObject();
				obj->DeleteComponent(joint);
			}
		}
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
