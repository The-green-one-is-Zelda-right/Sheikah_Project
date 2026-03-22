//#include "TriggerCallBack.h"
//
//namespace Phyzzle
//{
//	void TriggerCallBack::RegisterCallBackFunction(std::function<bool(const PurahEngine::Collider* other)> function, Type type)
//	{
//		switch (type)
//		{
//			case Phyzzle::TriggerCallBack::Type::OnTriggerEnter:
//			{
//				triggerEnterCallBackFunc.push_back(function);
//				break;
//			}
//			case Phyzzle::TriggerCallBack::Type::OnTriggerStay:
//			{
//				triggerStayCallBackFunc.push_back(function);
//				break;
//			}
//			case Phyzzle::TriggerCallBack::Type::OnTriggerExit:
//			{
//				triggerExitCallBackFunc.push_back(function);
//				break;
//			}
//			case Phyzzle::TriggerCallBack::Type::OnCollisionEnter:
//			{
//				collisionEnterCallBackFunc.push_back(function);
//				break;
//			}
//			case Phyzzle::TriggerCallBack::Type::OnCollisionStay:
//			{
//				collisionStayCallBackFunc.push_back(function);
//				break;
//			}
//			case Phyzzle::TriggerCallBack::Type::OnCollisionExit:
//			{
//				collisionExitCallBackFunc.push_back(function);
//				break;
//			}
//			default:
//			{
//				assert(0);
//				break;
//			}
//		}
//	}
//
//	void TriggerCallBack::OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other)
//	{
//	}
//
//	void TriggerCallBack::OnCollisionStay(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other)
//	{
//	}
//
//	void TriggerCallBack::OnCollisionExit(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other)
//	{
//	}
//
//	void TriggerCallBack::OnTriggerEnter(const PurahEngine::Collider* other)
//	{
//	}
//
//	void TriggerCallBack::OnTriggerStay(const PurahEngine::Collider* other)
//	{
//	}
//
//	void TriggerCallBack::OnTriggerExit(const PurahEngine::Collider* other)
//	{
//	}
//
//	void TriggerCallBack::PreSerialize(json& jsonData) const
//	{
//	}
//
//	void TriggerCallBack::PreDeserialize(const json& jsonData)
//	{
//	}
//
//	void TriggerCallBack::PostSerialize(json& jsonData) const
//	{
//	}
//
//	void TriggerCallBack::PostDeserialize(const json& jsonData)
//	{
//	}
//}