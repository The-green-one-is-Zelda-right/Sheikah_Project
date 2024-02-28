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

#ifndef PX_SIMULATION_EVENT_CALLBACK_H
#define PX_SIMULATION_EVENT_CALLBACK_H
/** \addtogroup physics
@{
*/

#include "foundation/PxVec3.h"
#include "foundation/PxTransform.h"
#include "foundation/PxMemory.h"
#include "PxPhysXConfig.h"
#include "PxFiltering.h"
#include "PxContact.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxShape;
class PxActor;
class PxRigidActor;
class PxRigidBody;
class PxConstraint;


/**
\brief Extra data item types for contact pairs.

@see PxContactPairExtraDataItem.type
*/
struct PxContactPairExtraDataType
{
	enum Enum
	{
		ePRE_SOLVER_VELOCITY,	//!< see #PxContactPairVelocity
		ePOST_SOLVER_VELOCITY,	//!< see #PxContactPairVelocity
		eCONTACT_EVENT_POSE,	//!< see #PxContactPairPose
		eCONTACT_PAIR_INDEX  	//!< see #PxContactPairIndex
	};
};


/**
\brief Base class for items in the extra data stream of contact pairs

@see PxContactPairHeader.extraDataStream
*/
struct PxContactPairExtraDataItem
{
public:
	PX_FORCE_INLINE PxContactPairExtraDataItem() {}

	/**
	\brief The type of the extra data stream item
	*/
	PxU8 type;
};


/**
\brief Velocities of the contact pair rigid bodies

This struct is shared by multiple types of extra data items. The #type field allows to distinguish between them:
\li PxContactPairExtraDataType::ePRE_SOLVER_VELOCITY: see #PxPairFlag::ePRE_SOLVER_VELOCITY
\li PxContactPairExtraDataType::ePOST_SOLVER_VELOCITY: see #PxPairFlag::ePOST_SOLVER_VELOCITY

\note For static rigid bodies, the velocities will be set to zero.

@see PxContactPairHeader.extraDataStream
*/
struct PxContactPairVelocity : public PxContactPairExtraDataItem
{
public:
	PX_FORCE_INLINE PxContactPairVelocity() {}

	/**
	\brief The linear velocity of the rigid bodies
	*/
	PxVec3 linearVelocity[2];
	
	/**
	\brief The angular velocity of the rigid bodies
	*/
	PxVec3 angularVelocity[2];
};


/**
\brief World space actor poses of the contact pair rigid bodies

@see PxContactPairHeader.extraDataStream PxPairFlag::eCONTACT_EVENT_POSE
*/
struct PxContactPairPose : public PxContactPairExtraDataItem
{
public:
	PX_FORCE_INLINE PxContactPairPose() {}

	/**
	\brief The world space pose of the rigid bodies
	*/
	PxTransform globalPose[2];
};


/**
\brief Marker for the beginning of a new item set in the extra data stream.

If CCD with multiple passes is enabled, then a fast moving object might bounce on and off the same
object multiple times. Also, different shapes of the same actor might gain and lose contact with an other
object over multiple passes. This marker allows to separate the extra data items for each collision case, as well as
distinguish the shape pair reports of different CCD passes.

Example:
Let us assume that an actor a0 with shapes s0_0 and s0_1 hits another actor a1 with shape s1.
First s0_0 will hit s1, then a0 will slightly rotate and s0_1 will hit s1 while s0_0 will lose contact with s1.
Furthermore, let us say that contact event pose information is requested as extra data.
The extra data stream will look like this:

PxContactPairIndexA | PxContactPairPoseA | PxContactPairIndexB | PxContactPairPoseB

The corresponding array of PxContactPair events (see #PxSimulationEventCallback.onContact()) will look like this:

PxContactPair(touch_found: s0_0, s1) | PxContactPair(touch_lost: s0_0, s1) | PxContactPair(touch_found: s0_1, s1)
 
The #index of PxContactPairIndexA will point to the first entry in the PxContactPair array, for PxContactPairIndexB,
#index will point to the third entry.

@see PxContactPairHeader.extraDataStream
*/
struct PxContactPairIndex : public PxContactPairExtraDataItem
{
public:
	PX_FORCE_INLINE PxContactPairIndex() {}

	/**
	\brief The next item set in the extra data stream refers to the contact pairs starting at #index in the reported PxContactPair array.
	*/
	PxU16 index;
};


/**
\brief A class to iterate over a contact pair extra data stream.

@see PxContactPairHeader.extraDataStream
*/
struct PxContactPairExtraDataIterator
{
	/**
	\brief Constructor
	\param[in] stream Pointer to the start of the stream.
	\param[in] size Size of the stream in bytes.
	*/
	PX_FORCE_INLINE PxContactPairExtraDataIterator(const PxU8* stream, PxU32 size) 
		: currPtr(stream), endPtr(stream + size), contactPairIndex(0)
	{
		clearDataPtrs();
	}

