#pragma once
#include "PurahEngineAPI.h"

#include <vector>
#include <string>

namespace PurahEngine
{
	class Component;
	class SceneManager;
	class Transform;

	class PURAHENGINE_API GameObject
	{
	public:

		virtual void Awake();
		virtual void Start();

		/// Update
		// 물리 관련 업데이트
		virtual void FixedUpdate();
		// 기존 업데이트
		virtual void Update();

		// 물리 관련 업데이트 2종 추가예정

		// Upate 후에 한번 더 업데이트 (주로 카메라 관련에서 사용한다고 한다)
		virtual void LateUpdate();

		void Enable();
		void Disable();


		/// OnCollision
		// 충돌체가 충돌했을 때 호출
		virtual void OnCollisionEnter();
		// 충돌체가 충돌을 유지할 때 호출
		virtual void OnCollisionStay();
		// 충돌체가 충돌을 벗어났을 때 호출
		virtual void OnCollisionExit();

		/// OnTrigger
		// 트리거가 충돌했을 때 호출
		virtual void OnTriggerEnter();
		// 트리거가 충돌율 유지할 때 호출
		virtual void OnTriggerStay();
		// 트리거가 충돌을 벗어났을 때 호출
		virtual void OnTriggerExit();

		/// OnMouse
		// 마우스가 오브젝트 위에 올라갔을 때 호출
		virtual void OnMouseEnter();
		// 마우스가 오브젝트 위에서 유지할 때 호출
		virtual void OnMouseStay();
		// 마우스가 오브젝트에서 벗어났을 때 호출
		virtual void OnMouseExit();

		void SetActive(bool isTrue);

		std::wstring GetName();

	private:
		// ComponentList로 Component 관리
		std::vector<Component*> componentList;

	private:
		GameObject(std::wstring objectname);
		GameObject(std::wstring objectname, bool isactive);
		virtual ~GameObject();

	private:
		enum class State
		{
			CREATE,
			ENABLE,
			DISABLE,
			DESTROY
		};

		State state;
		std::wstring name;
		bool isActive;
		bool isRun = false;

	public:
		bool IsEnable();
		bool isEnable;

	public:
		// ComponentList로 Component 추가
		template<typename T>
		T* AddComponent() 
		{
			T* t = new T;
			componentList.push_back(t);
			dynamic_cast<Component*>(t)->gameObject = this;
			return t; // 추가된 컴포넌트 포인터를 반환
		}

		template<typename T>
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
		friend SceneManager;
		friend Transform;
	};



}