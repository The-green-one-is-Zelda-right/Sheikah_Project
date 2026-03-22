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
	struct ZnCollision;
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
		// 물리 관련 업데이트
		virtual void FixedUpdateEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);
		// 기존 업데이트
		virtual void UpdateEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);

		// 물리 관련 업데이트 2종 추가예정

		// Upate 후에 한번 더 업데이트 (주로 카메라 관련에서 사용한다고 한다)
		virtual void LateUpdateEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);

		void DeleteChild(GameObject* child);
		void DeleteComponent(Component* component);

		void Destroy();

		void DisableEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable = true);
		void DestroyEvent(std::queue<GameObject*>& destroyQueue);


		/// OnCollision
		// 충돌체가 충돌했을 때 호출
		virtual void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const Collider*);
		// 충돌체가 충돌을 유지할 때 호출
		virtual void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const Collider*);
		// 충돌체가 충돌을 벗어났을 때 호출
		virtual void OnCollisionExit(const ZonaiPhysics::ZnCollision&, const Collider*);

		/// OnTrigger
		// 트리거가 충돌했을 때 호출
		virtual void OnTriggerEnter(const Collider*);
		// 트리거가 충돌율 유지할 때 호출
		virtual void OnTriggerStay(const Collider*);
		// 트리거가 충돌을 벗어났을 때 호출
		virtual void OnTriggerExit(const Collider*);

		/// OnMouse
		// 마우스가 오브젝트 위에 올라갔을 때 호출
		virtual void OnMouseEnter();
		// 마우스가 오브젝트 위에서 유지할 때 호출
		virtual void OnMouseStay();
		// 마우스가 오브젝트에서 벗어났을 때 호출
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
		// ComponentList로 Component 관리
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
		// ComponentList로 Component 추가
		template<componentType T>
		T* AddComponentInit()
		{
			T* t = new T;
			componentList.push_back(t);
			dynamic_cast<Component*>(t)->gameObject = this;
			dynamic_cast<Component*>(t)->state = Component::ComponentState::CREATE;

			t->Initialize();

			return t; // 추가된 컴포넌트 포인터를 반환
		}

		// ComponentList로 Component 추가
		template<componentType T>
		T* AddComponent()
		{
			T* t = new T;
			addQueue.push(t);
			dynamic_cast<Component*>(t)->gameObject = this;
			dynamic_cast<Component*>(t)->state = Component::ComponentState::CREATE;

			t->Initialize();

			return t; // 추가된 컴포넌트 포인터를 반환
		}

		template<componentType T>
		T* GetComponent() const
		{
			for (auto component : componentList)
			{
				T* t = dynamic_cast<T*>(component);
				if (t != nullptr)
				{
					return t; // 타입이 일치하는 컴포넌트를 찾았을 때 반환
				}
			}

			return nullptr; // 해당 타입의 컴포넌트를 찾지 못했을 때 nullptr 반환
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

			return components; // 해당 타입의 컴포넌트를 찾지 못했을 때 nullptr 반환
		}

	private:
		// DeSerialize한 Component 추가하기 위한 함수.
		Component* AddComponentToString(std::string componentName);

		friend SceneManager;
		friend Transform;
	};



}