	/**
	\brief ���ͷ����͸� ���� �Ϸ��� �߰� ������ �׸����� �̵���ŵ�ϴ�.

	contact pair extra data ��Ʈ������ �ش� #PxPairFlag �÷��׿� ���� ��û�� �׸� ��Ʈ�� ���ԵǾ� �ֽ��ϴ�.
	#PxPairFlag::ePRE_SOLVER_VELOCITY, #PxPairFlag::ePOST_SOLVER_VELOCITY, #PxPairFlag::eCONTACT_EVENT_POSE.
	�� ��Ʈ���� ���� �ϳ��� �׸�� PxContactPairIndex �׸��� ���Ե� �� �ֽ��ϴ�. �� �޼���� ��Ʈ���� �Ľ��ϰ� ���ͷ������� ��� ������ ���� ��Ʈ�� �ش� �׸����� �����մϴ� (�׸��� ��� ������ ���).
	CCD�� ��Ȱ��ȭ�� ��쿡�� �ϳ��� �׸� ��Ʈ�� ���� �մϴ�. CCD�� ���� �н��� Ȱ��ȭ�� ��쿡�� �ϳ� �̻��� �׸� ��Ʈ�� ���� �� �ֽ��ϴ�.

	\note	��� contact pair extra data�� ��� �� ������ ��û������,
			��� �� �� �׸� ��Ʈ�� ���� ���ϰ� ��ſ� ���� �� ������ �� ���� ��� �˴ϴ�.
			���� ���, 
			���Ϳ� �� ���� ����� �ְ� �� �� �ٸ� ���Ϳ� �浹�ϴ� ���, 
			�׸� ��Ʈ�� �ϳ��� ���� ���Դϴ� (�� ��� �ֿ� ��� ����Ǳ� ����).

	\return ��Ʈ���� �� ���� �߰� ������ �׸� ��Ʈ�� �ִ� ��� true, �׷��� ������ false.

	@see PxContactPairVelocity PxContactPairPose PxContactPairIndex
	*/
	PX_INLINE bool nextItemSet()
	{
		clearDataPtrs();
		
		bool foundEntry = false;
		bool endOfItemSet = false;
		while ((currPtr < endPtr) && (!endOfItemSet))
		{
			const PxContactPairExtraDataItem* edItem = reinterpret_cast<const PxContactPairExtraDataItem*>(currPtr);
			PxU8 type = edItem->type;

			switch(type)
			{
				case PxContactPairExtraDataType::ePRE_SOLVER_VELOCITY:
				{
					PX_ASSERT(!preSolverVelocity);
					preSolverVelocity = static_cast<const PxContactPairVelocity*>(edItem);
					currPtr += sizeof(PxContactPairVelocity);
					foundEntry = true;
				}
				break;
				
				case PxContactPairExtraDataType::ePOST_SOLVER_VELOCITY:
				{
					postSolverVelocity = static_cast<const PxContactPairVelocity*>(edItem);
					currPtr += sizeof(PxContactPairVelocity);
					foundEntry = true;
				}
				break;
				
				case PxContactPairExtraDataType::eCONTACT_EVENT_POSE:
				{
					eventPose = static_cast<const PxContactPairPose*>(edItem);
					currPtr += sizeof(PxContactPairPose);
					foundEntry = true;
				}
				break;
				
				case PxContactPairExtraDataType::eCONTACT_PAIR_INDEX:
				{
					if (!foundEntry)
					{
						contactPairIndex = static_cast<const PxContactPairIndex*>(edItem)->index;
						currPtr += sizeof(PxContactPairIndex);
						foundEntry = true;
					}
					else
						endOfItemSet = true;
				}
				break;
				
				default:
					return foundEntry;
			}
		}
		
		return foundEntry;
	}

private:
	/**
	\brief Internal helper
	*/
	PX_FORCE_INLINE void clearDataPtrs()
	{
		preSolverVelocity = NULL;
		postSolverVelocity = NULL;
		eventPose = NULL;
	}
	
public:
	/**
	\brief Current pointer in the stream.
	*/
	const PxU8* currPtr;
	
	/**
	\brief Pointer to the end of the stream.
	*/
	const PxU8* endPtr;
	
	/**
	\brief Pointer to the current pre solver velocity item in the stream. NULL if there is none.
	
	@see PxContactPairVelocity
	*/
	const PxContactPairVelocity* preSolverVelocity;
	
	/**
	\brief Pointer to the current post solver velocity item in the stream. NULL if there is none.
	
	@see PxContactPairVelocity
	*/
	const PxContactPairVelocity* postSolverVelocity;
	
	/**
	\brief Pointer to the current contact event pose item in the stream. NULL if there is none.
	
	@see PxContactPairPose
	*/
	const PxContactPairPose* eventPose;
	
	/**
	\brief The contact pair index of the current item set in the stream.
	
	@see PxContactPairIndex
	*/
	PxU32 contactPairIndex;
};


