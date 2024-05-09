#pragma once
#include "PurahEngineAPI.h"
#include "SerializableDefine.h"
#include "Transform.h"
#include "ComponentFactory.h"
#include "Tag.h"

#include <queue>
#include <vector>
#include <string>

namespace ZonaiPhysics
{
	class ZnCollision;
	class ZnCollider;
}


namespace PurahEngine
{
	class Component;
	class SceneManager;
	class Transform;
	class Collider;
	class DataManager;

	template <typename T>
	concept componentType = std::is_base_of_v<Component, T>;

	class PURAHENGINE_API GameObject : public Serializable
	{
	public:
		void PostInitializeEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);
		void AwakeEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);
		void EnableEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);
		void StartEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);

		/// Update
		// ���� ���� ������Ʈ
		virtual void FixedUpdateEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);
		// ���� ������Ʈ
		virtual void UpdateEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);

		// ���� ���� ������Ʈ 2�� �߰�����

		// Upate �Ŀ� �ѹ� �� ������Ʈ (�ַ� ī�޶� ���ÿ��� ����Ѵٰ� �Ѵ�)
		virtual void LateUpdateEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);

		void DeleteChild(GameObject* child);
		void DeleteComponent(Component* component);

		void Destroy();

		void DisableEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);
		void DestroyEvent(std::queue<GameObject*>& destroyQueue);


		/// OnCollision
		// �浹ü�� �浹���� �� ȣ��
		virtual void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const Collider*);
		// �浹ü�� �浹�� ������ �� ȣ��
		virtual void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const Collider*);
		// �浹ü�� �浹�� ����� �� ȣ��
		virtual void OnCollisionExit(const ZonaiPhysics::ZnCollision&, const Collider*);

		/// OnTrigger
		// Ʈ���Ű� �浹���� �� ȣ��
		virtual void OnTriggerEnter(const Collider*);
		// Ʈ���Ű� �浹�� ������ �� ȣ��
		virtual void OnTriggerStay(const Collider*);
		// Ʈ���Ű� �浹�� ����� �� ȣ��
		virtual void OnTriggerExit(const Collider*);

		/// OnMouse
		// ���콺�� ������Ʈ ���� �ö��� �� ȣ��
		virtual void OnMouseEnter();
		// ���콺�� ������Ʈ ������ ������ �� ȣ��
		virtual void OnMouseStay();
		// ���콺�� ������Ʈ���� ����� �� ȣ��
		virtual void OnMouseExit();

		void SetEnable(bool isTrue);
		bool GetEnable();
		bool IsRootEnable();

		void SetDontDestroy(bool checkDontDestroy);
		bool GetDontDestroy();

		void StateChangeEvent(bool parentEnable = true);

		Transform* GetTransform();
		std::vector<Component*> GetComponentList();

		std::wstring GetName();
		bool GetStateEnable();

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;


	private:
		// ComponentList�� Component ����
		std::vector<Component*> componentList;
		std::queue<Component*> addQueue;
		void AddComponentQueue();

	public:
		Tag tag;

	private:
		GameObject(std::wstring objectname);
		GameObject(std::wstring objectname, bool isenable);
		virtual ~GameObject();

	private:
		enum class ObjectState
		{
			CREATE,
			ENABLE,
			DISABLE,
			DESTROY
		};

		ObjectState state;
		std::wstring name;
		bool isEnable;
		bool isDestroy;
		bool isDontDestroy;

		Transform* trans;

	public:
		ObjectState GetState();
		void DontDestroyOnLoad();
		void EraseDontDestroy();

	public:
		// ComponentList�� Component �߰�
		template<componentType T>
		T* AddComponentInit()
		{
			T* t = new T;
			componentList.push_back(t);
			dynamic_cast<Component*>(t)->gameObject = this;
			dynamic_cast<Component*>(t)->state = Component::ComponentState::CREATE;

			t->Initialize();

			return t; // �߰��� ������Ʈ �����͸� ��ȯ
		}

		// ComponentList�� Component �߰�
		template<componentType T>
		T* AddComponent()
		{
			T* t = new T;
			addQueue.push(t);
			dynamic_cast<Component*>(t)->gameObject = this;
			dynamic_cast<Component*>(t)->state = Component::ComponentState::CREATE;

			t->Initialize();

			return t; // �߰��� ������Ʈ �����͸� ��ȯ
		}

		template<componentType T>
		T* GetComponent() const
		{
			for (auto component : componentList)
			{
				T* t = dynamic_cast<T*>(component);
				if (t != nullptr)
				{
					return t; // Ÿ���� ��ġ�ϴ� ������Ʈ�� ã���� �� ��ȯ
				}
			}

			return nullptr; // �ش� Ÿ���� ������Ʈ�� ã�� ������ �� nullptr ��ȯ
		}

		template<componentType T>
		std::vector<T*> GetComponents() const
		{
			std::vector<T*> components;
			for (auto component : componentList)
			{
				T* t = dynamic_cast<T*>(component);
				if (t != nullptr)
				{
					components.push_back(t);
				}
			}

			return components; // �ش� Ÿ���� ������Ʈ�� ã�� ������ �� nullptr ��ȯ
		}

	private:
		// DeSerialize�� Component �߰��ϱ� ���� �Լ�.
		Component* AddComponentToString(std::string componentName);

		friend SceneManager;
		friend Transform;
	};



}