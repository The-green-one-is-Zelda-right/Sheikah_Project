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
		\brief 동적 해결기에서 이 충돌 쌍의 연락을 처리합니다.

		\note	충돌한 액터가 강체인 경우에만 영향을 미칩니다.
		*/
		eSOLVE_CONTACT = (1 << 0),

		/**
		\brief 이 충돌 쌍에 대한 연락 수정 콜백을 호출합니다.
			
		\note	충돌한 액터가 강체인 경우에만 영향을 미칩니다.

		@see PxContactModifyCallback
		*/
		eMODIFY_CONTACTS = (1 << 1),

		/**
		\brief 이 충돌 쌍이 접촉을 시작할 때 연락 보고 콜백 또는 트리거 콜백을 호출합니다.

		두 충돌 객체 중 하나가 트리거 모양인 경우 (참조: #PxShapeFlag::eTRIGGER_SHAPE),
		다른 객체가 트리거 볼륨에 들어갈 때 즉시 트리거 콜백이 호출됩니다.
		두 충돌 객체가 트리거 모양이 아닌 경우, 이 충돌 쌍의 액터가 접촉을 시작할 때 연락 보고 콜백이 호출됩니다.

		\note	충돌한 액터가 강체인 경우에만 영향을 미칩니다.
		\note	eDETECT_DISCRETE_CONTACT 또는 eDETECT_CCD_CONTACT가 활성화된 경우에만 영향을 미칩니다.

		@see PxSimulationEventCallback.onContact() PxSimulationEventCallback.onTrigger()
		*/
		eNOTIFY_TOUCH_FOUND = (1 << 2),

		/**
		\brief 이 충돌 쌍이 접촉 중일 때 연락 보고 콜백을 호출합니다.
		ii
		두 충돌 객체가 트리거 모양이 아닌 경우, 이 충돌 쌍의 액터가 접촉 중일 때 연락 보고 콜백이 호출됩니다.

		\note	트리거는 이 이벤트를 지원하지 않습니다.
				지속적인 트리거 접촉은 eNOTIFY_TOUCH_FOUND/eNOTIFY_TOUCH_LOST 이벤트를 따로 관찰하여 추적되어야 합니다.
		\note	충돌한 액터가 강체인 경우에만 영향을 미칩니다.
		\note	접촉 중인 객체가 자고 있으면 보고가 전송되지 않습니다.
		\note	eDETECT_DISCRETE_CONTACT 또는 eDETECT_CCD_CONTACT가 활성화된 경우에만 영향을 미칩니다.
		\note	이미 접촉 중인 쌍에 대해이 플래그가 활성화되면, 쌍이 접촉을 잃고 다시 접촉할 때까지 eNOTIFY_TOUCH_PERSISTS 이벤트가 없을 것입니다.

		@see PxSimulationEventCallback.onContact() PxSimulationEventCallback.onTrigger()
		*/
		eNOTIFY_TOUCH_PERSISTS = (1 << 3),

		/**
		\brief 이 충돌 쌍이 접촉을 중단할 때 연락 보고 콜백이나 트리거 콜백을 호출합니다.

		두 충돌 객체 중 하나가 트리거 모양인 경우 (참조: #PxShapeFlag::eTRIGGER_SHAPE),
		다른 객체가 트리거 볼륨을 벗어날 때 즉시 트리거 콜백이 호출됩니다.
		두 충돌 객체가 트리거 모양이 아닌 경우, 이 충돌 쌍의 액터가 접촉을 중단할 때 연락 보고 콜백이 호출됩니다.

		\note	충돌한 액터가 강체인 경우에만 영향을 미칩니다.
		\note	이 이벤트는 충돌 객체 중 하나가 삭제되면 발생합니다.
		\note	eDETECT_DISCRETE_CONTACT 또는 eDETECT_CCD_CONTACT가 활성화된 경우에만 영향을 미칩니다.

		@see PxSimulationEventCallback.onContact() PxSimulationEventCallback.onTrigger()
		*/
		eNOTIFY_TOUCH_LOST = (1 << 4),

		/**
		\brief 이 충돌 쌍이 CCD 패스 동안 접촉 중일 때 연락 보고 콜백을 호출합니다.

		여러 패스를 사용한 CCD가 활성화된 경우, 빠르게 이동하는 객체가 동일한 객체에 여러 번 튀어올라갈 수 있습니다.
		따라서 동일한 쌍이 시뮬레이션 단계 동안 여러 번 접촉 중일 수 있습니다.
		이 플래그는 CCD 동안 모든 감지된 충돌을 보고할 것을 보장합니다. 성능상의 이유로 시스템은 항상 접촉 쌍이 이전의 CCD 패스 중 하나에서 접촉이 끊어졌는지 알 수 없으며, 따라서 새로운지 여부 또는 계속되는지 여부를 항상 알 수 없습니다. eNOTIFY_TOUCH_CCD는 충돌 객체가 CCD 패스 중에 서로 접촉으로 감지된 경우에만 보고합니다.

		\note 충돌한 액터가 강체인 경우에만 영향을 미칩니다.
		\note 트리거 모양은 지원되지 않습니다.
		\note eDETECT_CCD_CONTACT가 활성화된 경우에만 영향을 미칩니다.

		@see PxSimulationEventCallback.onContact() PxSimulationEventCallback.onTrigger()
		*/
		eNOTIFY_TOUCH_CCD = (1 << 5),

		/**
		\brief 이 충돌 쌍의 액터 간의 접촉 힘이 액터가 정의한 힘 임계값 중 하나를 초과할 때 연락 보고 콜백을 호출합니다.

		\note 충돌한 액터가 강체인 경우에만 영향을 미칩니다.
		\note eDETECT_DISCRETE_CONTACT 또는 eDETECT_CCD_CONTACT가 활성화된 경우에만 영향을 미칩니다.

		@see PxSimulationEventCallback.onContact()
		*/
		eNOTIFY_THRESHOLD_FORCE_FOUND = (1 << 6),

		/**
		\brief 이 충돌 쌍의 액터 간의 접촉 힘이 액터가 정의한 힘 임계값 중 하나를 계속 초과할 때 연락 보고 콜백을 호출합니다.

		\note 충돌한 액터가 강체인 경우에만 영향을 미칩니다.
		\note 쌍이 다시 필터링되고 이 플래그가 이전에 비활성화된 경우, 힘 임계값이 이전 프레임에서 초과되었더라도 동일한 프레임에서 보고가 발생하지 않을 것입니다 (이전 프레임에서 #eNOTIFY_THRESHOLD_FORCE_FOUND가 설정되었는지 여부에 관계없이).

		\note eDETECT_DISCRETE_CONTACT 또는 eDETECT_CCD_CONTACT가 활성화된 경우에만 영향을 미칩니다.

		@see PxSimulationEventCallback.onContact()
		*/
		eNOTIFY_THRESHOLD_FORCE_PERSISTS = (1 << 7),

		/**
		\brief 이 충돌 쌍의 액터 간의 접촉 힘이 액터가 정의한 힘 임계값 중
		하나 미만으로 떨어질 때 (이 충돌 쌍이 접촉을 중단하는 경우 포함) 연락 보고 콜백을 호출합니다.

		\note 충돌한 액터가 강체인 경우에만 영향을 미칩니다.
		\note 쌍이 다시 필터링되고 이 플래그가 이전에 비활성화된 경우,
		힘 임계값이 이전 프레임에서 미만이었더라도 동일한 프레임에서 보고가 발생하지 않을 것입니다
		(이전 프레임에서 #eNOTIFY_THRESHOLD_FORCE_FOUND 또는
		#eNOTIFY_THRESHOLD_FORCE_PERSISTS가 설정되었는지여부에 관계없이).

		\note eDETECT_DISCRETE_CONTACT 또는 eDETECT_CCD_CONTACT가 활성화된 경우에만 영향을 미칩니다.

		@see PxSimulationEventCallback.onContact()
		*/
		eNOTIFY_THRESHOLD_FORCE_LOST = (1 << 8),

		/**
		\brief 이 충돌 쌍의 연락 보고에서 연락점을 제공합니다.

		\note	충돌한 액터가 강체이고 eNOTIFY_TOUCH_... 또는
				eNOTIFY_THRESHOLD_FORCE_... 플래그와 함께 사용될 때만 영향을 미칩니다.
		\note	eDETECT_DISCRETE_CONTACT 또는
				eDETECT_CCD_CONTACT가 활성화된 경우에만 영향을 미칩니다.

		@see PxSimulationEventCallback.onContact() PxContactPair PxContactPair.extractContacts()
		*/
		eNOTIFY_CONTACT_POINTS = (1 << 9),

		/**
		\brief 이 플래그는 이 쌍이 이산 충돌 감지 연락을 생성하는지 여부를 나타냅니다.

		\note	연락은 eSOLVE_CONTACT가 활성화된 경우에만 응답됩니다.
		*/
		eDETECT_DISCRETE_CONTACT = (1 << 10),

		/**
		\brief 이 플래그는 이 쌍이 CCD(Continuous Collision Detection) 연락을 생성하는지 여부를 나타냅니다.

		\note	연락은 이 쌍에 eSOLVE_CONTACT가 활성화된 경우에만 응답됩니다.
		\note	이 기능을 사용하려면 씬에 PxSceneFlag::eENABLE_CCD가 활성화되어 있어야 합니다.
		\note	쌍의 비정적 바디는 올바르게 작동하려면 PxRigidBodyFlag::eENABLE_CCD가 지정되어야 합니다.
		\note	이 플래그는 트리거 모양과 함께 사용되지 않습니다.
				그러나 트리거 모양을 사용하지 않고 쌍에 eNOTIFY_TOUCH_FOUND 및
				eNOTIFY_TOUCH_LOST를 요청하고 쌍에 eSOLVE_CONTACT를 활성화하지 않도록 설정하여
				CCD 트리거 이벤트를 흉내낼 수 있습니다.

		@see PxRigidBodyFlag::eENABLE_CCD
		@see PxSceneFlag::eENABLE_CCD
		*/
		eDETECT_CCD_CONTACT = (1 << 11),

		/**
		\brief 이 충돌 쌍의 연락 보고에서 사전 해결 속도를 제공합니다.

		충돌 쌍이 연락 보고를 활성화한 경우, 연락이 해결되기 전의 강체의 속도가 제공됩니다.
		쌍이 접촉을 잃은 경우 데이터가 제공되지 않습니다.

		\note	일반적으로 이러한 속도를 요청하는 것은 필요하지 않습니다.
				왜냐하면 이러한 속도는 제공된 PxRigidActor 객체에서 직접 조회할 수 있기 때문입니다.
				그러나 시뮬레이션이 실행 중에 강체의 속도가 설정될 수 있는 경우가 있기 때문에
				시뮬레이션이 사용한 속도가 아닌 이 새로운 속도가 연락 보고 콜백에서 반환될 수 있습니다.

		@see PxSimulationEventCallback.onContact(), PxContactPairVelocity, PxContactPairHeader.extraDataStream
		*/
		ePRE_SOLVER_VELOCITY = (1 << 12),

		/**
		\brief 이 충돌 쌍의 연락 보고에서 사후 해결 속도를 제공합니다.

		충돌 쌍이 연락 보고를 활성화한 경우, 연락이 해결된 후의 강체의 속도가 제공됩니다.
		쌍이 접촉을 잃은 경우 데이터가 제공되지 않습니다.

		@see PxSimulationEventCallback.onContact(), PxContactPairVelocity, PxContactPairHeader.extraDataStream
		*/
		ePOST_SOLVER_VELOCITY = (1 << 13),

		/**
		\brief 이 충돌 쌍의 연락 보고에서 강체 포즈를 제공합니다.

		충돌 쌍이 연락 보고를 활성화한 경우, 연락 이벤트 시간의 강체 포즈가 제공됩니다.
		쌍이 접촉을 잃은 경우 데이터가 제공되지 않습니다.

		\note	보통은 이러한 포즈를 요청하는 것이 필요하지 않습니다.
				왜냐하면 이러한 포즈는 제공된 PxRigidActor 객체에서 직접 조회할 수 있기 때문입니다.
				그러나 시뮬레이션 실행 중에 강체의 포즈가 설정될 수 있는 경우가 있으며
				이러한 경우 연락 보고 콜백에서는 시뮬레이션이 사용한 포즈가 아닌 이 새로운 포즈가 반환될 수 있습니다.
				다른 사용 사례는 여러 패스가 활성화된 CCD의 경우입니다.
				빠르게 이동하는 객체가 동일한 객체에 여러 번 튀어올라갈 수 있습니다.
				이 플래그를 사용하면 각 충돌 이벤트에서 충돌 시점의 강체 포즈를 요청할 수 있습니다.

		@see PxSimulationEventCallback.onContact(), PxContactPairPose, PxContactPairHeader.extraDataStream
		*/
		eCONTACT_EVENT_POSE = (1 << 14),

		eNEXT_FREE = (1 << 15),        //!< 내부용으로만 사용됩니다.

		/**
		\brief 이 충돌 쌍에 대한 간단한 연락 처리를 수행하도록 기본 플래그를 제공합니다.
		*/
		eCONTACT_DEFAULT = eSOLVE_CONTACT | eDETECT_DISCRETE_CONTACT,

		/**
		\brief 이 충돌 쌍에 대한 일반적으로 사용되는 트리거 동작을 얻기 위한 기본 플래그를 제공합니다.
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
\brief 충돌 쌍에 대한 필터 작업을 설명하는 플래그 모음입니다.

@see PxFilterFlags PxSimulationFilterShader PxSimulationFilterCallback
*/
struct PxFilterFlag
{
	enum Enum
	{
		/**
		\brief 쌍 객체의 바운딩 볼륨이 겹치는 한 충돌 쌍을 무시합니다.

		죽은 쌍은 시뮬레이션에서 무시되고 다음 중 하나가 발생할 때까지 필터를 다시 실행하지 않습니다.

		\li 두 객체의 바운딩 볼륨이 다시 겹치게 되면(분리된 후)
		\li 사용자가 다시 필터링을 강제로 수행함(참조: #PxScene::resetFiltering())

		@see PxScene::resetFiltering()
		*/
		eKILL = (1 << 0),

