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

#ifndef PX_SHAPE_H
#define PX_SHAPE_H
/** \addtogroup physics
@{
*/

#include "PxPhysXConfig.h"
#include "common/PxBase.h"
#include "geometry/PxGeometry.h"
#include "geometry/PxGeometryHelpers.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxBoxGeometry;
class PxSphereGeometry;
class PxCapsuleGeometry;
class PxPlaneGeometry;
class PxConvexMeshGeometry;
class PxTriangleMeshGeometry;
class PxTetrahedronMeshGeometry;
class PxHeightFieldGeometry;
class PxParticleSystemGeometry;
class PxHairSystemGeometry;
class PxRigidActor;
struct PxFilterData;
class PxBaseMaterial;
class PxMaterial;
class PxFEMSoftBodyMaterial;
class PxFEMClothMaterial;

/**
\brief Flags which affect the behavior of PxShapes.

@see PxShape PxShape.setFlag()
*/
struct PxShapeFlag
{
	enum Enum
	{
		/**
		\brief 물리 시뮬레이션에서 이 모양은 충돌에 참여합니다.

		\note eSIMULATION_SHAPE 및 eTRIGGER_SHAPE 플래그를 동시에 올리는 것은 불법입니다.
		이러한 플래그 중 하나가 이미 올려져 있으면 다른 하나를 올리려는 시도는 거부됩니다.
		eSIMULATION_SHAPE를 올리려면 먼저 eTRIGGER_SHAPE가 이미 낮아져 있는지 확인하십시오.

		\note 해당 액터에 대한 시뮬레이션 비활성화가되어 있으면이 플래그는 효과가 없습니다 (참조 : #PxActorFlag::eDISABLE_SIMULATION).

		@see PxSimulationEventCallback.onContact() PxScene.setSimulationEventCallback() PxShape.setFlag(), PxShape.setFlags()
		*/
		eSIMULATION_SHAPE = (1 << 0),

		/**
		\brief 이 모양은 장면 쿼리 (레이 캐스트, 겹침 테스트, 스위프 등)에 참여합니다.
		*/
		eSCENE_QUERY_SHAPE = (1 << 1),

		/**
		\brief 이 모양은 다른 모양이 그 볼륨에 들어오거나 나갈 때마다 보고서를 보낼 수있는 트리거입니다.

		\note 삼각형 메쉬 및 높이 필드는 트리거가 될 수 없습니다. 이러한 경우 모양 생성이 실패합니다.

		\note 트리거로 표시된 모양은 다른 객체와 충돌하지 않습니다. 객체가 트리거 모양 및 충돌 모양 두 가지 역할을해야하는 경우
		두 가지 모양이있는 강체 본을 만드십시오. 하나는 트리거 모양이고 다른 하나는 충돌 모양입니다.
		eTRIGGER_SHAPE 및 eSIMULATION_SHAPE 플래그를 단일 PxShape 인스턴스에서 동시에 올리는 것은 불법입니다.
		이러한 플래그 중 하나가 이미 올려져 있으면 다른 하나를 올리려는 시도는 거부됩니다.
		eTRIGGER_SHAPE 플래그를 올리려면 먼저 eSIMULATION_SHAPE 플래그가 이미 낮아져 있는지 확인하십시오.

		\note 트리거 모양은 더 이상 다른 트리거 모양과의 상호 작용에 대한 알림 이벤트를 보내지 않습니다.

		\note 트리거로 표시된 모양은 eSCENE_QUERY_SHAPE 플래그가 설정되어 있는 경우 씬 쿼리에 참여할 수 있습니다.

		\note 해당 액터에 대한 시뮬레이션 비활성화가되어 있으면이 플래그는 효과가 없습니다 (참조 : #PxActorFlag::eDISABLE_SIMULATION).

		@see PxSimulationEventCallback.onTrigger() PxScene.setSimulationEventCallback() PxShape.setFlag(), PxShape.setFlags()
		*/
		eTRIGGER_SHAPE = (1 << 2),

		/**
		\brief 이 모양에 대한 디버그 렌더러를 활성화합니다.

		@see PxScene.getRenderBuffer() PxRenderBuffer PxVisualizationParameter
		*/
		eVISUALIZATION = (1 << 3)

	};
};

/**
\brief collection of set bits defined in PxShapeFlag.

@see PxShapeFlag
*/
typedef PxFlags<PxShapeFlag::Enum,PxU8> PxShapeFlags;
PX_FLAGS_OPERATORS(PxShapeFlag::Enum,PxU8)

