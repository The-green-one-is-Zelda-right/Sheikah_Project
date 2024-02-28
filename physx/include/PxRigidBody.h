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

#ifndef PX_RIGID_BODY_H
#define PX_RIGID_BODY_H
/** \addtogroup physics
@{
*/

#include "PxRigidActor.h"
#include "PxForceMode.h"
#include "PxNodeIndex.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

/**
\brief Collection of flags describing the behavior of a rigid body.

@see PxRigidBody.setRigidBodyFlag(), PxRigidBody.getRigidBodyFlags()
*/
struct PxRigidBodyFlag
{
	enum Enum
	{
		/**
		\brief 액터에 대한 키네마틱 모드를 활성화합니다.

		키네마틱 액터는 힘(중력과 같은)에 영향을 받지 않으며 모멘텀이 없는 특수한 동적 액터입니다.
		무한한 질량으로 간주되며 setKinematicTarget() 메서드를 사용하여 세계에서 이동할 수 있습니다.
		키네마틱 액터는 일반 동적 액터를 밀어서 통과할 수 있습니다.
		키네마틱 액터는 정적 또는 다른 키네마틱 객체와 충돌하지 않습니다.

		키네마틱 액터는 움직이는 플랫폼 또는 캐릭터와 같이 직접적인 움직임 제어가 필요한 경우에 적합합니다.

		주의: 이 플래그를 동적 액터에 설정하면 액터를 슬립하게하고 속도를 0으로 설정합니다.
		이 플래그가 해제되면 액터의 현재 슬립 상태가 유지됩니다.

		참고: 키네마틱 액터는 CCD(Continuous Collision Detection)와 호환되지 않으므로
		이 플래그를 설정하면 자동으로 eENABLE_CCD를 지웁니다.

		@see PxRigidDynamic.setKinematicTarget()
		*/
		eKINEMATIC	= (1<<0),		//!< Enable kinematic mode for the body.

		/**
		\brief 씬 쿼리에 대한 키네마틱 타겟 변환을 사용합니다.

		이 플래그가 설정된 경우 씬 쿼리는 키네마틱 타겟 변환을 현재 위치로 처리합니다 (실제 위치가 아닌). 이 플래그가 없으면 키네마틱 타겟은 시뮬레이션 단계 이후에만 씬 쿼리에 영향을 미칩니다.

		@see PxRigidDynamic.setKinematicTarget()
		*/
		eUSE_KINEMATIC_TARGET_FOR_SCENE_QUERIES	= (1<<1),

		/**
		\brief 액터의 스위프트 통합을 활성화합니다.

		이 플래그가 설정되고 씬에서 CCD가 활성화된 경우,
		이 본문은 충돌이 높은 속도의 동작으로 인해
		놓칠 수 있는 것을 방지하기 위해 CCD 시스템에 의해 시뮬레이션됩니다.
		개별 형상 쌍은 여전히 충돌 필터링에서 PxPairFlag::eDETECT_CCD_CONTACT를 활성화해야 하며
		이를 통해 CCD가 개별 상호 작용에 응답하도록 활성화되어야 합니다.

		참고: 키네마틱 액터는 CCD와 호환되지 않으므로 키네마틱 액터에 설정된 경우 이 플래그는 자동으로 지워집니다.
		*/
		eENABLE_CCD	= (1<<2),		//!< Enable CCD for the body.

		/**
		\brief 액터의 스위프트 통합에서 CCD를 활성화합니다.

		이 플래그를 설정하고 CCD가 활성화되면 CCD 상호 작용에서 마찰이 시뮬레이션됩니다.
		기본적으로 CCD 상호 작용에서 마찰은 시뮬레이션 아티팩트를 도입할 것으로 관찰되었으므로 기본적으로 비활성화됩니다.
		CCD 마찰은 이전 SDK 버전에서 활성화되었습니다.
		이 플래그를 설정하면 이전 SDK 버전과 더 유사한 동작을 얻을 수 있습니다.

		참고: 이 플래그를 사용하려면 PxRigidBodyFlag::eENABLE_CCD가 설정되어 있어야 합니다.
		*/
		eENABLE_CCD_FRICTION	= (1<<3),

