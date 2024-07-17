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

#ifndef PX_RIGID_ACTOR_H
#define PX_RIGID_ACTOR_H
/** \addtogroup physics
@{
*/

#include "PxActor.h"
#include "PxShape.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxConstraint;

/**
\brief PxRigidActor represents a base class shared between dynamic and static rigid bodies in the physics SDK.

PxRigidActor objects specify the geometry of the object by defining a set of attached shapes (see #PxShape).

@see PxActor
*/
class PxRigidActor : public PxActor
{
public:
	/**
	*\brief ��ü ���� ��ü�� �����մϴ�.

	���� ���Ϳ� ���õ� ��� ������ �����մϴ�.

	���͸� �����ϸ� ���Ϳ� ����� ��� ��ü(����Ʈ ��� ���� ���� ���̴�)�� ������ ��Ĩ�ϴ�.
	�̷��� ����� ��ü�� �� ���� �� �Ǵ� ����ڰ� ��������� �̷��� ��ü�鿡 ���� release()�� ȣ���Ͽ� �����˴ϴ�.
	���� ��ü�� �����ϱ� ���� �׻� ���͸� �����ϴ� ��� ��ü�� �����ϴ� ���� �����ϴ�.
	���� ����� ��ü�� ����� �˻��ϴ� ���� �Ұ����մϴ�.

	<b>������:</b> �� ȣ���� ������ ���Ϳ� ���� �Ǵ� ���������� �����ϴ� ��� ������ ���͸� ����ϴ�.

	#PxActor::release()�� ȣ���ϹǷ� �ش� �޼����� ���� Ȯ���ϴ� ���� �����ϴ�.

	@see PxActor::release()
	*/
	virtual		void			release() = 0;

	/**
	\brief Returns the internal actor index.

	\warning	This is only defined for actors that have been added to a scene.

	\return		The internal actor index, or 0xffffffff if the actor is not part of a scene.
	*/
	virtual PxU32				getInternalActorIndex() const = 0;

/************************************************************************************************/
/** @name Global Pose Manipulation
*/

	/**
	\brief Retrieves the actors world space transform.

	The getGlobalPose() method retrieves the actor's current actor space to world space transformation.

	\note It is not allowed to use this method while the simulation is running (except during PxScene::collide(),
	in PxContactModifyCallback or in contact report callbacks).

	\return Global pose of object.

	@see PxRigidDynamic.setGlobalPose() PxRigidStatic.setGlobalPose()
	*/
	virtual		PxTransform 	getGlobalPose()		const = 0;

	/**
	\brief ��ü�� �ڼ��� ���� ������ �����ϴ� �޼����Դϴ�.

	�� �޼���� ��ü �������� ���� ���������� ��ȯ�� ��� �����մϴ�.

	�� �޼���� �ַ� ���� ��ü(���� #PxRigidDynamic)�� ���� ���˴ϴ�. ���� ��ü�� �� �޼��带 ȣ���ϸ�
	���� ����ȭ ������ �����ؾ� �ϹǷ� ���� ���ϰ� �߻��� �� �ֽ��ϴ�.
	����, ���� ��ü�� �̵���Ű�� ���� ��ü�� ����Ʈ�� �ùٸ��� ��ȣ�ۿ����� ���� �� �ֽ��ϴ�.

	��ü�� ��ġ�� ���� �����ϰ� ���� ��ü �� ����Ʈ�� �ùٸ��� ��ȣ�ۿ��ϵ��� �Ϸ���,
	PxRigidBodyFlag::eKINEMATIC �÷��װ� ������ ���� ��ü�� ������ �� setKinematicTarget() ����� ����Ͽ� ��θ� �����Ͻʽÿ�.

	���� ��ü�� �̵��� ���� �� �޼��带 ����ϴ� ���� �����Ͻʽÿ�. ������ �� ������ ���Ͻʽÿ�:

	\li �ٸ� ��ü�� ��ġ���� ��ü�� �̵���Ű�� �� (��ȿ���� ���� ���� ����)

	\li ����Ʈ�� ����� ��ü�� �ٸ� ��ü�κ��� �ָ� �̵���Ű�� �� (����Ʈ ���� �߻�)

	\note ��ü�� ���� ��鿡 �߰����� ���� #PxPruningStructure�� �Ϻ��� ��� �� �޼��带 ����ϴ� ���� ������ �ʽ��ϴ�.

	<b>���� ����:</b> �� ȣ���� ���� ��ü�� ���� ������ ��� autowake �Ű������� true(�⺻��)�� �� ����� �մϴ�.

	\param[in] pose		��ü�� ���� �����ӿ��� �۷ι� ������������ ��ȯ�Դϴ�. <b>����:</b> ��ü ��ȯ.
	\param[in] autowake ��ü�� ������ ��� ����� ���� ���θ� �����մϴ�.
						�� �Ű������� ���� �Ǵ� Ű�׸�ƽ ��ü���� ������ ���� �ʽ��ϴ�.
						true�̰� ���� ����� ī���� ���� #PxSceneDesc::wakeCounterResetValue����
						������ �ش� ���� �缳�� ������ ������ŵ�ϴ�.

	@see getGlobalPose()
	*/
	virtual		void			setGlobalPose(const PxTransform& pose, bool autowake = true) = 0;

/************************************************************************************************/
/** @name Shapes
*/