/**
\brief Collection of flags providing information on contact report pairs.

@see PxContactPairHeader
*/
struct PxContactPairHeaderFlag
{
	enum Enum
	{
		eREMOVED_ACTOR_0				= (1<<0),			//!< The actor with index 0 has been removed from the scene.
		eREMOVED_ACTOR_1				= (1<<1)			//!< The actor with index 1 has been removed from the scene.
	};
};

/**
\brief Bitfield that contains a set of raised flags defined in PxContactPairHeaderFlag.

@see PxContactPairHeaderFlag
*/
typedef PxFlags<PxContactPairHeaderFlag::Enum, PxU16> PxContactPairHeaderFlags;
PX_FLAGS_OPERATORS(PxContactPairHeaderFlag::Enum, PxU16)


/**
\brief �� Ŭ������ �ν��Ͻ��� PxSimulationEventCallback.onContact()�� ���޵˴ϴ�.

@see PxSimulationEventCallback.onContact()
*/
struct PxContactPairHeader
{
	public:
		PX_INLINE	PxContactPairHeader() {}

	/**
	\brief	�˸� ��� ���� �� �����Դϴ�.

	\note	���� �����ʹ� ������ ���͸� ������ �� �ֽ��ϴ�.
			�̴� PxPairFlag::eNOTIFY_TOUCH_LOST
			�Ǵ� PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST �̺�Ʈ��
			�ֿ� ���� ��û�Ǿ����� ���õ� ���� �� �ϳ��� �����ǰų� ������ ���ŵ� ��쿡 �߻��մϴ�.
			�� ������� ���θ� Ȯ���Ϸ��� #flags ����� Ȯ���Ͻʽÿ�. ������ ���Ϳ� ���� �����͸� ���������� ���ʽÿ�.
			������ ���Ϳ� ���� �����ʹ� ������ ���� ������ �� �ִ� ����� ������ ������ ������Ʈ�� �� �ֵ��� �����˴ϴ�.

	@see PxActor
	*/
	PxActor*					actors[2];

	/**
	\brief �ùķ��̼� ������ PxPairFlag �÷��׿��� ��û�� ��� �߰� �����͸� �����ϴ� ��Ʈ���Դϴ�.

	�� �����ʹ� ����ó ���� �ֿ� ���� � ������ �߰� ������ ������ ��û���� ���� ��쿡�� ��ȿ�մϴ�
	(��: #PxPairFlag::ePOST_SOLVER_VELOCITY ��), �׷��� ������ NULL�� ���Դϴ�.

	@see PxPairFlag
	*/
	const PxU8*					extraDataStream;
	
	/**
	\brief �߰� ������ ��Ʈ���� ũ�� [����Ʈ]
	*/
	PxU16						extraDataStreamSize;

	/**
	\brief ���� ���� �ֿ� ���� �߰� �����Դϴ�.

	@see PxContactPairHeaderFlag
	*/
	PxContactPairHeaderFlags	flags;

	/**
	\brief ���� �ֿ� ���� �������Դϴ�.
	*/
	const struct PxContactPair*	pairs;

	/**
	\brief ���� ���� ���Դϴ�.
	*/
	PxU32						nbPairs;
};


/**
\brief ����ó ���� �ֿ� ���� ������ �����ϴ� �÷��� �����Դϴ�.

@see PxContactPair
*/
struct PxContactPairFlag
{
	enum Enum
	{
		/**
		\brief �ε����� 0�� ������ ����/������ ���ŵǾ����ϴ�.
		*/
		eREMOVED_SHAPE_0				= (1<<0),

		/**
		\brief �ε����� 1�� ����� ���/������ ���ŵǾ����ϴ�.
		*/
		eREMOVED_SHAPE_1				= (1<<1),

		/**
		\brief ù ��° ���� �� ����.

		������ ������ ���� �� ���� ���� ù ��° ������ ǥ���ϸ�, ���� �ùķ��̼� ������ ������ �ٸ� ������ ���� ������ ���� �����ϴ�.

		\note: �� ������ �ش� �ֿ� ���� #PxPairFlag::eNOTIFY_TOUCH_FOUND�� ����� ��쿡�� ����� �� �ֽ��ϴ�.
		*/
		eACTOR_PAIR_HAS_FIRST_TOUCH		= (1<<2),

		/**
		\brief ���� �� ���� ��� ������ �սǵǾ����ϴ�.

		�� ���� ���� ��� ������ ���������� ���� �ùķ��̼� ������ ���Ŀ��� � ������ �ֵ� �������� �ʽ��ϴ�.
		*/
		eACTOR_PAIR_LOST_TOUCH			= (1<<3),

		/**
		\brief ���� �÷���, #PxContactPair.extractContacts()�� ���˴ϴ�.

		����� ���� ���޽��� ��� �������� ���� �����˴ϴ�. 
		�� #PxPairFlag::eSOLVE_CONTACT�� ������ ��쿡 �ش��մϴ�.
		*/
		eINTERNAL_HAS_IMPULSES			= (1<<4),