		/**
		\brief 액터를 등록하여 속도를 기반으로 접촉 오프셋을 동적으로 조정하도록 설정합니다.
		이를 사용하여 CCD 효과를 얻을 수 있습니다.

		eENABLE_CCD 및 eENABLE_SPECULATIVE_CCD 두 플래그가 동일한 액터에 설정되어 있는 경우
		각도 모션은 추측성 접촉(eENABLE_SPECULATIVE_CCD)에 의해 처리되고
		선형 모션은 스위프트(eENABLE_CCD)에 의해 처리됩니다.
		*/
		eENABLE_SPECULATIVE_CCD	= (1<<4),

		/**
		\brief 시뮬레이션에서 물체의 자세 변화를 조기 단계에서 보고할 수 있도록 물체를 등록합니다.

		물체의 새로운 자세에 가능한 빨리 액세스하는 것이 유용한 경우가 있으며
		fetchResults() 호출이 반환될 때까지 기다릴 필요가 없습니다. 
		이 플래그를 설정하면 물체가 #PxSimulationEventCallback::onAdvance()에서 보고되도록 예약됩니다. 
		이 기능의 동작 및 제한 사항을 이해하기 위해 해당 콜백의 설명서를 참조하십시오.

		@see PxSimulationEventCallback::onAdvance()
		*/
		eENABLE_POSE_INTEGRATION_PREVIEW	= (1<<5),

		/**
		\brief CCD가 maxContactImpulse를 제한하도록 허용합니다.
		파괴 시스템과 같은 사용 사례에 유용하지만
		시각적 아티팩트를 유발할 수 있으므로 기본적으로 활성화되지 않습니다.
		*/
		eENABLE_CCD_MAX_CONTACT_IMPULSE = (1<<6),

		/**
		\brief 힘 및 가속도를 프레임 간에 지우지 않고 옮깁니다.
		*/
		eRETAIN_ACCELERATIONS = (1<<7),

		/**
		\brief 이 Actor에 대한 키네마틱-키네마틱 쌍 알림을 강제로 활성화합니다.

		이 플래그는 키네마틱 액터에 대한 전역 장면 수준의 PxPairFilteringMode 설정을 무시합니다.
		이것은 이 Actor를 포함한 쌍에 대해 PxPairFilteringMode::eKEEP와 동등합니다.

		대부분의 경우 다수의 키네마틱 액터가 있지만 몇몇 액터 간의 상호 작용에만 관심이 있는 경우에 유용합니다.
		이 경우, PxSceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKILL을 사용하고
		알림이 필요한 소수의 키네마틱 액터에 eFORCE_KINE_KINE_NOTIFICATIONS 플래그를 설정하는 것이 좋습니다.

		참고: PxRigidBodyFlag::eKINEMATIC이 설정되지 않은 경우에는 효과가 없습니다.

		경고: 이 플래그를 실행 중에 변경하더라도 액터를 장면에서 제거한 다음 다시 추가하지 않으면 효과가 없습니다.

		참조: PxPairFilteringMode PxSceneDesc.kineKineFilteringMode
		*/
		eFORCE_KINE_KINE_NOTIFICATIONS = (1<<8),

		/**
		\brief 이 Actor에 대한 정적-키네마틱 쌍 알림을 강제로 활성화합니다.

		이것은 정적-키네마틱 상호 작용에 대한 eFORCE_KINE_KINE_NOTIFICATIONS와 유사하지만
		정적-키네마틱 상호 작용에 대한 것입니다.

		참고: PxRigidBodyFlag::eKINEMATIC이 설정되지 않은 경우 효과가 없습니다.

		경고: 이 플래그를 실행 중에 변경하더라도 액터를 장면에서 제거한 다음 다시 추가하지 않으면 효과가 없습니다.

		참조: PxPairFilteringMode PxSceneDesc.staticKineFilteringMode
		*/
		eFORCE_STATIC_KINE_NOTIFICATIONS = (1<<9),

		/**
		\brief 이 리지드 바디에 대한 자이로스코픽 힘 계산을 활성화합니다.
		*/
		eENABLE_GYROSCOPIC_FORCES = (1<<10),

		/**
		\brief 내부 용도
		*/
		eRESERVED = (1<<15)
	};
};

/**
\brief collection of set bits defined in PxRigidBodyFlag.

@see PxRigidBodyFlag
*/
typedef PxFlags<PxRigidBodyFlag::Enum,PxU16> PxRigidBodyFlags;
PX_FLAGS_OPERATORS(PxRigidBodyFlag::Enum,PxU16)

