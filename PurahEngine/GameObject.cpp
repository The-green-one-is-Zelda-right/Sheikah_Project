#include "ZnCollider.h"
#include "ZnCollision.h"

#include "Component.h"
#include "EngineSetting.h"
#include "DataManager.h"

#include "GameObject.h"

void PurahEngine::GameObject::PostInitializeEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable /*= true*/)
{
	if (state == ObjectState::CREATE)
	{
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::CREATE)
				{
					// eventQueue에 이게 있는가? 없으면 push 있으면 스킵
					eventQueue.push({ componentList[i], (&Component::PostInitialize) });
				}
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->PostInitializeEvent(eventQueue, true);
			}
		}
	}
	else if (state == ObjectState::ENABLE)
	{
		if (isDestroy == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->PostInitializeEvent(eventQueue, false);
			}
		}

		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::CREATE)
				{
					// eventQueue에 이게 있는가? 없으면 push 있으면 스킵
					eventQueue.push({ componentList[i], (&Component::PostInitialize) });
				}
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->PostInitializeEvent(eventQueue, true);
			}
		}
		else
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->PostInitializeEvent(eventQueue, false);
			}
		}
	}
	else if (state == ObjectState::DISABLE)
	{
		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->PostInitializeEvent(eventQueue, false);
		}
	}
}

void PurahEngine::GameObject::AwakeEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable)
{
	if (state == ObjectState::CREATE)
	{
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::CREATE)
				{
					// eventQueue에 이게 있는가? 없으면 push 있으면 스킵
					eventQueue.push({ componentList[i], (&Component::Awake) });
				}
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, true);
			}
		}
	}
	else if (state == ObjectState::ENABLE)
	{
		if (isDestroy == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, false);
			}
		}

		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::CREATE)
				{
					// eventQueue에 이게 있는가? 없으면 push 있으면 스킵
					eventQueue.push({ componentList[i], (&Component::Awake) });
				}
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, true);
			}
		}
		else
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, false);
			}
		}
	}
	else if (state == ObjectState::DISABLE)
	{
		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, false);
		}
	}
}

void PurahEngine::GameObject::EnableEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable)
{
	if (state == ObjectState::CREATE)
	{
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::CREATE)
				{
					eventQueue.push({ componentList[i], (&Component::OnEnable) });
				}
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->EnableEvent(eventQueue, true);
			}
		}
	}
	else if (state == ObjectState::ENABLE)
	{
		if (isDestroy == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->EnableEvent(eventQueue, false);
			}
		}
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->EnableEvent(eventQueue, true);
			}
		}
		else
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->EnableEvent(eventQueue, false);
			}
		}
	}
	else if (state == ObjectState::DISABLE)
	{
		if (isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::ENABLE)
				{
					eventQueue.push({ componentList[i], (&Component::OnEnable) });
				}
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->EnableEvent(eventQueue, true);
			}
		}
	}
}

void PurahEngine::GameObject::StartEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable)
{
	if (state == ObjectState::CREATE)
	{
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::CREATE)
				{
					eventQueue.push({ componentList[i], (&Component::Start) });
				}
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StartEvent(eventQueue, true);
			}
		}
	}
	else if (state == ObjectState::ENABLE)
	{
		if (isDestroy == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StartEvent(eventQueue, false);
			}
		}

		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::CREATE)
				{
					eventQueue.push({ componentList[i], (&Component::Start) });
				}
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StartEvent(eventQueue, true);
			}
		}
		else
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StartEvent(eventQueue, false);
			}
		}
	}
	else if (state == ObjectState::DISABLE)
	{
		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->StartEvent(eventQueue, false);
		}
	}
}

void PurahEngine::GameObject::FixedUpdateEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable)
{
	if (state == ObjectState::ENABLE)
	{
		for (PurahEngine::Component* component : componentList)
		{
			if (component->GetState() == Component::ComponentState::ENABLE)
			{
				component->FixedUpdate();
			}
		}
		AddComponentQueue();

		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->FixedUpdateEvent(eventQueue, true);
		}
	}
}

void PurahEngine::GameObject::UpdateEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable)
{
	if (state == ObjectState::ENABLE)
	{
		for (PurahEngine::Component* component : componentList)
		{
			if (component->GetState() == Component::ComponentState::ENABLE)
			{
				component->Update();
			}
		}
		AddComponentQueue();

		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->UpdateEvent(eventQueue, true);
		}
	}
}

void PurahEngine::GameObject::LateUpdateEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable)
{
	if (state == ObjectState::ENABLE)
	{
		for (PurahEngine::Component* component : componentList)
		{
			if (component->GetState() == Component::ComponentState::ENABLE)
			{
				component->LateUpdate();
			}
		}
		AddComponentQueue();

		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->LateUpdateEvent(eventQueue, true);
		}
	}
}

