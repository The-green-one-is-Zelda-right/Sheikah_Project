#pragma once
#include "PurahEngineAPI.h"
#include "SerializableDefine.h"
#include "ZnCollider.h"
#include "ZnCollision.h"

namespace ZonaiPhysics
{
	class ZnCollision;
	class ZnCollider;
}

namespace PurahEngine
{
	class GameObject;
	class Collider;

	class PURAHENGINE_API Component : public Serializable
	{
	protected:
		Component();

	public:
		virtual ~Component();

	private:
		enum class ComponentState
		{
			CREATE,
			ENABLE,
			DISABLE,
			DESTROY
		};

	public:
		virtual void Awake();
		virtual void Start();
		virtual void OnDataLoadComplete();

		virtual void OnEnable();
		virtual void OnDisable();

		virtual void OnDestroy();

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

		// ���ӿ�����Ʈ ���� �Ŀ� �ʱ�ȭ�� �ϱ����� �Լ�.
		// �� ������ ����̴� ��� ��������.
		virtual void PreInitialize();
		virtual void Initialize();
		virtual void PostInitialize();

		GameObject* GetGameObject() const;
		ComponentState GetState();


	protected:
		GameObject* gameObject;
		ComponentState state;

		friend GameObject;
	};
}