/**
\brief PxRigidBody is a base class shared between dynamic rigid body objects.

@see PxRigidActor
*/

class PxRigidBody : public PxRigidActor
{
public:
	// Runtime modifications

/************************************************************************************************/
/** @name Mass Manipulation
*/

	/**
	\brief 액터에 대한 질량 중심 위치를 액터에 상대적으로 설정합니다.

	참고: 이 변환을 변경하더라도 액터를 월드에서 이동시키지는 않습니다!

	참고: 실제로 존재하지 않는 질량 중심을 설정하면 SDK가 제약 조건을 해결하는 데 어려움을 겪을 수 있습니다.
	따라서 불안정성과 떨림 현상을 유발할 수 있습니다.

	참고: 이 변환을 변경하더라도 getLinearVelocity()에 의해 보고되는 선형 속도는 질량 중심의 이동을 고려하여 업데이트되지 않습니다.
	이동을 반영해야 하는 경우 사용자는 setLinearVelocity()를 사용하여 속도를 업데이트해야 합니다.

	<b>기본값:</b> 항등 변환

	\param[in] pose 액터 프레임을 기준으로 한 질량 중심 오프셋 변환. <b>범위:</b> 강체 본 변환.

	@see getCMassLocalPose() getLinearVelocity()
	*/
	virtual		void	setCMassLocalPose(const PxTransform& pose) = 0;

	/**
	\brief 액터 프레임을 기준으로 한 질량 중심 위치를 검색합니다.

	\return 액터 프레임을 기준으로 한 질량 중심 위치.

	@see setCMassLocalPose()
	*/
	virtual		PxTransform 	getCMassLocalPose() const = 0;

	/**
	\brief 동적 액터의 질량을 설정합니다.

	질량은 양수여야 합니다.

	setMass()는 몸체의 관성 특성을 업데이트하지 않으며,
	관성 텐서를 변경하려면 setMassSpaceInertiaTensor() 또는
	PhysX 확장 메서드 #PxRigidBodyExt::updateMassAndInertia()를 사용하세요.

	참고: 값이 0인 경우 무한 질량으로 해석됩니다.
	참고: PxArticulationLink의 경우 값이 0은 허용되지 않지만 PxRigidDynamic의 경우 허용됩니다.

	<b>기본값:</b> 1.0

	<b>슬립:</b> 자동으로 액터를 깨우지 <b>않습니다</b>.

	\param[in] mass 액터의 새 질량 값. <b>범위:</b> [0, PX_MAX_F32)

	@see getMass() setMassSpaceInertiaTensor()
	*/
	virtual		void	setMass(PxReal mass) = 0;

	/**
	\brief 액터의 질량을 검색합니다.

	참고: 값이 0인 경우 무한 질량으로 해석됩니다.

	\return 이 액터의 질량.

	@see setMass() setMassSpaceInertiaTensor()
	*/
	virtual		PxReal	getMass() const = 0;

	/**
	\brief 액터의 역 질량을 검색합니다.

	\return 이 액터의 역 질량.

	@see setMass() setMassSpaceInertiaTensor()
	*/
	virtual		PxReal	getInvMass() const = 0;

	/**
	\brief 질량 공간 좌표로 지정된 매개변수를 사용하여 관성 텐서를 설정합니다.

	이러한 행렬은 대각선 행렬입니다. 전달된 벡터는 대각선입니다.

	만약 비대각선 월드/액터 공간 관성 텐서(3x3 행렬)가 있는 경우, 이를 대각화하고 적절한 질량 공간 변환을 설정해야 합니다.
	#setCMassLocalPose()를 참조하세요.

	관성 텐서 요소는 양수여야 합니다.

	참고: 요소 중 하나가 0인 경우 해당 축을 따라 무한한 관성으로 해석됩니다.
	참고: PxArticulationLink의 경우 0 값은 허용되지 않지만 PxRigidDynamic의 경우 허용됩니다.

	<b>기본값:</b> (1.0, 1.0, 1.0)

	<b>슬립:</b> 자동으로 액터를 깨우지 <b>않습니다</b>.

	\param[in] m 액터의 새 질량 공간 관성 텐서.

	@see getMassSpaceInertia() setMass() setCMassLocalPose()
	*/
	virtual		void	setMassSpaceInertiaTensor(const PxVec3& m) = 0;

