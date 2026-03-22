#pragma once
#include "eCallback.h"
#include "ZnCollision.h"

namespace ZonaiPhysics
{
	struct ZnCollision;
	class ZnCollider;

	struct CollisionEventBuffer
	{
		ZnCollider* col0{ nullptr };
		ZnCollider* col1{ nullptr };
		ZnCollision collisionData;
		eCallback event;

		CollisionEventBuffer() = default;

		CollisionEventBuffer(
			ZnCollider* _col0, ZnCollider* _col1, 
			const ZnCollision& _collision, eCallback _state)
			: col0(_col0), col1(_col1), collisionData(_collision), event(_state)
		{}

		CollisionEventBuffer(const CollisionEventBuffer& other)
			: col0(other.col0), col1(other.col1), 
			collisionData(other.collisionData), event(other.event)
		{}

		CollisionEventBuffer(CollisionEventBuffer&& other) noexcept
			: col0(std::move(other.col0)), col1(std::move(other.col1)), 
			collisionData(std::move(other.collisionData)), event(std::move(other.event))
		{}

		CollisionEventBuffer& operator=(const CollisionEventBuffer& other)
		{
			if (this != &other)
			{
				col0 = other.col0;
				col1 = other.col1;
				collisionData = other.collisionData;
				event = other.event;
			}
			return *this;
		}

		CollisionEventBuffer& operator=(CollisionEventBuffer&& other) noexcept
		{
			if (this != &other)
			{
				col0 = std::move(other.col0);
				col1 = std::move(other.col1);
				collisionData = std::move(other.collisionData);
				event = std::move(other.event);
			}
			return *this;
		}
	};

	struct TriggerEventBuffer
	{
		ZnCollider* col0{ nullptr };
		ZnCollider* col1{ nullptr };
		eCallback event{ eCallback::eError };

		TriggerEventBuffer() = default;

		TriggerEventBuffer(ZnCollider* _col0, ZnCollider* _col1, eCallback _state = eCallback::eError)
			: col0(_col0), col1(_col1), event(_state)
		{}

		TriggerEventBuffer(const TriggerEventBuffer& other)
			: col0(other.col0), col1(other.col1), event(other.event)
		{}

		TriggerEventBuffer(TriggerEventBuffer&& other) noexcept
			: col0(std::move(other.col0)), col1(std::move(other.col1)), event(std::move(other.event))
		{}

		TriggerEventBuffer& operator=(const TriggerEventBuffer& other)
		{
			if (this != &other)
			{
				col0 = other.col0;
				col1 = other.col1;
				event = other.event;
			}
			return *this;
		}

		TriggerEventBuffer& operator=(TriggerEventBuffer&& other) noexcept
		{
			if (this != &other)
			{
				col0 = std::move(other.col0);
				col1 = std::move(other.col1);
				event = std::move(other.event);
			}
			return *this;
		}

		bool operator==(const TriggerEventBuffer& other) const
		{
			return col0 == other.col0 && col1 == other.col1;
		}
	};
}