		/**
		\brief ���� �÷���, #PxContactPair.extractContacts()�� ���˴ϴ�.

		������ ���� ������ ���� ���� ���� �����Ͽ� �����˴ϴ�. �̴� �ַ� ���� �ﰢ�� �ε����� ������ ������ �ֽ��ϴ�.ggg
		*/
		eINTERNAL_CONTACTS_ARE_FLIPPED	= (1<<5)
	};
};

/**
\brief Bitfield that contains a set of raised flags defined in PxContactPairFlag.

@see PxContactPairFlag
*/
typedef PxFlags<PxContactPairFlag::Enum, PxU16> PxContactPairFlags;
PX_FLAGS_OPERATORS(PxContactPairFlag::Enum, PxU16)


/**
\brief A contact point as used by contact notification
*/
struct PxContactPairPoint
{
	/**
	\brief �� ���� ���� ���� ��ġ. ���� ���� ����.
	*/
	PxVec3	position;

	/**
	\brief ���������� ���� ���� �и�. ���� �и��� ħ���� ��Ÿ���ϴ�.
	*/
	PxReal	separation;

	/**
	\brief ���������� ���� ǥ���� ����. ���� ������ �� ��° ���󿡼� ù ��° �������� ���մϴ�.
	*/
	PxVec3	normal;

	/**
	\brief �������� ���� 0�� ǥ�� �ε���. �̸� ����Ͽ� ǥ�� ���縦 �ĺ��մϴ�.
	*/
	PxU32   internalFaceIndex0;

	/**
	\brief ���������� ���, ���� ���� ����. �ùķ��̼� Ÿ�� �������� ������ �� ���� ���� �� �ֽ��ϴ�.
	*/
	PxVec3	impulse;

	/**
	\brief �������� ���� 1�� ǥ�� �ε���. �̸� ����Ͽ� ǥ�� ���縦 �ĺ��մϴ�.
	*/
	PxU32   internalFaceIndex1;
};


/**
\brief ���� ���� �� ����.

�� Ŭ������ �ν��Ͻ��� PxSimulationEventCallback.onContact()�� ���޵˴ϴ�.
���� ��翡 ���� ���� ������ ��û�� ��� (����: #PxPairFlag),
�ش� ���� ������ �� ����ü�� ���� �����˴ϴ�.

@see PxSimulationEventCallback.onContact()
*/
struct PxContactPair
{
	public:
		PX_INLINE	PxContactPair() {}

	/**
	\brief �� �����ϴ� �� ����.

	\note	���� �����ʹ� ������ ������ ������ �� �ֽ��ϴ�. �̴� �� ���� #PxPairFlag::eNOTIFY_TOUCH_LOST
			�Ǵ� #PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST �̺�Ʈ�� ��û�Ǿ��� �� ���Ե� ���� �� �ϳ��� ������ ����Դϴ�.
			�� ��� #flags ����� Ȯ���Ͽ� �̰��� �ش�Ǵ��� Ȯ���ϼ���. ������ ���� ���� �����͸� ���������� ���ʽÿ�.
			������ ���� ���� �����ʹ� ������ ���� ������ �� �ִ� ����� ������ ������ ������Ʈ�ϱ� ���� �����˴ϴ�.

	@see PxShape
	*/
	PxShape*				shapes[2];

	/**
	\brief ���� ��ġ �����͸� �����ϴ� ����ó ��Ʈ���� ù ��° ��ġ ����� ����Ű�� ������.

	�� �����ʹ� ����ó ���� �� ���� ����ó ����Ʈ ������ ��û�� ��쿡�� ��ȿ�մϴ� (#PxPairFlag::eNOTIFY_CONTACT_POINTS ����).
	������ ���̾ƿ��� ���� ����� #extractContacts()�� ����ϼ���.
	*/
	const PxU8*				contactPatches;

	/**
	\brief ���� �����͸� �����ϴ� ����ó ��Ʈ���� ù ��° ����ó ������ ����Ű�� ������.

	�� �����ʹ� ����ó ���� �� ���� ����ó ����Ʈ ������ ��û�� ��쿡�� ��ȿ�մϴ� (#PxPairFlag::eNOTIFY_CONTACT_POINTS ����).
	������ ���̾ƿ��� ���� ����� #extractContacts()�� ����ϼ���.
	*/
	const PxU8*				contactPoints;

	/**
	\brief ����� ��� �����͸� �����ϴ� ����.

	�� �����ʹ� ����ó ���� �� ���� ����ó ����Ʈ ������ ��û�� ��쿡�� ��ȿ�մϴ� (#PxPairFlag::eNOTIFY_CONTACT_POINTS ����).
	������ ���̾ƿ��� ���� ����� #extractContacts()�� ����ϼ���.
	*/
	const PxReal*			contactImpulses;

	/**
	\brief ���Ե� �� ���۸� ������ ���� ��Ʈ���� ũ�� [����Ʈ]
	*/
	PxU32					requiredBufferSize;

	/**
	\brief ���� ��Ʈ���� ����� �������� ��
	*/
	PxU8					contactCount;

