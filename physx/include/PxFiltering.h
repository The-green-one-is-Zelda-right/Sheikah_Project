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

#ifndef PX_FILTERING_H
#define PX_FILTERING_H
/** \addtogroup physics
@{
*/

#include "PxPhysXConfig.h"
#include "foundation/PxFlags.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxActor;
class PxShape;

/**
\brief Collection of flags describing the actions to take for a collision pair.

@see PxPairFlags PxSimulationFilterShader.filter() PxSimulationFilterCallback
*/
struct PxPairFlag
{
	enum Enum
	{
		/**
		\brief ���� �ذ�⿡�� �� �浹 ���� ������ ó���մϴ�.

		\note	�浹�� ���Ͱ� ��ü�� ��쿡�� ������ ��Ĩ�ϴ�.
		*/
		eSOLVE_CONTACT = (1 << 0),

		/**
		\brief �� �浹 �ֿ� ���� ���� ���� �ݹ��� ȣ���մϴ�.
			
		\note	�浹�� ���Ͱ� ��ü�� ��쿡�� ������ ��Ĩ�ϴ�.

		@see PxContactModifyCallback
		*/
		eMODIFY_CONTACTS = (1 << 1),

		/**
		\brief �� �浹 ���� ������ ������ �� ���� ���� �ݹ� �Ǵ� Ʈ���� �ݹ��� ȣ���մϴ�.

		�� �浹 ��ü �� �ϳ��� Ʈ���� ����� ��� (����: #PxShapeFlag::eTRIGGER_SHAPE),
		�ٸ� ��ü�� Ʈ���� ������ �� �� ��� Ʈ���� �ݹ��� ȣ��˴ϴ�.
		�� �浹 ��ü�� Ʈ���� ����� �ƴ� ���, �� �浹 ���� ���Ͱ� ������ ������ �� ���� ���� �ݹ��� ȣ��˴ϴ�.

		\note	�浹�� ���Ͱ� ��ü�� ��쿡�� ������ ��Ĩ�ϴ�.
		\note	eDETECT_DISCRETE_CONTACT �Ǵ� eDETECT_CCD_CONTACT�� Ȱ��ȭ�� ��쿡�� ������ ��Ĩ�ϴ�.

		@see PxSimulationEventCallback.onContact() PxSimulationEventCallback.onTrigger()
		*/
		eNOTIFY_TOUCH_FOUND = (1 << 2),

		/**
		\brief �� �浹 ���� ���� ���� �� ���� ���� �ݹ��� ȣ���մϴ�.
		ii
		�� �浹 ��ü�� Ʈ���� ����� �ƴ� ���, �� �浹 ���� ���Ͱ� ���� ���� �� ���� ���� �ݹ��� ȣ��˴ϴ�.

		\note	Ʈ���Ŵ� �� �̺�Ʈ�� �������� �ʽ��ϴ�.
				�������� Ʈ���� ������ eNOTIFY_TOUCH_FOUND/eNOTIFY_TOUCH_LOST �̺�Ʈ�� ���� �����Ͽ� �����Ǿ�� �մϴ�.
		\note	�浹�� ���Ͱ� ��ü�� ��쿡�� ������ ��Ĩ�ϴ�.
		\note	���� ���� ��ü�� �ڰ� ������ ���� ���۵��� �ʽ��ϴ�.
		\note	eDETECT_DISCRETE_CONTACT �Ǵ� eDETECT_CCD_CONTACT�� Ȱ��ȭ�� ��쿡�� ������ ��Ĩ�ϴ�.
		\note	�̹� ���� ���� �ֿ� ������ �÷��װ� Ȱ��ȭ�Ǹ�, ���� ������ �Ұ� �ٽ� ������ ������ eNOTIFY_TOUCH_PERSISTS �̺�Ʈ�� ���� ���Դϴ�.

		@see PxSimulationEventCallback.onContact() PxSimulationEventCallback.onTrigger()
		*/
		eNOTIFY_TOUCH_PERSISTS = (1 << 3),

		/**
		\brief �� �浹 ���� ������ �ߴ��� �� ���� ���� �ݹ��̳� Ʈ���� �ݹ��� ȣ���մϴ�.

		�� �浹 ��ü �� �ϳ��� Ʈ���� ����� ��� (����: #PxShapeFlag::eTRIGGER_SHAPE),
		�ٸ� ��ü�� Ʈ���� ������ ��� �� ��� Ʈ���� �ݹ��� ȣ��˴ϴ�.
		�� �浹 ��ü�� Ʈ���� ����� �ƴ� ���, �� �浹 ���� ���Ͱ� ������ �ߴ��� �� ���� ���� �ݹ��� ȣ��˴ϴ�.

		\note	�浹�� ���Ͱ� ��ü�� ��쿡�� ������ ��Ĩ�ϴ�.
		\note	�� �̺�Ʈ�� �浹 ��ü �� �ϳ��� �����Ǹ� �߻��մϴ�.
		\note	eDETECT_DISCRETE_CONTACT �Ǵ� eDETECT_CCD_CONTACT�� Ȱ��ȭ�� ��쿡�� ������ ��Ĩ�ϴ�.

		@see PxSimulationEventCallback.onContact() PxSimulationEventCallback.onTrigger()
		*/
		eNOTIFY_TOUCH_LOST = (1 << 4),

