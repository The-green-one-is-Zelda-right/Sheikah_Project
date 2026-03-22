#include "PzObject.h"

#include "AttachSystem.h"


namespace Phyzzle
{
	PzObject::~PzObject()
	{
	}

	void PzObject::Start()
	{
		body = GetGameObject()->GetComponent<PurahEngine::RigidBody>();

		auto list = GetGameObject()->GetComponents<PurahEngine::Collider>();
		for (auto& col : list)
			colliders.emplace_back(col);

		assert(body != nullptr);
	}

	void PzObject::Update()
	{

	}

	void PzObject::LateUpdate()
	{
		bool enter = !preState && select;
		bool stay = preState && select;
		bool exit = preState && !select;

		if (attachable)
		{
			if (enter || stay)
			{
				AttachSystem::Instance()->EnableOutline(attachable, &(AttachSystem::Instance()->color2), nullptr);
			}
			else if (exit)
			{
				AttachSystem::Instance()->DisableOutline(attachable);
			}
		}

		preState = select;
	}

	IslandID PzObject::GetIslandID() const
	{
		return islandID;
	}

	void PzObject::ValiantStore()
	{
		if (!body)
			return;

		selectionSnapshot.Reset();
		selectionSnapshot.materials.reserve(colliders.size());

		selectionSnapshot.isKinematic = body->IsKinematic();
		selectionSnapshot.hasGravity = body->HasGravity();
		selectionSnapshot.mass = body->GetMass();
		selectionSnapshot.inertiaTensor = body->GetInertiaTensor();

		for (auto* col : colliders)
		{
			if (!col)
			{
				selectionSnapshot.materials.emplace_back();
				continue;
			}

			selectionSnapshot.materials.emplace_back(col->GetMaterial());
		}

		assert(selectionSnapshot.IsValid());
	}

	void PzObject::Selected()
	{
		if (select || !body)
			return;

		ValiantStore();

		if (!selectionSnapshot.IsValid())
			return;

		body->SetKinematic(selectionOverride.isKinematic);
		body->UseGravity(selectionOverride.hasGravity);
		body->SetMass(selectionOverride.mass);
		body->SetInertiaTensor(selectionOverride.inertiaTensor);

		for (auto* col : colliders)
		{
			if (col)
				col->SetMaterial(selectionOverride.material);
		}

		select = true;
	}

	void PzObject::ValiantRetrieve()
	{
		if (!select || !body)
			return;

		assert(selectionSnapshot.IsValid());

		if (!selectionSnapshot.IsValid())
		{
			select = false;
			selectionSnapshot.Reset();
			return;
		}

		body->SetKinematic(selectionSnapshot.isKinematic);
		body->UseGravity(selectionSnapshot.hasGravity);
		body->SetMass(selectionSnapshot.mass);
		body->SetInertiaTensor(selectionSnapshot.inertiaTensor);

		const size_t materialCount = colliders.size() < selectionSnapshot.materials.size()
			? colliders.size()
			: selectionSnapshot.materials.size();
		for (size_t i = 0; i < materialCount; i++)
		{
			if (colliders[i])
				colliders[i]->SetMaterial(selectionSnapshot.materials[i]);
		}

		selectionSnapshot.Reset();

		select = false;
	}

	void PzObject::OnCollisionEnter(
		const ZonaiPhysics::ZnCollision& _collision, 
		const PurahEngine::Collider* _collider)
	{
		if (attachable)
			return;

		PurahEngine::GameObject* obj = _collider->GetGameObject();
		
		if (obj->tag.IsContain(L"Attachable"))
		{
			attachable = obj->GetComponent<PzObject>();

			if (!attachable)
			{
				PHYSCIS_CAUTUON(Error: The subject does not possess a PzObject, Client.PzObject)
				return;
			}

			const bool otherIDNull = attachable->islandID == nullptr;
			const bool myIDNull = islandID == nullptr;
			const bool sameNull = otherIDNull && myIDNull;
			const bool sameID = attachable->islandID == islandID;
			bool diffID = !sameNull && sameID;					// 둘 다 널이 아니고 

			if (diffID)
			{
				attachable = nullptr;
				return;
			}

			// 앵커 위치
			Eigen::Vector3f acc = Eigen::Vector3f::Zero();
			for (int i = 0; i < _collision.contactCount; i++)
			{
				acc += _collision.contacts[i].point;
			}
			worldAnchor = (acc / _collision.contactCount);
		}
	}

	void PzObject::OnCollisionStay(const ZonaiPhysics::ZnCollision& _collision,
		const PurahEngine::Collider* _collider)
	{
		if (!attachable)
			return;

		PurahEngine::GameObject* obj = _collider->GetGameObject();

		if (attachable == obj->GetComponent<PzObject>())
		{
			Eigen::Vector3f acc = Eigen::Vector3f::Zero();
			for (int i = 0; i < _collision.contactCount; i++)
			{
				acc += _collision.contacts[i].point;
			}
			worldAnchor = (acc / _collision.contactCount);
		}
	}

	void PzObject::OnCollisionExit(
		const ZonaiPhysics::ZnCollision& _collision,
		const PurahEngine::Collider* _collider)
	{
		if (!attachable)
			return;

		PurahEngine::GameObject* obj = _collider->GetGameObject();

		if (obj->tag.IsContain(L"Attachable"))
		{
			if (attachable == obj->GetComponent<PzObject>())
			{
				if (select)
				{
					AttachSystem::Instance()->DisableOutline(attachable);
				}

				attachable = nullptr;
				worldAnchor = Eigen::Vector3f::Zero();
			}
		}
	}

	void PzObject::PreDeserialize(const json& jsonData)
	{
		// PREDESERIALIZE_BASE();
	}

	void PzObject::PostDeserialize(const json& jsonData)
	{
		//const json& materials = jsonData["connectedObject"];
		//size_t connectedCount = materials.size();
		//std::vector<PzObject*> deserialize(connectedCount);

		//for (size_t i = 0; i < connectedCount; i++)
		//{
		//	PzObject* connectedObject;
		//	POSTDESERIALIZE_PTR(connectedObject);
		//	deserialize[i] = connectedObject;
		//}
	}
}
