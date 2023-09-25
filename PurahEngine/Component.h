#pragma once

namespace PurahEngine
{
	class Component
	{
	public:
		virtual void Awake();
		virtual void Start();

		/// Update
		// ���� ������Ʈ
		virtual void Update();
		// ���� ���� ������Ʈ
		virtual void FixedUpdate();
		// Upate �Ŀ� �ѹ� �� ������Ʈ (�ַ� ī�޶� ���ÿ��� ����Ѵٰ� �Ѵ�)
		virtual void LateUpdate();

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
	};
}