		/**
		\brief �� �浹 ���� CCD �н� ���� ���� ���� �� ���� ���� �ݹ��� ȣ���մϴ�.

		���� �н��� ����� CCD�� Ȱ��ȭ�� ���, ������ �̵��ϴ� ��ü�� ������ ��ü�� ���� �� Ƣ��ö� �� �ֽ��ϴ�.
		���� ������ ���� �ùķ��̼� �ܰ� ���� ���� �� ���� ���� �� �ֽ��ϴ�.
		�� �÷��״� CCD ���� ��� ������ �浹�� ������ ���� �����մϴ�. ���ɻ��� ������ �ý����� �׻� ���� ���� ������ CCD �н� �� �ϳ����� ������ ���������� �� �� ������, ���� ���ο��� ���� �Ǵ� ��ӵǴ��� ���θ� �׻� �� �� �����ϴ�. eNOTIFY_TOUCH_CCD�� �浹 ��ü�� CCD �н� �߿� ���� �������� ������ ��쿡�� �����մϴ�.

		\note �浹�� ���Ͱ� ��ü�� ��쿡�� ������ ��Ĩ�ϴ�.
		\note Ʈ���� ����� �������� �ʽ��ϴ�.
		\note eDETECT_CCD_CONTACT�� Ȱ��ȭ�� ��쿡�� ������ ��Ĩ�ϴ�.

		@see PxSimulationEventCallback.onContact() PxSimulationEventCallback.onTrigger()
		*/
		eNOTIFY_TOUCH_CCD = (1 << 5),

		/**
		\brief �� �浹 ���� ���� ���� ���� ���� ���Ͱ� ������ �� �Ӱ谪 �� �ϳ��� �ʰ��� �� ���� ���� �ݹ��� ȣ���մϴ�.

		\note �浹�� ���Ͱ� ��ü�� ��쿡�� ������ ��Ĩ�ϴ�.
		\note eDETECT_DISCRETE_CONTACT �Ǵ� eDETECT_CCD_CONTACT�� Ȱ��ȭ�� ��쿡�� ������ ��Ĩ�ϴ�.

		@see PxSimulationEventCallback.onContact()
		*/
		eNOTIFY_THRESHOLD_FORCE_FOUND = (1 << 6),

		/**
		\brief �� �浹 ���� ���� ���� ���� ���� ���Ͱ� ������ �� �Ӱ谪 �� �ϳ��� ��� �ʰ��� �� ���� ���� �ݹ��� ȣ���մϴ�.

		\note �浹�� ���Ͱ� ��ü�� ��쿡�� ������ ��Ĩ�ϴ�.
		\note ���� �ٽ� ���͸��ǰ� �� �÷��װ� ������ ��Ȱ��ȭ�� ���, �� �Ӱ谪�� ���� �����ӿ��� �ʰ��Ǿ����� ������ �����ӿ��� ���� �߻����� ���� ���Դϴ� (���� �����ӿ��� #eNOTIFY_THRESHOLD_FORCE_FOUND�� �����Ǿ����� ���ο� �������).

		\note eDETECT_DISCRETE_CONTACT �Ǵ� eDETECT_CCD_CONTACT�� Ȱ��ȭ�� ��쿡�� ������ ��Ĩ�ϴ�.

		@see PxSimulationEventCallback.onContact()
		*/
		eNOTIFY_THRESHOLD_FORCE_PERSISTS = (1 << 7),

		/**
		\brief �� �浹 ���� ���� ���� ���� ���� ���Ͱ� ������ �� �Ӱ谪 ��
		�ϳ� �̸����� ������ �� (�� �浹 ���� ������ �ߴ��ϴ� ��� ����) ���� ���� �ݹ��� ȣ���մϴ�.

		\note �浹�� ���Ͱ� ��ü�� ��쿡�� ������ ��Ĩ�ϴ�.
		\note ���� �ٽ� ���͸��ǰ� �� �÷��װ� ������ ��Ȱ��ȭ�� ���,
		�� �Ӱ谪�� ���� �����ӿ��� �̸��̾����� ������ �����ӿ��� ���� �߻����� ���� ���Դϴ�
		(���� �����ӿ��� #eNOTIFY_THRESHOLD_FORCE_FOUND �Ǵ�
		#eNOTIFY_THRESHOLD_FORCE_PERSISTS�� �����Ǿ��������ο� �������).

		\note eDETECT_DISCRETE_CONTACT �Ǵ� eDETECT_CCD_CONTACT�� Ȱ��ȭ�� ��쿡�� ������ ��Ĩ�ϴ�.

		@see PxSimulationEventCallback.onContact()
		*/
		eNOTIFY_THRESHOLD_FORCE_LOST = (1 << 8),