void PurahEngine::GameObject::DeleteChild(GameObject* child)
{
	for (int i = 0; i < trans->children.size(); i++)
	{
		if (trans->children[i]->GetGameObject() == child)
		{
			delete trans->children[i]->GetGameObject();
			trans->children.erase(trans->children.begin() + i);
			break;
		}

		trans->children[i]->GetGameObject()->DeleteChild(child);
	}
}

void PurahEngine::GameObject::DeleteComponent(Component* component)
{
	component->state = Component::ComponentState::DESTROY;
}

void PurahEngine::GameObject::Destroy()
{
	SetEnable(false);
	isDestroy = true;
	state = ObjectState::DESTROY;

	for (int i = 0; i < trans->GetChildren().size(); i++)
	{
		trans->GetChildren()[i]->GetGameObject()->Destroy();
	}
}

void PurahEngine::GameObject::DisableEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable)
{
	if (state == ObjectState::CREATE)
	{
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->DisableEvent(eventQueue, true);
			}
		}
	}
	else if (state == ObjectState::ENABLE)
	{
		if (isDestroy == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::ENABLE)
				{
					eventQueue.push({ componentList[i], (&Component::OnDisable) });
				}
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->DisableEvent(eventQueue, false);
			}
		}
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->DisableEvent(eventQueue, true);
			}
		}
		else
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::ENABLE)
				{
					eventQueue.push({ componentList[i], (&Component::OnDisable) });
				}
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->DisableEvent(eventQueue, false);
			}
		}
	}
	else if (state == ObjectState::DISABLE)
	{
		if (isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->DisableEvent(eventQueue, true);
			}
		}
	}
}

void PurahEngine::GameObject::DestroyEvent(std::queue<GameObject*>& destroyQueue)
{
	if (state == ObjectState::DESTROY)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			if (componentList[i]->GetState() == Component::ComponentState::DESTROY)
			{
				componentList[i]->OnDestroy();
			}
		}
	}

	for (std::vector<Component*>::iterator iter = componentList.begin(); iter != componentList.end();)
	{
		if ((*iter)->GetState() == Component::ComponentState::DESTROY)
		{
			delete* iter;
			iter = componentList.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	for (int i = 0; i < trans->GetChildren().size(); i++)
	{
		trans->GetChildren()[i]->GetGameObject()->DestroyEvent(destroyQueue);
	}

	if (state == ObjectState::DESTROY)
	{
		destroyQueue.push(this);
	}
}


void PurahEngine::GameObject::StateChangeEvent(bool parentEnable)
{
	if (state == ObjectState::CREATE)
	{
		if (parentEnable == true && isEnable == true)
		{
			state = ObjectState::ENABLE;

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StateChangeEvent(true);
			}

			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::CREATE)
				{
					componentList[i]->state = Component::ComponentState::ENABLE;
				}
			}
		}
	}
	else if (state == ObjectState::ENABLE)
	{
		if (isDestroy == true)
		{
			state = ObjectState::DESTROY;

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StateChangeEvent(false);
			}

			for (int i = 0; i < componentList.size(); i++)
			{
				componentList[i]->state = Component::ComponentState::DESTROY;
			}
		}
		else if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StateChangeEvent(true);
			}

			for (int i = 0; i < componentList.size(); i++)
			{
				if (componentList[i]->GetState() == Component::ComponentState::CREATE)
				{
					componentList[i]->state = Component::ComponentState::ENABLE;
				}
			}
		}
		else
		{
			state = ObjectState::DISABLE;

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StateChangeEvent(false);
			}
		}
	}
	else if (state == ObjectState::DISABLE)
	{
		if (isDestroy == true)
		{
			state = ObjectState::DESTROY;

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StateChangeEvent(false);
			}

			for (int i = 0; i < componentList.size(); i++)
			{
				componentList[i]->state = Component::ComponentState::DESTROY;
			}
		}
		else if (isEnable == true)
		{
			state = ObjectState::ENABLE;

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StateChangeEvent(true);
			}
		}
	}
}

PurahEngine::Transform* PurahEngine::GameObject::GetTransform()
{
	return trans;
}

std::vector<PurahEngine::Component*> PurahEngine::GameObject::GetComponentList()
{
	return componentList;
}

void PurahEngine::GameObject::OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnCollisionEnter(collision, collider);
	}
}

void PurahEngine::GameObject::OnCollisionStay(const ZonaiPhysics::ZnCollision& collision, const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnCollisionStay(collision, collider);
	}
}

