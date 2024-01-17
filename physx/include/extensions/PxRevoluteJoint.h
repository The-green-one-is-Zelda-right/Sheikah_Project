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

#ifndef PX_REVOLUTE_JOINT_H
#define PX_REVOLUTE_JOINT_H
/** \addtogroup extensions
  @{
*/

#include "extensions/PxJoint.h"
#include "extensions/PxJointLimit.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxRevoluteJoint;

/**
\brief Create a revolute joint.

 \param[in] physics		The physics SDK
 \param[in] actor0		An actor to which the joint is attached. NULL may be used to attach the joint to a specific point in the world frame
 \param[in] localFrame0	The position and orientation of the joint relative to actor0
 \param[in] actor1		An actor to which the joint is attached. NULL may be used to attach the joint to a specific point in the world frame
 \param[in] localFrame1	The position and orientation of the joint relative to actor1 

@see PxRevoluteJoint
*/
PxRevoluteJoint*	PxRevoluteJointCreate(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1);

/**
\brief Flags specific to the Revolute Joint.

@see PxRevoluteJoint
*/
struct PxRevoluteJointFlag
{
	enum Enum
	{
		eLIMIT_ENABLED = 1 << 0,	//!< 제한 활성화
		eDRIVE_ENABLED = 1 << 1,	//!< 드라이브 활성화
		eDRIVE_FREESPIN = 1 << 2	//!< 기존 속도가 드라이브 속도를 초과하는 경우 힘을 추가하지 않음
	};
};

typedef PxFlags<PxRevoluteJointFlag::Enum, PxU16> PxRevoluteJointFlags;
PX_FLAGS_OPERATORS(PxRevoluteJointFlag::Enum, PxU16)

/**
\brief 힌지 또는 축과 유사한 방식으로 작동하는 조인트입니다.

힌지 조인트는 두 객체로부터 회전의 자유도를 제거합니다.
두 개체가 회전할 수 있는 축은 점과 방향 벡터로 지정됩니다.

힌지의 위치는 각 개체의 조인트 프레임의 원점으로 지정됩니다.
힌지의 축은 개체의 조인트 프레임에서 x-축의 방향으로 지정됩니다.

\image html revoluteJoint.png

회전 가능한 조인트에는 모터를 부착하여 연결된 액터를 회전시키는 힘을 가할 수 있습니다.
또한 힌지의 움직임을 특정 범위 내로 제한하기 위해 제한을 부여할 수 있습니다.
두 개체 간의 거리 또는 각도가 주어진 임계값을 초과하는 경우, 개체를 투영(project)할 수도 있습니다.

투영, 드라이브 및 제한은 조인트에 해당 플래그를 설정하여 활성화됩니다.

@see PxRevoluteJointCreate() PxJoint
*/
class PxRevoluteJoint : public PxJoint
{
public:

	/**
	\brief 조인트의 각도를 (-2*Pi, 2*Pi] 범위에서 반환합니다.
	*/
	virtual PxReal getAngle()	const	= 0;

	/**
	\brief 조인트의 속도를 반환합니다.
	*/
	virtual PxReal getVelocity()	const	= 0;

	/**
	\brief 조인트 제한 매개변수를 설정합니다.

	제한은 PxRevoluteJointFlag::eLIMIT_ENABLED 플래그를 사용하여 활성화됩니다.

	제한 각도 범위는 (-2*Pi, 2*Pi)입니다.

	\param[in] limits 조인트 제한 매개변수.

	@see PxJointAngularLimitPair getLimit()
	*/
	virtual void			setLimit(const PxJointAngularLimitPair& limits)	= 0;

	/**
	\brief 조인트 제한 매개변수를 가져옵니다.

	\return 조인트 제한 매개변수

	@see PxJointAngularLimitPair setLimit()
	*/
	virtual PxJointAngularLimitPair getLimit()	const	= 0;

