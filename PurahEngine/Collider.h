#pragma once
/// ZonaiPhysics의 Collider를 래핑해준다.
#include "PurahEngineAPI.h"
#include "Component.h"
#include "ZnCollider.h"

namespace PurahEngine
{
	class PURAHENGINE_API Collider : public Component
	{
	public:
		Collider();
		~Collider() override;

	protected:
		ZonaiPhysics::ZnCollider* collider;
	};
}