/**
\brief Abstract class for collision shapes.

Shapes are shared, reference counted objects.

An instance can be created by calling the createShape() method of the PxRigidActor class, or
the createShape() method of the PxPhysics class.

<h3>Visualizations</h3>
\li PxVisualizationParameter::eCOLLISION_AABBS
\li PxVisualizationParameter::eCOLLISION_SHAPES
\li PxVisualizationParameter::eCOLLISION_AXES

@see PxPhysics.createShape() PxRigidActor.createShape() PxBoxGeometry PxSphereGeometry PxCapsuleGeometry PxPlaneGeometry PxConvexMeshGeometry
PxTriangleMeshGeometry PxHeightFieldGeometry
*/
class PxShape : public PxRefCounted
{
public:
	/**
	\brief Decrements the reference count of a shape and releases it if the new reference count is zero.

	Note that in releases prior to PhysX 3.3 this method did not have reference counting semantics and was used to destroy a shape 
	created with PxActor::createShape(). In PhysX 3.3 and above, this usage is deprecated, instead, use PxRigidActor::detachShape() to detach
	a shape from an actor. If the shape to be detached was created with PxActor::createShape(), the actor holds the only counted reference,
	and so when the shape is detached it will also be destroyed. 

	@see PxRigidActor::createShape() PxPhysics::createShape() PxRigidActor::attachShape() PxRigidActor::detachShape()
	*/
	virtual		void	release() = 0;

	/**
	\brief 도형의 기하학을 조절합니다.

	\note 전달된 기하학의 유형은 도형의 기하학 유형과 일치해야 합니다.
	\note 도형의 기하학 유형을 변경하는 것은 허용되지 않습니다.
	\note 이 함수는 객체가 이전 또는 새 기하학 위에 안정적으로 존재할 때 올바르고 연속된 동작을 보장하지 않습니다.

	\param[in] geometry 도형의 새로운 기하학입니다.

	@see PxGeometry PxGeometryType getGeometryType()
	*/
	virtual		void	setGeometry(const PxGeometry& geometry) = 0;

	/**
	\brief Retrieve a reference to the shape's geometry.

	\warning The returned reference has the same lifetime as the PxShape it comes from.

	\return	Reference to internal PxGeometry object.

	@see PxGeometry PxGeometryType getGeometryType() setGeometry()
	*/
	virtual		const PxGeometry&	getGeometry() const = 0;

	/**
	\brief Retrieves the actor which this shape is associated with.

	\return The actor this shape is associated with, if it is an exclusive shape, else NULL

	@see PxRigidStatic, PxRigidDynamic, PxArticulationLink
	*/
	virtual		PxRigidActor*	getActor() const = 0;

/************************************************************************************************/

/** @name Pose Manipulation
*/
//@{

	/**
	\brief 도형을 액터 공간에서의 포즈로 설정합니다.
	즉, 해당 도형이 연결된 액터에 대한 상대적인 위치를 나타냅니다.

	기본적으로 이 변환은 항등 변환입니다.

	로컬 포즈는 도형의 속성이며, 따라서 해당 도형이 연결된 모든 액터에 적용됩니다.

	<b>휴면 상태:</b> 연관된 액터를 자동으로 깨우지 <b>않습니다.</b>

	<i>참고:</i> 소유 액터의 관성 속성을 자동으로 업데이트하지 않습니다(해당되는 경우);
	이를 수행하려면 PhysX 확장 메서드 #PxRigidBodyExt::updateMassAndInertia()를 사용하세요.

	<b>기본값:</b> 항등 변환

	\param[in] pose 액터 프레임에서 도형 프레임으로의 새로운 변환입니다. <b>범위:</b> 강체 변환

	@see getLocalPose()
	*/
	virtual		void	setLocalPose(const PxTransform& pose)		= 0;

	/**
	\brief 액터 공간에서의 도형의 포즈를 검색합니다.
	즉, 해당 도형이 소유한 액터를 기준으로 상대적인 위치를 나타냅니다.

	기본적으로 이 변환은 항등 변환입니다.

	\return 액터 프레임을 기준으로 한 도형의 포즈.

	@see setLocalPose()
	*/
	virtual		PxTransform	getLocalPose()	const	= 0;

//@}
/************************************************************************************************/

/** @name Collision Filtering
*/
//@{