	/**
	\brief 액터에 대한 질량 좌표 프레임과 관련된 대각 관성 텐서를 검색합니다.

	이 메서드는 질량 좌표 프레임 관성 벡터를 검색합니다.

	\return 이 액터의 질량 공간 관성 텐서.

	참고: 요소 중 하나가 0인 경우 해당 축을 따라 무한한 관성으로 해석됩니다.

	@see setMassSpaceInertiaTensor() setMass() setCMassLocalPose()
	*/
	virtual		PxVec3	getMassSpaceInertiaTensor()			const = 0;

	/**
	\brief 액터에 대한 질량 좌표 프레임과 관련된 대각 역 관성 텐서를 검색합니다.

	이 메서드는 질량 좌표 프레임 역 관성 벡터를 검색합니다.

	참고: 요소 중 하나가 0인 경우 해당 축을 따라 무한한 관성으로 해석됩니다.

	\return 이 액터의 질량 공간 역 관성 텐서.

	@see setMassSpaceInertiaTensor() setMass() setCMassLocalPose()
	*/
	virtual		PxVec3	getMassSpaceInvInertiaTensor()			const = 0;

	/************************************************************************************************/
	/** @name Damping
	*/

	/**
	\brief 선형 감쇠 계수를 설정합니다.

	0은 감쇠가 없음을 나타냅니다. 감쇠 계수는 양수여야 합니다.

	<b>기본값:</b> PxArticulationLink의 경우 0.05, PxRigidDynamic의 경우 0.0

	\param[in] linDamp 선형 감쇠 계수. <b>범위:</b> [0, PX_MAX_F32)

	@see getLinearDamping() setAngularDamping()
	*/
	virtual		void	setLinearDamping(PxReal linDamp) = 0;

	/**
	\brief 선형 감쇠 계수를 검색합니다.

	\return 이 액터와 관련된 선형 감쇠 계수.

	@see setLinearDamping() getAngularDamping()
	*/
	virtual		PxReal	getLinearDamping() const = 0;

	/**
	\brief 각도 감쇠 계수를 설정합니다.

	0은 감쇠가 없음을 나타냅니다. 각도 감쇠 계수는 양수여야 합니다.

	<b>기본값:</b> 0.05

	\param[in] angDamp 각도 감쇠 계수. <b>범위:</b> [0, PX_MAX_F32)

	@see getAngularDamping() setLinearDamping()
	*/
	virtual		void	setAngularDamping(PxReal angDamp) = 0;

	/**
	\brief 각도 감쇠 계수를 검색합니다.

	\return 이 액터와 관련된 각도 감쇠 계수.

	@see setAngularDamping() getLinearDamping()
	*/
	virtual		PxReal	getAngularDamping() const = 0;


/************************************************************************************************/
/** @name Velocity
*/

	/**
	\brief Retrieves the linear velocity of an actor.

	\note It is not allowed to use this method while the simulation is running (except during PxScene::collide(),
	in PxContactModifyCallback or in contact report callbacks).

	\note The linear velocity is reported with respect to the rigid body's center of mass and not the actor frame origin.

	\return The linear velocity of the actor.

	@see PxRigidDynamic.setLinearVelocity() getAngularVelocity()
	*/
	virtual		PxVec3	getLinearVelocity()		const = 0;

	/**
	\brief Retrieves the angular velocity of the actor.

	\note It is not allowed to use this method while the simulation is running (except during PxScene::collide(),
	in PxContactModifyCallback or in contact report callbacks).

	\return The angular velocity of the actor.

	@see PxRigidDynamic.setAngularVelocity() getLinearVelocity() 
	*/
	virtual		PxVec3	getAngularVelocity()	const = 0;

	/**
	\brief 이 액터에 허용되는 최대 선형 속도를 설정할 수 있습니다.

	이 함수를 사용하여 이 강체 바디(Rigid Body)에 허용되는 최대 선형 속도를 설정할 수 있습니다.
	더 높은 선형 속도는 이 값으로 제한됩니다.

	참고: 선형 속도는 설정된 값 이전에 솔버 앞에서 클램핑(clamping)되므로,
	제한은 일시적으로 초과될 수 있습니다.

	<b>기본값:</b> PxArticulationLink의 경우 100 * PxTolerancesScale::length, PxRigidDynamic의 경우 1e^16

	\param[in] maxLinVel 액터에 대한 최대 허용 선형 속도. <b>범위:</b> [0, 1e^16)

	@see getMaxAngularVelocity()
	*/
	virtual		void	setMaxLinearVelocity(PxReal maxLinVel) = 0;

