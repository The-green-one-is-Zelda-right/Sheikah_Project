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

	private:
		// ComponentList�� Component ����
		std::vector<Component*> componentList;

	private:
		GameObject(std::wstring objectname);
		virtual ~GameObject();

	private:
		std::wstring name;

	public:
		// ComponentList�� Component �߰�
		template<typename T>
		T* AddComponent()
		{
			T* t = new T;
			componentList.push_back(t);
			t->gameObject = this;
			return t; // �߰��� ������Ʈ �����͸� ��ȯ
		}

		template<typename T>
		T* GetComponent()
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