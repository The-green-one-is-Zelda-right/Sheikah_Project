#pragma once
#include "PurahEngineAPI.h"
#include "Transform.h"
#include "ZnCollider.h"
#include "ZnCollision.h"

#include <vector>
#include <string>


namespace PurahEngine
{
	class Component;
	class SceneManager;
	class Transform;

	template <typename T>
	concept componentType = std::is_base_of_v<Component, T>;

	class PURAHENGINE_API GameObject
	{
	public:

		virtual void AwakeEvent();
		virtual void StartEvent();

		/// Update
		// ���� ���� ������Ʈ
		virtual void FixedUpdateEvent();
		// ���� ������Ʈ
		virtual void UpdateEvent();

		// ���� ���� ������Ʈ 2�� �߰�����

		// Upate �Ŀ� �ѹ� �� ������Ʈ (�ַ� ī�޶� ���ÿ��� ����Ѵٰ� �Ѵ�)
		virtual void LateUpdateEvent();

		void Enable();
		void Disable();


		/// OnCollision
		// �浹ü�� �浹���� �� ȣ��
		virtual void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const GameObject*);
		// �浹ü�� �浹�� ������ �� ȣ��
		virtual void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const GameObject*);
		// �浹ü�� �浹�� ����� �� ȣ��
		virtual void OnCollisionExit(const ZonaiPhysics::ZnCollision&, const GameObject*);

		/// OnTrigger
		// Ʈ���Ű� �浹���� �� ȣ��
		virtual void OnTriggerEnter(const ZonaiPhysics::ZnCollider*);
		// Ʈ���Ű� �浹�� ������ �� ȣ��
		virtual void OnTriggerStay(const ZonaiPhysics::ZnCollider*);
		// Ʈ���Ű� �浹�� ����� �� ȣ��
		virtual void OnTriggerExit(const ZonaiPhysics::ZnCollider*);

		/// OnMouse
		// ���콺�� ������Ʈ ���� �ö��� �� ȣ��
		virtual void OnMouseEnter();
		// ���콺�� ������Ʈ ������ ������ �� ȣ��
		virtual void OnMouseStay();
		// ���콺�� ������Ʈ���� ����� �� ȣ��
		virtual void OnMouseExit();

		void SetEnable(bool isTrue);

		bool IsEnable();
		bool IsRootEnable();

		Transform* GetTransform();


		std::wstring GetName();


	private:
		// ComponentList�� Component ����
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
		// ComponentList�� Component �߰�
		template<componentType T>
		T* AddComponent() 
		{
			T* t = new T;
			componentList.push_back(t);
			dynamic_cast<Component*>(t)->gameObject = this;
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
		friend SceneManager;
		friend Transform;
	};



}