void PurahEngine::GameObject::OnCollisionExit(const ZonaiPhysics::ZnCollision& collision, const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnCollisionExit(collision, collider);
	}
}

void PurahEngine::GameObject::OnTriggerEnter(const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnTriggerEnter(collider);
	}
}

void PurahEngine::GameObject::OnTriggerStay(const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnTriggerStay(collider);
	}
}

void PurahEngine::GameObject::OnTriggerExit(const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnTriggerExit(collider);
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
	if (isDestroy == true && isEnable == true)
	{
		return;
	}

	isEnable = isTrue;
}

bool PurahEngine::GameObject::GetEnable()
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
		rootEnable = GetStateEnable();
	}

	bool ownEnable = GetStateEnable() && rootEnable;

	return ownEnable;
}

void PurahEngine::GameObject::SetDontDestroy(bool checkDontDestroy)
{
	isDontDestroy = checkDontDestroy;

	for (int i = 0; i < trans->GetChildren().size(); i++)
	{
		trans->GetChildren()[i]->GetGameObject()->SetDontDestroy(checkDontDestroy);
	}
}

bool PurahEngine::GameObject::GetDontDestroy()
{
	return isDontDestroy;
}

std::wstring PurahEngine::GameObject::GetName()
{
	return name;
}

bool PurahEngine::GameObject::GetStateEnable()
{
	return state == ObjectState::ENABLE;
}

void PurahEngine::GameObject::PreSerialize(json& jsonData) const
{

}

void PurahEngine::GameObject::PreDeserialize(const json& jsonData)
{
	auto& fManager = FileManager::GetInstance();
	fManager.SetAddress(jsonData["__Base__instanceID"], this);
	trans->PreDeserialize(jsonData["transform"]);
	bool active = jsonData["active"];
	//bool checkDontDestroy = jsonData["checkDontDestroy"];
	SetEnable(active);
	//SetDontDestroy(checkDontDestroy);
	for (int i = 0; i < jsonData["components"].size(); i++)
	{
		std::string tmp = jsonData["components"][i]["__Base__Component"];
		Component* component = AddComponentToString(jsonData["components"][i]["__Base__Component"]);
		auto tmp2 = jsonData["components"][i];
		component->PreDeserialize(jsonData["components"][i]);
	}

	std::vector<std::wstring> tags;
	for (int i = 0; i < jsonData["tag"].size(); i++)
	{
		std::string tagName = jsonData["tag"][i];
		std::wstring wtagName(tagName.begin(), tagName.end());
		tags.push_back(wtagName);
	}
	tag = tags;
}

void PurahEngine::GameObject::PostSerialize(json& jsonData) const
{

}

void PurahEngine::GameObject::PostDeserialize(const json& jsonData)
{
	int count = 0;
	trans->PostDeserialize(jsonData["transform"]);
	for (int i = 0; i < componentList.size(); i++)
	{
		if (dynamic_cast<Transform*>(componentList[i]) == nullptr)
		{
			componentList[i]->PostDeserialize(jsonData["components"][count]);
			count++;
		}
	}
}

void PurahEngine::GameObject::AddComponentQueue()
{
	while (!addQueue.empty())
	{
		componentList.push_back(addQueue.front());
		addQueue.pop();
	}
}

PurahEngine::GameObject::GameObject(std::wstring objectname)
	: trans(nullptr)
{
	state = ObjectState::CREATE;
	name = objectname;
	isEnable = true;
	isDestroy = false;
	isDontDestroy = false;
}

PurahEngine::GameObject::GameObject(std::wstring objectname, bool isenable)
	: trans(nullptr)
{
	state = ObjectState::CREATE;
	name = objectname;
	isEnable = isenable;
	isDestroy = false;
	isDontDestroy = false;
}

PurahEngine::GameObject::~GameObject()
{
	for (int i = 0; i < componentList.size(); i++)
	{
		delete componentList[i];
	}
	componentList.clear();
}

PurahEngine::GameObject::ObjectState PurahEngine::GameObject::GetState()
{
	return state;
}

void PurahEngine::GameObject::DontDestroyOnLoad()
{
	isDontDestroy = true;
}

void PurahEngine::GameObject::EraseDontDestroy()
{
	SetDontDestroy(false);
}

PurahEngine::Component* PurahEngine::GameObject::AddComponentToString(std::string componentName)
{
	Component* component = ComponentFactory::GetInstance().componentFactory[componentName]();
	componentList.push_back(component);
	component->gameObject = this;
	component->Initialize();

	return component; // 추가된 컴포넌트 포인터를 반환
}