	/**
	\brief Retrieves the maximum angular velocity permitted for this actor.

	\return The maximum allowed angular velocity for this actor.

	@see setMaxLinearVelocity
	*/
	virtual		PxReal	getMaxLinearVelocity()	const = 0;

	/**
	\brief 이 액터에 허용되는 최대 각속도를 설정할 수 있습니다.

	다양한 내부 계산을 위해 매우 빠르게 회전하는 액터는 시뮬레이션에 오차를 도입하며,
	이로 인해 원하지 않는 결과가 발생할 수 있습니다.

	이 함수를 사용하여 이 강체 바디에 허용되는 최대 각속도를 설정할 수 있습니다.
	더 높은 각속도는 이 값으로 제한됩니다.

	참고: 각속도는 설정된 값 이전에 솔버 앞에서 클램핑(clamping)되므로,
	제한은 일시적으로 초과될 수 있습니다.

	<b>기본값:</b> PxArticulationLink의 경우 50.0, PxRigidDynamic의 경우 100.0

	<b>범위:</b> [0, 1e^16)

	\param[in] maxAngVel 액터에 대한 최대 허용 각속도.

	@see getMaxAngularVelocity()
	*/
	virtual		void	setMaxAngularVelocity(PxReal maxAngVel) = 0;

	/**
	\brief 이 액터에 허용된 최대 각속도를 검색합니다.

	\return 이 액터에 대한 최대 허용 각속도.

	@see setMaxAngularVelocity
	*/
	virtual		PxReal	getMaxAngularVelocity()	const = 0;

/************************************************************************************************/
/** @name Forces
*/

	/**
	\brief 액터의 질량 중심에 정의된 글로벌 좌표 프레임에서 힘(또는 충격)을 적용합니다.

	<b>이것은 회전력을 유발하지 않습니다</b>.

	::PxForceMode는 힘이 일반적인 힘 또는 충격인지를 결정합니다.

	각 액터는 가속도 및 속도 변경 누산기(accumulator)를 직접 수정합니다.
	가속도 및 속도 변경에 대한 모드는 각각 PxForceMode::eACCELERATION 및 PxForceMode::eVELOCITY_CHANGE를 사용합니다.
	PxForceMode::eFORCE 및 PxForceMode::eIMPULSE 모드도 동일한 누산기를 수정하며,
	단순히 역 질량으로 벡터 매개변수를 곱하고 PxForceMode::eACCELERATION 및 PxForceMode::eVELOCITY_CHANGE를 각각 사용합니다.

	참고: 이 메서드는 액터가 이미 시뮬레이션에 추가되지 않았거나 PxActorFlag::eDISABLE_SIMULATION이 설정된 경우에 사용할 수 없습니다.

	참고: PxForceMode::eIMPULSE 및 PxForceMode::eVELOCITY_CHANGE 모드는 아티큘레이션 링크에 적용할 수 없습니다.

	참고: 이것이 아티큘레이션 링크에서 호출된 경우 링크만 업데이트되며 전체 아티큘레이션이 업데이트되지 않습니다.

	참고: impulsive force의 적용에서 발생하는 선형 속도 변경을 계산하는 방법에 대한 자세한 내용은
	#PxRigidBodyExt::computeVelocityDeltaFromImpulse을 참조하십시오. 여기서 순간적인 힘은 힘을 타임스텝으로 곱한 것입니다.

	<b>슬립:</b> 이 호출은 액터가 자고 있으면 깨우고, autowake 매개변수가 true(기본값)이거나 힘이 0이면 깨웁니다.

	\param[in] force 글로벌 프레임에서 정의된 적용할 힘/충격.
	\param[in] mode 힘/충격을 적용할 때 사용할 모드(참조: #PxForceMode)
	\param[in] autowake 현재 자고 있는 액터를 깨울지 여부를 지정합니다. true이고 현재 깨어 있는 카운터 값이
	#PxSceneDesc::wakeCounterResetValue보다 작으면 reset 값으로 증가합니다.

	@see PxForceMode addTorque
	*/
	virtual		void	addForce(const PxVec3& force, PxForceMode::Enum mode = PxForceMode::eFORCE, bool autowake = true) = 0;

