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
		eLIMIT_ENABLED	= 1<<0,	//!< enable the limit
		eDRIVE_ENABLED	= 1<<1,	//!< enable the drive
		eDRIVE_FREESPIN	= 1<<2	//!< if the existing velocity is beyond the drive velocity, do not add force
	};
};

typedef PxFlags<PxRevoluteJointFlag::Enum, PxU16> PxRevoluteJointFlags;
PX_FLAGS_OPERATORS(PxRevoluteJointFlag::Enum, PxU16)

/**
\brief ���� �Ǵ� ��� ������ ������� �����ϴ� ����.

���� ������ �� ��ü���� �ϳ��� ȸ�� �������� �����մϴ�.
�� ��ü�� ȸ���� �� �ִ� ���� ���� ���� ���ͷ� �����˴ϴ�.

������ ��ġ�� �� ��ü�� ���� �������� �������� �����˴ϴ�.
������ ���� ��ü�� ���� �������� x-���� �������� �����˴ϴ�.

\image html revoluteJoint.png

���� ������ ȸ������ �����Ͽ� ����� ���͸� ȸ����ų �� �ֽ��ϴ�.
���� ������ �ο��Ͽ� ���� ����� Ư�� ���� ���� ������ �� �ֽ��ϴ�.
���� �Ÿ� �Ǵ� ������ �־��� �Ӱ谪�� �ʰ��ϴ� ��� ��ü�� ������ �� �ֽ��ϴ�.

����, ����̺� �� ������ ������ �ش��ϴ� �÷��׸� �����Ͽ� Ȱ��ȭ�˴ϴ�.

@see PxRevoluteJointCreate() PxJoint
*/
class PxRevoluteJoint : public PxJoint
{
public:

	/**
	\brief ������ ������ �����ɴϴ�. ������ (-2 * Pi, 2 * Pi]�Դϴ�.
	*/
	virtual PxReal getAngle()	const	= 0;

	/**
	\brief ������ �ӵ��� �����ɴϴ�.
	*/
	virtual PxReal getVelocity()	const	= 0;

	/**
	\brief ���� ���� �Ű������� �����մϴ�.

	�Ѱ�� PxRevoluteJointFlag::eLIMIT_ENABLED �÷��׸� ����Ͽ� Ȱ��ȭ�˴ϴ�.

	�Ѱ� ���� ������ (-2 * Pi, 2 * Pi)�Դϴ�.

	\param[in] limits ���� ���� �Ű�����.

	@see PxJointAngularLimitPair getLimit()
	*/
	virtual void			setLimit(const PxJointAngularLimitPair& limits)	= 0;

	/**
	\brief ���� ���� �Ű������� �����ɴϴ�.

	\return ���� ���� �Ű�����

	@see PxJointAngularLimitPair setLimit()
	*/
	virtual PxJointAngularLimitPair getLimit()	const	= 0;

	/**
	\brief ����̺� ���� ��� �ӵ��� �����մϴ�.

	����̺�� maxForce�� ����� ū ��쿡�� �� �ӵ��� ������ �� �ֽ��ϴ�.
	������ �� �ӵ����� �� ���� ȸ�� ���̸� ����̺�� ������ �극��ũ�� �õ��մϴ�
	(PxRevoluteJointFlag::eDRIVE_FREESPIN ����).

	�� ������ ��ȣ�� ȸ�� ������ �����ϸ�, ��� ���� ��� ���� ���� �������� ����.
	�ſ� ū ��� �ӵ��� �����ϸ� ������ �ʴ� ����� �߻��� �� �ֽ��ϴ�.

	\param[in]	velocity
				����̺� ��� �ӵ�
	\param[in]	autowake
				���� ���̱׵带 ����� ��� true

	<b>����:</b> (-PX_MAX_F32, PX_MAX_F32)<br>
	<b>�⺻��:</b> 0.0

	@see PxRevoluteFlags::eDRIVE_FREESPIN
	*/
	virtual void			setDriveVelocity(PxReal velocity, bool autowake = true)	= 0;

	/**
	\brief ����̺� ���� ��� �ӵ��� �����ɴϴ�.

	\return ����̺� ��� �ӵ�

	@see setDriveVelocity()
	*/
	virtual PxReal			getDriveVelocity()	const	= 0;

	/**
	\brief ����̺갡 ������ �� �ִ� �ִ� ��ũ�� �����մϴ�.

	���⿡ ������ ���� ��� ���� �Ǵ� �� �������� ���� �� ������,
	PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES �÷��׿� ���� �ٸ��ϴ�.

	<b>����:</b> [0, PX_MAX_F32)<br>
	<b>�⺻��:</b> PX_MAX_F32

	@see setDriveVelocity()
	*/
	virtual void			setDriveForceLimit(PxReal limit)	= 0;

	/**
	\brief ����̺갡 ������ �� �ִ� �ִ� ��ũ�� �����ɴϴ�.

	\return ��ũ ����

	@see setDriveVelocity()
	*/
	virtual PxReal			getDriveForceLimit()	const	= 0;

	/**
	\brief ����̺��� ��� ������ �����մϴ�.

	����̺� ������ ������ �� ù ��° ������ �ӵ��� �� ������ �������� �����Ǹ�,
	����̺� ��ũ�� ���� ������ �ٿ���ϵ˴ϴ�.
	���� ����̺� ��� �ӵ��� 0�̸� �� ��° ���ʹ� ��� ������ �����ϵ� ù ��°�� �ӵ��� ȸ���˴ϴ�.

	<b>����:</b> [0, PX_MAX_F32)<br>
	<b>�⺻��:</b> 1.0

	\param[in]	ratio
				����̺� ��� ����

	@see getDriveGearRatio()
	*/
	virtual void			setDriveGearRatio(PxReal ratio)	= 0;

	/**
	\brief ��� ������ �����ɴϴ�.

	\return ����̺� ��� ����

	@see setDriveGearRatio()
	*/
	virtual PxReal			getDriveGearRatio()		const	= 0;

	/**
	\brief ���� ������ Ưȭ�� �÷��׸� �����մϴ�.

	<b>�⺻��</b> PxRevoluteJointFlags(0)

	\param[in]	flags
				���� �÷���.

	@see PxRevoluteJointFlag setFlag() getFlags()
	*/
	virtual void			setRevoluteJointFlags(PxRevoluteJointFlags flags) = 0;

	/**
	\brief ���� ������ Ưȭ�� ���� �÷��׸� �����մϴ�.

	\param[in]	flag
				���� �Ǵ� ���� �÷���.
	
	\param[in]	value
				�÷��׸� ������ ��

	@see PxRevoluteJointFlag, getFlags() setFlags()
	*/
	virtual void			setRevoluteJointFlag(PxRevoluteJointFlag::Enum flag, bool value) = 0;

	/**
	\brief ���� ������ Ưȭ�� �÷��׸� �����ɴϴ�.

	\return ���� �÷���

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
