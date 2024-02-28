// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2023 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#ifndef PX_MATERIAL_H
#define PX_MATERIAL_H
/** \addtogroup physics
@{
*/

#include "PxBaseMaterial.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxScene;

/**
\brief Flags which control the behavior of a material.

@see PxMaterial 
*/
struct PxMaterialFlag
{
	enum Enum
	{
		/**
		\brief If this flag is set, friction computations are always skipped between shapes with this material and any other shape.
		*/
		eDISABLE_FRICTION = 1 << 0,

		/**
		\brief Whether to use strong friction.
		The difference between "normal" and "strong" friction is that the strong friction feature
		remembers the "friction error" between simulation steps. The friction is a force trying to
		hold objects in place (or slow them down) and this is handled in the solver. But since the
		solver is only an approximation, the result of the friction calculation can include a small
		"error" - e.g. a box resting on a slope should not move at all if the static friction is in
		action, but could slowly glide down the slope because of a small friction error in each 
		simulation step. The strong friction counter-acts this by remembering the small error and
		taking it to account during the next simulation step.

		However, in some cases the strong friction could cause problems, and this is why it is
		possible to disable the strong friction feature by setting this flag. One example is
		raycast vehicles that are sliding fast across the surface, but still need a precise
		steering behavior. It may be a good idea to reenable the strong friction when objects
		are coming to a rest, to prevent them from slowly creeping down inclines.

		Note: This flag only has an effect if the PxMaterialFlag::eDISABLE_FRICTION bit is 0.
		*/
		eDISABLE_STRONG_FRICTION = 1 << 1,

		/**
		\brief Whether to correct the friction force applied by the patch friction model to better match analytical models.
		This flag only has an effect if the PxFrictionType::ePATCH friction model is used.

		When using the patch friction model, up to two friction anchors are generated per patch. The normal force of all contacts
		in the patch is accumulated and equally distributed among the anchors in order to compute friction forces. If this flag
		is disabled, the legacy behavior is active which produces double the expected friction force in the case of two anchors,
		since the full accumulated normal force is used in both anchors for the friction computation.
		*/
		eIMPROVED_PATCH_FRICTION = 1 << 2,

		/**
		\brief This flag has the effect of enabling an implicit spring model for the normal force computation.
		@see PxMaterial.setRestitution, PxMaterial.setDamping
		*/
		eCOMPLIANT_CONTACT = 1 << 3
	};
};

/**
\brief collection of set bits defined in PxMaterialFlag.

@see PxMaterialFlag
*/
typedef PxFlags<PxMaterialFlag::Enum,PxU16> PxMaterialFlags;
PX_FLAGS_OPERATORS(PxMaterialFlag::Enum,PxU16)


/**
\brief 두 재질 속성이 충돌의 마찰 또는 반발력 계수를 생성하기 위해 결합되는 방식을 결정하는 열거형입니다.

두 개의 액터가 서로 접촉할 때, 각각은 다양한 계수를 가진 재질을 가지지만, 
우리는 그 두 액터에 대한 하나의 계수 집합만 필요합니다.

물리학은 계수가 각각의 사례별로 경험적으로 결정되기 때문에 별도의 조합을 가지지 않습니다. 
그러나 이를 쌍별 조회 테이블로 시뮬레이션하는 것은 종종 비현실적입니다.

이러한 이유로 다음과 같은 결합 동작이 제공됩니다:

eAVERAGE
eMIN
eMULTIPLY
eMAX

쌍의 효과적인 결합 모드는 maximum(material0.combineMode, material1.combineMode)입니다.

@see PxMaterial.setFrictionCombineMode() PxMaterial.getFrictionCombineMode() PxMaterial.setRestitutionCombineMode() PxMaterial.getFrictionCombineMode()
*/
struct PxCombineMode
{
	enum Enum
	{
		eAVERAGE	= 0,		//!< Average: (a + b)/2
		eMIN		= 1,		//!< Minimum: minimum(a,b)
		eMULTIPLY	= 2,		//!< Multiply: a*b
		eMAX		= 3,		//!< Maximum: maximum(a,b)
		eN_VALUES	= 4,		//!< This is not a valid combine mode, it is a sentinel to denote the number of possible values. We assert that the variable's value is smaller than this.
		ePAD_32		= 0x7fffffff //!< This is not a valid combine mode, it is to assure that the size of the enum type is big enough.
	};
};

