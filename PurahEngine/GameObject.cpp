#include "GameObject.h"
#include "Component.h"

void PurahEngine::GameObject::Awake()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->Awake();
	}
}

void PurahEngine::GameObject::Start()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->Start();
	}
}

void PurahEngine::GameObject::FixedUpdate()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->FixedUpdate();
	}
}

void PurahEngine::GameObject::Update()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->Update();
	}
}

void PurahEngine::GameObject::LateUpdate()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->LateUpdate();
	}
}

void PurahEngine::GameObject::OnCollisionEnter()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnCollisionEnter();
	}
}

void PurahEngine::GameObject::OnCollisionStay()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnCollisionStay();
	}
}

void PurahEngine::GameObject::OnCollisionExit()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnCollisionExit();
	}
}

void PurahEngine::GameObject::OnTriggerEnter()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnTriggerEnter();
	}
}

void PurahEngine::GameObject::OnTriggerStay()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnTriggerStay();
	}
}

void PurahEngine::GameObject::OnTriggerExit()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnTriggerExit();
	}
}

void PurahEngine::GameObject::OnMouseEnter()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnMouseEnter();
	}
}

void PurahEngine::GameObject::OnMouseStay()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnMouseStay();
	}
}

void PurahEngine::GameObject::OnMouseExit()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnMouseExit();
	}
}

std::wstring PurahEngine::GameObject::GetName()
{
	return name;
}

PurahEngine::GameObject::GameObject(std::wstring objectname)
{
	name = objectname;
}

PurahEngine::GameObject::~GameObject()
{
	componentList.clear();
}