		/**
		\brief �� �浹 ���� ���� ������ �������� �����մϴ�.

		\note	�浹�� ���Ͱ� ��ü�̰� eNOTIFY_TOUCH_... �Ǵ�
				eNOTIFY_THRESHOLD_FORCE_... �÷��׿� �Բ� ���� ���� ������ ��Ĩ�ϴ�.
		\note	eDETECT_DISCRETE_CONTACT �Ǵ�
				eDETECT_CCD_CONTACT�� Ȱ��ȭ�� ��쿡�� ������ ��Ĩ�ϴ�.

		@see PxSimulationEventCallback.onContact() PxContactPair PxContactPair.extractContacts()
		*/
		eNOTIFY_CONTACT_POINTS = (1 << 9),

		/**
		\brief �� �÷��״� �� ���� �̻� �浹 ���� ������ �����ϴ��� ���θ� ��Ÿ���ϴ�.

		\note	������ eSOLVE_CONTACT�� Ȱ��ȭ�� ��쿡�� ����˴ϴ�.
		*/
		eDETECT_DISCRETE_CONTACT = (1 << 10),

		/**
		\brief �� �÷��״� �� ���� CCD(Continuous Collision Detection) ������ �����ϴ��� ���θ� ��Ÿ���ϴ�.

		\note	������ �� �ֿ� eSOLVE_CONTACT�� Ȱ��ȭ�� ��쿡�� ����˴ϴ�.
		\note	�� ����� ����Ϸ��� ���� PxSceneFlag::eENABLE_CCD�� Ȱ��ȭ�Ǿ� �־�� �մϴ�.
		\note	���� ������ �ٵ�� �ùٸ��� �۵��Ϸ��� PxRigidBodyFlag::eENABLE_CCD�� �����Ǿ�� �մϴ�.
		\note	�� �÷��״� Ʈ���� ���� �Բ� ������ �ʽ��ϴ�.
				�׷��� Ʈ���� ����� ������� �ʰ� �ֿ� eNOTIFY_TOUCH_FOUND ��
				eNOTIFY_TOUCH_LOST�� ��û�ϰ� �ֿ� eSOLVE_CONTACT�� Ȱ��ȭ���� �ʵ��� �����Ͽ�
				CCD Ʈ���� �̺�Ʈ�� �䳻�� �� �ֽ��ϴ�.

		@see PxRigidBodyFlag::eENABLE_CCD
		@see PxSceneFlag::eENABLE_CCD
		*/
		eDETECT_CCD_CONTACT = (1 << 11),

		/**
		\brief �� �浹 ���� ���� ������ ���� �ذ� �ӵ��� �����մϴ�.

		�浹 ���� ���� ���� Ȱ��ȭ�� ���, ������ �ذ�Ǳ� ���� ��ü�� �ӵ��� �����˴ϴ�.
		���� ������ ���� ��� �����Ͱ� �������� �ʽ��ϴ�.

		\note	�Ϲ������� �̷��� �ӵ��� ��û�ϴ� ���� �ʿ����� �ʽ��ϴ�.
				�ֳ��ϸ� �̷��� �ӵ��� ������ PxRigidActor ��ü���� ���� ��ȸ�� �� �ֱ� �����Դϴ�.
				�׷��� �ùķ��̼��� ���� �߿� ��ü�� �ӵ��� ������ �� �ִ� ��찡 �ֱ� ������
				�ùķ��̼��� ����� �ӵ��� �ƴ� �� ���ο� �ӵ��� ���� ���� �ݹ鿡�� ��ȯ�� �� �ֽ��ϴ�.

		@see PxSimulationEventCallback.onContact(), PxContactPairVelocity, PxContactPairHeader.extraDataStream
		*/
		ePRE_SOLVER_VELOCITY = (1 << 12),

		/**
		\brief �� �浹 ���� ���� ������ ���� �ذ� �ӵ��� �����մϴ�.

		�浹 ���� ���� ���� Ȱ��ȭ�� ���, ������ �ذ�� ���� ��ü�� �ӵ��� �����˴ϴ�.
		���� ������ ���� ��� �����Ͱ� �������� �ʽ��ϴ�.

		@see PxSimulationEventCallback.onContact(), PxContactPairVelocity, PxContactPairHeader.extraDataStream
		*/
		ePOST_SOLVER_VELOCITY = (1 << 13),

		/**
		\brief �� �浹 ���� ���� ������ ��ü ��� �����մϴ�.

		�浹 ���� ���� ���� Ȱ��ȭ�� ���, ���� �̺�Ʈ �ð��� ��ü ��� �����˴ϴ�.
		���� ������ ���� ��� �����Ͱ� �������� �ʽ��ϴ�.

		\note	������ �̷��� ��� ��û�ϴ� ���� �ʿ����� �ʽ��ϴ�.
				�ֳ��ϸ� �̷��� ����� ������ PxRigidActor ��ü���� ���� ��ȸ�� �� �ֱ� �����Դϴ�.
				�׷��� �ùķ��̼� ���� �߿� ��ü�� ��� ������ �� �ִ� ��찡 ������
				�̷��� ��� ���� ���� �ݹ鿡���� �ùķ��̼��� ����� ��� �ƴ� �� ���ο� ��� ��ȯ�� �� �ֽ��ϴ�.
				�ٸ� ��� ��ʴ� ���� �н��� Ȱ��ȭ�� CCD�� ����Դϴ�.
				������ �̵��ϴ� ��ü�� ������ ��ü�� ���� �� Ƣ��ö� �� �ֽ��ϴ�.
				�� �÷��׸� ����ϸ� �� �浹 �̺�Ʈ���� �浹 ������ ��ü ��� ��û�� �� �ֽ��ϴ�.

		@see PxSimulationEventCallback.onContact(), PxContactPairPose, PxContactPairHeader.extraDataStream
		*/
		eCONTACT_EVENT_POSE = (1 << 14),