	/**
	\brief ���� ��Ʈ���� ����� ���� ��ġ�� ��
	*/
	PxU8					patchCount;

	/**
	\brief �� ���۸� ������ ���� ��Ʈ���� ũ�� [����Ʈ]
	*/
	PxU16					contactStreamSize;

	/**
	\brief ���� ���� �ֿ� ���� �߰� ����.

	@see PxContactPairFlag
	*/
	PxContactPairFlags		flags;

	/**
	\brief �������� ���� �÷���.

	�̺�Ʈ �ʵ�� ������ �����մϴ�:

	<ul>
	<li>PxPairFlag::eNOTIFY_TOUCH_FOUND,</li>
	<li>PxPairFlag::eNOTIFY_TOUCH_PERSISTS,</li>
	<li>PxPairFlag::eNOTIFY_TOUCH_LOST,</li>
	<li>PxPairFlag::eNOTIFY_TOUCH_CCD,</li>
	<li>PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND,</li>
	<li>PxPairFlag::eNOTIFY_THRESHOLD_FORCE_PERSISTS,</li>
	<li>PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST</li>
	</ul>

	������ ���� ������ #PxPairFlag�� ������ �����ϼ���.

	\note	eNOTIFY_TOUCH_CCD�� �ش� ���� �� �̺�Ʈ�� ��û���� �ʾҴ��� �߻��� �� �ֽ��ϴ�.
			�׷��� �� ��쿡�� �ٸ� �÷��׿� ���յǾ� �ٸ� �̺�Ʈ�� CCD �н� �߿� �߻������� ��Ÿ���ϴ�.

	@see PxPairFlag
	*/
	PxPairFlags				events;

	PxU32					internalData[2];	// For internal use only

	/**
	\brief ��Ʈ������ ����ó ������ �����ϰ� ���� �������� �����մϴ�.

	\param[out] userBuffer
				PxContactPairPoint ����ü�� �迭�� ����ó ������ �����մϴ�.
				�ִ� ����ó ���� #contactCount�� ���� ���ǵ˴ϴ�.
	
	\param[in]	bufferSize
				������ ���ۿ� ������ �� �ִ� PxContactPairPoint ����ü�� ���Դϴ�.
	
	\return		���ۿ� ��ϵ� ����ó ������ ���Դϴ�.

	@see PxContactPairPoint
	*/
	PX_INLINE PxU32			extractContacts(PxContactPairPoint* userBuffer, PxU32 bufferSize) const;

	/**
	\brief ����ó ���� �����ϰ� ����ó ������ ��Ʈ���� ����� ���۷� �����ϴ� ����� �޼����Դϴ�.

	����ó ������ ��Ʈ���� ����ó ���� �ݹ� ���ȿ��� �׼����� �� �ֽ��ϴ�.
	�� ����� �Լ��� ����ó ��Ʈ�� ������ ���߿� �׼����� �� �ֵ��� ����� ���ۿ� ���� ����� �����մϴ�.

	\param[out] newPair
				����ó �� ������ �� �ν��Ͻ��� ����˴ϴ�.
				���纻�� ����ó ������ ��Ʈ�� �����ʹ� ������ ����� ���۷� ���𷺼ǵ˴ϴ�.
				����ó �� ���� �۾��� �ǳ� �ٷ��� NULL�� ����ϼ���.
	
	\param[out] bufferMemory
				����ó ������ ��Ʈ���� ������ �޸� ����Դϴ�.
				�ִ� #requiredBufferSize ����Ʈ�� ���ۿ� ���� ���Դϴ�.
	*/
	PX_INLINE void				bufferContacts(PxContactPair* newPair, PxU8* bufferMemory) const;

	PX_INLINE const PxU32*		getInternalFaceIndices() const;
};


PX_INLINE PxU32 PxContactPair::extractContacts(PxContactPairPoint* userBuffer, PxU32 bufferSize) const
{
	PxU32 nbContacts = 0;

	if(contactCount && bufferSize)
	{
		PxContactStreamIterator iter(contactPatches, contactPoints, getInternalFaceIndices(), patchCount, contactCount);

		const PxReal* impulses = contactImpulses;

		const PxU32 flippedContacts = (flags & PxContactPairFlag::eINTERNAL_CONTACTS_ARE_FLIPPED);
		const PxU32 hasImpulses = (flags & PxContactPairFlag::eINTERNAL_HAS_IMPULSES);

		while(iter.hasNextPatch())
		{
			iter.nextPatch();
			while(iter.hasNextContact())
			{
				iter.nextContact();
				PxContactPairPoint& dst = userBuffer[nbContacts];
				dst.position = iter.getContactPoint();
				dst.separation = iter.getSeparation();
				dst.normal = iter.getContactNormal();
				if(!flippedContacts)
				{
					dst.internalFaceIndex0 = iter.getFaceIndex0();
					dst.internalFaceIndex1 = iter.getFaceIndex1();
				}
				else
				{
					dst.internalFaceIndex0 = iter.getFaceIndex1();
					dst.internalFaceIndex1 = iter.getFaceIndex0();
				}

				if(hasImpulses)
				{
					const PxReal impulse = impulses[nbContacts];
					dst.impulse = dst.normal * impulse;
				}
				else
					dst.impulse = PxVec3(0.0f);
				++nbContacts;
				if(nbContacts == bufferSize)
					return nbContacts;
			}
		}
	}

	return nbContacts;
}