	/**
	\brief Sets the user definable collision filter data.
	
	<b>Sleeping:</b> Does wake up the actor if the filter data change causes a formerly suppressed
	collision pair to be enabled.

	<b>Default:</b> (0,0,0,0)

	@see getSimulationFilterData() 
	*/
	virtual		void	setSimulationFilterData(const PxFilterData& data)	= 0;

	/**
	\brief Retrieves the shape's collision filter data.

	@see setSimulationFilterData() 
	*/
	virtual		PxFilterData	getSimulationFilterData()	const	= 0;

	/**
	\brief Sets the user definable query filter data.

	<b>Default:</b> (0,0,0,0)

	@see getQueryFilterData() 
	*/
	virtual		void	setQueryFilterData(const PxFilterData& data)	= 0;

	/**
	 *
	\brief Retrieves the shape's Query filter data.

	@see setQueryFilterData() 
	*/
	virtual		PxFilterData	getQueryFilterData()	const	= 0;

//@}
/************************************************************************************************/

	/**
	\brief Assigns material(s) to the shape. Will remove existing materials from the shape.
	
	<b>Sleeping:</b> Does <b>NOT</b> wake the associated actor up automatically.

	\param[in] materials List of material pointers to assign to the shape. See #PxMaterial
	\param[in] materialCount The number of materials provided.

	@see PxPhysics.createMaterial() getMaterials() 
	*/
	virtual		void	setMaterials(PxMaterial*const* materials, PxU16 materialCount) = 0;

	/**
	\brief Assigns FEM soft body material(s) to the shape. Will remove existing materials from the shape.

	<b>Sleeping:</b> Does <b>NOT</b> wake the associated actor up automatically.

	\param[in] materials List of material pointers to assign to the shape. See #PxFEMSoftBodyMaterial
	\param[in] materialCount The number of materials provided.

	@see PxPhysics.createFEMSoftBodyMaterial() getSoftBodyMaterials() 
	*/
	virtual		void	setSoftBodyMaterials(PxFEMSoftBodyMaterial*const* materials, PxU16 materialCount) = 0;

	/**
	\brief Assigns FEM cloth material(s) to the shape. Will remove existing materials from the shape.
	\warning Feature under development, only for internal usage.

	<b>Sleeping:</b> Does <b>NOT</b> wake the associated actor up automatically.

	\param[in] materials List of material pointers to assign to the shape. See #PxFEMClothMaterial
	\param[in] materialCount The number of materials provided.

	@see PxPhysics.createFEMClothMaterial() getClothMaterials() 
	*/
	virtual		void	setClothMaterials(PxFEMClothMaterial*const* materials, PxU16 materialCount) = 0;

	/**
	\brief Returns the number of materials assigned to the shape.

	You can use #getMaterials() to retrieve the material pointers.

	\return Number of materials associated with this shape.

	@see PxMaterial getMaterials()
	*/
	virtual		PxU16	getNbMaterials()		const	= 0;

	/**
	\brief Retrieve all the material pointers associated with the shape.

	You can retrieve the number of material pointers by calling #getNbMaterials()

	Note: The returned data may contain invalid pointers if you release materials using #PxMaterial::release().

	\param[out] userBuffer The buffer to store the material pointers.
	\param[in] bufferSize Size of provided user buffer.
	\param[in] startIndex Index of first material pointer to be retrieved
	\return Number of material pointers written to the buffer.

	@see PxMaterial getNbMaterials() PxMaterial::release()
	*/
	virtual		PxU32	getMaterials(PxMaterial** userBuffer, PxU32 bufferSize, PxU32 startIndex=0) const = 0;

	/**
	\brief Retrieve all the FEM soft body material pointers associated with the shape.

	You can retrieve the number of material pointers by calling #getNbMaterials()

	Note: The returned data may contain invalid pointers if you release materials using #PxMaterial::release().

	\param[out] userBuffer The buffer to store the material pointers.
	\param[in] bufferSize Size of provided user buffer.
	\param[in] startIndex Index of first material pointer to be retrieved
	\return Number of material pointers written to the buffer.

	@see PxFEMSoftBodyMaterial getNbMaterials() PxMaterial::release()
	*/
	virtual		PxU32	getSoftBodyMaterials(PxFEMSoftBodyMaterial** userBuffer, PxU32 bufferSize, PxU32 startIndex = 0) const = 0;

