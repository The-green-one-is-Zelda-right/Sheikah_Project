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
//		// 충돌체가 충돌했을 때 호출
//		void OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other) override;
//		// 충돌체가 충돌을 유지할 때 호출
//		void OnCollisionStay(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other) override;
//		// 충돌체가 충돌을 벗어났을 때 호출
//		void OnCollisionExit(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other) override;
//
//		// 트리거가 충돌했을 때 호출
//		void OnTriggerEnter(const PurahEngine::Collider* other) override;
//		// 트리거가 충돌율 유지할 때 호출
//		void OnTriggerStay(const PurahEngine::Collider* other) override;
//		// 트리거가 충돌을 벗어났을 때 호출
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