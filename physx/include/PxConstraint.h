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

#ifndef PX_CONSTRAINT_H
#define PX_CONSTRAINT_H

/** \addtogroup physics
@{
*/

#include "PxPhysXConfig.h"
#include "PxConstraintDesc.h"
#include "common/PxBase.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxRigidActor;
class PxScene;
class PxConstraintConnector;

/**
\brief constraint flags

\note eBROKEN is a read only flag
*/
struct PxConstraintFlag
{
	enum Enum
	{
		eBROKEN = 1 << 0,						//!< 제약이 깨졌는지 여부

		eCOLLISION_ENABLED = 1 << 3,			//!< 이 제약이 제한하는 객체 간에 접촉을 생성해야 하는지 여부

		eVISUALIZATION = 1 << 4,				//!< 이 제약이 시각화되어야 하는지 여부, 시각화가 활성화된 경우

		eDRIVE_LIMITS_ARE_FORCES = 1 << 5,		//!< 드라이브 강도의 제한이 충돌 대신 힘으로 작동하는지 여부

		eIMPROVED_SLERP = 1 << 7,				//!< D6 Slerp Drive에서 향상된 정확도를 위한 전처리를 수행
												//	(전처리가 더 이상 필요하지 않을 때 이 플래그는 향후 릴리스에서 제거될 것임)

		eDISABLE_PREPROCESSING = 1 << 8,		//!< 제약 전처리를 억제, rowResponseThreshold와 함께 사용하기 위한 것.
												//	 악조건의 제약에 대해 악화된 솔버 정확도를 초래할 수 있음.

		eENABLE_EXTENDED_LIMITS = 1 << 9,		//!< 각도 제한에 대한 확장된 범위 활성화 (예: 제한 값 > PxPi 또는 < -PxPi)

		eGPU_COMPATIBLE = 1 << 10,				//!< 제약 유형이 GPU 다이내믹스에서 지원됨

		eALWAYS_UPDATE = 1 << 11,				//!< 매 프레임마다 제약을 업데이트

		eDISABLE_CONSTRAINT = 1 << 12			//!< 제약 비활성화. 이 제약에 대한 SolverPrep 함수는 호출되지 않을 것임.
	};
};

/**
\brief constraint flags
@see PxConstraintFlag
*/
typedef PxFlags<PxConstraintFlag::Enum, PxU16> PxConstraintFlags;
PX_FLAGS_OPERATORS(PxConstraintFlag::Enum, PxU16)

/**
\brief a table of function pointers for a constraint

@see PxConstraint
*/
struct PxConstraintShaderTable
{
	PxConstraintSolverPrep	solverPrep;	//!< solver constraint generation function
	PxConstraintVisualize	visualize;	//!< constraint visualization function
	PxConstraintFlag::Enum	flag;		//!< constraint flags
};

/**
\brief 제약 조건을 구현하기 위한 플러그인 클래스

@see PxPhysics.createConstraint
*/
class PxConstraint : public PxBase
{
public:

	/**
	\brief PxConstraint 인스턴스를 해제합니다.

	\note 이 호출은 연결된 강체들을 깨우지 않습니다.

	@see PxPhysics.createConstraint, PxBase.release()
	*/
	virtual void				release()														= 0;

	/**
	\brief 이 제약 조건이 속한 씬을 검색합니다.

	\return 소유자 씬. 씬에 속해 있지 않은 경우 NULL입니다.

	@see PxScene
	*/
	virtual PxScene*			getScene()												const	= 0;

	/**
	\brief 이 제약 조건의 액터를 검색합니다.

	\param[out] actor0
				첫 번째 액터에 대한 포인터에 대한 참조
	
	\param[out] actor1
				두 번째 액터에 대한 포인터에 대한 참조

	@see PxActor
	*/
	virtual void				getActors(PxRigidActor*& actor0, PxRigidActor*& actor1)	const	= 0;

	/**
	\brief 이 제약 조건의 액터를 설정합니다.

	\param[in]	actor0
				첫 번째 액터에 대한 포인터에 대한 참조
	\param[in]	actor1
				두 번째 액터에 대한 포인터에 대한 참조

	@see PxActor
	*/
	virtual void				setActors(PxRigidActor* actor0, PxRigidActor* actor1)			= 0;

	/**
	\brief Notify the scene that the constraint shader data has been updated by the application
	*/
	virtual void				markDirty()														= 0;