	/**
	\brief ����� ���Ϳ� �����մϴ�.

	�� ȣ���� ����� ���� Ƚ���� ������ŵ�ϴ�.

	\note ���� ��ü ������ ���� �Ӽ��� �ڵ����� ������� �ʾ� ���ο� ����� �Ͻ��ϴ� �� ���� ������ �ݿ����� �ʽ��ϴ�. �� ȣ�� ���Ŀ��� PhysX Ȯ�� �޼��� #PxRigidBodyExt::updateMassAndInertia()�� ȣ���Ͽ� �̸� �����ϼ���.

	eSIMULATION_SHAPE�� ������ �ﰢ�� �޽�, ������ �ʵ� �Ǵ� ��� ������Ʈ�� ����� ��-Ű�׸�ƽ PxRigidDynamic �ν��Ͻ��� ���� �������� �ʽ��ϴ�.

	<b>�޸� ����:</b> ���͸� �ڵ����� ������ <b>�ʽ��ϴ�.</b>

	\param[in] shape ������ ����Դϴ�.

	\return �����ϸ� true.
	*/
	virtual bool				attachShape(PxShape& shape) = 0;


	/**
	\brief ���Ϳ��� ����� �и��մϴ�.

	�̴� ���� PxShape�� ���� Ƚ���� ���ҽ�Ű��, ���� Ƚ���� 0�̵Ǹ� �ش� ����� �����˴ϴ�.

	<b>�޸� ����:</b> ���͸� �ڵ����� ������ <b>�ʽ��ϴ�.</b>

	\param[in] shape �и��� ����Դϴ�.
	\param[in] wakeOnLostTouch ���� �����ӿ��� �����ߴ� ��ü�� ���� �����ӿ��� ����� ���θ� �����մϴ�.
	PxArticulationReducedCoordinate �� PxRigidActor �������� ����˴ϴ�.
	*/
	virtual void				detachShape(PxShape& shape, bool wakeOnLostTouch = true) = 0;

	/**
	\brief Returns the number of shapes assigned to the actor.

	You can use #getShapes() to retrieve the shape pointers.

	\return Number of shapes associated with this actor.

	@see PxShape getShapes()
	*/
	virtual		PxU32			getNbShapes()		const	= 0;

	/**
	\brief �ش� ���Ϳ� ���� ��� ��� �����͸� �˻��մϴ�.

	�̷��� ����� ���Ͱ� �浹 ������ ����ϴ� ����Դϴ�.

	��� �������� ���� #getNbShapes()�� ȣ���Ͽ� �˻��� �� �ֽ��ϴ�.

	����: #PxShape::release()�� ����Ͽ� ����� �����ϸ� ���ŵ� ����� �����Ͱ� ��ȿȭ�˴ϴ�.

	\param[out]	userBuffer	��� �����͸� ������ �����Դϴ�.
	\param[in]	bufferSize	������ ����� ������ ũ���Դϴ�.
	\param[in]	startIndex	�˻��� ù ��° ��� �������� �ε����Դϴ�.
	\return ���ۿ� ������ ��� �������� ���Դϴ�.

	@see PxShape getNbShapes() PxShape::release()
	*/
	virtual		PxU32			getShapes(PxShape** userBuffer, PxU32 bufferSize, PxU32 startIndex=0)			const	= 0;

/************************************************************************************************/
/** @name Constraints
*/

	/**
	\brief Returns the number of constraint shaders attached to the actor.

	You can use #getConstraints() to retrieve the constraint shader pointers.

	\return Number of constraint shaders attached to this actor.

	@see PxConstraint getConstraints()
	*/
	virtual		PxU32			getNbConstraints()		const	= 0;

	/**
	\brief Retrieve all the constraint shader pointers belonging to the actor.

	You can retrieve the number of constraint shader pointers by calling #getNbConstraints()

	Note: Removing constraint shaders with #PxConstraint::release() will invalidate the pointer of the released constraint.

	\param[out] userBuffer The buffer to store the constraint shader pointers.
	\param[in] bufferSize Size of provided user buffer.
	\param[in] startIndex Index of first constraint pointer to be retrieved
	\return Number of constraint shader pointers written to the buffer.

	@see PxConstraint getNbConstraints() PxConstraint::release()
	*/
	virtual		PxU32			getConstraints(PxConstraint** userBuffer, PxU32 bufferSize, PxU32 startIndex=0)		const	= 0;

protected:
	PX_INLINE					PxRigidActor(PxType concreteType, PxBaseFlags baseFlags) : PxActor(concreteType, baseFlags) {}
	PX_INLINE					PxRigidActor(PxBaseFlags baseFlags) : PxActor(baseFlags) {}
	virtual						~PxRigidActor()	{}
	virtual		bool			isKindOf(const char* name)	const	{	PX_IS_KIND_OF(name, "PxRigidActor", PxActor); }
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
