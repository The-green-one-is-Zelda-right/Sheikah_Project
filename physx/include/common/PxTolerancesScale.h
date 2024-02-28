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

#ifndef PX_TOLERANCES_SCALE_H
#define PX_TOLERANCES_SCALE_H

/** \addtogroup common
  @{
*/

#include "common/PxPhysXCommonConfig.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

/**
\brief 시뮬레이션이 실행되는 스케일을 정의하는 클래스입니다.
대부분의 시뮬레이션 허용 오차는 여기에 정의된 값에 따라 계산됩니다.

\note 시뮬레이션 스케일을 변경하는 경우, 장면의 중력의 기본값도 변경하려고 할 것이며,
안정된 시뮬레이션을 위해서는 장면의 bounceThreshold를 변경해야 할 것입니다.
*/

class PxTolerancesScale
{
public: 

	/**
	\brief 시뮬레이션 내의 물체의 대략적인 크기입니다.

	미터 단위로 대략 인간 크기를 시뮬레이션하는 경우, 1이 좋은 선택입니다.
	만약 센티미터 단위로 시뮬레이션을 수행하는 경우, 
	대신 100을 사용하십시오. 이 값은 특정 길이 관련 허용 오차를 추정하는 데 사용됩니다.
	*/
	PxReal	length;

	/**
	\brief 시뮬레이션 내의 물체의 일반적인 속도 크기입니다.
	이 값은 충돌이 튕김 또는 정지로 처리되어야 하는지를 판단하고,
	시뮬레이션에서 물체를 슬립 상태로 전환할 때 사용됩니다.

	일반적인 물리적 환경에서 좋은 선택은 중력에 따라 1초 동안 떨어지는 물체의 근사 속도입니다.
	*/
	PxReal	speed;

	/**
	\brief 생성자, 기본값으로 설정합니다.

	\param[in]	defaultLength	기본 길이
	\param[in]	defaultSpeed	기본 속도
	*/
	PX_INLINE explicit PxTolerancesScale(float defaultLength=1.0f, float defaultSpeed=10.0f);

	/**
	\brief 디스크립터가 유효한 경우 true를 반환합니다.
	\return 현재 설정이 유효한 경우 true를 반환합니다 (항상 true를 반환합니다).
	*/
	PX_INLINE bool isValid() const;

};

PX_INLINE PxTolerancesScale::PxTolerancesScale(float defaultLength, float defaultSpeed) :
	length	(defaultLength),
	speed	(defaultSpeed)
	{
	}

PX_INLINE bool PxTolerancesScale::isValid() const
{
	return length>0.0f;
}

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
