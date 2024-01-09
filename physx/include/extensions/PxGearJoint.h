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

#ifndef PX_GEAR_JOINT_H
#define PX_GEAR_JOINT_H
/** \addtogroup extensions
  @{
*/

#include "extensions/PxJoint.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

	class PxGearJoint;

	/**
	\brief Create a gear Joint.

	\param[in] physics		The physics SDK
	\param[in] actor0		An actor to which the joint is attached. NULL may be used to attach the joint to a specific point in the world frame
	\param[in] localFrame0	The position and orientation of the joint relative to actor0
	\param[in] actor1		An actor to which the joint is attached. NULL may be used to attach the joint to a specific point in the world frame
	\param[in] localFrame1	The position and orientation of the joint relative to actor1

	@see PxGearJoint
	*/
	PxGearJoint*	PxGearJointCreate(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1);

	/**
	\brief �� ���� ���� ȸ�� ����Ʈ�� �����ϰ� ���ο� ���� ������� ���ӵ��� ��ġ�� �����ϴ� ����Ʈ�Դϴ�.

	@see PxGearJointCreate PxJoint
	*/
	class PxGearJoint : public PxJoint
	{
	public:

		/**
		\brief ��� ����Ʈ�� ���� ����� ����/ȸ�� ����Ʈ�� �����մϴ�.

		���޵� ����Ʈ�� PxRevoluteJoint, PxD6Joint
		�Ǵ� PxArticulationJointReducedCoordinate ���ϳ��� �� �ֽ��ϴ�.
		����Ʈ�� Ʈ����Ʈ �� ������ �������� �����ؾ� �մϴ�.
		����Ʈ�� null�� �� �� �����ϴ�.

		�� �Լ��� ȣ���ϸ� �̷��� ����Ʈ�� ��ġ ���� ���� ���� ����ϴ� ������ ���Ǹ�,
		����Ʈ�� ���� ���� �������� �帮��Ʈ�� �����ϴ� �� ���˴ϴ�.
		��� ����Ʈ�� �� �Լ��� ȣ������ �ʰ� ����� �� ������,
		�� ��� ��� �޽��� ġ�� ���̿� �ð��� ������ ���� �Ϻ� �ð����� ��ħ�� �߻��� �� �ֽ��ϴ�.

		\note	�� �Լ��� ȣ���ϸ� ���� ��ġ ���� ���� ���� �缳���˴ϴ�.

		\param[in]	hinge0		ù ��° ���� ����Ʈ
		\param[in]	hinge1		�� ��° ���� ����Ʈ
		\return		�����ϸ� true
		*/
		virtual	bool		setHinges(const PxBase* hinge0, const PxBase* hinge1)	= 0;

		/**
		\brief ��� ����Ʈ�� ���� ����� ����/ȸ�� ����Ʈ�� �����ɴϴ�.

		\param[out]	hinge0		ù ��° ���� ����Ʈ
		\param[out]	hinge1		�� ��° ���� ����Ʈ
		*/
		virtual	void		getHinges(const PxBase*& hinge0, const PxBase*& hinge1)	const	= 0;

		/**
		\brief ���ϴ� ��� ������ �����մϴ�.

		�� ����� �̻� ���� ���� n0 �� n1�̸� ��� ������ n0/n1�Դϴ�.

		\note	���õ� ������ ����Ʈ �������� ������ ��������
				���ĵǾ� ���� ���� ��� ���� ��� ������ ����ؾ� �� �� �ֽ��ϴ�.

		\note	�� �Լ��� ȣ���ϸ� ���� ��ġ ���� ���� ���� �缳���˴ϴ�.

		\param[in]	ratio	�� ���� ���� ���ϴ� ����
		*/
		virtual	void		setGearRatio(float ratio)	= 0;

		/**
		\brief ��� ������ �����ɴϴ�.

		\return		���� ����
		*/
		virtual	float		getGearRatio()	const		= 0;

		virtual	const char*	getConcreteTypeName() const { return "PxGearJoint"; }

	protected:

		PX_INLINE			PxGearJoint(PxType concreteType, PxBaseFlags baseFlags) : PxJoint(concreteType, baseFlags) {}

		PX_INLINE			PxGearJoint(PxBaseFlags baseFlags) : PxJoint(baseFlags)	{}

		virtual	bool		isKindOf(const char* name) const { PX_IS_KIND_OF(name, "PxGearJoint", PxJoint);	}
	};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
