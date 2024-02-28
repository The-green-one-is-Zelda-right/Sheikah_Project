#pragma once
#include "PurahEngineAPI.h"
#include "SerializableDefine.h"
#include "Transform.h"
#include "ComponentFactory.h"

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

	template <typename T>
	concept componentType = std::is_base_of_v<Component, T>;

	class PURAHENGINE_API GameObject : public Serializable
	{
	public:

		virtual void AwakeEvent();
		virtual void StartEvent();

		/// Update
		// 물리 관련 업데이트
		virtual void FixedUpdateEvent();
		// 기존 업데이트
		virtual void UpdateEvent();

		// 물리 관련 업데이트 2종 추가예정

		// Upate 후에 한번 더 업데이트 (주로 카메라 관련에서 사용한다고 한다)
		virtual void LateUpdateEvent();

		void Enable();
		void Disable();


		/// OnCollision
		// 충돌체가 충돌했을 때 호출
		virtual void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const ZonaiPhysics::ZnCollider*);
		// 충돌체가 충돌을 유지할 때 호출
		virtual void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const ZonaiPhysics::ZnCollider*);
		// 충돌체가 충돌을 벗어났을 때 호출
		virtual void OnCollisionExit(const ZonaiPhysics::ZnCollision&, const ZonaiPhysics::ZnCollider*);

		/// OnTrigger
		// 트리거가 충돌했을 때 호출
		virtual void OnTriggerEnter(const ZonaiPhysics::ZnCollider*);
		// 트리거가 충돌율 유지할 때 호출
		virtual void OnTriggerStay(const ZonaiPhysics::ZnCollider*);
		// 트리거가 충돌을 벗어났을 때 호출
		virtual void OnTriggerExit(const ZonaiPhysics::ZnCollider*);

		/// OnMouse
		// 마우스가 오브젝트 위에 올라갔을 때 호출
		virtual void OnMouseEnter();
		// 마우스가 오브젝트 위에서 유지할 때 호출
		virtual void OnMouseStay();
		// 마우스가 오브젝트에서 벗어났을 때 호출
		virtual void OnMouseExit();

		void SetEnable(bool isTrue);

		bool IsEnable();
		bool IsRootEnable();

		Transform* GetTransform();


		std::wstring GetName();

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;


	private:
		// ComponentList로 Component 관리
		std::vector<Component*> componentList;

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
		bool isRun = false;

		Transform* trans;

	public:
		// ComponentList로 Component 추가
		template<componentType T>
		T* AddComponent()
		{
			T* t = new T;
			componentList.push_back(t);
			dynamic_cast<Component*>(t)->gameObject = this;
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

	private:
		// DeSerialize한 Component 추가하기 위한 함수.
		Component* AddComponentToString(std::string componentName);

		friend SceneManager;
		friend Transform;
	};



}