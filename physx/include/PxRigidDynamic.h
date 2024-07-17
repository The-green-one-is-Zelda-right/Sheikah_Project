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

#ifndef PX_RIGID_DYNAMIC_H
#define PX_RIGID_DYNAMIC_H
/** \addtogroup physics
@{
*/

#include "PxRigidBody.h"

#if !PX_DOXYGEN
namespace physx
{
#endif


/**
\brief Collection of flags providing a mechanism to lock motion along/around a specific axis.

@see PxRigidDynamic.setRigidDynamicLockFlag(), PxRigidBody.getRigidDynamicLockFlags()
*/
struct PxRigidDynamicLockFlag
{
	enum Enum
	{
		eLOCK_LINEAR_X = (1 << 0),
		eLOCK_LINEAR_Y = (1 << 1),
		eLOCK_LINEAR_Z = (1 << 2),
		eLOCK_ANGULAR_X = (1 << 3),
		eLOCK_ANGULAR_Y = (1 << 4),
		eLOCK_ANGULAR_Z = (1 << 5)
	};
};

typedef PxFlags<PxRigidDynamicLockFlag::Enum, PxU8> PxRigidDynamicLockFlags;
PX_FLAGS_OPERATORS(PxRigidDynamicLockFlag::Enum, PxU8)

/**
\brief PxRigidDynamic represents a dynamic rigid simulation object in the physics SDK.

<h3>Creation</h3>
Instances of this class are created by calling #PxPhysics::createRigidDynamic() and deleted with #release().


<h3>Visualizations</h3>
\li #PxVisualizationParameter::eACTOR_AXES
\li #PxVisualizationParameter::eBODY_AXES
\li #PxVisualizationParameter::eBODY_MASS_AXES
\li #PxVisualizationParameter::eBODY_LIN_VELOCITY
\li #PxVisualizationParameter::eBODY_ANG_VELOCITY

@see PxRigidBody  PxPhysics.createRigidDynamic()  release()
*/

class PxRigidDynamic : public PxRigidBody
{
public:
	// Runtime modifications


/************************************************************************************************/
/** @name Kinematic Actors
*/

	/**
	\brief ���� ���迡�� �������� ����Ǵ� ���͸� �̵���ŵ�ϴ�.

	PxRigidBodyFlag::eKINEMATIC �÷��׸� ����Ͽ� ���� ���͸� Ű�׸�ƽ���� �����մϴ�.
	setRigidBodyFlag()�� ����ϼ���.

	�̵� ����� ��ü�� ���ϴ� �ڼ��� �̵���Ű�� �ӵ��� ����� �մϴ�.
	�̵��� ���� �ð� �ܰ� ���� ����� �� �ӵ��� 0���� ��ȯ�˴ϴ�.
	���� Ű�׸�ƽ ���͸� ����ؼ� ȣ���Ͽ� ��θ� ���� �̵��ϵ����ؾ� �մϴ�.

	�� �Լ��� ���� �ùķ��̼� �ܰ谡 ó���� ������ �̵� ����� �����ϱ⸸ �մϴ�.
	���� ȣ���� �ܼ��� ����� ��� ������ ����ϴ�.

	��� �׻� ������ ����˴ϴ�.

	\note:	�� �޼ҵ带 ����ϴ� ���� ���Ͱ� �̹� ���� �߰��Ǿ� ���� �ʰų�
			PxActorFlag::eDISABLE_SIMULATION �÷��װ� ������ ��쿡�� �߸��˴ϴ�.

	<b>������:</b>
			�� ȣ���� ���Ͱ� ���� ������ �����
			wake ī���͸� #PxSceneDesc::wakeCounterResetValue�� �����մϴ�.

	\param[in]	destination 
				Ű�׸�ƽ ������ ���ϴ� �ڼ��Դϴ�. ���� ������ ������. <b>����:</b> ��ü ��ȯ.

	@see getKinematicTarget() PxRigidBodyFlag setRigidBodyFlag()
	*/
	virtual		void				setKinematicTarget(const PxTransform& destination) = 0;

