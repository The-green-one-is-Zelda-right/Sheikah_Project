//#pragma once
//
//#include "PurahEngine.h"
//
//#include <functional>
//
//namespace Phyzzle
//{
//	class TriggerCallBack final : public PurahEngine::Component
//	{
//	public:
//		enum class Type
//		{
//			OnTriggerEnter,
//			OnTriggerStay,
//			OnTriggerExit,
//			OnCollisionEnter,
//			OnCollisionStay,
//			OnCollisionExit
//		};
//
//		void RegisterCallBackFunction(std::function<bool(const PurahEngine::Collider* other)> function, Type type);
//
//		// �浹ü�� �浹���� �� ȣ��
//		void OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other) override;
//		// �浹ü�� �浹�� ������ �� ȣ��
//		void OnCollisionStay(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other) override;
//		// �浹ü�� �浹�� ����� �� ȣ��
//		void OnCollisionExit(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other) override;
//
//		// Ʈ���Ű� �浹���� �� ȣ��
//		void OnTriggerEnter(const PurahEngine::Collider* other) override;
//		// Ʈ���Ű� �浹�� ������ �� ȣ��
//		void OnTriggerStay(const PurahEngine::Collider* other) override;
//		// Ʈ���Ű� �浹�� ����� �� ȣ��
//		void OnTriggerExit(const PurahEngine::Collider* other) override;
//		
//	private:
//		std::vector<std::function<bool(const PurahEngine::Collider* other)>> triggerEnterCallBackFunc;
//		std::vector<std::function<bool(const PurahEngine::Collider* other)>> triggerStayCallBackFunc;
//		std::vector<std::function<bool(const PurahEngine::Collider* other)>> triggerExitCallBackFunc;
//
//		std::vector<std::function<bool(const PurahEngine::Collider* other)>> collisionEnterCallBackFunc;
//		std::vector<std::function<bool(const PurahEngine::Collider* other)>> collisionStayCallBackFunc;
//		std::vector<std::function<bool(const PurahEngine::Collider* other)>> collisionExitCallBackFunc;
//
//	private:
//		void PreSerialize(json& jsonData) const override;
//		void PreDeserialize(const json& jsonData) override;
//		void PostSerialize(json& jsonData) const override;
//		void PostDeserialize(const json& jsonData) override;
//	};
//}