		eNEXT_FREE = (1 << 15),        //!< ���ο����θ� ���˴ϴ�.

		/**
		\brief �� �浹 �ֿ� ���� ������ ���� ó���� �����ϵ��� �⺻ �÷��׸� �����մϴ�.
		*/
		eCONTACT_DEFAULT = eSOLVE_CONTACT | eDETECT_DISCRETE_CONTACT,

		/**
		\brief �� �浹 �ֿ� ���� �Ϲ������� ���Ǵ� Ʈ���� ������ ��� ���� �⺻ �÷��׸� �����մϴ�.
		*/
		eTRIGGER_DEFAULT = eNOTIFY_TOUCH_FOUND | eNOTIFY_TOUCH_LOST | eDETECT_DISCRETE_CONTACT
	};
};

/**
\brief Bitfield that contains a set of raised flags defined in PxPairFlag.

@see PxPairFlag
*/
typedef PxFlags<PxPairFlag::Enum, PxU16> PxPairFlags;
PX_FLAGS_OPERATORS(PxPairFlag::Enum, PxU16)



/**
\brief �浹 �ֿ� ���� ���� �۾��� �����ϴ� �÷��� �����Դϴ�.

@see PxFilterFlags PxSimulationFilterShader PxSimulationFilterCallback
*/
struct PxFilterFlag
{
	enum Enum
	{
		/**
		\brief �� ��ü�� �ٿ�� ������ ��ġ�� �� �浹 ���� �����մϴ�.

		���� ���� �ùķ��̼ǿ��� ���õǰ� ���� �� �ϳ��� �߻��� ������ ���͸� �ٽ� �������� �ʽ��ϴ�.

		\li �� ��ü�� �ٿ�� ������ �ٽ� ��ġ�� �Ǹ�(�и��� ��)
		\li ����ڰ� �ٽ� ���͸��� ������ ������(����: #PxScene::resetFiltering())

		@see PxScene::resetFiltering()
		*/
		eKILL = (1 << 0),

		/**
		\brief �� ��ü�� �ٿ�� ������ ��ġ�� �� �Ǵ� �� �浹 ��ü�� ���� ���� �����Ͱ� ����� ������ �浹 ���� �����մϴ�.

		������ ���� �ùķ��̼ǿ��� ���õǰ� ���� �� �ϳ��� �߻��� ������ �ٽ� ���͸� ��û�� �������� �ʽ��ϴ�.

		\li ���� �ְ� ���� ����(����: #eKILL)
		\li �浹 ��ü �� �ϳ��� ���� ������ �Ǵ� ���� ��ü �Ӽ��� ����� ��

		@see PxFilterData PxFilterObjectAttributes
		*/
		eSUPPRESS = (1 << 1),

		/**
		\brief �� �浹 �ֿ� ���� ���� �ݹ�(#PxSimulationFilterCallback::pairFound())�� ȣ���մϴ�.

		@see PxSimulationFilterCallback
		*/
		eCALLBACK = (1 << 2),

		/**
		\brief �� �浹 ���� ���� �ݹ� ��Ŀ�������� �����մϴ�.

		�浹 ���� �ٿ�� ������ ������ �ҰԵǸ� ���� �ݹ� #PxSimulationFilterCallback::pairLost()�� ȣ��˴ϴ�.
		����, �浹 ���� ���� ���´� �� �����ӿ� �� ��(#PxSimulationFilterCallback::statusChange()�� ����) ������ �� �ֽ��ϴ�(���� �սǵ� ������).

		@see PxSimulationFilterCallback
		*/
		eNOTIFY = (1 << 3) | eCALLBACK,

		/**
		\brief ǥ�� ������ ��� ���� ������ �⺻��:

		�ٿ�� ������ ��ġ�� ��� ���ø����̼��� ���� �浹 �Ӽ��� �� �� �����ϰ�
		�� �ֿ� ���� �ٽ� ����� �ʰ�, ��ġ�� ���� �Ǵ� ���� �Ӽ��� ����ǰų� �ٽ� ���͸��� ��û�� �������Դϴ�.

		�ٿ�� ������ �սǵ� ���� �˸��� �������� �ʽ��ϴ�.

		���� �װų� �������� �����Ƿ� �浹 ������ ó���� ���Դϴ�.
		*/
		eDEFAULT = 0
	};
};

