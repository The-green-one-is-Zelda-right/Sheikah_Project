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

		// 게임오브젝트 생성 후에 초기화를 하기위한 함수.
		// 걍 생성자 대용이다 라고 생각하자.
		virtual void Initialize();

		GameObject* GetGameObject() const;

	protected:
		GameObject* gameObject;

		friend GameObject;
	};
}