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

#ifndef PX_JOINT_H
#define PX_JOINT_H
/** \addtogroup extensions
  @{
*/

#include "foundation/PxTransform.h"
#include "PxRigidActor.h"
#include "PxConstraint.h"
#include "common/PxBase.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxRigidActor;
class PxScene;
class PxPhysics;
class PxConstraint;

/**
\brief an enumeration of PhysX' built-in joint types

@see PxJoint
*/
struct PxJointConcreteType
{
	enum Enum
	{
		eSPHERICAL = PxConcreteType::eFIRST_PHYSX_EXTENSION,
		eREVOLUTE,
		ePRISMATIC,
		eFIXED,
		eDISTANCE,
		eD6,
		eCONTACT,
		eGEAR,
		eRACK_AND_PINION,
		eLast
	};
};

PX_DEFINE_TYPEINFO(PxJoint,					PxConcreteType::eUNDEFINED)
PX_DEFINE_TYPEINFO(PxRackAndPinionJoint,	PxJointConcreteType::eRACK_AND_PINION)
PX_DEFINE_TYPEINFO(PxGearJoint,				PxJointConcreteType::eGEAR)
PX_DEFINE_TYPEINFO(PxD6Joint,				PxJointConcreteType::eD6)
PX_DEFINE_TYPEINFO(PxDistanceJoint,			PxJointConcreteType::eDISTANCE)
PX_DEFINE_TYPEINFO(PxContactJoint,			PxJointConcreteType::eCONTACT)
PX_DEFINE_TYPEINFO(PxFixedJoint,			PxJointConcreteType::eFIXED)
PX_DEFINE_TYPEINFO(PxPrismaticJoint,		PxJointConcreteType::ePRISMATIC)
PX_DEFINE_TYPEINFO(PxRevoluteJoint,			PxJointConcreteType::eREVOLUTE)
PX_DEFINE_TYPEINFO(PxSphericalJoint,		PxJointConcreteType::eSPHERICAL)


/**
\brief an enumeration for specifying one or other of the actors referenced by a joint

@see PxJoint
*/

struct PxJointActorIndex
{
	enum Enum
	{
		eACTOR0,
		eACTOR1,
		COUNT
	};
};

/** 
\brief a base interface providing common functionality for PhysX joints
*/

class PxJoint : public PxBase
{
public:

	/**
	\brief 이 조인트의 액터를 설정합니다.

	액터는 월드 프레임을 나타내기 위해 NULL일 수 있습니다. 액터 중 최대 하나만 NULL일 수 있습니다.

	\param[in] actor0 첫 번째 액터.
	\param[in] actor1 두 번째 액터

	@see getActors()
	*/
	virtual void				setActors(PxRigidActor* actor0, PxRigidActor* actor1)	= 0;

	/**
	\brief 이 조인트의 액터를 가져옵니다.

	\param[out] actor0 첫 번째 액터.
	\param[out] actor1 두 번째 액터

	@see setActors()
	*/
	virtual void				getActors(PxRigidActor*& actor0, PxRigidActor*& actor1)	const	= 0;

	/**
	\brief 액터의 조인트 로컬 포즈를 설정합니다.

	이것은 조인트 프레임을 액터에 상대적으로 배치하는 상대적인 포즈입니다.

	\param[in] actor 첫 번째 액터는 0, 두 번째 액터는 1입니다.
	\param[in] localPose 이 조인트의 액터에 대한 로컬 포즈입니다.

	@see getLocalPose()
	*/
	virtual void				setLocalPose(PxJointActorIndex::Enum actor, const PxTransform& localPose) = 0;

	/**
	\brief 액터의 조인트 로컬 포즈를 얻습니다.

	\param[in] actor 첫 번째 액터는 0, 두 번째 액터는 1입니다.

	조인트의 이 로컬 포즈를 반환합니다.

	@see setLocalPose()
	*/
	virtual PxTransform			getLocalPose(PxJointActorIndex::Enum actor) const = 0;

	/**
	\brief 이 조인트의 상대적인 포즈를 얻습니다.

	이 함수는 actor1의 조인트 프레임의 포즈를 actor0에 상대적으로 반환합니다.

	@see setLocalPose()
	*/
	virtual PxTransform			getRelativeTransform()	const	= 0;