	/**
	\brief 드라이브 모델의 목표 속도를 설정합니다.

	모터는 이 속도에 도달하기 위해 maxForce가 충분히 큰 경우에만 가능합니다.
	만약 조인트가 이 속도보다 빠르게 회전 중이라면, 모터는 실제로 브레이크를 시도하려고 합니다
	(참조: PxRevoluteJointFlag::eDRIVE_FREESPIN.)

	이 변수의 부호는 회전 방향을 결정하며, 양수 값은 양의 조인트 각도와 동일한 방향으로 갑니다.
	매우 큰 목표 속도를 설정하는 것은 원하지 않는 결과를 초래할 수 있습니다.

	\param[in] velocity 드라이브의 목표 속도
	\param[in] autowake 조인트 리지드가 자세히 있을 경우 깨워야 하는지 여부

	<b>범위:</b> (-PX_MAX_F32, PX_MAX_F32)<br>
	<b>기본값:</b> 0.0

	@see PxRevoluteFlags::eDRIVE_FREESPIN
	*/
	virtual void			setDriveVelocity(PxReal velocity, bool autowake = true)	= 0;

	/**
	\brief 드라이브 모델의 대상 속도를 가져옵니다.

	\return 드라이브 대상 속도

	@see setDriveVelocity()
	*/
	virtual PxReal			getDriveVelocity()	const	= 0;

	/**
	\brief 드라이브가 적용할 수 있는 최대 토크를 설정합니다.

	여기에 설정된 값은 충격 제한 또는 힘 제한으로 사용될 수 있으며,
	PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES 플래그에 따라 다릅니다.

	<b>범위:</b> [0, PX_MAX_F32)<br>
	<b>기본값:</b> PX_MAX_F32

	@see setDriveVelocity()
	*/
	virtual void			setDriveForceLimit(PxReal limit)	= 0;

	/**
	\brief 드라이브가 적용할 수 있는 최대 토크를 가져옵니다.

	\return 토크 제한

	@see setDriveVelocity()
	*/
	virtual PxReal			getDriveForceLimit()	const	= 0;

	/**
	\brief 드라이브의 기어 비율을 설정합니다.

	드라이브 제약을 설정할 때 첫 번째 액터의 속도는 이 값으로 스케일이 조정되며,
	드라이브 토크에 대한 응답이 다운스케일됩니다.
	따라서 드라이브 대상 속도가 0이면 두 번째 액터는 기어 비율로 스케일된 첫 번째의 속도로 회전됩니다.

	<b>범위:</b> [0, PX_MAX_F32)<br>
	<b>기본값:</b> 1.0

	\param[in]	ratio
				드라이브 기어 비율

	@see getDriveGearRatio()
	*/
	virtual void			setDriveGearRatio(PxReal ratio)	= 0;

	/**
	\brief 기어 비율을 가져옵니다.

	\return 드라이브 기어 비율

	@see setDriveGearRatio()
	*/
	virtual PxReal			getDriveGearRatio()		const	= 0;

	/**
	\brief 힌지 관절에 특화된 플래그를 설정합니다.

	<b>기본값</b> PxRevoluteJointFlags(0)

	\param[in]	flags
				관절 플래그.

	@see PxRevoluteJointFlag setFlag() getFlags()
	*/
	virtual void			setRevoluteJointFlags(PxRevoluteJointFlags flags) = 0;

	/**
	\brief 힌지 관절에 특화된 단일 플래그를 설정합니다.

	\param[in]	flag
				설정 또는 지울 플래그.
	
	\param[in]	value
				플래그를 설정할 값

	@see PxRevoluteJointFlag, getFlags() setFlags()
	*/
	virtual void			setRevoluteJointFlag(PxRevoluteJointFlag::Enum flag, bool value) = 0;

	/**
	\brief 힌지 관절에 특화된 플래그를 가져옵니다.

	\return 관절 플래그

	@see PxRevoluteJoint::flags, PxRevoluteJointFlag setFlag() setFlags()
	*/
	virtual PxRevoluteJointFlags	getRevoluteJointFlags()	const	= 0;

	/**
	\brief Returns string name of PxRevoluteJoint, used for serialization
	*/
	virtual	const char*			getConcreteTypeName() const { return "PxRevoluteJoint"; }

protected:

	//serialization

	/**
	\brief Constructor
	*/
	PX_INLINE					PxRevoluteJoint(PxType concreteType, PxBaseFlags baseFlags) : PxJoint(concreteType, baseFlags) {}

	/**
	\brief Deserialization constructor
	*/
	PX_INLINE					PxRevoluteJoint(PxBaseFlags baseFlags) : PxJoint(baseFlags) {}

	/**
	\brief Returns whether a given type name matches with the type of this instance
	*/
	virtual	bool				isKindOf(const char* name) const { PX_IS_KIND_OF(name, "PxRevoluteJoint", PxJoint); }
	
	//~serialization
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
