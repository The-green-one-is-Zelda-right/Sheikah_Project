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
	\brief 이터레이터를 다음 일련의 추가 데이터 항목으로 이동시킵니다.

	contact pair extra data 스트림에는 해당 #PxPairFlag 플래그에 따라 요청된 항목 세트가 포함되어 있습니다.
	#PxPairFlag::ePRE_SOLVER_VELOCITY, #PxPairFlag::ePOST_SOLVER_VELOCITY, #PxPairFlag::eCONTACT_EVENT_POSE.
	한 세트에는 각각 하나의 항목과 PxContactPairIndex 항목이 포함될 수 있습니다. 이 메서드는 스트림을 파싱하고 이터레이터의 멤버 변수를 현재 세트의 해당 항목으로 설정합니다 (항목이 사용 가능한 경우).
	CCD가 비활성화된 경우에는 하나의 항목 세트만 얻어야 합니다. CCD가 여러 패스로 활성화된 경우에는 하나 이상의 항목 세트를 얻을 수 있습니다.

	\note	비록 contact pair extra data가 모양 쌍 당으로 요청되지만,
			모양 쌍 당 항목 세트를 얻지 못하고 대신에 액터 쌍 당으로 한 번만 얻게 됩니다.
			예를 들어, 
			액터에 두 개의 모양이 있고 둘 다 다른 액터와 충돌하는 경우, 
			항목 세트는 하나만 있을 것입니다 (두 모양 쌍에 모두 적용되기 때문).

	\return 스트림에 더 많은 추가 데이터 항목 세트가 있는 경우 true, 그렇지 않으면 false.

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
\brief 이 클래스의 인스턴스가 PxSimulationEventCallback.onContact()에 전달됩니다.

@see PxSimulationEventCallback.onContact()
*/
struct PxContactPairHeader
{
	public:
		PX_INLINE	PxContactPairHeader() {}

	/**
	\brief	알림 모양 쌍의 두 액터입니다.

	\note	액터 포인터는 삭제된 액터를 참조할 수 있습니다.
			이는 PxPairFlag::eNOTIFY_TOUCH_LOST
			또는 PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST 이벤트가
			쌍에 대해 요청되었으며 관련된 액터 중 하나가 삭제되거나 씬에서 제거된 경우에 발생합니다.
			이 경우인지 여부를 확인하려면 #flags 멤버를 확인하십시오. 삭제된 액터에 대한 포인터를 역참조하지 마십시오.
			삭제된 액터에 대한 포인터는 포인터 값에 의존할 수 있는 사용자 데이터 구조를 업데이트할 수 있도록 제공됩니다.

	@see PxActor
	*/
	PxActor*					actors[2];

	/**
	\brief 시뮬레이션 필터의 PxPairFlag 플래그에서 요청한 대로 추가 데이터를 포함하는 스트림입니다.

	이 포인터는 연락처 보고 쌍에 대한 어떤 종류의 추가 데이터 정보도 요청되지 않은 경우에만 유효합니다
	(예: #PxPairFlag::ePOST_SOLVER_VELOCITY 등), 그렇지 않으면 NULL일 것입니다.

	@see PxPairFlag
	*/
	const PxU8*					extraDataStream;
	
	/**
	\brief 추가 데이터 스트림의 크기 [바이트]
	*/
	PxU16						extraDataStreamSize;

	/**
	\brief 접촉 보고 쌍에 대한 추가 정보입니다.

	@see PxContactPairHeaderFlag
	*/
	PxContactPairHeaderFlags	flags;

	/**
	\brief 접촉 쌍에 대한 포인터입니다.
	*/
	const struct PxContactPair*	pairs;

	/**
	\brief 접촉 쌍의 수입니다.
	*/
	PxU32						nbPairs;
};


/**
\brief 연락처 보고서 쌍에 대한 정보를 제공하는 플래그 모음입니다.

@see PxContactPair
*/
struct PxContactPairFlag
{
	enum Enum
	{
		/**
		\brief 인덱스가 0인 도형은 액터/씬에서 제거되었습니다.
		*/
		eREMOVED_SHAPE_0				= (1<<0),