	/**
	\brief 조인트의 상대적인 선형 속도를 얻습니다.

	이 함수는 actor0의 제약 조인트 프레임의 원점에 상대적인 actor1의 제약 조인트 프레임의 선형 속도를 반환합니다.
	이 값은 actor0의 제약 조인트 프레임 내에서 반환됩니다.
	*/
	virtual PxVec3				getRelativeLinearVelocity()	const	= 0;

	/**
	\brief 조인트의 상대적인 각 속도를 얻습니다.

	이 함수는 actor0의 제약 조인트 프레임 내에서 반환된 값을 사용하여 actor1의 각 속도를 actor0에 상대적으로 반환합니다.
	*/
	virtual PxVec3				getRelativeAngularVelocity()	const	= 0;

	/**
	\brief 이 조인트의 파단 힘을 설정합니다.

	조인트에 가해지는 제약 힘이나 토크가 지정된 값보다 크면 조인트가 파단되며,
	그 시점에서 두 액터를 제약하지 않으며 PxConstraintFlag::eBROKEN 플래그가 설정됩니다.
	힘과 토크는 첫 번째 액터의 조인트 프레임에서 측정됩니다.

	\param[in] force 파단되기 전에 조인트가 가할 수 있는 최대 힘
	\param[in] torque 파단되기 전에 조인트가 가할 수 있는 최대 토크
	*/
	virtual void				setBreakForce(PxReal force, PxReal torque)	= 0;

	/**
	\brief 이 관절의 파단 힘을 가져옵니다.

	\param[out] force 파단하기 전에 관절이 적용할 수 있는 최대 힘
	\param[out] torque 파단하기 전에 관절이 적용할 수 있는 최대 토크

	@see setBreakForce()
	*/
	virtual void				getBreakForce(PxReal& force, PxReal& torque)	const	= 0;

	/**
	\brief 이 관절에 대한 제약 조건 플래그를 설정합니다.

	\param[in] flags 제약 조건 플래그

	@see PxConstraintFlag
	*/
	virtual void				setConstraintFlags(PxConstraintFlags flags)	= 0;

	/**
	\brief 이 관절에 대한 제약 조건 플래그를 지정된 값으로 설정합니다.

	\param[in] flag 제약 조건 플래그
	\param[in] value 플래그를 설정할 값

	@see PxConstraintFlag
	*/
	virtual void				setConstraintFlag(PxConstraintFlag::Enum flag, bool value)	= 0;

	/**
	\brief get the constraint flags for this joint. 
	
	\return the constraint flags

	@see PxConstraintFlag
	*/
	virtual PxConstraintFlags	getConstraintFlags()	const	= 0;
	
	/**
	\brief actor0의 역질량 스케일을 설정합니다.

	\param[in] invMassScale 이 관절을 해결하기 위해 actor 0의 역질량에 적용할 스케일

	@see getInvMassScale0
	*/
	virtual void				setInvMassScale0(PxReal invMassScale)	= 0;

	/**
	\brief actor0의 역질량 스케일을 가져옵니다.

	\return actor0의 역질량 스케일

	@see setInvMassScale0
	*/
	virtual PxReal				getInvMassScale0()	const	= 0;

	/**
	\brief actor0의 역관성 스케일을 설정합니다.

	\param[in] invInertiaScale 이 제약을 해결하는 데에 actor0의 역관성에 적용할 스케일

	@see getInvMassScale0
	*/
	virtual void				setInvInertiaScale0(PxReal invInertiaScale)	= 0;

	/**
	\brief actor0의 역관성 스케일을 가져옵니다.

	\return actor0에 대한 역관성 스케일

	@see setInvInertiaScale0
	*/
	virtual PxReal				getInvInertiaScale0()	const	= 0;

	/**
	\brief set the inverse mass scale for actor1.

	\param[in] invMassScale the scale to apply to the inverse mass of actor 1 for resolving this constraint

	@see getInvMassScale1
	*/
	virtual void				setInvMassScale1(PxReal invMassScale)	= 0;

	/**
	\brief get the inverse mass scale for actor1.

	\return inverse mass scale for actor1

	@see setInvMassScale1
	*/
	virtual PxReal				getInvMassScale1()	const	= 0;

