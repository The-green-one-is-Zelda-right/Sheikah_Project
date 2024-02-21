#pragma once
#include "PurahEngineAPI.h"

namespace ZonaiPhysics
{
	class ZnCollision;
}

namespace PurahEngine
{
	class GameObject;
	class Collider;

	class PURAHENGINE_API Component
	{
	protected:
		Component();

	public:
		virtual ~Component();

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

		/// OnCollision
			// �浹ü�� �浹���� �� ȣ��
		virtual void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*);
		// �浹ü�� �浹�� ������ �� ȣ��
		virtual void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*);
		// �浹ü�� �浹�� ����� �� ȣ��
		virtual void OnCollisionExit(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*);

		/// OnTrigger
		// Ʈ���Ű� �浹���� �� ȣ��
		virtual void OnTriggerEnter(const PurahEngine::Collider*);
		// Ʈ���Ű� �浹�� ������ �� ȣ��
		virtual void OnTriggerStay(const PurahEngine::Collider*);
		// Ʈ���Ű� �浹�� ����� �� ȣ��
		virtual void OnTriggerExit(const PurahEngine::Collider*);

		/// OnMouse
		// ���콺�� ������Ʈ ���� �ö��� �� ȣ��
		virtual void OnMouseEnter();
		// ���콺�� ������Ʈ ������ ������ �� ȣ��
		virtual void OnMouseStay();
		// ���콺�� ������Ʈ���� ����� �� ȣ��
		virtual void OnMouseExit();

		// ���ӿ�����Ʈ ���� �Ŀ� �ʱ�ȭ�� �ϱ����� �Լ�.
		// �� ������ ����̴� ��� ��������.
		virtual void Initialize();

		GameObject* GetGameObject() const;

	protected:
		GameObject* gameObject;

		friend GameObject;
	};
}