	/**
	\brief Get target pose of a kinematically controlled dynamic actor.

	\param[out] target Transform to write the target pose to. Only valid if the method returns true.
	\return True if the actor is a kinematically controlled dynamic and the target has been set, else False.

	@see setKinematicTarget() PxRigidBodyFlag setRigidBodyFlag()
	*/
	virtual		bool				getKinematicTarget(PxTransform& target)	const	= 0;


/************************************************************************************************/
/** @name Sleeping
*/

	/**
	\brief �� ��ü�� ���� �������� ���θ� ��ȯ�մϴ�.

	��ü�� ���� �ð� ���� �������� ������ �ùķ��̼ǿ��� ���ܵǾ� �ð��� �����մϴ�. �� ���¸� ���� ���¶�� �մϴ�.
	�׷��� ��ü�� �����ִ� ��ü�� ���� ���˵ǰų� ����ڰ� �Ӽ� �� �ϳ��� �����ϸ� �ڵ����� ����� ������,
	��ü ���� ��Ŀ������ ����ڿ��� �����ؾ� �մϴ�.

	�Ϲ�������, ���� ��ü ��ü�� ���� ���� �� �ϳ� �̻��� �����Ǹ� �����ִ� �������� ����˴ϴ�:

	\li ����� ī���Ͱ� ����Դϴ� (���� #setWakeCounter()).
	\li ���� �Ǵ� ���ӵ��� 0�� �ƴմϴ�.
	\li 0�� �ƴ� ���̳� ��ũ�� ����Ǿ����ϴ�.

	���� ��ü ��ü�� ���� ������ ���, ���� ���°� ����˴ϴ�:

	\li ����� ī���Ͱ� 0�Դϴ�.
	\li ���� �� ���ӵ��� 0�Դϴ�.
	\li ��� ���� �� ������Ʈ�� �����ϴ�.

	��ü�� ��鿡 ���ԵǸ� ���� ��� ������ �����Ǹ� ���� ���·� ���ֵǰ�, �׷��� ������ �����ִ� ���·� ó���˴ϴ�.

	PxScene::fetchResults() ȣ�� �� ��ü�� ���� ������ ���, ��ü�� �ڼ��� ������� �ʾ����� �����մϴ�.
	�� ������ ����Ͽ� ���� ��ü�� ��ȯ�� ������Ʈ���� ���� �� �ֽ��ϴ�.

	\note ��ǥ �ڼ��� �������� ���� ���, Ű�׸�ƽ ��ü�� ���� �����Դϴ� 
	(��ǥ �ڼ��� �������� ���� ��� �� ���� ���� �ùķ��̼� �ܰ谡 ������ ������ �����ִ� ���·� �����˴ϴ�). 
	��ǥ �ڼ��� ������ ���, ����� ī���ʹ� 0 �Ǵ� �缳�� �� #PxSceneDesc::wakeCounterResetValue�� �����Ǿ�
	���� ���ǿ� �ϰ����� �����մϴ�.

	\note ��ü�� ���� ��鿡 �߰����� ���� ��� �� �޼��带 ����ϴ� ���� ��ȿ���� �ʽ��ϴ�.

	\note �ùķ��̼��� ���� ���� ���� �� �޼��带 ����ϴ� ���� ������ �ʽ��ϴ�.

	\return ��ü�� ���� ������ ��� True�� ��ȯ�մϴ�.

	@see isSleeping() wakeUp() putToSleep() getSleepThreshold()
	*/
	virtual		bool				isSleeping() const = 0;

    /**
	\brief Sets the mass-normalized kinetic energy threshold below which an actor may go to sleep.

	Actors whose kinetic energy divided by their mass is below this threshold will be candidates for sleeping.

	<b>Default:</b> 5e-5f * PxTolerancesScale::speed * PxTolerancesScale::speed

	\param[in] threshold Energy below which an actor may go to sleep. <b>Range:</b> [0, PX_MAX_F32)

	@see isSleeping() getSleepThreshold() wakeUp() putToSleep() PxTolerancesScale
	*/
	virtual		void				setSleepThreshold(PxReal threshold) = 0;

	/**
	\brief Returns the mass-normalized kinetic energy below which an actor may go to sleep.

	\return The energy threshold for sleeping.

	@see isSleeping() wakeUp() putToSleep() setSleepThreshold()
	*/
	virtual		PxReal				getSleepThreshold() const = 0;

