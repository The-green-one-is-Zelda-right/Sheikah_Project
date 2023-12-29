#pragma once
/// ZonaiPhysics�� Collider�� �������ش�.
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
