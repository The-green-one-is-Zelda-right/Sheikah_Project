#include "GameObject.h"
#include "Component.h"

void PurahEngine::GameObject::AwakeEvent()
{
	for (PurahEngine::Component* component : componentList)
	{
		if (state == ObjectState::CREATE)
		{
			component->Awake();
		}
	}
	if (isEnable == true)
	{
		Enable();
	}
	else
	{
		Disable();
	}
}

void PurahEngine::GameObject::StartEvent()
{
	if (state == ObjectState::ENABLE)
	{
		for (PurahEngine::Component* component : componentList)
		{
			component->Start();
		}
	}
}

void PurahEngine::GameObject::FixedUpdateEvent()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->FixedUpdate();
	}
}

void PurahEngine::GameObject::UpdateEvent()
{
	if (state == ObjectState::ENABLE)
	{
		for (PurahEngine::Component* component : componentList)
		{
			component->Update();
		}
	}
}

void PurahEngine::GameObject::LateUpdateEvent()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->LateUpdate();
	}
}

void PurahEngine::GameObject::Enable()
{
	state = ObjectState::ENABLE;
	isEnable = true;
}

void PurahEngine::GameObject::Disable()
{
	state = ObjectState::DISABLE;
	isEnable = false;

	if (trans->GetParent() != nullptr)
	{
		state = ObjectState::DISABLE;
		isEnable = false;
	}
	else
	{
		for (PurahEngine::Transform* test : trans->GetChildren())
		{
			test->GetGameObject()->state = ObjectState::DISABLE;
			test->GetGameObject()->isEnable = false;
		}
	}
}

bool PurahEngine::GameObject::IsEnable()
{
	return isEnable;
}

bool PurahEngine::GameObject::IsRootEnable()
{
	bool rootEnable = true;

	if (trans->GetParent() != nullptr)
	{
		rootEnable = trans->GetParent()->GetGameObject()->IsRootEnable();
	}
	else
	{
		rootEnable = IsEnable();
	}

	bool ownEnable = IsEnable() && rootEnable;

	return ownEnable;
}

PurahEngine::Transform* PurahEngine::GameObject::GetTransform()
{
	return trans;
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

void PurahEngine::GameObject::SetEnable(bool isTrue)
{
	isEnable = isTrue;
}

std::wstring PurahEngine::GameObject::GetName()
{
	return name;
}

PurahEngine::GameObject::GameObject(std::wstring objectname)
	: trans(nullptr)
{
	state = ObjectState::CREATE;
	name = objectname;
	isEnable = true;
}

PurahEngine::GameObject::GameObject(std::wstring objectname, bool isenable)
	: trans(nullptr)
{
	state = ObjectState::CREATE;
	name = objectname;
	isEnable = isenable;
}

PurahEngine::GameObject::~GameObject()
{
	componentList.clear();
}