		/**
		\brief 쌍 객체의 바운딩 볼륨이 겹치는 한 또는 한 충돌 객체의 필터 관련 데이터가 변경될 때까지 충돌 쌍을 무시합니다.

		억제된 쌍은 시뮬레이션에서 무시되고 다음 중 하나가 발생할 때까지 다시 필터링 요청을 수행하지 않습니다.

		\li 죽은 쌍과 같은 조건(참조: #eKILL)
		\li 충돌 객체 중 하나의 필터 데이터 또는 필터 객체 속성이 변경될 때

		@see PxFilterData PxFilterObjectAttributes
		*/
		eSUPPRESS = (1 << 1),

		/**
		\brief 이 충돌 쌍에 대해 필터 콜백(#PxSimulationFilterCallback::pairFound())을 호출합니다.

		@see PxSimulationFilterCallback
		*/
		eCALLBACK = (1 << 2),

		/**
		\brief 이 충돌 쌍을 필터 콜백 메커니즘으로 추적합니다.

		충돌 쌍의 바운딩 볼륨이 접촉을 잃게되면 필터 콜백 #PxSimulationFilterCallback::pairLost()이 호출됩니다.
		또한, 충돌 쌍의 필터 상태는 한 프레임에 한 번(#PxSimulationFilterCallback::statusChange()를 통해) 조정할 수 있습니다(쌍이 손실될 때까지).

		@see PxSimulationFilterCallback
		*/
		eNOTIFY = (1 << 3) | eCALLBACK,