/**
\brief Bitfield that contains a set of raised flags defined in PxFilterFlag.

@see PxFilterFlag
*/
typedef PxFlags<PxFilterFlag::Enum, PxU16> PxFilterFlags;
PX_FLAGS_OPERATORS(PxFilterFlag::Enum, PxU16)


/**
\brief PxFilterData�� �浹 ���͸� ���̴���/�Ǵ� �ݹ����� ���޵Ǵ� ����� ���� �������Դϴ�.

@see PxShape.setSimulationFilterData() PxShape.getSimulationFilterData() PxSimulationFilterShader PxSimulationFilterCallback
*/
struct PxFilterData
{
	PX_INLINE PxFilterData(const PxEMPTY)
	{
	}

	/**
	\brief Default constructor.
	*/
	PX_INLINE PxFilterData() 
	{
		word0 = word1 = word2 = word3 = 0;
	}

	/**
	\brief Copy constructor.
	*/
	PX_INLINE PxFilterData(const PxFilterData& fd) : word0(fd.word0), word1(fd.word1), word2(fd.word2), word3(fd.word3)	{}

	/**
	\brief Constructor to set filter data initially.
	*/
	PX_INLINE PxFilterData(PxU32 w0, PxU32 w1, PxU32 w2, PxU32 w3) : word0(w0), word1(w1), word2(w2), word3(w3) {}

	/**
	\brief (re)sets the structure to the default.	
	*/
	PX_INLINE void setToDefault()
	{
		*this = PxFilterData();
	}

	/**
	\brief Assignment operator
	*/
	PX_INLINE void operator = (const PxFilterData& fd)
	{
		word0 = fd.word0;
		word1 = fd.word1;
		word2 = fd.word2;
		word3 = fd.word3;
	}

	/**
	\brief Comparison operator to allow use in Array.
	*/
	PX_INLINE bool operator == (const PxFilterData& a) const
	{
		return a.word0 == word0 && a.word1 == word1 && a.word2 == word2 && a.word3 == word3;
	}

	/**
	\brief Comparison operator to allow use in Array.
	*/
	PX_INLINE bool operator != (const PxFilterData& a) const
	{
		return !(a == *this);
	}

	PxU32 word0;
	PxU32 word1;
	PxU32 word2;
	PxU32 word3;
};


/**
\brief Identifies each type of filter object.

@see PxGetFilterObjectType()
*/
struct PxFilterObjectType
{
	enum Enum
	{
		/**
		\brief ���� ��ü ��ü
		@see PxRigidStatic
		*/
		eRIGID_STATIC,

		/**
		\brief ���� ��ü ��ü
		@see PxRigidDynamic
		*/
		eRIGID_DYNAMIC,

		/**
		\brief ��Ƽŧ���̼�
		@see PxArticulationReducedCoordinate
		*/
		eARTICULATION,

		/**
		\brief ���� �ý���
		@see PxParticleSystem
		*/
		ePARTICLESYSTEM,

		/**
		\brief FEM ��� ����Ʈ �ٵ�
		@see PxSoftBody
		*/
		eSOFTBODY,

		/**
		\brief FEM ��� ��
		\note ���� ��
		@see PxFEMCloth
		*/
		eFEMCLOTH,

		/**
		\brief ��� �ý���
		\note ���� ��
		@see PxHairSystem
		*/
		eHAIRSYSTEM,

		//! \brief �������� ��� ����!
		eMAX_TYPE_COUNT = 16,

		//! \brief �������� ��� ����!
		eUNDEFINED = eMAX_TYPE_COUNT - 1
	};
};


// For internal use only
struct PxFilterObjectFlag
{
	enum Enum
	{
		eKINEMATIC	= (1<<4),
		eTRIGGER	= (1<<5),

		eNEXT_FREE	= (1<<6)	// Used internally
	};
};


/**
\brief �浹 ���͸� ���̴� ��/�Ǵ� �ݹ鿡 ���޵Ǵ� ����ü��, �浹 ���� ��ü�� ���� �߰� ������ �����մϴ�.

@see PxSimulationFilterShader PxSimulationFilterCallback getActorType() PxFilterObjectIsKinematic() PxFilterObjectIsTrigger()
*/
typedef PxU32 PxFilterObjectAttributes;


/**
\brief �浹 �� ��ü�� ���� �Ӽ����� ���� ��ü ������ �����մϴ�.

\param[in]	attr �浹 �� ��ü�� ���� �Ӽ�
\return		�浹 �� ��ü�� ����.

@see PxFilterObjectType
*/
PX_INLINE PxFilterObjectType::Enum PxGetFilterObjectType(PxFilterObjectAttributes attr)
{
	return PxFilterObjectType::Enum(attr & (PxFilterObjectType::eMAX_TYPE_COUNT-1));
}


/**
\brief �浹 �� ��ü�� Ű�׸�ƽ ��ü�� ���ϴ��� ���θ� �����մϴ�.

\param[in]	attr �浹 �� ��ü�� ���� �Ӽ�
\return		��ü�� Ű�׸�ƽ ��ü�� ���ϴ� ��� true, �׷��� ������ false

@see PxRigidBodyFlag::eKINEMATIC
*/
PX_INLINE bool PxFilterObjectIsKinematic(PxFilterObjectAttributes attr)
{
	return (attr & PxFilterObjectFlag::eKINEMATIC) != 0;
}


