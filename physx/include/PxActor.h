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

#ifndef PX_ACTOR_H
#define PX_ACTOR_H

/** \addtogroup physics
  @{
*/

#include "PxPhysXConfig.h"
#include "foundation/PxBounds3.h"
#include "PxClient.h"
#include "common/PxBase.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxRigidActor;
class PxRigidBody;
class PxRigidStatic;
class PxRigidDynamic;
class PxArticulationLink;
class PxScene;

/**
\brief Group index which allows to specify 1- or 2-way interaction
*/
typedef PxU8 PxDominanceGroup;		// Must be < 32, PxU8.

/**
\brief Flags which control the behavior of an actor.

@see PxActorFlags PxActor PxActor.setActorFlag() PxActor.getActorFlags()
*/
struct PxActorFlag
{
	enum Enum
	{
		/**
		\brief Enable debug renderer for this actor

		@see PxScene.getRenderBuffer() PxRenderBuffer PxVisualizationParameter
		*/
		eVISUALIZATION					= (1<<0),

		/**
		\brief Disables scene gravity for this actor
		*/
		eDISABLE_GRAVITY				= (1<<1),

		/**
		\brief Enables the sending of PxSimulationEventCallback::onWake() and PxSimulationEventCallback::onSleep() notify events

		@see PxSimulationEventCallback::onWake() PxSimulationEventCallback::onSleep()
		*/
		eSEND_SLEEP_NOTIFIES			= (1<<2),

		/**
		\brief 액터의 시뮬레이션을 비활성화합니다.

		\note 이 기능은 PxRigidStatic 및 PxRigidDynamic 액터에서만 지원되며, 강체 액터를
		씬 쿼리 전용으로 사용할 때 메모리 풋프린트를 줄이는 데에 사용할 수 있습니다.

		\note 이 플래그를 설정하면 액터에 연결된 모든 제약 조건이 씬에서 제거됩니다.

		\note 이 플래그가 설정된 경우 다음의 호출이 금지됩니다:
		\li PxRigidBody: setLinearVelocity(), setAngularVelocity(), addForce(), addTorque(), clearForce(), clearTorque(), setForceAndTorque()
		\li PxRigidDynamic: setKinematicTarget(), setWakeCounter(), wakeUp(), putToSleep()

		\par <b>슬리핑:</b>
		이 플래그를 설정하면 모든 속도 및 웨이크 카운터를 0으로 설정하고, 모든 힘을 지우며, 키네마틱 타겟을 지우고, 액터를 슬립 상태로 변경하며, 이전 프레임에서 접촉한 모든 액터를 깨웁니다.
		*/
		eDISABLE_SIMULATION				= (1<<3)
	};
};

/**
\brief collection of set bits defined in PxActorFlag.

@see PxActorFlag
*/
typedef PxFlags<PxActorFlag::Enum,PxU8> PxActorFlags;
PX_FLAGS_OPERATORS(PxActorFlag::Enum,PxU8)

/**
\brief Identifies each type of actor.
@see PxActor 
*/
struct PxActorType
{
	enum Enum
	{
		/**
		\brief A static rigid body
		@see PxRigidStatic
		*/
		eRIGID_STATIC,

		/**
		\brief A dynamic rigid body
		@see PxRigidDynamic
		*/
		eRIGID_DYNAMIC,
		
		/**
		\brief An articulation link
		@see PxArticulationLink
		*/
		eARTICULATION_LINK,

		/**
		\brief A FEM-based soft body
		@see PxSoftBody
		*/
		eSOFTBODY,

		/**
		\brief A FEM-based cloth
		\note In development
		@see PxFEMCloth
		*/
		eFEMCLOTH,

		/**
		\brief A PBD ParticleSystem
		@see PxPBDParticleSystem
		*/
		ePBD_PARTICLESYSTEM,

		/**
		\brief A FLIP ParticleSystem
		\note In development
		@see PxFLIPParticleSystem
		*/
		eFLIP_PARTICLESYSTEM,

		/**
		\brief A MPM ParticleSystem
		\note In development
		@see PxMPMParticleSystem
		*/
		eMPM_PARTICLESYSTEM,

		/**
		\brief A HairSystem
		\note In development
		@see PxHairSystem
		*/
		eHAIRSYSTEM,

		//! \brief internal use only!
		eACTOR_COUNT,

		//! \brief internal use only!
		eACTOR_FORCE_DWORD = 0x7fffffff
	};
};

/**
\brief PxActor is the base class for the main simulation objects in the physics SDK.

The actor is owned by and contained in a PxScene.
*/
class PxActor : public PxBase
{
public:
	/**
	\brief 액터를 삭제합니다.

	삭제된 인스턴스에 대한 참조를 유지하지 마십시오.

	액터가 #PxAggregate 객체에 속하는 경우 해당 액터는 자동으로 집합에서 제거됩니다.

	@see PxBase.release(), PxAggregate
	*/
	virtual		void			release() = 0;