	/**
	\brief 액터에 정의된 글로벌 좌표 프레임에서 적용할 순간 회전 토크를 적용합니다.

	::PxForceMode는 토크가 전통적인 토크인지 순간적인 토크인지를 결정합니다.

	각 액터는 각 가속도 및 각속도 변경 누산기(accumulator)를 직접 수정합니다.
	각 가속도 및 각속도 변경에 대한 모드는 각각 PxForceMode::eACCELERATION 및 PxForceMode::eVELOCITY_CHANGE을 사용합니다.
	PxForceMode::eFORCE 및 PxForceMode::eIMPULSE 모드도 동일한 누산기를 수정하며,
	단순히 벡터 매개변수를 역 관성으로 곱한 다음 PxForceMode::eACCELERATION 및 PxForceMode::eVELOCITY_CHANGE를 각각 사용합니다.

	참고: 이 메서드는 액터가 이미 시뮬레이션에 추가되지 않았거나 PxActorFlag::eDISABLE_SIMULATION이 설정된 경우에 사용할 수 없습니다.

	참고: PxForceMode::eIMPULSE 및 PxForceMode::eVELOCITY_CHANGE 모드는 아티큘레이션 링크에 적용할 수 없습니다.

	참고: 이것이 아티큘레이션 링크에서 호출된 경우 링크만 업데이트되며 전체 아티큘레이션은 업데이트되지 않습니다.

	참고: 순간 회전 토크를 적용하는 데 따른 각속도 변경을 계산하는 방법에 대한 자세한 내용은
	#PxRigidBodyExt::computeVelocityDeltaFromImpulse을 참조하십시오. 여기서 순간 회전 토크는 시간 단계로 곱한 적용 토크입니다.

	<b>슬립:</b> 이 호출은 액터가 자고 있으면 깨우고, autowake 매개변수가 true(기본값)이거나 토크가 0이면 깨웁니다.

	\param[in] torque 글로벌 프레임에서 정의된 적용할 토크. <b>범위:</b> 토크 벡터
	\param[in] mode 힘/충격을 적용할 때 사용할 모드(참조: #PxForceMode).
	\param[in] autowake 현재 자고 있는 액터를 깨울지 여부를 지정합니다. true이고 현재 깨어 있는 카운터 값이
	#PxSceneDesc::wakeCounterResetValue보다 작으면 reset 값으로 증가합니다.

	@see PxForceMode addForce()
	*/
	virtual		void	addTorque(const PxVec3& torque, PxForceMode::Enum mode = PxForceMode::eFORCE, bool autowake = true) = 0;

	/**
	\brief 누적된 힘을 지우고(누적된 힘을 제로로 설정)합니다.

	각 액터는 가속도 및 속도 변경 누산기(accumulator)를 직접 수정합니다.
	각 가속도 및 속도 변경에 대한 모드는 각각 PxForceMode::eACCELERATION 및 PxForceMode::eVELOCITY_CHANGE을 사용합니다.
	PxForceMode::eFORCE 및 PxForceMode::eIMPULSE 모드도 동일한 누산기를 수정하며(자세한 내용은 PxRigidBody::addForce() 참조),
	따라서 clearForce(PxForceMode::eFORCE)를 호출하는 효과는 clearForce(PxForceMode::eACCELERATION)을 호출하는 것과 동등하며,
	clearForce(PxForceMode::eIMPULSE)를 호출하는 효과는 clearForce(PxForceMode::eVELOCITY_CHANGE)을 호출하는 것과 동등합니다.

	::PxForceMode는 지워진 힘이 전통적인 힘인지 순간적인 힘인지를 결정합니다.

	참고: PxForceMode::eIMPULSE 및 PxForceMode::eVELOCITY_CHANGE 모드는 아티큘레이션 링크에 적용할 수 없습니다.

	참고: 이 메서드는 액터가 이미 시뮬레이션에 추가되지 않았거나 PxActorFlag::eDISABLE_SIMULATION이 설정된 경우에 사용할 수 없습니다.

	\param[in] mode 힘/충격을 지울 때 사용할 모드(참조: #PxForceMode)

	@see PxForceMode addForce
	*/
	virtual		void	clearForce(PxForceMode::Enum mode = PxForceMode::eFORCE) = 0;