/**
\brief �浹 �� ��ü�� Ʈ���� ������� ���θ� �����մϴ�.

\param[in]	attr �浹 �� ��ü�� ���� �Ӽ�
\return		��ü�� Ʈ���� ����� ��� true, �׷��� ������ false

@see PxShapeFlag::eTRIGGER_SHAPE
*/
PX_INLINE bool PxFilterObjectIsTrigger(PxFilterObjectAttributes attr)
{
	return (attr & PxFilterObjectFlag::eTRIGGER) != 0;
}

/**
\brief ���������� �浹�� �� �ִ� �� ��ü ���� �ùķ��̼ǿ��� ��� ó���Ǿ�� �ϴ��� �����ϴ� ���� �޼����Դϴ�.

�浹 ���͸��� �� ��ü�� �ٿ�� ������ ��ġ�� ��� �� ��ü ���� ��� ó�������� �����ϴ� ��Ŀ�����Դϴ�.
������ ���� �浹 ���ʹ� �浹 ���� ó���Ǿ�� �ϴ��� ����,
�Ͻ������� ���õǾ�� �ϴ��� ���� �Ǵ� �������� �ϴ��� ���θ� �����մϴ�.
�浹 ���� ó���Ǿ�� �ϴ� ��� ���ʹ� �߰��� ��� ó���Ǿ�� �ϴ����� ������ �� �ֽ��ϴ�.
���� ���, ������ �ذ��ؾ� �ϴ���, � �ݹ��� ȣ���ؾ� �ϴ��� �Ǵ� � ������ ������ �ϴ��� ���Դϴ�.
�� �Լ��� �浹 �ֿ� ���� �ùķ��̼��� �����ؾ� �� �۾��� �����ϴ� PxFilterFlag �÷��׸� ��ȯ�ϰ� PxPairFlag �÷��׸� �����մϴ�.

\note	���� ���̴��� �⺻ ������ PhysX Ȯ�� ���̺귯���� �����Ǹ�
#PxDefaultSimulationFilterShader�� �����Ͻʽÿ�.

�� �޼���� ���� ��쿡 ȣ��˴ϴ�.
\li �� ��ü�� �ٿ�� ������ ��ġ�� �������� ��.
\li �� ��ü�� �ٿ�� ������ ��ġ�� �� ��ü�� ���� ������ �Ǵ� ���� �Ӽ��� ����� ���
\li resetFiltering()�� ���� �ٽ� ���͸��� ������ ��û�� ���(����: #PxScene::resetFiltering())
\li ��� �������� ���͸��� ��û�� ���

\note	Ư�� ��ü ���� �׻� ���õǸ� �� �޼��尡 ȣ����� �ʽ��ϴ�. �̴� ������ ���� ����Դϴ�.
\li �� ���� ��ü ������ ��
\li ���� ��ü ���Ϳ� � ��ü ������ ��(�ϳ��� Ʈ���Ű� �ƴ� �� �Ǵ�
	��������� PxPairFilteringMode::eKEEP�� ���� Ȱ��ȭ���� ���� ���)

\li �� � ��ü ������ ��(�ϳ��� Ʈ���Ű� �ƴ� �� �Ǵ�
	��������� PxPairFilteringMode::eKEEP�� ���� Ȱ��ȭ���� ���� ���)

\li �� ����Ʈ�� ��ü ��ü�� �浹�� ��Ȱ��ȭ�ϵ��� ���ǵ� ���
\li �� ������ ��ü ��ü�� ���� ������ ���� ����� ���

\note	�̴� ���ɿ� �߿��� �޼����̸� ���¸� ���缭�� �� �˴ϴ�.
		�� �޼��忡�� �ܺ� ��ü�� �׼����ϰų� �ζ��ε��� ���� �ܺ� �޼��带 ȣ���ؼ��� �� �˴ϴ�.
		�浹 ���� ���͸��ϴ� �� ������ ���� �ʿ��� ����� �ֿ� ���� ���� �ݹ� ��Ŀ������ ����Ͻʽÿ�
		(����: #PxSimulationFilterCallback, #PxFilterFlag::eCALLBACK, #PxFilterFlag::eNOTIFY).

\param[in]	attributes0 ù ��° ��ü�� ���� �Ӽ�
\param[in]	filterData0 ù ��° ��ü�� ����� ���� ���� ������
\param[in]	attributes1 �� ��° ��ü�� ���� �Ӽ�
\param[in]	filterData1 �� ��° ��ü�� ����� ���� ���� ������
\param[out] pairFlags ���� ���� ó���ϴ� ����� ���� �߰� ������ �����ϴ� �÷���
\param[in]	constantBlock ��� ���� ���� ������(����: #PxSceneDesc.filterShaderData)
\param[in]	constantBlockSize ���� ���� �������� ũ��(����: #PxSceneDesc.filterShaderDataSize)
\return		���� �����ų� �Ͻ������� �����ϰų� ó���ؾ� �ϴ��� ���� ��
			�� �ֿ� ���� ���� �ݹ��� ȣ���ؾ� �ϴ��� ���θ� �����ϴ� ���� �÷����Դϴ�.

@see PxSimulationFilterCallback PxFilterData PxFilterObjectAttributes PxFilterFlag PxFilterFlags PxPairFlag PxPairFlags PxSceneDesc.filterShader
*/
typedef PxFilterFlags (*PxSimulationFilterShader)
	(PxFilterObjectAttributes attributes0, PxFilterData filterData0, 
	 PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	 PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);