		/**
		\brief 인덱스가 1인 모양이 배우/씬에서 제거되었습니다.
		*/
		eREMOVED_SHAPE_1				= (1<<1),

		/**
		\brief 첫 번째 액터 쌍 접촉.

		제공된 셰이프 쌍은 두 액터 간의 첫 번째 접촉을 표시하며, 현재 시뮬레이션 프레임 이전에 다른 셰이프 쌍이 접촉한 적이 없습니다.

		\note: 이 정보는 해당 쌍에 대해 #PxPairFlag::eNOTIFY_TOUCH_FOUND가 선언된 경우에만 사용할 수 있습니다.
		*/
		eACTOR_PAIR_HAS_FIRST_TOUCH		= (1<<2),

		/**
		\brief 액터 쌍 간의 모든 접촉이 손실되었습니다.

		두 액터 간의 모든 접촉이 끊어졌으며 현재 시뮬레이션 프레임 이후에는 어떤 셰이프 쌍도 접촉하지 않습니다.
		*/
		eACTOR_PAIR_LOST_TOUCH			= (1<<3),

		/**
		\brief 내부 플래그, #PxContactPair.extractContacts()에 사용됩니다.

		적용된 접촉 임펄스는 모든 접촉점에 대해 제공됩니다. 
		페어에 #PxPairFlag::eSOLVE_CONTACT가 설정된 경우에 해당합니다.
		*/
		eINTERNAL_HAS_IMPULSES			= (1<<4),

		/**
		\brief 내부 플래그, #PxContactPair.extractContacts()에 사용됩니다.

		제공된 접점 정보는 접점 쌍의 모양과 관련하여 반전됩니다. 이는 주로 내부 삼각형 인덱스의 순서와 관련이 있습니다.ggg
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
	\brief 두 형상 간의 접점 위치. 월드 공간 기준.
	*/
	PxVec3	position;

	/**
	\brief 접점에서의 형상 간의 분리. 음의 분리는 침투를 나타냅니다.
	*/
	PxReal	separation;

	/**
	\brief 접점에서의 연락 표면의 법선. 법선 방향은 두 번째 형상에서 첫 번째 형상으로 향합니다.
	*/
	PxVec3	normal;

	/**
	\brief 접점에서 형상 0의 표면 인덱스. 이를 사용하여 표면 소재를 식별합니다.
	*/
	PxU32   internalFaceIndex0;

	/**
	\brief 접점에서의 충격, 월드 공간 기준. 시뮬레이션 타임 스텝으로 나누어 힘 값을 얻을 수 있습니다.
	*/
	PxVec3	impulse;

	/**
	\brief 접점에서 형상 1의 표면 인덱스. 이를 사용하여 표면 소재를 식별합니다.
	*/
	PxU32   internalFaceIndex1;
};


/**
\brief 연락 보고서 쌍 정보.

이 클래스의 인스턴스는 PxSimulationEventCallback.onContact()에 전달됩니다.
쌍의 모양에 대한 연락 보고서가 요청된 경우 (참조: #PxPairFlag),
해당 연락 정보는 이 구조체를 통해 제공됩니다.

@see PxSimulationEventCallback.onContact()
*/
struct PxContactPair
{
	public:
		PX_INLINE	PxContactPair() {}

	/**
	\brief 페어를 구성하는 두 형상.

	\note	형상 포인터는 삭제된 형상을 참조할 수 있습니다. 이는 페어에 대해 #PxPairFlag::eNOTIFY_TOUCH_LOST
			또는 #PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST 이벤트가 요청되었고 페어에 포함된 형상 중 하나가 삭제된 경우입니다.
			이 경우 #flags 멤버를 확인하여 이것이 해당되는지 확인하세요. 삭제된 형상에 대한 포인터를 역참조하지 마십시오.
			삭제된 형상에 대한 포인터는 포인터 값에 의존할 수 있는 사용자 데이터 구조를 업데이트하기 위해 제공됩니다.

	@see PxShape
	*/
	PxShape*				shapes[2];

