
#include "Component.h"

PurahEngine::Component::Component() : gameObject(nullptr), state(ComponentState::CREATE)
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

void PurahEngine::Component::OnEnable()
{

}

void PurahEngine::Component::OnDisable()
{

}

void PurahEngine::Component::OnDestroy()
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

void PurahEngine::Component::OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const Collider* collider)
{

}

void PurahEngine::Component::OnCollisionStay(const ZonaiPhysics::ZnCollision& collision, const Collider* collider)
{

}

void PurahEngine::Component::OnCollisionExit(const ZonaiPhysics::ZnCollision& collision, const Collider* collider)
{

}

void PurahEngine::Component::OnTriggerEnter(const Collider* collider)
{

}

void PurahEngine::Component::OnTriggerStay(const Collider* collider)
{

}

void PurahEngine::Component::OnTriggerExit(const Collider* collider)
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

void PurahEngine::Component::PreInitialize()
{

}

void PurahEngine::Component::Initialize()
{

}

void PurahEngine::Component::PostInitialize()
{

}

PurahEngine::GameObject* PurahEngine::Component::GetGameObject() const
{
	return gameObject;
}

PurahEngine::Component::ComponentState PurahEngine::Component::GetState()
{
	return state;
}