		/**
		\brief 표준 동작을 얻기 위해 제공된 기본값:

		바운딩 볼륨이 겹치는 경우 애플리케이션이 쌍의 충돌 속성을 한 번 구성하고
		그 쌍에 대해 다시 물어보지 않고, 겹치는 상태 또는 필터 속성이 변경되거나 다시 필터링이 요청될 때까지입니다.

		바운딩 볼륨이 손실될 때는 알림이 제공되지 않습니다.

		쌍이 죽거나 억제되지 않으므로 충돌 감지가 처리될 것입니다.
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
\brief PxFilterData는 충돌 필터링 셰이더와/또는 콜백으로 전달되는 사용자 정의 데이터입니다.

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
		\brief 정적 강체 몸체
		@see PxRigidStatic
		*/
		eRIGID_STATIC,

		/**
		\brief 동적 강체 몸체
		@see PxRigidDynamic
		*/
		eRIGID_DYNAMIC,

		/**
		\brief 아티큘레이션
		@see PxArticulationReducedCoordinate
		*/
		eARTICULATION,

		/**
		\brief 입자 시스템
		@see PxParticleSystem
		*/
		ePARTICLESYSTEM,

		/**
		\brief FEM 기반 소프트 바디
		@see PxSoftBody
		*/
		eSOFTBODY,