	 /**
	\brief Sets the mass-normalized kinetic energy threshold below which an actor may participate in stabilization.

	Actors whose kinetic energy divided by their mass is above this threshold will not participate in stabilization.

	This value has no effect if PxSceneFlag::eENABLE_STABILIZATION was not enabled on the PxSceneDesc.

	<b>Default:</b> 1e-5f * PxTolerancesScale::speed * PxTolerancesScale::speed

	\param[in] threshold Energy below which an actor may participate in stabilization. <b>Range:</b> [0,inf)

	@see  getStabilizationThreshold() PxSceneFlag::eENABLE_STABILIZATION
	*/
	virtual		void				setStabilizationThreshold(PxReal threshold) = 0;

	/**
	\brief Returns the mass-normalized kinetic energy below which an actor may participate in stabilization.

	Actors whose kinetic energy divided by their mass is above this threshold will not participate in stabilization. 

	\return The energy threshold for participating in stabilization.

	@see setStabilizationThreshold() PxSceneFlag::eENABLE_STABILIZATION
	*/
	virtual		PxReal				getStabilizationThreshold() const = 0;


	/**
	\brief Reads the PxRigidDynamic lock flags.

	See the list of flags #PxRigidDynamicLockFlag

	\return The values of the PxRigidDynamicLock flags.

	@see PxRigidDynamicLockFlag setRigidDynamicLockFlag()
	*/
	virtual		PxRigidDynamicLockFlags getRigidDynamicLockFlags() const = 0;

	/**
	\brief Raises or clears a particular rigid dynamic lock flag.

	See the list of flags #PxRigidDynamicLockFlag

	<b>Default:</b> no flags are set


	\param[in] flag		The PxRigidDynamicLockBody flag to raise(set) or clear. See #PxRigidBodyFlag.
	\param[in] value	The new boolean value for the flag.

	@see PxRigidDynamicLockFlag getRigidDynamicLockFlags()
	*/
	virtual		void				setRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum flag, bool value) = 0;
	virtual		void				setRigidDynamicLockFlags(PxRigidDynamicLockFlags flags) = 0;
	
	/**
	\brief Retrieves the linear velocity of an actor.

	\note It is not allowed to use this method while the simulation is running (except during PxScene::collide(),
	in PxContactModifyCallback or in contact report callbacks).

	\note The linear velocity is reported with respect to the rigid dynamic's center of mass and not the actor frame origin.

	\return The linear velocity of the actor.

	@see PxRigidDynamic.setLinearVelocity() getAngularVelocity()
	*/
	virtual		PxVec3			getLinearVelocity()		const = 0;

	/**
	\brief ��ü�� ���� �ӵ��� �����մϴ�.

	��ü�� �ӵ��� ����ؼ� ���� ������ ���, �߷��̳� ������ ���� ���� ��Ÿ���� ���� �� �ֽ��ϴ�.
	�̴� ���� ��ü�� �ӵ�/����� ���������� ������ ��ġ�� �����Դϴ�.

	<b>�⺻��:</b> (0.0, 0.0, 0.0)

	<b>���� ����:</b>	�� ȣ���� ��ü�� ���� ������ ���, autowake �Ű������� true(�⺻��)�̰ų�
						���ο� �ӵ��� 0�� �ƴ� ��� ��ü�� ����ϴ�.

	\note PxActorFlag::eDISABLE_SIMULATION�� ������ ��� �� �޼��带 ����ϴ� ���� ��ȿ���� �ʽ��ϴ�.

	\note ���� �ӵ��� ��ü ������ ������ �ƴ� ��ü �������� ���� �߽ɿ� ���� ����˴ϴ�.

	\param[in] linVel	���ο� ��ü�� ���� �ӵ��Դϴ�. <b>����:</b> �ӵ� ����
	\param[in] autowake ��ü�� ���� ������ ��� ������ ���θ� �����մϴ�. true�� ���,
						���� ����� ī���� ���� #PxSceneDesc::wakeCounterResetValue���� ������
						�ش� ���� �缳�� ������ ������ŵ�ϴ�.

	@see getLinearVelocity() setAngularVelocity()
	*/
	virtual		void			setLinearVelocity(const PxVec3& linVel, bool autowake = true) = 0;

