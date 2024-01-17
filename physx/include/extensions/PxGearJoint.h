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
	\brief 두 개의 기존 회전 조인트를 연결하고 서로에 대한 상대적인 각속도와 위치를 제약하는 조인트입니다.

	@see PxGearJointCreate PxJoint
	*/
	class PxGearJoint : public PxJoint
	{
	public:

		/**
		\brief 기어 조인트에 의해 연결된 힌지/회전 조인트를 설정합니다.

		전달된 조인트는 PxRevoluteJoint, PxD6Joint
		또는 PxArticulationJointReducedCoordinate 중하나일 수 있습니다.
		조인트는 트위스트 축 주위의 자유도를 정의해야 합니다.
		조인트는 null이 될 수 없습니다.

		이 함수를 호출하면 이러한 조인트는 위치 오차 보정 항을 계산하는 데에만 사용되며,
		조인트된 액터 간의 잠재적인 드리프트를 조정하는 데 사용됩니다.
		기어 조인트는 이 함수를 호출하지 않고도 실행될 수 있지만,
		이 경우 기어 메쉬의 치아 사이에 시간이 지남에 따라 일부 시각적인 겹침이 발생할 수 있습니다.

		\note	이 함수를 호출하면 내부 위치 오차 보정 항이 재설정됩니다.

		\param[in]	hinge0		첫 번째 힌지 조인트
		\param[in]	hinge1		두 번째 힌지 조인트
		\return		성공하면 true
		*/
		virtual	bool		setHinges(const PxBase* hinge0, const PxBase* hinge1)	= 0;

		/**
		\brief 기어 조인트에 의해 연결된 힌지/회전 조인트를 가져옵니다.

		\param[out]	hinge0		첫 번째 힌지 조인트
		\param[out]	hinge1		두 번째 힌지 조인트
		*/
		virtual	void		getHinges(const PxBase*& hinge0, const PxBase*& hinge1)	const	= 0;

		/**
		\brief 원하는 기어 비율을 설정합니다.

		두 기어의 이빨 수가 각각 n0 및 n1이면 기어 비율은 n0/n1입니다.

		\note	관련된 액터의 조인트 프레임이 동일한 방향으로
				정렬되어 있지 않은 경우 음수 기어 비율을 사용해야 할 수 있습니다.

		\note	이 함수를 호출하면 내부 위치 오차 보정 항이 재설정됩니다.

		\param[in]	ratio	두 힌지 간의 원하는 비율
		*/
		virtual	void		setGearRatio(float ratio)	= 0;

		/**
		\brief 기어 비율을 가져옵니다.

		\return		현재 비율
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