/**
\brief �浹 ���� ó���� �����ϴ� ���� �ݹ��Դϴ�.

�� Ŭ������ �� �����ϰ� ������ �浹 �� ���͸� ������ �����ϱ� ���� �����Ǹ� ���� ��� ����� ���� ���α׷��� ���¸� ����� �� �ֽ��ϴ�.
���� �ݹ��� ���� ����ڿ��� �浹 ���� �����ϰ� ���� ���¸� ������Ʈ�� ��ȸ�� �����մϴ�.

�� �Ϲ����� ������ �����ϰ� �ִ� #PxSimulationFilterShader�� ���� ������ Ȯ���ϴ� ���� �����ϴ�.

\note	�ݹ� ������ SDK ���¸� �����ؼ��� �� �˴ϴ�. Ư�� ��ü�� �����ϰų� �ı��ؼ��� �� �˴ϴ�.
		���¸� �����ؾ� �ϴ� ��� ���� ������ ���ۿ� �����ϰ� �ùķ��̼� �ܰ� ���Ŀ� �����ؾ� �մϴ�.

\note	�ݹ��� ����� ������ �Ǵ� �ùķ��̼� �����忡�� ����� �� ������, �̵��� ���ÿ� ����� �� �ֽ��ϴ�.
		�ش� ��ü�� �̹� ������ �߿� ���� ���α׷��� ���� ������ �� �ֽ��ϴ�. 
		SDK API�� �ݹ鿡�� ����ϴ� ���� ���� ���α׷� �������� �ùķ��̼� ���� ���� ȣ���� �ϸ� ���� ������ �߻����� �ʵ��� �����ؾ� �մϴ�.
		���� �ݹ��� ������κ��� �����ϵ��� �ϴ� ���� ���� ���α׷��� å���̸�, 
		�ݹ��� ������ �߿� ���� ȣ��Ǵ����� ���� �޶��� �� �ִ� ��ȯ ���� �ùķ��̼ǿ� ��������� ������ ���Դϴ�.

@see PxSceneDesc.filterCallback PxSimulationFilterShader
*/
class PxSimulationFilterCallback
{
public:

	/**
	\brief ���������� �浹�� �� �ִ� �� ��ü�� ���� ��� ó���Ǿ�� �ϴ����� �����ϴ� ���� �޼����Դϴ�.

	�� �޼���� ���� ���̴����� ��ȯ�� ���� �÷���(#PxSimulationFilterShader ����)��
	���� �ݹ��� ȣ���ؾ� �Ѵٴ� ���� ��Ÿ�� �� ȣ��˴ϴ� (#PxFilterFlag::eCALLBACK �Ǵ� #PxFilterFlag::eNOTIFY�� ������).
	�־��� �浹 �ֿ� ���� �ùķ��̼��� ��� �����ؾ� �ϴ��� �����ϱ� ���� PxFilterFlag �÷��׸� ��ȯ�ϰ� PxPairFlag �÷��׸� �����մϴ�.

	\param[in] pairID			���� �浹�� ���� ���� ���� ������ �����ϴ� �浹 ���� ���� ID (����: #statusChange())
	\param[in] attributes0		ù ��° ��ü�� ���� �Ӽ�
	\param[in] filterData0		ù ��° ��ü�� ����� ���� ���� ������
	\param[in] a0				ù ��° ��ü�� ���� ������
	\param[in] s0				ù ��° ��ü�� ��� ������ (��ü�� ����� ������ NULL)
	\param[in] attributes1		�� ��° ��ü�� ���� �Ӽ�
	\param[in] filterData1		�� ��° ��ü�� ����� ���� ���� ������
	\param[in] a1				�� ��° ��ü�� ���� ������
	\param[in] s1				�� ��° ��ü�� ��� ������ (��ü�� ����� ������ NULL)
	\param[in,out] pairFlags	In: ���� ���̴����� ��ȯ�� ��� �÷���. Out: ���� �� ��� ó���ؾ� �ϴ����� ���� �߰� ����
	\return						���� �������� �ϴ���, �Ͻ������� ���õǾ�� �ϴ��� �Ǵ� ó���Ǿ�� �ϴ��� �� ���� �����ϰ�
								���� �ݹ��� ���� �� ������ ���� ������ �������� �����ϴ� ���� �÷���

	@see PxSimulationFilterShader PxFilterData PxFilterObjectAttributes PxFilterFlag PxPairFlag
	*/
	virtual		PxFilterFlags	pairFound(	PxU64 pairID,
											PxFilterObjectAttributes attributes0, PxFilterData filterData0, const PxActor* a0, const PxShape* s0,
											PxFilterObjectAttributes attributes1, PxFilterData filterData1, const PxActor* a1, const PxShape* s1,
											PxPairFlags& pairFlags) = 0;