	/**
	\brief Retrieves the angular velocity of the actor.

	\note It is not allowed to use this method while the simulation is running (except during PxScene::collide(),
	in PxContactModifyCallback or in contact report callbacks).

	\return The angular velocity of the actor.

	@see PxRigidDynamic.setAngularVelocity() getLinearVelocity()
	*/
	virtual		PxVec3			getAngularVelocity()	const = 0;

	/**
	\brief ��ü�� ���ӵ��� �����մϴ�.

	��ü�� ���ӵ��� ����ؼ� ���� ������ ���, ������ ���� ���� ��ü�� ȸ����Ű�� ���� �� �ֽ��ϴ�.
	�̴� ���� ��ü�� �ӵ�/������� ���������� ������ ��ġ�� �����Դϴ�.

	<b>�⺻��:</b> (0.0, 0.0, 0.0)

	<b>���� ����:</b> �� ȣ���� ��ü�� ���� ������ ���,
	autowake �Ű������� true(�⺻��)�̰ų� ���ο� �ӵ��� 0�� �ƴ� ��� ��ü�� ����ϴ�.

	\note PxActorFlag::eDISABLE_SIMULATION�� ������ ��� �� �޼��带 ����ϴ� ���� ��ȿ���� �ʽ��ϴ�.

	\param[in] angVel	���ο� ��ü�� ���ӵ��Դϴ�. <b>����:</b> ���ӵ� ����
	\param[in] autowake ��ü�� ���� ������ ��� ������ ���θ� �����մϴ�.
						true�� ���, ���� ����� ī���� ���� #PxSceneDesc::wakeCounterResetValue���� ������
						�ش� ���� �缳�� ������ ������ŵ�ϴ�.

	@see getAngularVelocity() setLinearVelocity()
	*/
	virtual		void			setAngularVelocity(const PxVec3& angVel, bool autowake = true) = 0;
	/**
	\brief Sets the wake counter for the actor.

	The wake counter value determines the minimum amount of time until the body can be put to sleep. Please note
	that a body will not be put to sleep if the energy is above the specified threshold (see #setSleepThreshold())
	or if other awake bodies are touching it.

	\note Passing in a positive value will wake the actor up automatically.

	\note It is invalid to use this method for kinematic actors since the wake counter for kinematics is defined
	based on whether a target pose has been set (see the comment in #isSleeping()).

	\note It is invalid to use this method if PxActorFlag::eDISABLE_SIMULATION is set.

	<b>Default:</b> 0.4 (which corresponds to 20 frames for a time step of 0.02)

	\param[in] wakeCounterValue Wake counter value. <b>Range:</b> [0, PX_MAX_F32)

	@see isSleeping() getWakeCounter()
	*/
	virtual		void				setWakeCounter(PxReal wakeCounterValue) = 0;

	/**
	\brief Returns the wake counter of the actor.

	\note It is not allowed to use this method while the simulation is running.

	\return The wake counter of the actor.

	@see isSleeping() setWakeCounter()
	*/
	virtual		PxReal				getWakeCounter() const = 0;

	/**
	\brief Wakes up the actor if it is sleeping.

	The actor will get woken up and might cause other touching actors to wake up as well during the next simulation step.

	\note This will set the wake counter of the actor to the value specified in #PxSceneDesc::wakeCounterResetValue.

	\note It is invalid to use this method if the actor has not been added to a scene already or if PxActorFlag::eDISABLE_SIMULATION is set.

	\note It is invalid to use this method for kinematic actors since the sleep state for kinematics is defined
	based on whether a target pose has been set (see the comment in #isSleeping()).

	@see isSleeping() putToSleep()
	*/
	virtual		void				wakeUp() = 0;