	/**
	\brief Retrieves the type of actor.

	\return The actor type of the actor.

	@see PxActorType
	*/
	virtual		PxActorType::Enum	getType()	const = 0;

	/**
	\brief Retrieves the scene which this actor belongs to.

	\return Owner Scene. NULL if not part of a scene.

	@see PxScene
	*/
	virtual		PxScene*		getScene()	const = 0;

	// Runtime modifications

	/**
	\brief Sets a name string for the object that can be retrieved with getName().
	
	This is for debugging and is not used by the SDK. The string is not copied by the SDK, 
	only the pointer is stored.

	\param[in] name String to set the objects name to.

	<b>Default:</b> NULL

	@see getName()
	*/
	virtual		void			setName(const char* name)		= 0;

	/**
	\brief Retrieves the name string set with setName().

	\return Name string associated with object.

	@see setName()
	*/
	virtual		const char*		getName()			const	= 0;

	/**
	\brief Retrieves the axis aligned bounding box enclosing the actor.

	\note It is not allowed to use this method while the simulation is running (except during PxScene::collide(),
	in PxContactModifyCallback or in contact report callbacks).

	\param[in] inflation  Scale factor for computed world bounds. Box extents are multiplied by this value.

	\return The actor's bounding box.

	@see PxBounds3
	*/
	virtual		PxBounds3		getWorldBounds(float inflation=1.01f) const = 0;

	/**
	\brief Raises or clears a particular actor flag.
	
	See the list of flags #PxActorFlag

	<b>Sleeping:</b> Does <b>NOT</b> wake the actor up automatically.

	\param[in] flag  The PxActor flag to raise(set) or clear. See #PxActorFlag.
	\param[in] value The boolean value to assign to the flag.

	@see PxActorFlag getActorFlags() 
	*/
	virtual		void			setActorFlag(PxActorFlag::Enum flag, bool value) = 0;

	/**
	\brief Sets the actor flags.
	
	See the list of flags #PxActorFlag
	@see PxActorFlag setActorFlag() 
	*/
	virtual		void			setActorFlags( PxActorFlags inFlags ) = 0;

	/**
	\brief Reads the PxActor flags.
	
	See the list of flags #PxActorFlag

	\return The values of the PxActor flags.

	@see PxActorFlag setActorFlag() 
	*/
	virtual		PxActorFlags	getActorFlags()	const = 0;

	/**
	\brief Assigns dynamic actors a dominance group identifier.
	
	PxDominanceGroup is a 5 bit group identifier (legal range from 0 to 31).
	
	The PxScene::setDominanceGroupPair() lets you set certain behaviors for pairs of dominance groups.
	By default every dynamic actor is created in group 0.

	<b>Default:</b> 0

	<b>Sleeping:</b> Changing the dominance group does <b>NOT</b> wake the actor up automatically.

	\param[in] dominanceGroup The dominance group identifier. <b>Range:</b> [0..31]

	@see getDominanceGroup() PxDominanceGroup PxScene::setDominanceGroupPair()
	*/
	virtual		void			setDominanceGroup(PxDominanceGroup dominanceGroup)		 = 0;
	
	/**
	\brief Retrieves the value set with setDominanceGroup().

	\return The dominance group of this actor.

	@see setDominanceGroup() PxDominanceGroup PxScene::setDominanceGroupPair()
	*/
	virtual		PxDominanceGroup	getDominanceGroup() const = 0;

	
	/**
	\brief Sets the owner client of an actor.

	This cannot be done once the actor has been placed into a scene.

	<b>Default:</b> PX_DEFAULT_CLIENT

	@see PxClientID PxScene::createClient() 
	*/
	virtual		void			setOwnerClient( PxClientID inClient ) = 0;

	/**
	\brief Returns the owner client that was specified at creation time.

	This value cannot be changed once the object is placed into the scene.

	@see PxClientID PxScene::createClient()
	*/
	virtual		PxClientID		getOwnerClient() const = 0;

	/**
	\brief Retrieves the aggregate the actor might be a part of.

	\return The aggregate the actor is a part of, or NULL if the actor does not belong to an aggregate.

	@see PxAggregate
	*/
	virtual		PxAggregate*	getAggregate()	const = 0;

	//public variables:
				void*			userData;	//!< user can assign this to whatever, usually to create a 1:1 relationship with a user object.

protected:
	PX_INLINE					PxActor(PxType concreteType, PxBaseFlags baseFlags) : PxBase(concreteType, baseFlags), userData(NULL) {}
	PX_INLINE					PxActor(PxBaseFlags baseFlags) : PxBase(baseFlags) {}
	virtual						~PxActor()	{}
	virtual		bool			isKindOf(const char* name)	const		{ PX_IS_KIND_OF(name, "PxActor", PxBase); }


};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