	/**
	\brief 액터에 정의된 글로벌 좌표 프레임에서 순간 회전 토크를 지웁니다.

	::PxForceMode는 지워진 토크가 전통적인 토크인지 순간적인 토크인지를 결정합니다.

	각 액터는 각 가속도 및 각속도 변경 누산기(accumulator)를 직접 수정합니다.
	각 가속도 및 각속도 변경에 대한 모드는 각각 PxForceMode::eACCELERATION 및 PxForceMode::eVELOCITY_CHANGE을 사용합니다.
	PxForceMode::eFORCE 및 PxForceMode::eIMPULSE 모드도 동일한 누산기를 수정하며(자세한 내용은 PxRigidBody::addTorque() 참조),
	따라서 clearTorque(PxForceMode::eFORCE)를 호출하는 효과는 clearTorque(PxForceMode::eACCELERATION)을 호출하는 것과 동등하며,
	clearTorque(PxForceMode::eIMPULSE)를 호출하는 효과는 clearTorque(PxForceMode::eVELOCITY_CHANGE)을 호출하는 것과 동등합니다.

	::PxForceMode는 지워진 토크가 전통적인 토크인지 순간적인 토크인지를 결정합니다.

	참고: PxForceMode::eIMPULSE 및 PxForceMode::eVELOCITY_CHANGE 모드는 아티큘레이션 링크에 적용할 수 없습니다.

	참고: 이 메서드는 액터가 이미 시뮬레이션에 추가되지 않았거나 PxActorFlag::eDISABLE_SIMULATION이 설정된 경우에 사용할 수 없습니다.

	\param[in] mode 토크/충격을 지울 때 사용할 모드(참조: #PxForceMode).

	@see PxForceMode addTorque
	*/
	virtual		void	clearTorque(PxForceMode::Enum mode = PxForceMode::eFORCE) = 0;

	/**
	\brief Sets the impulsive force and torque defined in the global coordinate frame to the actor.

	::PxForceMode determines if the cleared torque is to be conventional or impulsive.

	\note The force modes PxForceMode::eIMPULSE and PxForceMode::eVELOCITY_CHANGE can not be applied to articulation links.

	\note It is invalid to use this method if the actor has not been added to a scene already or if PxActorFlag::eDISABLE_SIMULATION is set.

	@see PxForceMode addTorque
	*/
	virtual		void	setForceAndTorque(const PxVec3& force, const PxVec3& torque, PxForceMode::Enum mode = PxForceMode::eFORCE) = 0;

	/**
	\brief Raises or clears a particular rigid body flag.
	
	See the list of flags #PxRigidBodyFlag

	<b>Default:</b> no flags are set

	<b>Sleeping:</b> Does <b>NOT</b> wake the actor up automatically.

	\param[in] flag		The PxRigidBody flag to raise(set) or clear. See #PxRigidBodyFlag.
	\param[in] value	The new boolean value for the flag.

	@see PxRigidBodyFlag getRigidBodyFlags() 
	*/
	virtual		void	setRigidBodyFlag(PxRigidBodyFlag::Enum flag, bool value) = 0;
	virtual		void	setRigidBodyFlags(PxRigidBodyFlags inFlags) = 0;

	/**
	\brief Reads the PxRigidBody flags.
	
	See the list of flags #PxRigidBodyFlag

	\return The values of the PxRigidBody flags.

	@see PxRigidBodyFlag setRigidBodyFlag()
	*/
	virtual		PxRigidBodyFlags	getRigidBodyFlags()	const = 0;