/**
\brief Material class to represent a set of surface properties. 

@see PxPhysics.createMaterial
*/
class PxMaterial : public PxBaseMaterial
{
public:

	/**
	\brief Sets the coefficient of dynamic friction.
	
	The coefficient of dynamic friction should be in [0, PX_MAX_F32). If set to greater than staticFriction, the effective value of staticFriction will be increased to match.

	<b>Sleeping:</b> Does <b>NOT</b> wake any actors which may be affected.

	\param[in] coef Coefficient of dynamic friction. <b>Range:</b> [0, PX_MAX_F32)

	@see getDynamicFriction()
	*/
	virtual		void	setDynamicFriction(PxReal coef) = 0;

	/**
	\brief Retrieves the DynamicFriction value.

	\return The coefficient of dynamic friction.

	@see setDynamicFriction
	*/
	virtual		PxReal	getDynamicFriction() const = 0;

	/**
	\brief Sets the coefficient of static friction
	
	The coefficient of static friction should be in the range [0, PX_MAX_F32)

	<b>Sleeping:</b> Does <b>NOT</b> wake any actors which may be affected.

	\param[in] coef Coefficient of static friction. <b>Range:</b> [0, PX_MAX_F32)

	@see getStaticFriction() 
	*/
	virtual		void	setStaticFriction(PxReal coef) = 0;

	/**
	\brief Retrieves the coefficient of static friction.
	\return The coefficient of static friction.

	@see setStaticFriction 
	*/
	virtual		PxReal	getStaticFriction() const = 0;

	/**
	\brief Sets the coefficient of restitution 
	
	A coefficient of 0 makes the object bounce as little as possible, higher values up to 1.0 result in more bounce.

	This property is overloaded when PxMaterialFlag::eCOMPLIANT_CONTACT flag is enabled. This permits negative values for restitution to be provided.
	The negative values are converted into spring stiffness terms for an implicit spring simulated at the contact site, with the spring positional error defined by
	the contact separation value. Higher stiffness terms produce stiffer springs that behave more like a rigid contact.

	<b>Sleeping:</b> Does <b>NOT</b> wake any actors which may be affected.

	\param[in] rest Coefficient of restitution. <b>Range:</b> [-INF,1]

	@see getRestitution() 
	*/
	virtual		void	setRestitution(PxReal rest) = 0;

	/**
	\brief Retrieves the coefficient of restitution.     

	See #setRestitution.

	\return The coefficient of restitution.

	@see setRestitution() 
	*/
	virtual		PxReal	getRestitution() const = 0;

	/**
	\brief 댐핑 계수를 설정합니다.

	이 속성은 오직 PxMaterialFlag::eCOMPLIANT_CONTACT가 활성화되어 있을 때 시뮬레이션에 영향을 미칩니다.
	댐핑은 음의 복구 값을 통해 설정된 스프링 강도와 함께 작동합니다. 스프링 강도는 위치 오류를 보정하고,
	댐핑은 상대 속도에 저항합니다. 높은 댐핑 계수를 설정하면 스펀지 상태의 접촉을 생성할 수 있습니다.

	<b>슬립:</b> 영향을 받을 수 있는 어떤 액터도 깨우지 <b>않습니다.</b>

	\param[in] damping 댐핑 계수. <b>범위:</b> [0, 무한대]

	@see getDamping()
	*/
	virtual		void	setDamping(PxReal damping) = 0;

