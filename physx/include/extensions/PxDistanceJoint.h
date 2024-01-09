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

#ifndef PX_DISTANCE_JOINT_H
#define PX_DISTANCE_JOINT_H
/** \addtogroup extensions
  @{
*/

#include "extensions/PxJoint.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxDistanceJoint;

/**
\brief Create a distance Joint.

 \param[in] physics		The physics SDK
 \param[in] actor0		An actor to which the joint is attached. NULL may be used to attach the joint to a specific point in the world frame
 \param[in] localFrame0	The position and orientation of the joint relative to actor0
 \param[in] actor1		An actor to which the joint is attached. NULL may be used to attach the joint to a specific point in the world frame
 \param[in] localFrame1	The position and orientation of the joint relative to actor1 

@see PxDistanceJoint
*/
PxDistanceJoint*	PxDistanceJointCreate(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1);


/** 
\brief flags for configuring the drive of a PxDistanceJoint

@see PxDistanceJoint
*/
struct PxDistanceJointFlag
{
	enum Enum
	{
		eMAX_DISTANCE_ENABLED	= 1<<1,
		eMIN_DISTANCE_ENABLED	= 1<<2,
		eSPRING_ENABLED			= 1<<3
	};
};

typedef PxFlags<PxDistanceJointFlag::Enum, PxU16> PxDistanceJointFlags;
PX_FLAGS_OPERATORS(PxDistanceJointFlag::Enum, PxU16)

/**
\brief 다른 객체의 두 지점 간의 거리에 대한 상한 또는 하한(또는 둘 다)을 유지하는 조인트

@see PxDistanceJointCreate PxJoint
*/
class PxDistanceJoint : public PxJoint
{
public:

	/**
	\brief 조인트의 현재 거리를 반환합니다.
	*/
	virtual PxReal getDistance() const = 0;

	/**
	\brief 조인트의 허용 최소 거리를 설정합니다.

	최소 거리는 최대 거리 이상이어야 합니다.

	<b>기본값</b> 0.0f
	<b>범위</b> [0, PX_MAX_F32)

	\param[in] distance 최소 거리

	@see PxDistanceJoint::minDistance, PxDistanceJointFlag::eMIN_DISTANCE_ENABLED getMinDistance()
	*/
	virtual void setMinDistance(PxReal distance) = 0;

	/**
	\brief 조인트의 허용 최소 거리를 가져옵니다.

	\return 허용되는 최소 거리

	@see PxDistanceJoint::minDistance, PxDistanceJointFlag::eMIN_DISTANCE_ENABLED setMinDistance()
	*/
	virtual PxReal getMinDistance() const = 0;

	/**
	\brief 조인트의 허용 최대 거리를 설정합니다.

	최대 거리는 최소 거리 이하이어야 합니다.

	<b>기본값</b> 0.0f
	<b>범위</b> [0, PX_MAX_F32)

	\param[in] distance 최대 거리

	@see PxDistanceJoint::maxDistance, PxDistanceJointFlag::eMAX_DISTANCE_ENABLED getMinDistance()
	*/
	virtual void setMaxDistance(PxReal distance) = 0;

	/**
	\brief 조인트의 허용 최대 거리를 가져옵니다.

	\return 허용되는 최대 거리

	@see PxDistanceJoint::maxDistance, PxDistanceJointFlag::eMAX_DISTANCE_ENABLED setMaxDistance()
	*/
	virtual PxReal getMaxDistance() const = 0;

	/**
	\brief 조인트의 에러 허용 범위를 설정합니다.

	\param[in] tolerance 조인트가 활성화되는 허용 범위를 벗어난 거리

	@see PxDistanceJoint::tolerance, getTolerance()
	*/
	virtual void setTolerance(PxReal tolerance) = 0;

