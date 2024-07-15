#include "AttachSystem.h"

#include <numbers>

#include "ZnBound3.h"
#include "PzObject.h"
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
		if (_arr.size() < 2)
		{
			return nullptr;
		}

		IslandID newID = CreateIslandID();

		attachIsland.insert(std::make_pair(newID, _arr));

		for (auto& e : _arr)
		{
			e->islandID = newID;
		}

		return newID;
	}

	// �� ����
	void AttachSystem::RemoveIsland(IslandID _id)
	{
		if (_id == nullptr || !attachIsland.contains(_id))
		{
			return;
		}

		const AttachIsland& arr = attachIsland[_id];

		for (auto& e : arr)
		{
			e->islandID = nullptr;
		}

		RemoveIslandID(_id);
	}

	void AttachSystem::Clear()
	{
		attachIsland.clear();

		std::queue<IslandID> emptyQueue;
		std::swap(removedIndex, emptyQueue);
	}

	// ��ü ����
	void AttachSystem::SelectBody(PzObject* _body)
	{
		const IslandID id = _body->GetIslandID();

		AttachIsland island;

		if (!HasAttachIsland(id, island))
			island.emplace_back(_body);

		for (size_t i = 0; i < island.size(); i++)
		{
			island[i]->Selected();
		}
	}

	// ��ü ���� ����
	void AttachSystem::DeselectBody(PzObject* _body)
	{
		const IslandID id = _body->GetIslandID();

		AttachIsland island;

		if (!HasAttachIsland(id, island))
			island.emplace_back(_body);

		for (size_t i = 0; i < island.size(); i++)
		{
			island[i]->ValiantRetrieve();
		}
	}

	void AttachSystem::SetOutlineColor(Eigen::Vector4f* const _color0, Eigen::Vector4f* const _color1, Eigen::Vector4f* const _color2)
	{
		if (_color0)
			color0 = *_color0;
		if (_color1)
			color1 = *_color1;
		if (_color2)
			color2 = *_color2;
	}

	void AttachSystem::ApplyOutlineSettings(PzObject* obj, bool value, Eigen::Vector4f* const color)
	{
		auto children = obj->GetGameObject()->GetTransform()->GetChildren();

		auto model = obj->GetGameObject()->GetComponent<PurahEngine::ModelRenderer>();
		if (model)
		{
			if (color)
				model->SetOutLineColor(*color);
			
			model->SetOutLine(value);
		}

		auto mesh = obj->GetGameObject()->GetComponent<PurahEngine::MeshRenderer>();
		if (mesh)
		{
			if (color)
				mesh->SetOutLineColor(*color);
			mesh->SetOutLine(value);
		}

		for (auto& child : children)
		{
			auto model = child->GetGameObject()->GetComponent<PurahEngine::ModelRenderer>();
			if (model)
			{
				if (color)
					model->SetOutLineColor(*color);
				model->SetOutLine(value);
			}

			auto mesh = child->GetGameObject()->GetComponent<PurahEngine::MeshRenderer>();
			if (mesh)
			{
				if (color)
					mesh->SetOutLineColor(*color);
				mesh->SetOutLine(value);
			}
		}
	}

	void AttachSystem::ApplyDShadowSettings(PzObject* obj, bool value)
	{
		auto children = obj->GetGameObject()->GetTransform()->GetChildren();

		auto model = obj->GetGameObject()->GetComponent<PurahEngine::ModelRenderer>();
		if (model)
		{
			model->SetDShadow(value);
		}

		auto mesh = obj->GetGameObject()->GetComponent<PurahEngine::MeshRenderer>();
		if (mesh)
		{
			mesh->SetDShadow(value);
		}

		for (auto& child : children)
		{
			auto model = child->GetGameObject()->GetComponent<PurahEngine::ModelRenderer>();
			if (model)
			{
				model->SetDShadow(value);
			}

			auto mesh = child->GetGameObject()->GetComponent<PurahEngine::MeshRenderer>();
			if (mesh)
			{
				mesh->SetDShadow(value);
			}
		}
	}

	void AttachSystem::EnableOutline(
		PzObject* _obj, 
		Eigen::Vector4f* const _targetColor,
		Eigen::Vector4f* const _subColor)
	{
		const IslandID objID = _obj->GetIslandID();
		AttachIsland island;

		if (!HasAttachIsland(objID, island))
			island.emplace_back(_obj);

		if (_subColor)
		{
			for (auto& obj : island)
			{
				ApplyOutlineSettings(obj, true, _subColor);
			}
		}

		if (_targetColor)
		{
			ApplyOutlineSettings(_obj, true, _targetColor);
		}
	}

	void AttachSystem::DisableOutline(PzObject* _obj)
	{
		const IslandID objID = _obj->GetIslandID();
		AttachIsland island;

		if (!HasAttachIsland(objID, island))
			island.emplace_back(_obj);

		for (auto& obj : island)
		{
			ApplyOutlineSettings(obj, false, nullptr);
		}

		ApplyOutlineSettings(_obj, false, nullptr);
	}

	void AttachSystem::EnableDShadow(PzObject* _obj)
	{
		const IslandID objID = _obj->GetIslandID();
		AttachIsland island;

		if (!HasAttachIsland(objID, island))
			island.emplace_back(_obj);

		for (auto& obj : island)
		{
			ApplyDShadowSettings(obj, true);
		}

		ApplyDShadowSettings(_obj, true);
	}

	void AttachSystem::DisableDShadow(PzObject* _obj)
	{
		const IslandID objID = _obj->GetIslandID();
		AttachIsland island;

		if (!HasAttachIsland(objID, island))
			island.emplace_back(_obj);

		for (auto& obj : island)
		{
			ApplyDShadowSettings(obj, false);
		}

		ApplyDShadowSettings(_obj, false);
	}

	// ��ü ����
	bool AttachSystem::TryAttach(PzObject* _object)
	{
		// ������ ������Ʈ�� ���� ������ �ִ��� �ƴ��� üũ
		const IslandID obj0ID = _object->GetIslandID();
		AttachIsland island0;

		if (!HasAttachIsland(obj0ID, island0))
			island0.emplace_back(_object);

		for (const auto& e : island0)
		{
			if (e->attachable && _object != e->attachable)
			{
				Attach(e, e->attachable);
				e->attachable->attachable = nullptr;
				e->attachable = nullptr;
				return true;
			}
		}

		return false;
	}

	bool AttachSystem::Attach(PzObject* _base, PzObject* _other)
	{
		// ����Ʈ�� �������ְ� ��嵵 ��������
		const auto joint = CreateJoint(_base, _other);
		if (!joint)
		{
			MessageBox(0, L"Joint is NULL", L"Attach Error", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		ConnectNode(_base, _other, joint);

		// �� �ٽ� ����
		RebuildIsland(_base, _other);

		return true;
	}

	bool AttachSystem::Dettach(PzObject* _object)
	{
		// ����Ȱ� ������ �ƹ��͵� ����.
		if (_object->connectedObjects.empty())
			return false;

		RemoveIsland(_object->islandID);
		AttachIsland temp = _object->connectedObjects;

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
			std::queue<PzObject*> search;
			search.push(_other);

			while (!search.empty())
			{
				auto obj = search.front();
				search.pop();

				// ���� ����
				obj->ValiantRetrieve();
				island.emplace_back(obj);

				for (auto& e : obj->connectedObjects)
				{
					// ����� �ֵ��߿� ���� �Ǿ��ִ� ģ������ ť�� �ְ� Ž��
					if (e->select)
						search.push(e);
				}
			}

			CreateIsland(island);

			DisableOutline(island.front());
		}

		return true;
	}

	void AttachSystem::ConnectNode(PzObject* _base, PzObject* _other, PurahEngine::FixedJoint* _joint)
	{
		_base->connectedObjects.push_back(_other);
		_other->connectedObjects.push_back(_base);
	}

	void AttachSystem::DisconnectNode(PzObject* _base, PzObject* _other)
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

	PurahEngine::FixedJoint* AttachSystem::CreateJoint(PzObject* _base, PzObject* _other)
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

	void AttachSystem::BreakJoint(PzObject* _base, PzObject* _other)
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

	void AttachSystem::RebuildIsland(PzObject* _base, PzObject* _other)
	{
		const IslandID obj0ID = _base->GetIslandID();
		const IslandID obj1ID = _other->GetIslandID();

		AttachIsland island0;
		AttachIsland island1;

		// ���� ������ �ӽ� �迭�� ����
		if (!HasAttachIsland(obj0ID, island0))
			island0.push_back(_base);

		// ���� ������ �ӽ� �迭�� ����
		if (!HasAttachIsland(obj1ID, island1))
			island1.push_back(_other);

		// ���ο� ID�� �ο�
		island0.insert(island0.end(), island1.begin(), island1.end());
		RemoveIslandID(obj0ID);
		RemoveIslandID(obj1ID);
		CreateIsland(island0);
	}

	void AttachSystem::CalculateLocalAnchor(
		const Eigen::Vector3f& _anchorP, const Eigen::Quaternionf& _anchorQ,
		const PzObject* _base,
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

	ZonaiPhysics::ZnBound3 AttachSystem::CalculateBoundingBox(PzObject* const _base, const Eigen::Matrix4f& _mat)
	{
		using namespace Eigen;

		// ��Ʈ�������� �������̶� �����̼��� ����
		Transform<float, 3, Eigen::Affine> transform{ _mat };
		const Vector3f pos{ transform.translation() };
		const Quaternionf rot{ transform.rotation() };
		
		const IslandID id = _base->GetIslandID();
		AttachIsland island;
		if (!HasAttachIsland(id, island))
			island.emplace_back(_base);

		Vector3f globalMinBounds(FLT_MAX, FLT_MAX, FLT_MAX);
		Vector3f globalMaxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (auto& obj : island)
		{
			PurahEngine::RigidBody* const body = obj->GetGameObject()->GetComponent<PurahEngine::RigidBody>();

			if (body)
			{
				ZonaiPhysics::ZnBound3 bound = body->GetBoundingBox(pos, rot);

				globalMinBounds = globalMinBounds.cwiseMin(bound.minimum);
				globalMaxBounds = globalMaxBounds.cwiseMax(bound.maximum);
			}
		}

		return ZonaiPhysics::ZnBound3(globalMinBounds, globalMaxBounds);
	}
}