	/**
	\brief ���� ���� �浹 �� ��������� �˸��� �ݹ��Դϴ�.

	�� �޼���� �浹 �� ������ų� �ٽ� ���͸� �� �� ȣ��˴ϴ�.
	�̴� pairFound()���� (#PxFilterFlag::eNOTIFY�� ������) ���� �ݹ� ���� ������ �浹 ���� ����˴ϴ�.

	\param[in]					pairID �浹 ����� ���� ID
	\param[in]					attributes0 ù ��° ��ü�� ���� �Ӽ�
	\param[in]					filterData0 ù ��° ��ü�� ����� ���� ���� ������
	\param[in]					attributes1 �� ��° ��ü�� ���� �Ӽ�
	\param[in]					filterData1 �� ��° ��ü�� ����� ���� ���� ������
	\param[in]					objectRemoved ��ü �� �ϳ��� ������ ���ŵǾ� �� �սǵ� ��� true

	@see pairFound() PxSimulationFilterShader PxFilterData PxFilterObjectAttributes
	*/
	virtual		void			pairLost(	PxU64 pairID,
											PxFilterObjectAttributes attributes0, PxFilterData filterData0,
											PxFilterObjectAttributes attributes1, PxFilterData filterData1,
											bool objectRemoved) = 0;

	/**
	\brief Callback to give the opportunity to change the filter state of a tracked collision pair.

	This method gets called once per simulation step to let the application change the filter and pair
	flags of a collision pair that has been reported in #pairFound() and requested callbacks by
	setting #PxFilterFlag::eNOTIFY. To request a change of filter status, the target pair has to be
	specified by its ID, the new filter and pair flags have to be provided and the method should return true.

	\note If this method changes the filter status of a collision pair and the pair should keep being tracked
	by the filter callbacks then #PxFilterFlag::eNOTIFY has to be set.

	\note The application is responsible to ensure that this method does not get called for pairs that have been
	reported as lost, see #pairLost().

	\param[out] pairID			ID of the collision pair for which the filter status should be changed
	\param[out] pairFlags		The new pairFlags to apply to the collision pair
	\param[out] filterFlags		The new filterFlags to apply to the collision pair
	\return True if the changes should be applied. In this case the method will get called again. False if
	no more status changes should be done in the current simulation step. In that case the provided flags will be discarded.

	@see pairFound() pairLost() PxFilterFlag PxPairFlag
	*/
	virtual		bool			statusChange(PxU64& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags) = 0;

protected:
	virtual						~PxSimulationFilterCallback() {}
};

struct PxPairFilteringMode
{
	enum Enum
	{
		/**
		\brief Output pair from BP, potentially send to user callbacks, create regular interaction object.

		Enable contact pair filtering between kinematic/static or kinematic/kinematic rigid bodies.
		
		By default contacts between these are suppressed (see #PxFilterFlag::eSUPPRESS) and don't get reported to the filter mechanism.
		Use this mode if these pairs should go through the filtering pipeline nonetheless.

		\note This mode is not mutable, and must be set in PxSceneDesc at scene creation.
		*/
		eKEEP,

		/**
		\brief Output pair from BP, create interaction marker. Can be later switched to regular interaction.
		*/
		eSUPPRESS,

		/**
		\brief Don't output pair from BP. Cannot be later switched to regular interaction, needs "resetFiltering" call.
		*/
		eKILL,

		/**
		\brief Default is eSUPPRESS for compatibility with previous PhysX versions.
		*/
		eDEFAULT = eSUPPRESS
	};
};

/**
\brief Struct for storing a particle/vertex - rigid filter pair with comparison operators
*/
struct PxParticleRigidFilterPair
{
	PX_CUDA_CALLABLE PxParticleRigidFilterPair() {}

	PX_CUDA_CALLABLE PxParticleRigidFilterPair(const PxU64 id0, const PxU64 id1): mID0(id0), mID1(id1) {}

	PxU64 mID0; //!< Rigid node index
	PxU64 mID1; //!< Particle/vertex id

	PX_CUDA_CALLABLE bool operator<(const PxParticleRigidFilterPair& other) const
	{
		if(mID0 < other.mID0)
			return true;
		if(mID0 == other.mID0 && mID1 < other.mID1)
			return true;
		return false;
	}

	PX_CUDA_CALLABLE bool operator>(const PxParticleRigidFilterPair& other) const
	{
		if(mID0 > other.mID0)
			return true;
		if(mID0 == other.mID0 && mID1 > other.mID1)
			return true;
		return false;
	}

	PX_CUDA_CALLABLE bool operator==(const PxParticleRigidFilterPair& other) const
	{
		return (mID0 == other.mID0 && mID1 == other.mID1);
	}
};


#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