	/**
	\brief Retrieve all the FEM cloth material pointers associated with the shape.
	\warning Feature under development, only for internal usage.

	You can retrieve the number of material pointers by calling #getNbMaterials()

	Note: The returned data may contain invalid pointers if you release materials using #PxMaterial::release().

	\param[out] userBuffer The buffer to store the material pointers.
	\param[in] bufferSize Size of provided user buffer.
	\param[in] startIndex Index of first material pointer to be retrieved
	\return Number of material pointers written to the buffer.

	@see PxFEMClothMaterial getNbMaterials() PxMaterial::release()
	*/
	virtual		PxU32	getClothMaterials(PxFEMClothMaterial** userBuffer, PxU32 bufferSize, PxU32 startIndex = 0) const = 0;

	/**
	\brief Retrieve material from given triangle index.

	The input index is the internal triangle index as used inside the SDK. This is the index
	returned to users by various SDK functions such as raycasts.
	
	This function is only useful for triangle meshes or heightfields, which have per-triangle
	materials. For other shapes or SDF triangle meshes, the function returns the single material
	associated with the	shape, regardless of the index.

	\param[in] faceIndex The internal triangle index whose material you want to retrieve.
	\return Material from input triangle

	\note If faceIndex value of 0xFFFFffff is passed as an input for mesh and heightfield shapes, this function will issue a warning and return NULL.
	\note Scene queries set the value of PxQueryHit::faceIndex to 0xFFFFffff whenever it is undefined or does not apply.

	@see PxMaterial getNbMaterials() PxMaterial::release()
	*/
	virtual		PxBaseMaterial*		getMaterialFromInternalFaceIndex(PxU32 faceIndex) const = 0;

	/**
	\brief Sets the contact offset.

	Shapes whose distance is less than the sum of their contactOffset values will generate contacts. The contact offset must be positive and
	greater than the rest offset. Having a contactOffset greater than than the restOffset allows the collision detection system to
	predictively enforce the contact constraint even when the objects are slightly separated. This prevents jitter that would occur
	if the constraint were enforced only when shapes were within the rest distance.

	<b>Default:</b> 0.02f * PxTolerancesScale::length

	<b>Sleeping:</b> Does <b>NOT</b> wake the associated actor up automatically.

	\param[in] contactOffset <b>Range:</b> [maximum(0,restOffset), PX_MAX_F32)

	@see getContactOffset PxTolerancesScale setRestOffset
	*/
	virtual		void	setContactOffset(PxReal contactOffset)	= 0;

	/**
	\brief Retrieves the contact offset. 

	\return The contact offset of the shape.

	@see setContactOffset()
	*/
	virtual		PxReal	getContactOffset() const	= 0;

	/**
	\brief Sets the rest offset. 

	Two shapes will come to rest at a distance equal to the sum of their restOffset values. If the restOffset is 0, they should converge to touching 
	exactly.  Having a restOffset greater than zero is useful to have objects slide smoothly, so that they do not get hung up on irregularities of 
	each others' surfaces.

	<b>Default:</b> 0.0f

	<b>Sleeping:</b> Does <b>NOT</b> wake the associated actor up automatically.

	\param[in] restOffset	<b>Range:</b> (-PX_MAX_F32, contactOffset)

	@see getRestOffset setContactOffset
	*/
	virtual		void	setRestOffset(PxReal restOffset)	= 0;

	/**
	\brief Retrieves the rest offset. 

	\return The rest offset of the shape.

	@see setRestOffset()
	*/
	virtual		PxReal	getRestOffset() const	= 0;

	/**
	\brief Sets the density used to interact with fluids.

	To be physically accurate, the density of a rigid body should be computed as its mass divided by its volume. To
	simplify tuning the interaction of fluid and rigid bodies, the density for fluid can differ from the real density. This
	allows to create floating bodies, even if they are supposed to sink with their mass and volume.

	<b>Default:</b> 800.0f

	\param[in] densityForFluid	<b>Range:</b> (0, PX_MAX_F32)

	@see getDensityForFluid
	*/
	virtual		void	setDensityForFluid(PxReal densityForFluid)	= 0;

	/**
	\brief Retrieves the density used to interact with fluids.

	\return The density of the body when interacting with fluid.

	@see setDensityForFluid()
	*/
	virtual		PxReal	getDensityForFluid() const	= 0;