	/**
	\brief Sets the CCD minimum advance coefficient.

	The CCD minimum advance coefficient is a value in the range [0, 1] that is used to control the minimum amount of time a body is integrated when
	it has a CCD contact. The actual minimum amount of time that is integrated depends on various properties, including the relative speed and collision shapes
	of the bodies involved in the contact. From these properties, a numeric value is calculated that determines the maximum distance (and therefore maximum time) 
	which these bodies could be integrated forwards that would ensure that these bodies did not pass through each-other. This value is then scaled by CCD minimum advance
	coefficient to determine the amount of time that will be consumed in the CCD pass.

	<b>Things to consider:</b> 
	A large value (approaching 1) ensures that the objects will always advance some time. However, larger values increase the chances of objects gently drifting through each-other in
	scenes which the constraint solver can't converge, e.g. scenes where an object is being dragged through a wall with a constraint.
	A value of 0 ensures that the pair of objects stop at the exact time-of-impact and will not gently drift through each-other. However, with very small/thin objects initially in 
	contact, this can lead to a large amount of time being dropped and increases the chances of jamming. Jamming occurs when the an object is persistently in contact with an object 
	such that the time-of-impact is	0, which results in no time being advanced for those objects in that CCD pass.

	The chances of jamming can be reduced by increasing the number of CCD mass @see PxSceneDesc.ccdMaxPasses. However, increasing this number increases the CCD overhead.

	\param[in] advanceCoefficient The CCD min advance coefficient. <b>Range:</b> [0, 1] <b>Default:</b> 0.15
	*/
	virtual void setMinCCDAdvanceCoefficient(PxReal advanceCoefficient) = 0;

	/**
	\brief Gets the CCD minimum advance coefficient.

	\return The value of the CCD min advance coefficient.

	@see setMinCCDAdvanceCoefficient
	*/
	virtual PxReal getMinCCDAdvanceCoefficient() const = 0;

	/**
	\brief Sets the maximum depenetration velocity permitted to be introduced by the solver.
	This value controls how much velocity the solver can introduce to correct for penetrations in contacts. 
	\param[in] biasClamp The maximum velocity to de-penetrate by <b>Range:</b> (0, PX_MAX_F32].
	*/
	virtual void setMaxDepenetrationVelocity(PxReal biasClamp) = 0;

	/**
	\brief Returns the maximum depenetration velocity the solver is permitted to introduced.
	This value controls how much velocity the solver can introduce to correct for penetrations in contacts. 
	\return The maximum penetration bias applied by the solver.
	*/
	virtual PxReal getMaxDepenetrationVelocity() const = 0;

	/**
	\brief Sets a limit on the impulse that may be applied at a contact. The maximum impulse at a contact between two dynamic or kinematic
	bodies will be the minimum	of the two limit values. For a collision between a static and a dynamic body, the impulse is limited
	by the value for the dynamic body.

	\param[in] maxImpulse the maximum contact impulse. <b>Range:</b> [0, PX_MAX_F32] <b>Default:</b> PX_MAX_F32

	@see getMaxContactImpulse
	*/
	virtual void setMaxContactImpulse(PxReal maxImpulse) = 0;

	/**
	\brief Returns the maximum impulse that may be applied at a contact.

	\return The maximum impulse that may be applied at a contact

	@see setMaxContactImpulse
	*/
	virtual PxReal getMaxContactImpulse() const = 0;

	/**
	\brief Sets a distance scale whereby the angular influence of a contact on the normal constraint in a contact is 
	zeroed if normal.cross(offset) falls below this tolerance. Rather than acting as an absolute value, this tolerance
	is scaled by the ratio rXn.dot(angVel)/normal.dot(linVel) such that contacts that have relatively larger angular velocity
	than linear normal velocity (e.g. rolling wheels) achieve larger slop values as the angular velocity increases.

	\param[in] slopCoefficient the Slop coefficient. <b>Range:</b> [0, PX_MAX_F32] <b>Default:</b> 0

	@see getContactSlopCoefficient
	*/
	virtual void setContactSlopCoefficient(PxReal slopCoefficient) = 0;

	/**
	\brief Returns the contact slop coefficient.

	\return The contact slop coefficient.

	@see setContactSlopCoefficient
	*/
	virtual PxReal getContactSlopCoefficient() const = 0;

	/**
	\brief Returns the island node index

	\return The island node index.
	*/
	virtual PxNodeIndex getInternalIslandNodeIndex() const = 0;

protected:
	PX_INLINE			PxRigidBody(PxType concreteType, PxBaseFlags baseFlags) : PxRigidActor(concreteType, baseFlags) {}
	PX_INLINE			PxRigidBody(PxBaseFlags baseFlags) : PxRigidActor(baseFlags) {}
	virtual				~PxRigidBody()	{}
	virtual		bool	isKindOf(const char* name)	const	{ PX_IS_KIND_OF(name, "PxRigidBody", PxRigidActor); }
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