	/**
	\brief 조인트의 에러 허용 범위를 가져옵니다.

	조인트의 [min, max] 범위를 벗어난 거리로 조인트가 활성화되기 전의 거리입니다.

	<b>기본값</b> 0.25f * PxTolerancesScale::length
	<b>범위</b> (0, PX_MAX_F32)

	이 값은 최소 거리가 0이고 스프링 함수를 사용하는 경우 스프링의 잔상 길이가 0이 아님을 보장하기 위해 사용되어야 합니다.

	@see PxDistanceJoint::tolerance, setTolerance()
	*/
	virtual PxReal getTolerance() const = 0;

	/**
	\brief 조인트 스프링의 강도를 설정합니다.

	스프링은 활성화되고 거리가 [min-error, max+error] 범위를 초과하는 경우 사용됩니다.

	<b>기본값</b> 0.0f
	<b>범위</b> [0, PX_MAX_F32)

	\param[in] stiffness 조인트의 스프링 강도

	@see PxDistanceJointFlag::eSPRING_ENABLED getStiffness()
	*/
	virtual void setStiffness(PxReal stiffness) = 0;

	/**
	\brief 조인트 스프링의 강도를 가져옵니다.

	\return 조인트의 스프링 강도

	@see PxDistanceJointFlag::eSPRING_ENABLED setStiffness()
	*/
	virtual PxReal getStiffness() const = 0;

	/**
	\brief 조인트 스프링의 감쇠를 설정합니다.

	스프링은 활성화되고 거리가 [min-error, max+error] 범위를 초과하는 경우 사용됩니다.

	<b>기본값</b> 0.0f
	<b>범위</b> [0, PX_MAX_F32)

	\param[in] damping 조인트 스프링의 감쇠 정도

	@see PxDistanceJointFlag::eSPRING_ENABLED setDamping()
	*/
	virtual void setDamping(PxReal damping) = 0;

	/**
	\brief 조인트 스프링의 감쇠를 가져옵니다.

	\return 조인트 스프링의 감쇠 정도

	@see PxDistanceJointFlag::eSPRING_ENABLED setDamping()
	*/
	virtual PxReal getDamping() const = 0;

	/**
	\brief Distance Joint에 특화된 플래그를 설정합니다.

	<b>기본값</b> PxDistanceJointFlag::eMAX_DISTANCE_ENABLED

	\param[in] flags 조인트 플래그

	@see PxDistanceJointFlag setFlag() getFlags()
	*/
	virtual void setDistanceJointFlags(PxDistanceJointFlags flags) = 0;

	/**
	\brief Distance Joint에 특화된 단일 플래그를 true 또는 false로 설정합니다.

	\param[in] flag 설정 또는 지우려는 플래그
	\param[in] value 플래그를 설정할 값

	@see PxDistanceJointFlag, getFlags() setFlags()
	*/
	virtual void setDistanceJointFlag(PxDistanceJointFlag::Enum flag, bool value) = 0;

	/**
	\brief Distance Joint에 특화된 플래그를 가져옵니다.

	\return 조인트 플래그

	@see PxDistanceJoint::flags, PxDistanceJointFlag setFlag() setFlags()
	*/
	virtual PxDistanceJointFlags getDistanceJointFlags() const = 0;

	/**
	\brief PxDistanceJoint의 문자열 이름을 반환합니다. 직렬화에 사용됩니다.
	*/
	virtual const char* getConcreteTypeName() const { return "PxDistanceJoint"; }

protected:

	// 직렬화

	/**
	\brief 생성자
	*/
	PX_INLINE PxDistanceJoint(PxType concreteType, PxBaseFlags baseFlags) : PxJoint(concreteType, baseFlags) {}

	/**
	\brief 역직렬화 생성자
	*/
	PX_INLINE PxDistanceJoint(PxBaseFlags baseFlags) : PxJoint(baseFlags) {}

	/**
	\brief 주어진 타입 이름이 이 인스턴스의 타입과 일치하는지 여부를 반환합니다.
	*/
	virtual bool isKindOf(const char* name) const { PX_IS_KIND_OF(name, "PxDistanceJoint", PxJoint); }

	// ~직렬화
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