	/**
	\brief Sets torsional patch radius.
	
	This defines the radius of the contact patch used to apply torsional friction. If the radius is 0 (and minTorsionalPatchRadius
	is 0 too, see #setMinTorsionalPatchRadius), no torsional friction will be applied. If the radius is > 0, some torsional friction
	will be applied. This is proportional to the penetration depth so, if the shapes are separated or penetration is zero, no
	torsional friction will be applied. It is used to approximate rotational friction introduced by the compression of contacting surfaces.

	\note Will only be active, if the friction patch has a single anchor point only. This is for example the case, if a contact patch
	      has a single contact point.

	\note Only supported in combination with solver type PxSolverType::eTGS.

	<b>Default:</b> 0.0

	\param[in] radius	<b>Range:</b> [0, PX_MAX_F32)
	*/
	virtual		void	setTorsionalPatchRadius(PxReal radius) = 0;

	/**
	\brief Gets torsional patch radius.

	See #setTorsionalPatchRadius for more info.

	\return The torsional patch radius of the shape.
	*/
	virtual		PxReal	getTorsionalPatchRadius() const = 0;

	/**
	\brief Sets minimum torsional patch radius.

	This defines the minimum radius of the contact patch used to apply torsional friction. If the radius is 0, the amount of torsional friction
	that will be applied will be entirely dependent on the value of torsionalPatchRadius. 
	
	If the radius is > 0, some torsional friction will be applied regardless of the value of torsionalPatchRadius or the amount of penetration.

	\note Will only be active in certain cases, see #setTorsionalPatchRadius for details.

	<b>Default:</b> 0.0

	\param[in] radius	<b>Range:</b> [0, PX_MAX_F32)
	*/
	virtual		void	setMinTorsionalPatchRadius(PxReal radius) = 0;

	/**
	\brief Gets minimum torsional patch radius.

	See #setMinTorsionalPatchRadius for more info.

	\return The minimum torsional patch radius of the shape.
	*/
	virtual		PxReal	getMinTorsionalPatchRadius() const = 0;

	/**
	\brief Gets internal shape id

	The internal shape id can be used to reference a specific shape when processing data on the gpu.
	
	\return The shape id

	@see PxScene evaluateSDFDistances()
	*/
	virtual		PxU32	getInternalShapeIndex() const = 0;

/************************************************************************************************/

	/**
	\brief Sets shape flags

	<b>Sleeping:</b> Does <b>NOT</b> wake the associated actor up automatically.

	\param[in] flag The shape flag to enable/disable. See #PxShapeFlag.
	\param[in] value True to set the flag. False to clear the flag specified in flag.

	<b>Default:</b> PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE

	@see PxShapeFlag getFlags()
	*/
	virtual		void	setFlag(PxShapeFlag::Enum flag, bool value) = 0;

	/**
	\brief Sets shape flags

	@see PxShapeFlag getFlags()
	*/
	virtual		void	setFlags(PxShapeFlags inFlags) = 0;

	/**
	\brief Retrieves shape flags.

	\return The values of the shape flags.

	@see PxShapeFlag setFlag()
	*/
	virtual		PxShapeFlags	getFlags() const = 0;

	/**
	\brief Returns true if the shape is exclusive to an actor.
	
	@see PxPhysics::createShape()
	*/
	virtual		bool	isExclusive() const	= 0;

	/**
	\brief Sets a name string for the object that can be retrieved with #getName().
	
	This is for debugging and is not used by the SDK.
	The string is not copied by the SDK, only the pointer is stored.

	<b>Default:</b> NULL
	
	\param[in] name The name string to set the objects name to.

	@see getName()
	*/
	virtual		void	setName(const char* name)		= 0;

	/**
	\brief retrieves the name string set with setName().
	\return The name associated with the shape.

	@see setName()
	*/
	virtual		const char*	getName()			const	= 0;


	virtual		const char*	getConcreteTypeName() const	{ return "PxShape"; }

/************************************************************************************************/

				void*		userData;	//!< user can assign this to whatever, usually to create a 1:1 relationship with a user object.

protected:
	PX_INLINE				PxShape(PxBaseFlags baseFlags) : PxRefCounted(baseFlags) {}
	PX_INLINE				PxShape(PxType concreteType, PxBaseFlags baseFlags) : PxRefCounted(concreteType, baseFlags), userData(NULL) {}
	virtual					~PxShape() {}
	virtual		bool		isKindOf(const char* name) const { PX_IS_KIND_OF(name, "PxShape", PxRefCounted); }
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