	/**
	\brief set the inverse inertia scale for actor1.

	\param[in] invInertiaScale the scale to apply to the inverse inertia of actor1 for resolving this constraint

	@see getInvInertiaScale1
	*/
	virtual void				setInvInertiaScale1(PxReal invInertiaScale)	= 0;

	/**
	\brief get the inverse inertia scale for actor1.

	\return inverse inertia scale for actor1

	@see setInvInertiaScale1
	*/
	virtual PxReal				getInvInertiaScale1()	const	= 0;

	/**
	\brief Retrieves the PxConstraint corresponding to this joint.
	
	This can be used to determine, among other things, the force applied at the joint.

	\return the constraint
	*/
	virtual PxConstraint*		getConstraint()	const	= 0;

	/**
	\brief getName()로 검색할 수 있는 개체의 이름 문자열을 설정합니다.

	이것은 디버깅용이며 SDK에서는 사용되지 않습니다.
	이 문자열은 SDK에 의해 복사되지 않으며 포인터만 저장됩니다.

	\param[in] name 객체의 이름을 설정할 문자열입니다.

	@see getName()
	*/
	virtual void				setName(const char* name)	= 0;

	/**
	\brief setName()으로 설정한 이름 문자열을 검색합니다.

	\return 개체와 관련된 이름 문자열입니다.

	@see setName()
	*/
	virtual const char*			getName()	const	= 0;

	/**
	\brief Deletes the joint.

	\note This call does not wake up the connected rigid bodies.
	*/
	virtual void				release()	= 0;

	/**
	\brief Retrieves the scene which this joint belongs to.

	\return Owner Scene. NULL if not part of a scene.

	@see PxScene
	*/
	virtual PxScene*			getScene()	const	= 0;

	void*						userData;	//!< user can assign this to whatever, usually to create a 1:1 relationship with a user object.

	//serialization

	/**
	\brief Put class meta data in stream, used for serialization
	*/
	static	void				getBinaryMetaData(PxOutputStream& stream);

	//~serialization
					
protected:
	virtual						~PxJoint() {}

	//serialization

	/**
	\brief Constructor
	*/
	PX_INLINE					PxJoint(PxType concreteType, PxBaseFlags baseFlags) : PxBase(concreteType, baseFlags), userData(NULL) {}
	
	/**
	\brief Deserialization constructor
	*/
	PX_INLINE					PxJoint(PxBaseFlags baseFlags)	: PxBase(baseFlags)	{}

	/**
	\brief Returns whether a given type name matches with the type of this instance
	*/
	virtual	bool				isKindOf(const char* name) const { PX_IS_KIND_OF(name, "PxJoint", PxBase); }

	//~serialization
};

class PxSpring
{
public:

	PxReal stiffness; //!< 드라이브의 스프링 강도: 즉, 위치 오차에 비례하는 힘
	PxReal damping;   //!< 드라이브의 댐핑 강도: 즉, 속도 오차에 비례하는 힘

	PxSpring(PxReal stiffness_, PxReal damping_): stiffness(stiffness_), damping(damping_) {}
};


#if !PX_DOXYGEN
} // namespace physx
#endif

/** \brief 조인의 전역 프레임을 설정하는 헬퍼 함수

	이 함수는 이전 SDK 버전에서 다음 함수들을 대체합니다:

	void NxJointDesc::setGlobalAnchor(const NxVec3& wsAnchor);
	void NxJointDesc::setGlobalAxis(const NxVec3& wsAxis);

	이 함수는 월드 공간 입력 매개변수를 사용하여 조인의 로컬 포즈를 설정합니다.

	\param[in] wsAnchor 전역 프레임 기준의 앵커 지점. <b>범위:</b> 위치 벡터
	\param[in] wsAxis 전역 프레임 기준의 축. <b>범위:</b> 방향 벡터
	\param[in,out] joint 전역 프레임을 설정하는 조인.
*/

PX_C_EXPORT void PX_CALL_CONV PxSetJointGlobalFrame(physx::PxJoint& joint, const physx::PxVec3* wsAnchor, const physx::PxVec3* wsAxis);

/** @} */
#endif
