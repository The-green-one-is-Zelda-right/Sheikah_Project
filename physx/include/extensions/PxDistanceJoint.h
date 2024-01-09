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
\brief �ٸ� ��ü�� �� ���� ���� �Ÿ��� ���� ���� �Ǵ� ����(�Ǵ� �� ��)�� �����ϴ� ����Ʈ

@see PxDistanceJointCreate PxJoint
*/
class PxDistanceJoint : public PxJoint
{
public:

	/**
	\brief ����Ʈ�� ���� �Ÿ��� ��ȯ�մϴ�.
	*/
	virtual PxReal getDistance() const = 0;

	/**
	\brief ����Ʈ�� ��� �ּ� �Ÿ��� �����մϴ�.

	�ּ� �Ÿ��� �ִ� �Ÿ� �̻��̾�� �մϴ�.

	<b>�⺻��</b> 0.0f
	<b>����</b> [0, PX_MAX_F32)

	\param[in] distance �ּ� �Ÿ�

	@see PxDistanceJoint::minDistance, PxDistanceJointFlag::eMIN_DISTANCE_ENABLED getMinDistance()
	*/
	virtual void setMinDistance(PxReal distance) = 0;

	/**
	\brief ����Ʈ�� ��� �ּ� �Ÿ��� �����ɴϴ�.

	\return ���Ǵ� �ּ� �Ÿ�

	@see PxDistanceJoint::minDistance, PxDistanceJointFlag::eMIN_DISTANCE_ENABLED setMinDistance()
	*/
	virtual PxReal getMinDistance() const = 0;

	/**
	\brief ����Ʈ�� ��� �ִ� �Ÿ��� �����մϴ�.

	�ִ� �Ÿ��� �ּ� �Ÿ� �����̾�� �մϴ�.

	<b>�⺻��</b> 0.0f
	<b>����</b> [0, PX_MAX_F32)

	\param[in] distance �ִ� �Ÿ�

	@see PxDistanceJoint::maxDistance, PxDistanceJointFlag::eMAX_DISTANCE_ENABLED getMinDistance()
	*/
	virtual void setMaxDistance(PxReal distance) = 0;

	/**
	\brief ����Ʈ�� ��� �ִ� �Ÿ��� �����ɴϴ�.

	\return ���Ǵ� �ִ� �Ÿ�

	@see PxDistanceJoint::maxDistance, PxDistanceJointFlag::eMAX_DISTANCE_ENABLED setMaxDistance()
	*/
	virtual PxReal getMaxDistance() const = 0;

	/**
	\brief ����Ʈ�� ���� ��� ������ �����մϴ�.

	\param[in] tolerance ����Ʈ�� Ȱ��ȭ�Ǵ� ��� ������ ��� �Ÿ�

	@see PxDistanceJoint::tolerance, getTolerance()
	*/
	virtual void setTolerance(PxReal tolerance) = 0;

	/**
	\brief ����Ʈ�� ���� ��� ������ �����ɴϴ�.

	����Ʈ�� [min, max] ������ ��� �Ÿ��� ����Ʈ�� Ȱ��ȭ�Ǳ� ���� �Ÿ��Դϴ�.

	<b>�⺻��</b> 0.25f * PxTolerancesScale::length
	<b>����</b> (0, PX_MAX_F32)

	�� ���� �ּ� �Ÿ��� 0�̰� ������ �Լ��� ����ϴ� ��� �������� �ܻ� ���̰� 0�� �ƴ��� �����ϱ� ���� ���Ǿ�� �մϴ�.

	@see PxDistanceJoint::tolerance, setTolerance()
	*/
	virtual PxReal getTolerance() const = 0;

	/**
	\brief ����Ʈ �������� ������ �����մϴ�.

	�������� Ȱ��ȭ�ǰ� �Ÿ��� [min-error, max+error] ������ �ʰ��ϴ� ��� ���˴ϴ�.

	<b>�⺻��</b> 0.0f
	<b>����</b> [0, PX_MAX_F32)

	\param[in] stiffness ����Ʈ�� ������ ����

	@see PxDistanceJointFlag::eSPRING_ENABLED getStiffness()
	*/
	virtual void setStiffness(PxReal stiffness) = 0;

	/**
	\brief ����Ʈ �������� ������ �����ɴϴ�.

	\return ����Ʈ�� ������ ����

	@see PxDistanceJointFlag::eSPRING_ENABLED setStiffness()
	*/
	virtual PxReal getStiffness() const = 0;

	/**
	\brief ����Ʈ �������� ���踦 �����մϴ�.

	�������� Ȱ��ȭ�ǰ� �Ÿ��� [min-error, max+error] ������ �ʰ��ϴ� ��� ���˴ϴ�.

	<b>�⺻��</b> 0.0f
	<b>����</b> [0, PX_MAX_F32)

	\param[in] damping ����Ʈ �������� ���� ����

	@see PxDistanceJointFlag::eSPRING_ENABLED setDamping()
	*/
	virtual void setDamping(PxReal damping) = 0;

	/**
	\brief ����Ʈ �������� ���踦 �����ɴϴ�.

	\return ����Ʈ �������� ���� ����

	@see PxDistanceJointFlag::eSPRING_ENABLED setDamping()
	*/
	virtual PxReal getDamping() const = 0;

	/**
	\brief Distance Joint�� Ưȭ�� �÷��׸� �����մϴ�.

	<b>�⺻��</b> PxDistanceJointFlag::eMAX_DISTANCE_ENABLED

	\param[in] flags ����Ʈ �÷���

	@see PxDistanceJointFlag setFlag() getFlags()
	*/
	virtual void setDistanceJointFlags(PxDistanceJointFlags flags) = 0;

	/**
	\brief Distance Joint�� Ưȭ�� ���� �÷��׸� true �Ǵ� false�� �����մϴ�.

	\param[in] flag ���� �Ǵ� ������� �÷���
	\param[in] value �÷��׸� ������ ��

	@see PxDistanceJointFlag, getFlags() setFlags()
	*/
	virtual void setDistanceJointFlag(PxDistanceJointFlag::Enum flag, bool value) = 0;

	/**
	\brief Distance Joint�� Ưȭ�� �÷��׸� �����ɴϴ�.

	\return ����Ʈ �÷���

	@see PxDistanceJoint::flags, PxDistanceJointFlag setFlag() setFlags()
	*/
	virtual PxDistanceJointFlags getDistanceJointFlags() const = 0;

	/**
	\brief PxDistanceJoint�� ���ڿ� �̸��� ��ȯ�մϴ�. ����ȭ�� ���˴ϴ�.
	*/
	virtual const char* getConcreteTypeName() const { return "PxDistanceJoint"; }

protected:

	// ����ȭ

	/**
	\brief ������
	*/
	PX_INLINE PxDistanceJoint(PxType concreteType, PxBaseFlags baseFlags) : PxJoint(concreteType, baseFlags) {}

	/**
	\brief ������ȭ ������
	*/
	PX_INLINE PxDistanceJoint(PxBaseFlags baseFlags) : PxJoint(baseFlags) {}

	/**
	\brief �־��� Ÿ�� �̸��� �� �ν��Ͻ��� Ÿ�԰� ��ġ�ϴ��� ���θ� ��ȯ�մϴ�.
	*/
	virtual bool isKindOf(const char* name) const { PX_IS_KIND_OF(name, "PxDistanceJoint", PxJoint); }

	// ~����ȭ
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