PX_INLINE void PxContactPair::bufferContacts(PxContactPair* newPair, PxU8* bufferMemory) const
{
	PxU8* patches = bufferMemory;
	PxU8* contacts = NULL;
	if(patches)
	{
		contacts = bufferMemory + patchCount * sizeof(PxContactPatch);
		PxMemCopy(patches, contactPatches, sizeof(PxContactPatch)*patchCount);
		PxMemCopy(contacts, contactPoints, contactStreamSize - (sizeof(PxContactPatch)*patchCount));
	}

	if(contactImpulses)
	{
		PxMemCopy(bufferMemory + ((contactStreamSize + 15) & (~15)), contactImpulses, sizeof(PxReal) * contactCount);
	}

	if (newPair)
	{
		*newPair = *this;
		newPair->contactPatches = patches;
		newPair->contactPoints = contacts;
	}
}


PX_INLINE const PxU32* PxContactPair::getInternalFaceIndices() const
{
	return reinterpret_cast<const PxU32*>(contactImpulses + contactCount);
}

/**
\brief Collection of flags providing information on trigger report pairs.

@see PxTriggerPair
*/
struct PxTriggerPairFlag
{
	enum Enum
	{
		eREMOVED_SHAPE_TRIGGER					= (1<<0),					//!< Ʈ���� ����� ����/��鿡�� ���ŵǾ����ϴ�.
		eREMOVED_SHAPE_OTHER					= (1<<1),					//!< Ʈ���� �̺�Ʈ�� �߻���Ų ����� ����/��鿡�� ���ŵǾ����ϴ�.
		eNEXT_FREE								= (1<<2)					//!< �������� �뵵�θ� ���˴ϴ�.
	};
};

/**
\brief Bitfield that contains a set of raised flags defined in PxTriggerPairFlag.

@see PxTriggerPairFlag
*/
typedef PxFlags<PxTriggerPairFlag::Enum, PxU8> PxTriggerPairFlags;
PX_FLAGS_OPERATORS(PxTriggerPairFlag::Enum, PxU8)


/**
\brief Ʈ���� �� ���� �����Դϴ�.

�̷��� ����ü�� �迭�� PxSimulationEventCallback::onTrigger() ����Ʈ�� ���޵˴ϴ�.

\note	���� �����ʹ� ������ ������ ������ �� �ֽ��ϴ�.
		�� ���� #PxPairFlag::eNOTIFY_TOUCH_LOST �̺�Ʈ�� ��û�Ǿ���
		���õ� ���� �� �ϳ��� ������ ��� �̷��� ����Դϴ�.
		#flags ����� Ȯ���Ͽ� �ش� ���θ� Ȯ���Ͻʽÿ�.
		������ ���� ���� �����͸� �������� ���ʽÿ�.
		������ ���� ���� �����ʹ� �ش� ������ ���� ������ �� ��
		����� ������ ������ ������Ʈ�ϱ� ���� �����˴ϴ�.

@see PxSimulationEventCallback.onTrigger()
*/
struct PxTriggerPair
{
	PX_INLINE PxTriggerPair() {}

	PxShape*				triggerShape;		//!< Ʈ���ŷ� ǥ�õ� �����Դϴ�.
	PxActor*				triggerActor;		//!< triggerShape�� ����� �����Դϴ�.
	PxShape*				otherShape;			//!< Ʈ���� �̺�Ʈ�� �����ϴ� �����Դϴ�. \deprecated (����: #PxSimulationEventCallback::onTrigger()) Ʈ���� ���� �� �浹�� Ȱ��ȭ�� ���, �� ����� Ʈ���� ������ ����ų ���� �ֽ��ϴ�.
	PxActor*				otherActor;			//!< otherShape�� ����� �����Դϴ�.
	PxPairFlag::Enum		status;				//!< Ʈ���� �̺�Ʈ�� ���� (eNOTIFY_TOUCH_FOUND �Ǵ� eNOTIFY_TOUCH_LOST). eNOTIFY_TOUCH_PERSISTS �̺�Ʈ�� �������� �ʽ��ϴ�.
	PxTriggerPairFlags		flags;				//!< �� ���� �߰� ���� (����: #PxTriggerPairFlag)
};


/**
\brief �ջ�� ���� ���ǿ� ���� �������Դϴ�.

�� ����ü�� �迭�� PxSimulationEventCallback::onConstraintBreak() ������ ���޵˴ϴ�.

@see PxConstraint PxSimulationEventCallback.onConstraintBreak()
*/
struct PxConstraintInfo
{
	PX_INLINE PxConstraintInfo() {}
	PX_INLINE PxConstraintInfo(PxConstraint* c, void* extRef, PxU32 t) : constraint(c), externalReference(extRef), type(t) {}