	/**
	\brief 접촉 패치 데이터를 포함하는 연락처 스트림의 첫 번째 패치 헤더를 가리키는 포인터.

	이 포인터는 연락처 보고서 페어에 대한 연락처 포인트 정보가 요청된 경우에만 유효합니다 (#PxPairFlag::eNOTIFY_CONTACT_POINTS 참조).
	데이터 레이아웃에 대한 참고로 #extractContacts()를 사용하세요.
	*/
	const PxU8*				contactPatches;

	/**
	\brief 접점 데이터를 포함하는 연락처 스트림의 첫 번째 연락처 지점을 가리키는 포인터.

	이 포인터는 연락처 보고서 페어에 대한 연락처 포인트 정보가 요청된 경우에만 유효합니다 (#PxPairFlag::eNOTIFY_CONTACT_POINTS 참조).
	데이터 레이아웃에 대한 참고로 #extractContacts()를 사용하세요.
	*/
	const PxU8*				contactPoints;

	/**
	\brief 적용된 충격 데이터를 포함하는 버퍼.

	이 포인터는 연락처 보고서 페어에 대한 연락처 포인트 정보가 요청된 경우에만 유효합니다 (#PxPairFlag::eNOTIFY_CONTACT_POINTS 참조).
	데이터 레이아웃에 대한 참고로 #extractContacts()를 사용하세요.
	*/
	const PxReal*			contactImpulses;

	/**
	\brief 포함된 힘 버퍼를 포함한 연락 스트림의 크기 [바이트]
	*/
	PxU32					requiredBufferSize;

	/**
	\brief 연락 스트림에 저장된 연락점의 수
	*/
	PxU8					contactCount;

	/**
	\brief 연락 스트림에 저장된 연락 패치의 수
	*/
	PxU8					patchCount;

	/**
	\brief 힘 버퍼를 제외한 연락 스트림의 크기 [바이트]
	*/
	PxU16					contactStreamSize;

	/**
	\brief 연락 보고서 쌍에 대한 추가 정보.

	@see PxContactPairFlag
	*/
	PxContactPairFlags		flags;

	/**
	\brief 연락으로 인한 플래그.

	이벤트 필드는 다음을 포함합니다:

	<ul>
	<li>PxPairFlag::eNOTIFY_TOUCH_FOUND,</li>
	<li>PxPairFlag::eNOTIFY_TOUCH_PERSISTS,</li>
	<li>PxPairFlag::eNOTIFY_TOUCH_LOST,</li>
	<li>PxPairFlag::eNOTIFY_TOUCH_CCD,</li>
	<li>PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND,</li>
	<li>PxPairFlag::eNOTIFY_THRESHOLD_FORCE_PERSISTS,</li>
	<li>PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST</li>
	</ul>

	각각에 대한 설명은 #PxPairFlag의 문서를 참조하세요.

	\note	eNOTIFY_TOUCH_CCD는 해당 쌍이 이 이벤트를 요청하지 않았더라도 발생할 수 있습니다.
			그러나 이 경우에는 다른 플래그와 결합되어 다른 이벤트가 CCD 패스 중에 발생했음을 나타냅니다.

	@see PxPairFlag
	*/
	PxPairFlags				events;

	PxU32					internalData[2];	// For internal use only

	/**
	\brief 스트림에서 연락처 지점을 추출하고 편리한 형식으로 저장합니다.

	\param[out] userBuffer
				PxContactPairPoint 구조체의 배열로 연락처 지점을 추출합니다.
				쌍당 연락처 수는 #contactCount에 의해 정의됩니다.
	
	\param[in]	bufferSize
				제공된 버퍼에 저장할 수 있는 PxContactPairPoint 구조체의 수입니다.
	
	\return		버퍼에 기록된 연락처 지점의 수입니다.

	@see PxContactPairPoint
	*/
	PX_INLINE PxU32			extractContacts(PxContactPairPoint* userBuffer, PxU32 bufferSize) const;

