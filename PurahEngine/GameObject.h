#pragma once
#include "PurahEngineAPI.h"
#include "Transform.h"

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
		// ���� ���� ������Ʈ
		virtual void FixedUpdate();
		// ���� ������Ʈ
		virtual void Update();

		// ���� ���� ������Ʈ 2�� �߰�����

		// Upate �Ŀ� �ѹ� �� ������Ʈ (�ַ� ī�޶� ���ÿ��� ����Ѵٰ� �Ѵ�)
		virtual void LateUpdate();

		void Enable();
		void Disable();


		/// OnCollision
		// �浹ü�� �浹���� �� ȣ��
		virtual void OnCollisionEnter();
		// �浹ü�� �浹�� ������ �� ȣ��
		virtual void OnCollisionStay();
		// �浹ü�� �浹�� ����� �� ȣ��
		virtual void OnCollisionExit();

		/// OnTrigger
		// Ʈ���Ű� �浹���� �� ȣ��
		virtual void OnTriggerEnter();
		// Ʈ���Ű� �浹�� ������ �� ȣ��
		virtual void OnTriggerStay();
		// Ʈ���Ű� �浹�� ����� �� ȣ��
		virtual void OnTriggerExit();

		/// OnMouse
		// ���콺�� ������Ʈ ���� �ö��� �� ȣ��
		virtual void OnMouseEnter();
		// ���콺�� ������Ʈ ������ ������ �� ȣ��
		virtual void OnMouseStay();
		// ���콺�� ������Ʈ���� ����� �� ȣ��
		virtual void OnMouseExit();

		void SetActive(bool isTrue);

		std::wstring GetName();

	private:
		// ComponentList�� Component ����
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

		Transform* trans = this->GetComponent<Transform>();

	public:
		bool IsEnable();
		bool IsRootEnable();
		bool isEnable;

	public:
		// ComponentList�� Component �߰�
		template<typename T>
		T* AddComponent() 
		{
			T* t = new T;
			componentList.push_back(t);
			dynamic_cast<Component*>(t)->gameObject = this;
			return t; // �߰��� ������Ʈ �����͸� ��ȯ
		}

		template<typename T>
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