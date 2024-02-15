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

void PurahEngine::Component::FixedUpdate()
{

}

void PurahEngine::Component::Update()
{

}

void PurahEngine::Component::LateUpdate()
{

}

void PurahEngine::Component::OnCollisionEnter()
{

}

void PurahEngine::Component::OnCollisionStay()
{

}

void PurahEngine::Component::OnCollisionExit()
{

}

void PurahEngine::Component::OnTriggerEnter()
{

}

void PurahEngine::Component::OnTriggerStay()
{

}

void PurahEngine::Component::OnTriggerExit()
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