	/**
	\brief 연락처 쌍을 복제하고 연락처 데이터 스트림을 사용자 버퍼로 복사하는 도우미 메서드입니다.

	연락처 데이터 스트림은 연락처 보고서 콜백 동안에만 액세스할 수 있습니다.
	이 도우미 함수는 연락처 스트림 정보를 나중에 액세스할 수 있도록 사용자 버퍼에 복사 기능을 제공합니다.

	\param[out] newPair
				연락처 쌍 정보가 이 인스턴스에 복사됩니다.
				복사본의 연락처 데이터 스트림 포인터는 제공된 사용자 버퍼로 리디렉션됩니다.
				연락처 쌍 복사 작업을 건너 뛰려면 NULL을 사용하세요.
	
	\param[out] bufferMemory
				연락처 데이터 스트림을 저장할 메모리 블록입니다.
				최대 #requiredBufferSize 바이트가 버퍼에 쓰일 것입니다.
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
		eREMOVED_SHAPE_TRIGGER					= (1<<0),					//!< 트리거 모양이 액터/장면에서 제거되었습니다.
		eREMOVED_SHAPE_OTHER					= (1<<1),					//!< 트리거 이벤트를 발생시킨 모양이 액터/장면에서 제거되었습니다.
		eNEXT_FREE								= (1<<2)					//!< 내부적인 용도로만 사용됩니다.
	};
};

/**
\brief Bitfield that contains a set of raised flags defined in PxTriggerPairFlag.

@see PxTriggerPairFlag
*/
typedef PxFlags<PxTriggerPairFlag::Enum, PxU8> PxTriggerPairFlags;
PX_FLAGS_OPERATORS(PxTriggerPairFlag::Enum, PxU8)


/**
\brief 트리거 페어에 대한 설명서입니다.

이러한 구조체의 배열이 PxSimulationEventCallback::onTrigger() 리포트로 전달됩니다.

\note	형상 포인터는 삭제된 형상을 참조할 수 있습니다.
		페어에 대한 #PxPairFlag::eNOTIFY_TOUCH_LOST 이벤트가 요청되었고
		관련된 형상 중 하나가 삭제된 경우 이러한 경우입니다.
		#flags 멤버를 확인하여 해당 여부를 확인하십시오.
		삭제된 형상에 대한 포인터를 참조하지 마십시오.
		삭제된 형상에 대한 포인터는 해당 포인터 값에 의존할 수 있
		사용자 데이터 구조를 업데이트하기 위해 제공됩니다.

@see PxSimulationEventCallback.onTrigger()
*/
struct PxTriggerPair
{
	PX_INLINE PxTriggerPair() {}

	PxShape*				triggerShape;		//!< 트리거로 표시된 형상입니다.
	PxActor*				triggerActor;		//!< triggerShape이 연결된 액터입니다.
	PxShape*				otherShape;			//!< 트리거 이벤트를 유발하는 형상입니다. \deprecated (참조: #PxSimulationEventCallback::onTrigger()) 트리거 형상 간 충돌이 활성화된 경우, 이 멤버는 트리거 형상을 가리킬 수도 있습니다.
	PxActor*				otherActor;			//!< otherShape이 연결된 액터입니다.
	PxPairFlag::Enum		status;				//!< 트리거 이벤트의 유형 (eNOTIFY_TOUCH_FOUND 또는 eNOTIFY_TOUCH_LOST). eNOTIFY_TOUCH_PERSISTS 이벤트는 지원되지 않습니다.
	PxTriggerPairFlags		flags;				//!< 페어에 대한 추가 정보 (참조: #PxTriggerPairFlag)
};


/**
\brief 손상된 제약 조건에 대한 설명자입니다.

이 구조체의 배열은 PxSimulationEventCallback::onConstraintBreak() 보고서에 전달됩니다.

@see PxConstraint PxSimulationEventCallback.onConstraintBreak()
*/
struct PxConstraintInfo
{
	PX_INLINE PxConstraintInfo() {}
	PX_INLINE PxConstraintInfo(PxConstraint* c, void* extRef, PxU32 t) : constraint(c), externalReference(extRef), type(t) {}