	PxConstraint*	constraint;				//!< �ջ�� ���������Դϴ�.
	void*			externalReference;      //!< ���������� �����ϴ� �ܺ� ��ü�Դϴ� (����: #PxConstraintConnector::getExternalReference()).
	PxU32			type;                   //!< �ܺ� ��ü�� ������ ���� �ĺ����Դϴ�. ������ �ܺ� ������ ������ �������� ĳ��Ʈ�� �� �ֵ��� �մϴ�.
};


/**
\brief �ùķ��̼� �̺�Ʈ�� �����Ϸ��� ����ڰ� ������ �� �ִ� �������̽� Ŭ�����Դϴ�.

onAdvance()�� ������ �� �̺�Ʈ���� #PxScene::fetchResults() �Ǵ� #PxScene::flushSimulation() �� �ϳ��� ȣ���� ��
sendPendingReports=true�� ���۵˴ϴ�.
onAdvance()�� �ùķ��̼��� ����Ǵ� ����(PxScene::simulate() �Ǵ� PxScene::advance()�� PxScene::fetchResults() ����) ȣ��˴ϴ�.

\note	SDK ���´� �ݹ� ������ �����Ǿ�� �� �˴ϴ�.
		Ư�� ��ü�� �����ϰų� �ı��ؼ��� �� �˴ϴ�.
		���� ������ �ʿ��� ��� ���� ������ ���ۿ� �����ϰ� �ùķ��̼� �ܰ� ���Ŀ� �����ؾ� �մϴ�.

<b>������:</b>		onAdvance()�� �����ϰ�� �̷��� �ݹ��� ������ �����ϰ� ���� �ʿ䰡 �����ϴ�.
					�ֳ��ϸ� �̷��� �ݹ��� ����� �������� �ƶ������� ȣ��Ǳ� �����Դϴ�.

@see PxScene.setSimulationEventCallback() PxScene.getSimulationEventCallback()
*/
class PxSimulationEventCallback
	{
	public:
	/**
	\brief �ı� ������ ���� ������ �ı��� �� ȣ��˴ϴ�.

	\note	�� ȣ�� ������ ���� ���̴��� �����ؼ��� �� �˴ϴ�!

	\note	���� ������ �ı������� �ð� �ܰ谡 ���� �ùķ��̼� ���� �� ���� ������ �����Ǹ� �̺�Ʈ�� ������� �ʽ��ϴ�.

	\param[in]	constraints
				�ı��� ���� ���ǵ��Դϴ�.
	
	\param[in]	count
				���� ������ ��

	@see PxConstraint PxConstraintDesc.linearBreakForce PxConstraintDesc.angularBreakForce
	*/
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) = 0;


	/**
	\brief ��� ��� ���͵鿡 ���� ȣ��˴ϴ�.

	\note	����� ��ü �ٵ� ���ؼ��� �����˴ϴ�.
	\note	eSEND_SLEEP_NOTIFIES �÷��װ� ������ ���Ϳ� ���ؼ��� ȣ��˴ϴ�.
	\note	���� �������� fetchResults()�� ���� �������� fetchResults() ���̿��� �߻��� �ֽ� ���� ���� ���̸� ����˴ϴ�.
			���� ��� ���� A�� ���� �ְ�, A->putToSleep()�� ȣ��ǰ�, ���߿� A->wakeUp()�� ȣ��Ǹ�
			���� �ùķ���Ʈ/fetchResults() �ܰ迡���� ������ �����̱� ������ onWake() �̺�Ʈ�� Ʈ���ŵ˴ϴ�.

	\note	���Ͱ� ����� �Ǵ� �ùķ��̼ǿ� ���� ������� �ʰ� ���� ���� ���·� ���� ���� �߰��� ��쿡��
			���� �ùķ���Ʈ/fetchResults() �ܰ迡�� onWake() �̺�Ʈ�� ���۵˴ϴ�.

	\param[in]	actors
				��� ��� ���͵��Դϴ�.
	
	\param[in]	count
				������ ��

	@see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback PxActorFlag PxActor.setActorFlag()
	*/
	virtual void onWake(PxActor** actors, PxU32 count) = 0;


	/**
	\brief ��� ���ڰ� �ִ� ���͵鿡 ���� ȣ��˴ϴ�.

	\note	����� ��ü �ٵ� ���ؼ��� �����˴ϴ�.
	\note	eSEND_SLEEP_NOTIFIES �÷��װ� ������ ���Ϳ� ���ؼ��� ȣ��˴ϴ�.
	
	\note	���� �������� fetchResults()�� ���� �������� fetchResults() ���̿��� �߻��� �ֽ� ���� ���� ���̸� ����˴ϴ�.
			���� ��� ���� A�� ���ڰ� �ְ�, A->wakeUp()�� ȣ��ǰ�, ���߿� A->putToSleep()�� ȣ��Ǹ�
			���� �ùķ���Ʈ/fetchResults() �ܰ迡���� ������ �����̱� ������ onSleep() �̺�Ʈ�� Ʈ���ŵ˴ϴ� (�ùķ��̼��� ���͸� ������ ���� ���).
	
	\note	���Ͱ� ����� �Ǵ� �ùķ��̼ǿ� ���� ������� �ʰ� ���ڰ� ���� ���·� ���� ���� �߰��� ��쿡��
			���� �ùķ���Ʈ/fetchResults() �ܰ迡�� onSleep() �̺�Ʈ�� ���۵˴ϴ�.

	\param[in]	actors
				��� ���ڰ� �ִ� ���͵��Դϴ�.
	
	\param[in]	count
				������ ��

	@see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback PxActorFlag PxActor.setActorFlag()
	*/
	virtual void onSleep(PxActor** actors, PxU32 count) = 0;


	/** \brief Ư���� ���� �̺�Ʈ�� �߻��� �� ȣ��˴ϴ�.

	�� �޼���� �浹 ���� �� �� �ϳ��� ���� �˸��� ��û�� ��쿡 �� ������ �ֿ� ���� ȣ��˴ϴ�.
	� �̺�Ʈ�� ����Ǵ����� ���� ���̴�/�ݹ� ��Ŀ������ ����Ͽ� ��û�ϸ�
	(#PxSimulationFilterShader, #PxSimulationFilterCallback, #PxPairFlag ����),
	����� ��ü�� ���� ������ �������� ���ʽÿ�. �� �Լ��� ��ȯ�� �Ŀ��� ��ȿȭ�� ���Դϴ�.

	\param[in]	pairHeader
				���� ���� Ʈ������ �� ���Ϳ� ���� �����Դϴ�.
	
	\param[in]	pairs
				���� ���� ��û�� �� ������ ���� ���Դϴ�. ����: #PxContactPair.
	
	\param[in]	nbPairs
				������ ���� ���� ���Դϴ�.

	@see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback PxContactPair PxPairFlag PxSimulationFilterShader PxSimulationFilterCallback
	*/
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) = 0;


	/**
	\brief ���� Ʈ���� �� �̺�Ʈ�� �Բ� ȣ��˴ϴ�.

	PxShapeFlag::eTRIGGER_SHAPE�� ����Ͽ� Ʈ���ŷ� ǥ�õ� ������ ���� ���̴����� ��õ� �� �÷��� �԰ݿ� ���� �̺�Ʈ�� �����մϴ�
	(#PxPairFlag, #PxSimulationFilterShader ����).

	\note	Ʈ���� ������ �� �̻� �ٸ� Ʈ���� ������� ��ȣ �ۿ뿡 ���� �˸� �̺�Ʈ�� ������ �ʽ��ϴ�.

	\param[in]	pairs
				Ʈ���� �� �̺�Ʈ�Դϴ�.
	
	\param[in]	count
				Ʈ���� �� �̺�Ʈ�� ���Դϴ�.

	@see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback PxPairFlag PxSimulationFilterShader PxShapeFlag PxShape.setFlag()
	*/
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) = 0;

	/**
	\brief �̵� ���� ��ü �ٵ��� ���ο� ��� ������ ������ �� �ֵ��� �����մϴ�.

	�� ȣ���� �߻��ϸ� #PxRigidBodyFlag::eENABLE_POSE_INTEGRATION_PREVIEW �÷��װ� ������ ��ü �ٵ���� �ùķ��̼ǿ� ���� �̵��Ǿ�����,
	�׵��� ���ο� ��� ������ ���۸� ���� ������ �� �ֽ��ϴ�.

	\note	������ ���۴� ���� #PxScene::simulate() �Ǵ� #PxScene::collide() ȣ����� ��ȿ�ϸ� ���� �� �ֽ��ϴ�.

	\note	�� �ݹ��� �ùķ��̼��� ����Ǵ� ���� Ʈ���ŵ˴ϴ�.
			��ü�� �Ӽ��� �б� ���� ������ ��ü �ٵ� ������ ����ϴ� ��� �ݹ��� ���ÿ� ������ ��ü�� ���Ⱑ ������ �����ؾ� �մϴ�.

	\note	�� �ݹ��� �ڵ�� �淮�̾�� �մϴ�. ��, #PxScene::fetchResults() ȣ���� ������ �� �ֽ��ϴ�.

	\param[in]	bodyBuffer
				�̵��ϰ� �̸� ���� ���� ��û�� ��ü �ٵ���Դϴ�.
	
	\param[in]	poseBuffer
				bodyBuffer�� ������ ��ü �ٵ���� ���յ� ��ü �ٵ� �����Դϴ�.
	
	\param[in]	count
				������ ������ �׸� ���Դϴ�.

	@see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback PxRigidBodyFlag::eENABLE_POSE_INTEGRATION_PREVIEW
	*/
	virtual void onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) = 0;

	virtual ~PxSimulationEventCallback() {}
	};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