		/**
		\brief FEM 기반 옷
		\note 개발 중
		@see PxFEMCloth
		*/
		eFEMCLOTH,

		/**
		\brief 헤어 시스템
		\note 개발 중
		@see PxHairSystem
		*/
		eHAIRSYSTEM,

		//! \brief 내부적인 사용 전용!
		eMAX_TYPE_COUNT = 16,

		//! \brief 내부적인 사용 전용!
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
\brief Structure which gets passed into the collision filtering shader and/or callback providing additional information on objects of a collision pair

@see PxSimulationFilterShader PxSimulationFilterCallback getActorType() PxFilterObjectIsKinematic() PxFilterObjectIsTrigger()
*/
typedef PxU32 PxFilterObjectAttributes;


/**
\brief Extract filter object type from the filter attributes of a collision pair object

\param[in] attr The filter attribute of a collision pair object
\return The type of the collision pair object.

@see PxFilterObjectType
*/
PX_INLINE PxFilterObjectType::Enum PxGetFilterObjectType(PxFilterObjectAttributes attr)
{
	return PxFilterObjectType::Enum(attr & (PxFilterObjectType::eMAX_TYPE_COUNT-1));
}


/**
\brief Specifies whether the collision object belongs to a kinematic rigid body

\param[in] attr The filter attribute of a collision pair object
\return True if the object belongs to a kinematic rigid body, else false

@see PxRigidBodyFlag::eKINEMATIC
*/
PX_INLINE bool PxFilterObjectIsKinematic(PxFilterObjectAttributes attr)
{
	return (attr & PxFilterObjectFlag::eKINEMATIC) != 0;
}


/**
\brief Specifies whether the collision object is a trigger shape

\param[in] attr The filter attribute of a collision pair object
\return True if the object is a trigger shape, else false

@see PxShapeFlag::eTRIGGER_SHAPE
*/
PX_INLINE bool PxFilterObjectIsTrigger(PxFilterObjectAttributes attr)
{
	return (attr & PxFilterObjectFlag::eTRIGGER) != 0;
}

/**
\brief 잠재적으로 충돌할 수 있는 두 객체 쌍이 시뮬레이션에서 어떻게 처리되어야 하는지 지정하는 필터 메서드입니다.

충돌 필터링은 두 객체의 바운딩 볼륨이 겹치는 경우 두 객체 쌍이 어떻게 처리될지를 지정하는 메커니즘입니다.
간단히 말해 충돌 필터는 충돌 쌍이 처리되어야 하는지 여부,
일시적으로 무시되어야 하는지 여부 또는 버려져야 하는지 여부를 결정합니다.
충돌 쌍이 처리되어야 하는 경우 필터는 추가로 어떻게 처리되어야 하는지를 지정할 수 있습니다.
예를 들어, 접촉을 해결해야 하는지, 어떤 콜백을 호출해야 하는지 또는 어떤 보고서를 보내야 하는지 등입니다.
이 함수는 충돌 쌍에 대해 시뮬레이션이 수행해야 할 작업을 정의하는 PxFilterFlag 플래그를 반환하고 PxPairFlag 플래그를 설정합니다.

\note 필터 셰이더의 기본 구현은 PhysX 확장 라이브러리에 제공되며
#PxDefaultSimulationFilterShader를 참조하십시오.

이 메서드는 다음 경우에 호출됩니다.
\li 두 객체의 바운딩 볼륨이 겹치기 시작했을 때.
\li 두 객체의 바운딩 볼륨이 겹치고 한 객체의 필터 데이터 또는 필터 속성이 변경된 경우
\li resetFiltering()을 통해 다시 필터링을 강제로 요청한 경우(참조: #PxScene::resetFiltering())
\li 장면 쿼리에서 필터링이 요청된 경우

\note 특정 객체 쌍은 항상 무시되며 이 메서드가 호출되지 않습니다. 이는 다음과 같은 경우입니다.
\li 두 정적 강체 액터의 쌍
\li 정적 강체 액터와 운동 강체 액터의 쌍(하나가 트리거가 아닌 한 또는
명시적으로 PxPairFilteringMode::eKEEP을 통해 활성화되지 않은 경우)

\li 두 운동 강체 액터의 쌍(하나가 트리거가 아닌 한 또는
명시적으로 PxPairFilteringMode::eKEEP을 통해 활성화되지 않은 경우)

\li 두 조인트된 강체 몸체가 충돌을 비활성화하도록 정의된 경우
\li 두 관절된 강체 몸체의 쌍이 관절을 통해 연결된 경우

\note 이는 성능에 중요한 메서드이며 상태를 갖춰서는 안 됩니다.
이 메서드에서 외부 객체에 액세스하거나 인라인되지 않은 외부 메서드를 호출해서는 안 됩니다.
충돌 쌍을 필터링하는 더 복잡한 논리가 필요한 경우이 쌍에 대한 필터 콜백 메커니즘을 사용하십시오
(참조: #PxSimulationFilterCallback, #PxFilterFlag::eCALLBACK, #PxFilterFlag::eNOTIFY).

\param[in] attributes0 첫 번째 객체의 필터 속성
\param[in] filterData0 첫 번째 객체의 사용자 정의 필터 데이터
\param[in] attributes1 두 번째 객체의 필터 속성
\param[in] filterData1 두 번째 객체의 사용자 정의 필터 데이터
\param[out] pairFlags 허용된 쌍을 처리하는 방법에 대한 추가 정보를 제공하는 플래그
\param[in] constantBlock 상수 전역 필터 데이터(참조: #PxSceneDesc.filterShaderData)
\param[in] constantBlockSize 전역 필터 데이터의 크기(참조: #PxSceneDesc.filterShaderDataSize)
\return 쌍을 버리거나 일시적으로 무시하거나 처리해야 하는지 여부 및
이 쌍에 대한 필터 콜백을 호출해야 하는지 여부를 정의하는 필터 플래그입니다.

@see PxSimulationFilterCallback PxFilterData PxFilterObjectAttributes PxFilterFlag PxFilterFlags PxPairFlag PxPairFlags PxSceneDesc.filterShader
*/
typedef PxFilterFlags (*PxSimulationFilterShader)
	(PxFilterObjectAttributes attributes0, PxFilterData filterData0, 
	 PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	 PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);



/**
\brief Filter callback to specify handling of collision pairs.

This class is provided to implement more complex and flexible collision pair filtering logic, for instance, taking
the state of the user application into account. Filter callbacks also give the user the opportunity to track collision
pairs and update their filter state.

You might want to check the documentation on #PxSimulationFilterShader as well since it includes more general information
on filtering.

\note SDK state should not be modified from within the callbacks. In particular objects should not
be created or destroyed. If state modification is needed then the changes should be stored to a buffer
and performed after the simulation step.

\note The callbacks may execute in user threads or simulation threads, possibly simultaneously. The corresponding objects 
may have been deleted by the application earlier in the frame. It is the application's responsibility to prevent race conditions
arising from using the SDK API in the callback while an application thread is making write calls to the scene, and to ensure that
the callbacks are thread-safe. Return values which depend on when the callback is called during the frame will introduce nondeterminism 
into the simulation.

@see PxSceneDesc.filterCallback PxSimulationFilterShader
*/
class PxSimulationFilterCallback
{
public:

	/**
	\brief Filter method to specify how a pair of potentially colliding objects should be processed.

	This method gets called when the filter flags returned by the filter shader (see #PxSimulationFilterShader)
	indicate that the filter callback should be invoked (#PxFilterFlag::eCALLBACK or #PxFilterFlag::eNOTIFY set).
	Return the PxFilterFlag flags and set the PxPairFlag flags to define what the simulation should do with the given 
	collision pair.

	\param[in] pairID			Unique ID of the collision pair used to issue filter status changes for the pair (see #statusChange())
	\param[in] attributes0		The filter attribute of the first object
	\param[in] filterData0		The custom filter data of the first object
	\param[in] a0				Actor pointer of the first object
	\param[in] s0				Shape pointer of the first object (NULL if the object has no shapes)
	\param[in] attributes1		The filter attribute of the second object
	\param[in] filterData1		The custom filter data of the second object
	\param[in] a1				Actor pointer of the second object
	\param[in] s1				Shape pointer of the second object (NULL if the object has no shapes)
	\param[in,out] pairFlags	In: Pair flags returned by the filter shader. Out: Additional information on how an accepted pair should get processed
	\return Filter flags defining whether the pair should be discarded, temporarily ignored or processed and whether the pair
	should be tracked and send a report on pair deletion through the filter callback

	@see PxSimulationFilterShader PxFilterData PxFilterObjectAttributes PxFilterFlag PxPairFlag
	*/
	virtual		PxFilterFlags	pairFound(	PxU64 pairID,
											PxFilterObjectAttributes attributes0, PxFilterData filterData0, const PxActor* a0, const PxShape* s0,
											PxFilterObjectAttributes attributes1, PxFilterData filterData1, const PxActor* a1, const PxShape* s1,
											PxPairFlags& pairFlags) = 0;

	/**
	\brief Callback to inform that a tracked collision pair is gone.

	This method gets called when a collision pair disappears or gets re-filtered. Only applies to
	collision pairs which have been marked as filter callback pairs (#PxFilterFlag::eNOTIFY set in #pairFound()).

	\param[in] pairID			Unique ID of the collision pair that disappeared
	\param[in] attributes0		The filter attribute of the first object
	\param[in] filterData0		The custom filter data of the first object
	\param[in] attributes1		The filter attribute of the second object
	\param[in] filterData1		The custom filter data of the second object
	\param[in] objectRemoved	True if the pair was lost because one of the objects got removed from the scene

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