	PxConstraint*	constraint;				//!< 손상된 제약조건입니다.
	void*			externalReference;      //!< 제약조건을 소유하는 외부 객체입니다 (참조: #PxConstraintConnector::getExternalReference()).
	PxU32			type;                   //!< 외부 객체의 고유한 유형 식별자입니다. 제공된 외부 참조를 적절한 형식으로 캐스트할 수 있도록 합니다.
};


/**
\brief 시뮬레이션 이벤트를 수신하려면 사용자가 구현할 수 있는 인터페이스 클래스입니다.

onAdvance()를 제외한 이 이벤트들은 #PxScene::fetchResults() 또는 #PxScene::flushSimulation() 중 하나를 호출할 때
sendPendingReports=true로 전송됩니다.
onAdvance()는 시뮬레이션이 실행되는 동안(PxScene::simulate() 또는 PxScene::advance()와 PxScene::fetchResults() 사이) 호출됩니다.

\note	SDK 상태는 콜백 내에서 수정되어서는 안 됩니다.
		특히 객체를 생성하거나 파괴해서는 안 됩니다.
		상태 수정이 필요한 경우 변경 사항을 버퍼에 저장하고 시뮬레이션 단계 이후에 수행해야 합니다.

<b>쓰레딩:</b>		onAdvance()를 제외하고는 이러한 콜백을 스레드 안전하게 만들 필요가 없습니다.
					왜냐하면 이러한 콜백은 사용자 스레드의 맥락에서만 호출되기 때문입니다.

@see PxScene.setSimulationEventCallback() PxScene.getSimulationEventCallback()
*/
class PxSimulationEventCallback
	{
	public:
	/**
	\brief 파괴 가능한 제약 조건이 파괴될 때 호출됩니다.

	\note	이 호출 내에서 제약 쉐이더를 해제해서는 안 됩니다!

	\note	제약 조건이 파괴되지만 시간 단계가 아직 시뮬레이션 중일 때 제약 조건이 삭제되면 이벤트가 보고되지 않습니다.

	\param[in]	constraints
				파괴된 제약 조건들입니다.
	
	\param[in]	count
				제약 조건의 수

	@see PxConstraint PxConstraintDesc.linearBreakForce PxConstraintDesc.angularBreakForce
	*/
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) = 0;


	/**
	\brief 방금 깨어난 액터들에 대해 호출됩니다.

	\note	현재는 강체 바디에 대해서만 지원됩니다.
	\note	eSEND_SLEEP_NOTIFIES 플래그가 설정된 액터에 대해서만 호출됩니다.
	\note	이전 프레임의 fetchResults()와 현재 프레임의 fetchResults() 사이에서 발생한 최신 슬립 상태 전이만 보고됩니다.
			예를 들어 액터 A가 깨어 있고, A->putToSleep()이 호출되고, 나중에 A->wakeUp()이 호출되면
			다음 시뮬레이트/fetchResults() 단계에서는 마지막 전이이기 때문에 onWake() 이벤트만 트리거됩니다.

	\note	액터가 사용자 또는 시뮬레이션에 의해 변경되지 않고 깨어 있음 상태로 씬에 새로 추가된 경우에는
			다음 시뮬레이트/fetchResults() 단계에서 onWake() 이벤트가 전송됩니다.

	\param[in]	actors
				방금 깨어난 액터들입니다.
	
	\param[in]	count
				액터의 수

	@see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback PxActorFlag PxActor.setActorFlag()
	*/
	virtual void onWake(PxActor** actors, PxU32 count) = 0;


