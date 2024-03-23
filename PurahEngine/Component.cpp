
#include "Component.h"

PurahEngine::Component::Component() : gameObject(nullptr)
{

}

PurahEngine::Component::~Component()
= default;

void PurahEngine::Component::Awake()
{

}

void PurahEngine::Component::Start()
{

}

void PurahEngine::Component::OnDataLoadComplete()
{
}

void PurahEngine::Component::FixedUpdate()
{

}

void PurahEngine::Component::Update()
{

}

void PurahEngine::Component::LateUpdate()
{

}

void PurahEngine::Component::OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const ZonaiPhysics::ZnCollider* collider)
{

}

void PurahEngine::Component::OnCollisionStay(const ZonaiPhysics::ZnCollision& collision, const ZonaiPhysics::ZnCollider* collider)
{

}

void PurahEngine::Component::OnCollisionExit(const ZonaiPhysics::ZnCollision& collision, const ZonaiPhysics::ZnCollider* collider)
{

}

void PurahEngine::Component::OnTriggerEnter(const ZonaiPhysics::ZnCollider* collider)
{

}

void PurahEngine::Component::OnTriggerStay(const ZonaiPhysics::ZnCollider* collider)
{

}

void PurahEngine::Component::OnTriggerExit(const ZonaiPhysics::ZnCollider* collider)
{

}

void PurahEngine::Component::OnMouseEnter()
{

}

void PurahEngine::Component::OnMouseStay()
{

}

void PurahEngine::Component::OnMouseExit()
{

}

void PurahEngine::Component::Initialize()
{

}

PurahEngine::GameObject* PurahEngine::Component::GetGameObject() const
{
	return gameObject;
}