	/**
	\brief Retrieve the flags for this constraint

	\return the constraint flags
	@see PxConstraintFlags
	*/
	virtual PxConstraintFlags	getFlags()												const	= 0;

	/**
	\brief Set the flags for this constraint

	\param[in] flags the new constraint flags

	default: PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES

	@see PxConstraintFlags
	*/
	virtual void				setFlags(PxConstraintFlags flags)								= 0;

	/**
	\brief Set a flag for this constraint

	\param[in] flag the constraint flag
	\param[in] value the new value of the flag

	@see PxConstraintFlags
	*/
	virtual void				setFlag(PxConstraintFlag::Enum flag, bool value)				= 0;


	/**
	\brief 이 제약을 유지하기 위해 가장 최근에 적용된 제약 힘을 검색합니다.

	\note	시뮬레이션이 실행 중일 때
			(PxScene::collide() 내에서 또는 PxContactModifyCallback이나
			연락처 보고서 콜백 중에는 사용할 수 없습니다).

	\param[out] linear 제약 힘
	\param[out] angular 제약 토크
	*/
	virtual void				getForce(PxVec3& linear, PxVec3& angular)				const	= 0;

	/**
	\brief whether the constraint is valid. 
	
	A constraint is valid if it has at least one dynamic rigid body or articulation link. A constraint that
	is not valid may not be inserted into a scene, and therefore a static actor to which an invalid constraint
	is attached may not be inserted into a scene.

	Invalid constraints arise only when an actor to which the constraint is attached has been deleted.
	*/
	virtual bool				isValid() const													= 0;

	/**
	\brief Set the break force and torque thresholds for this constraint. 
	
	If either the force or torque measured at the constraint exceed these thresholds the constraint will break.

	\param[in] linear the linear break threshold
	\param[in] angular the angular break threshold
	*/
	virtual	void				setBreakForce(PxReal linear, PxReal angular)				= 0;

	/**
	\brief Retrieve the constraint break force and torque thresholds
	
	\param[out] linear the linear break threshold
	\param[out] angular the angular break threshold
	*/
	virtual	void				getBreakForce(PxReal& linear, PxReal& angular)		const	= 0;

	/**
	\brief Set the minimum response threshold for a constraint row 
	
	When using mass modification for a joint or infinite inertia for a jointed body, very stiff solver constraints can be generated which 
	can destabilize simulation. Setting this value to a small positive value (e.g. 1e-8) will cause constraint rows to be ignored if very 
	large changes in impulses will generate only small changes in velocity. When setting this value, also set 
	PxConstraintFlag::eDISABLE_PREPROCESSING. The solver accuracy for this joint may be reduced.

	\param[in] threshold the minimum response threshold

	@see PxConstraintFlag::eDISABLE_PREPROCESSING
	*/
	virtual	void				setMinResponseThreshold(PxReal threshold)					= 0;

	/**
	\brief Retrieve the constraint break force and torque thresholds

	\return the minimum response threshold for a constraint row
	*/
	virtual	PxReal				getMinResponseThreshold()							const	= 0;

	/**
	\brief Fetch external owner of the constraint.
	
	Provides a reference to the external owner of a constraint and a unique owner type ID.

	\param[out] typeID Unique type identifier of the external object.
	\return Reference to the external object which owns the constraint.

	@see PxConstraintConnector.getExternalReference()
	*/
	virtual void*				getExternalReference(PxU32& typeID)							= 0;

	/**
	\brief Set the constraint functions for this constraint
	
	\param[in] connector the constraint connector object by which the SDK communicates with the constraint.
	\param[in] shaders the shader table for the constraint
 
	@see PxConstraintConnector PxConstraintSolverPrep PxConstraintVisualize
	*/
	virtual	void				setConstraintFunctions(PxConstraintConnector& connector, const PxConstraintShaderTable& shaders)	= 0;

	virtual	const char*			getConcreteTypeName() const PX_OVERRIDE { return "PxConstraint"; }

			void*				userData;	//!< user can assign this to whatever, usually to create a 1:1 relationship with a user object.

protected:
	PX_INLINE					PxConstraint(PxType concreteType, PxBaseFlags baseFlags) : PxBase(concreteType, baseFlags), userData(NULL) {}
	PX_INLINE					PxConstraint(PxBaseFlags baseFlags) : PxBase(baseFlags), userData(NULL) {}
	virtual						~PxConstraint() {}
	virtual	bool				isKindOf(const char* name) const PX_OVERRIDE { PX_IS_KIND_OF(name, "PxConstraint", PxBase); }
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