	/**
	\brief 방금 잠자고 있는 액터들에 대해 호출됩니다.

	\note	현재는 강체 바디에 대해서만 지원됩니다.
	\note	eSEND_SLEEP_NOTIFIES 플래그가 설정된 액터에 대해서만 호출됩니다.
	
	\note	이전 프레임의 fetchResults()와 현재 프레임의 fetchResults() 사이에서 발생한 최신 슬립 상태 전이만 보고됩니다.
			예를 들어 액터 A가 잠자고 있고, A->wakeUp()이 호출되고, 나중에 A->putToSleep()이 호출되면
			다음 시뮬레이트/fetchResults() 단계에서는 마지막 전이이기 때문에 onSleep() 이벤트만 트리거됩니다 (시뮬레이션이 액터를 깨우지 않은 경우).
	
	\note	액터가 사용자 또는 시뮬레이션에 의해 변경되지 않고 잠자고 있음 상태로 씬에 새로 추가된 경우에는
			다음 시뮬레이트/fetchResults() 단계에서 onSleep() 이벤트가 전송됩니다.

	\param[in]	actors
				방금 잠자고 있는 액터들입니다.
	
	\param[in]	count
				액터의 수

	@see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback PxActorFlag PxActor.setActorFlag()
	*/
	virtual void onSleep(PxActor** actors, PxU32 count) = 0;


	/** \brief 특정한 접촉 이벤트가 발생할 때 호출됩니다.

	이 메서드는 충돌 형상 쌍 중 하나가 접촉 알림을 요청한 경우에 두 액터의 쌍에 대해 호출됩니다.
	어떤 이벤트가 보고되는지는 필터 쉐이더/콜백 메커니즘을 사용하여 요청하며
	(#PxSimulationFilterShader, #PxSimulationFilterCallback, #PxPairFlag 참조),
	통과된 객체에 대한 참조를 유지하지 마십시오. 이 함수가 반환된 후에는 무효화될 것입니다.

	\param[in]	pairHeader
				접촉 보고를 트리거한 두 액터에 대한 정보입니다.
	
	\param[in]	pairs
				접촉 보고가 요청된 두 액터의 접촉 쌍입니다. 참조: #PxContactPair.
	
	\param[in]	nbPairs
				제공된 접촉 쌍의 수입니다.

	@see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback PxContactPair PxPairFlag PxSimulationFilterShader PxSimulationFilterCallback
	*/
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) = 0;


	/**
	\brief 현재 트리거 쌍 이벤트와 함께 호출됩니다.

	PxShapeFlag::eTRIGGER_SHAPE를 사용하여 트리거로 표시된 형상은 필터 쉐이더에서 명시된 쌍 플래그 규격에 따라 이벤트를 전송합니다
	(#PxPairFlag, #PxSimulationFilterShader 참조).

	\note	트리거 형상은 더 이상 다른 트리거 형상과의 상호 작용에 대한 알림 이벤트를 보내지 않습니다.

	\param[in]	pairs
				트리거 쌍 이벤트입니다.
	
	\param[in]	count
				트리거 쌍 이벤트의 수입니다.

	@see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback PxPairFlag PxSimulationFilterShader PxShapeFlag PxShape.setFlag()
	*/
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) = 0;

	/**
	\brief 이동 중인 강체 바디의 새로운 포즈에 빠르게 접근할 수 있도록 제공합니다.

	이 호출이 발생하면 #PxRigidBodyFlag::eENABLE_POSE_INTEGRATION_PREVIEW 플래그가 설정된 강체 바디들은 시뮬레이션에 의해 이동되었으며,
	그들의 새로운 포즈에 제공된 버퍼를 통해 접근할 수 있습니다.

	\note	제공된 버퍼는 다음 #PxScene::simulate() 또는 #PxScene::collide() 호출까지 유효하며 읽을 수 있습니다.

	\note	이 콜백은 시뮬레이션이 실행되는 동안 트리거됩니다.
			객체의 속성을 읽기 위해 제공된 강체 바디 참조를 사용하는 경우 콜백은 동시에 동일한 객체에 쓰기가 없음을 보장해야 합니다.

	\note	이 콜백의 코드는 경량이어야 합니다. 즉, #PxScene::fetchResults() 호출을 차단할 수 있습니다.

	\param[in]	bodyBuffer
				이동하고 이른 포즈 보고를 요청한 강체 바디들입니다.
	
	\param[in]	poseBuffer
				bodyBuffer에 나열된 강체 바디들의 통합된 강체 바디 포즈입니다.
	
	\param[in]	count
				제공된 버퍼의 항목 수입니다.

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
