#pragma once
#include "ZnResourceID.h"
#include "MaterialEnum.h"

namespace ZonaiPhysics
{
	struct MaterialDesc
	{
		MaterialDesc() :
			staticFriction(0.5f), dynamicFriction(0.5f), restitution(0.8f),
			eFriction(eAVERAGE), eRestitution(eAVERAGE) {}

		MaterialDesc(
			float _sF, float _dF, float _r,
			eCombineMode _eF = eAVERAGE, eCombineMode _eR = eAVERAGE) :
			staticFriction(_sF), dynamicFriction(_dF), restitution(_r),
			eFriction(_eF), eRestitution(_eR) {}

		MaterialDesc(const MaterialDesc& _desc) :
			staticFriction(_desc.staticFriction), 
			dynamicFriction(_desc.dynamicFriction),
			restitution(_desc.eRestitution),
			eFriction(_desc.eFriction), 
			eRestitution(_desc.eRestitution) {}

		MaterialDesc(MaterialDesc&& _desc) noexcept :
			staticFriction(_desc.staticFriction),
			dynamicFriction(_desc.dynamicFriction),
			restitution(_desc.eRestitution),
			eFriction(_desc.eFriction),
			eRestitution(_desc.eRestitution) {}

		MaterialDesc& operator= (const MaterialDesc& _desc)
		{
			if (this != &_desc)
			{
				staticFriction = _desc.staticFriction;
				dynamicFriction = _desc.dynamicFriction;
				restitution = _desc.restitution;
				eFriction = _desc.eFriction;
				eRestitution = _desc.eRestitution;
			}

			return *this;
		}

		MaterialDesc& operator= (MaterialDesc&& _desc) noexcept
		{
			if (this != &_desc)
			{
				staticFriction	= std::move(_desc.staticFriction);
				dynamicFriction = std::move(_desc.dynamicFriction);
				restitution		= std::move(_desc.restitution);
				eFriction		= std::move(_desc.eFriction);
				eRestitution	= std::move(_desc.eRestitution);
			}

			return *this;
		}

		float staticFriction = 0.5f;
		float dynamicFriction = 0.5f;
		float restitution = 0.2f;
		eCombineMode eFriction = eAVERAGE;
		eCombineMode eRestitution = eAVERAGE;
	};
}