	/**
	\brief ���͸� ���� ���·� ����ϴ�.

	���� �ٸ� �������� �ʴ� ���Ϳ� ���� ��ġ���� �ʴ´ٸ�, 
	���� �ùķ��̼� �ܰ迡���� ���ʹ� ���� ���·� �����˴ϴ�.

	����: ����� ���� ��������, ������ �ӵ��� ���� �ִ� ī����(wake counter)�� 0���� �����˴ϴ�.

	����: �� �޼���� ���Ͱ� �̹� �ùķ��̼ǿ� �߰����� �ʾҰų� PxActorFlag::eDISABLE_SIMULATION�� ������ ��쿡 ����� �� �����ϴ�.

	����: Ű�׸�ƽ ������ ��� �� �޼��带 ����ϴ� ���� ��ȿ�Դϴ�. 
	Ű�׸�ƽ ������ ���� ���´� ��� ��� �����Ǿ����� ���ο� ���� ���ǵ˴ϴ�(����: #isSleeping()�� �ּ�).

	@see isSleeping() wakeUp()
	*/
	virtual		void				putToSleep() = 0;

/************************************************************************************************/

	/**
	\brief ��ü�� ���� �ֹ� �ݺ� Ƚ���� �����մϴ�.

	�ֹ� �ݺ� Ƚ���� ����Ʈ�� ������ �󸶳� ��Ȯ�ϰ� �ذ�Ǵ����� �����մϴ�.
	����Ʈ�� ����� ��ü�� �����ϰų� �ұ�Ģ�ϰ� �����ϴ� ���, ��ġ �ݺ� Ƚ���� ���̸� �������� ������ �� �ֽ��ϴ�.

	�����ϴ� ��ü�� �ʹ� ���ϰ� �и��Ǵ� ���, �ӵ� �ݺ� Ƚ���� �ø��ʽÿ�.
	�ӵ� �ݺ� Ƚ���� �������� �����ϴ� ��ü�� ����� ��Ż �ӵ��� �����¿� ���� �ùٸ� ���� �� ��������ϴ�.

	<b>�⺻��:</b> ��ġ �ݺ� 4ȸ, �ӵ� �ݺ� 1ȸ

	\param[in] minPositionIters �� ��ü�� ���� �ֹ��� �����ؾ� �ϴ� ��ġ �ݺ� Ƚ���Դϴ�. <b>����:</b> [1,255]
	\param[in] minVelocityIters �� ��ü�� ���� �ֹ��� �����ؾ� �ϴ� �ӵ� �ݺ� Ƚ���Դϴ�. <b>����:</b> [0,255]

	@see getSolverIterationCounts()
	*/
	virtual		void				setSolverIterationCounts(PxU32 minPositionIters, PxU32 minVelocityIters = 1) = 0;

	/**
	\brief Retrieves the solver iteration counts.

	@see setSolverIterationCounts()
	*/
	virtual		void				getSolverIterationCounts(PxU32& minPositionIters, PxU32& minVelocityIters) const = 0;

	/**
	\brief Retrieves the force threshold for contact reports.

	The contact report threshold is a force threshold. If the force between 
	two actors exceeds this threshold for either of the two actors, a contact report 
	will be generated according to the contact report threshold flags provided by
	the filter shader/callback.
	See #PxPairFlag.

	The threshold used for a collision between a dynamic actor and the static environment is 
    the threshold of the dynamic actor, and all contacts with static actors are summed to find 
    the total normal force.

	<b>Default:</b> PX_MAX_F32

	\return Force threshold for contact reports.

	@see setContactReportThreshold PxPairFlag PxSimulationFilterShader PxSimulationFilterCallback
	*/
	virtual     PxReal				getContactReportThreshold() const = 0;

	/**
	\brief Sets the force threshold for contact reports.

	See #getContactReportThreshold().

	\param[in] threshold Force threshold for contact reports. <b>Range:</b> [0, PX_MAX_F32)

	@see getContactReportThreshold PxPairFlag
	*/
	virtual     void				setContactReportThreshold(PxReal threshold) = 0;

	virtual		const char*			getConcreteTypeName() const { return "PxRigidDynamic"; }

protected:
	PX_INLINE						PxRigidDynamic(PxType concreteType, PxBaseFlags baseFlags) : PxRigidBody(concreteType, baseFlags) { }
	PX_INLINE						PxRigidDynamic(PxBaseFlags baseFlags) : PxRigidBody(baseFlags) {}
	virtual							~PxRigidDynamic() {}
	virtual		bool				isKindOf(const char* name) const { PX_IS_KIND_OF(name, "PxRigidDynamic", PxRigidBody); }

};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