	/**
	\brief 댐핑 계수를 검색합니다.

	#setDamping을 참조하십시오.

	\return 댐핑 계수.

	@see setDamping()
	*/
	virtual		PxReal	getDamping() const = 0;

	/**
	\brief 특정한 머티리얼 플래그를 설정하거나 해제합니다.

	플래그 목록은 #PxMaterialFlag를 참조하십시오.

	<b>기본값:</b> eIMPROVED_PATCH_FRICTION

	<b>슬립:</b> 영향을 받을 수 있는 어떤 액터도 깨우지 <b>않습니다.</b>

	\param[in]	flag 설정(올리기) 또는 해제할 PxMaterial 플래그입니다.
	\param[in]	b 플래그의 새로운 상태

	@see getFlags() setFlags() PxMaterialFlag
	*/
	virtual		void	setFlag(PxMaterialFlag::Enum flag, bool b) = 0;

	/**
	\brief 모든 머티리얼 플래그를 설정합니다.

	플래그 목록은 #PxMaterialFlag를 참조하십시오.

	<b>기본값:</b> eIMPROVED_PATCH_FRICTION

	<b>슬립:</b> 영향을 받을 수 있는 어떤 액터도 깨우지 <b>않습니다.</b>

	\param[in]	flags 모든 PxMaterial 플래그

	@see getFlags() setFlag() PxMaterialFlag
	*/
	virtual		void 	setFlags(PxMaterialFlags flags) = 0;

	/**
	\brief Retrieves the flags. See #PxMaterialFlag.

	\return The material flags.

	@see PxMaterialFlag setFlags()
	*/
	virtual		PxMaterialFlags	getFlags() const = 0;

	/**
	\brief Sets the friction combine mode.
	
	See the enum ::PxCombineMode .

	<b>Default:</b> PxCombineMode::eAVERAGE

	<b>Sleeping:</b> Does <b>NOT</b> wake any actors which may be affected.

	\param[in] combMode Friction combine mode to set for this material. See #PxCombineMode.

	@see PxCombineMode getFrictionCombineMode setStaticFriction() setDynamicFriction()
	*/
	virtual		void	setFrictionCombineMode(PxCombineMode::Enum combMode) = 0;

	/**
	\brief Retrieves the friction combine mode.
	
	See #setFrictionCombineMode.

	\return The friction combine mode for this material.

	@see PxCombineMode setFrictionCombineMode() 
	*/
	virtual		PxCombineMode::Enum	getFrictionCombineMode() const = 0;

	/**
	\brief Sets the restitution combine mode.
	
	See the enum ::PxCombineMode .

	<b>Default:</b> PxCombineMode::eAVERAGE

	<b>Sleeping:</b> Does <b>NOT</b> wake any actors which may be affected.

	\param[in] combMode Restitution combine mode for this material. See #PxCombineMode.

	@see PxCombineMode getRestitutionCombineMode() setRestitution()
	*/
	virtual		void	setRestitutionCombineMode(PxCombineMode::Enum combMode) = 0;

	/**
	\brief Retrieves the restitution combine mode.
	
	See #setRestitutionCombineMode.

	\return The coefficient of restitution combine mode for this material.

	@see PxCombineMode setRestitutionCombineMode getRestitution()
	*/
	virtual		PxCombineMode::Enum	getRestitutionCombineMode() const = 0;

	// PxBase
	virtual		const char*	getConcreteTypeName() const	PX_OVERRIDE	{ return "PxMaterial"; }
	//~PxBase

protected:
	PX_INLINE			PxMaterial(PxType concreteType, PxBaseFlags baseFlags) : PxBaseMaterial(concreteType, baseFlags)	{}
	PX_INLINE			PxMaterial(PxBaseFlags baseFlags) : PxBaseMaterial(baseFlags) {}
	virtual				~PxMaterial() {}
	virtual		bool	isKindOf(const char* name) const { PX_IS_KIND_OF(name, "PxMaterial", PxBaseMaterial); }
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
