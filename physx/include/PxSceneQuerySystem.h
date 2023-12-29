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

#ifndef PX_SCENE_QUERY_SYSTEM_H
#define PX_SCENE_QUERY_SYSTEM_H
/** \addtogroup physics
@{ */

#include "foundation/PxSimpleTypes.h"
#include "foundation/PxBitMap.h"
#include "foundation/PxTransform.h"
#include "PxSceneQueryDesc.h"
#include "PxQueryReport.h"
#include "PxQueryFiltering.h"
#include "geometry/PxGeometryQueryFlags.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

	class PxBaseTask;
	class PxRenderOutput;
	class PxGeometry;
	class PxRigidActor;
	class PxShape;
	class PxBVH;
	class PxPruningStructure;

	/**
	\brief Built-in enum for default PxScene pruners

	This is passed as a pruner index to various functions in the following APIs.

	@see PxSceneQuerySystemBase::forceRebuildDynamicTree PxSceneQuerySystem::preallocate
	@see PxSceneQuerySystem::visualize PxSceneQuerySystem::sync PxSceneQuerySystem::prepareSceneQueryBuildStep
	*/
	enum PxScenePrunerIndex
	{
		PX_SCENE_PRUNER_STATIC		= 0,
		PX_SCENE_PRUNER_DYNAMIC		= 1,
		PX_SCENE_COMPOUND_PRUNER	= 0xffffffff
	};

	/**
	\brief Base class for the scene-query system.

	Methods defined here are common to both the traditional PxScene API and the PxSceneQuerySystem API.

	@see PxScene PxSceneQuerySystem
	*/
	class PxSceneQuerySystemBase
	{
		protected:
						PxSceneQuerySystemBase()	{}
		virtual			~PxSceneQuerySystemBase()	{}

		public:

		/** @name Scene Query
		*/
		//@{

		/**
		\brief Sets the rebuild rate of the dynamic tree pruning structures.

		\param[in] dynamicTreeRebuildRateHint Rebuild rate of the dynamic tree pruning structures.

		@see PxSceneQueryDesc.dynamicTreeRebuildRateHint getDynamicTreeRebuildRateHint() forceRebuildDynamicTree()
		*/
		virtual	void	setDynamicTreeRebuildRateHint(PxU32 dynamicTreeRebuildRateHint) = 0;

		/**
		\brief Retrieves the rebuild rate of the dynamic tree pruning structures.

		\return The rebuild rate of the dynamic tree pruning structures.

		@see PxSceneQueryDesc.dynamicTreeRebuildRateHint setDynamicTreeRebuildRateHint() forceRebuildDynamicTree()
		*/
		virtual PxU32	getDynamicTreeRebuildRateHint() const = 0;

		/**
		\brief Forces dynamic trees to be immediately rebuilt.

		\param[in] prunerIndex	Index of pruner containing the dynamic tree to rebuild

		\note PxScene will call this function with the PX_SCENE_PRUNER_STATIC or PX_SCENE_PRUNER_DYNAMIC value.

		@see PxSceneQueryDesc.dynamicTreeRebuildRateHint setDynamicTreeRebuildRateHint() getDynamicTreeRebuildRateHint()
		*/
		virtual void	forceRebuildDynamicTree(PxU32 prunerIndex)	= 0;

		/**
		\brief Sets scene query update mode	

		\param[in] updateMode	Scene query update mode.

		@see PxSceneQueryUpdateMode::Enum
		*/
		virtual	void	setUpdateMode(PxSceneQueryUpdateMode::Enum updateMode)	= 0;

		/**
		\brief Gets scene query update mode	

		\return Current scene query update mode.

		@see PxSceneQueryUpdateMode::Enum
		*/
		virtual	PxSceneQueryUpdateMode::Enum	getUpdateMode()	const	= 0;

		/**
		\brief Retrieves the system's internal scene query timestamp, increased each time a change to the
		static scene query structure is performed.

		\return scene query static timestamp
		*/
		virtual	PxU32	getStaticTimestamp()	const	= 0;

		/**
		\brief Flushes any changes to the scene query representation.

		This method updates the state of the scene query representation to match changes in the scene state.

		By default, these changes are buffered until the next query is submitted. Calling this function will not change
		the results from scene queries, but can be used to ensure that a query will not perform update work in the course of 
		its execution.
	
		A thread performing updates will hold a write lock on the query structure, and thus stall other querying threads. In multithread
		scenarios it can be useful to explicitly schedule the period where this lock may be held for a significant period, so that
		subsequent queries issued from multiple threads will not block.
		*/
		virtual	void	flushUpdates()	= 0;

		/**
		\brief 씬에서 객체에 대한 레이캐스트를 수행하고,
		결과를 PxRaycastBuffer 객체나 PxRaycastCallback에서 파생된 사용자 정의 콜백 구현을 통해 반환합니다.

		\note 터치 히트는 순서대로 정렬되지 않습니다.
		\note 객체 내에서 레이를 발사하면 형상 유형에 따라 다른 결과가 나타납니다. 자세한 내용은 사용자 가이드의 SceneQuery 기사를 확인하세요. 사용자는 제공된 필터 메커니즘 중 하나를 사용하여 이러한 객체를 무시할 수 있습니다.

		\param[in] origin		레이의 원점.
		\param[in] unitDir		레이의 정규화된 방향.
		\param[in] distance		레이의 길이. [0, inf) 범위에 있어야 합니다.
		\param[out] hitCall		레이캐스트 히트 버퍼 또는 레이캐스트 히트를 보고하기 위해 사용되는 사용자 정의 콜백 객체.
		\param[in] hitFlags		히트당 계산되고 히트 콜백을 통해 반환되어야 하는 속성을 지정합니다.
		\param[in] filterData	필터 쉐이더로 전달되는 필터링 데이터.
		\param[in] filterCall	사용자 정의 필터 로직 (선택 사항). 해당 #PxQueryFlag 플래그가 설정된 경우에만 사용됩니다. NULL이면 모든 히트가 차단되었다고 가정됩니다.
		\param[in] cache		캐시된 히트 형상 (선택 사항). 레이는 먼저 캐시된 형상과 비교됩니다. 히트가 없으면 레이가 씬에 대해 쿼리됩니다.
								참고: 제공된 경우 캐시된 형상에 대한 필터링은 실행되지 않습니다. 대신 히트가 발생하면 항상 차단 히트로 간주됩니다.
								참고: 지난 터치 히트를 캐시로 사용하면 캐시된 히트가 항상 차단 히트로 처리되기 때문에 잘못된 동작을 일으킬 수 있습니다.
		\param[in] queryFlags	쿼리를 제어하는 선택적 플래그.

		\return 어떤 터치 히트나 차단 히트가 발견되었거나 PxQueryFlag::eANY_HIT이 지정된 경우 어떤 히트가 발견되었으면 true를 반환합니다.

		@see PxRaycastCallback PxRaycastBuffer PxQueryFilterData PxQueryFilterCallback PxQueryCache PxRaycastHit PxQueryFlag PxQueryFlag::eANY_HIT PxGeometryQueryFlag
		*/
		virtual bool	raycast(const PxVec3& origin, const PxVec3& unitDir, const PxReal distance,
								PxRaycastCallback& hitCall, PxHitFlags hitFlags = PxHitFlag::eDEFAULT,
								const PxQueryFilterData& filterData = PxQueryFilterData(), PxQueryFilterCallback* filterCall = NULL,
								const PxQueryCache* cache = NULL, PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT) const = 0;

		/**
		\brief 씬에서 객체에 대한 스윕 테스트를 수행하고,
		결과를 PxSweepBuffer 객체나 PxSweepCallback에서 파생된 사용자 정의 콜백 구현을 통해 반환합니다.
	
		\note 터치 히트는 순서대로 정렬되지 않습니다.
		\note 씬의 형상이 쿼리 형상과 시작 위치에서 이미 겹치는 경우 eASSUME_NO_INITIAL_OVERLAP이 지정되지 않은 한 해당 히트가 반환됩니다.

		\param[in] geometry		스윕할 객체의 기하학(지원되는 유형은: 박스, 스피어, 캡슐, 복소체)입니다.
		\param[in] pose			스윕 객체의 포즈입니다.
		\param[in] unitDir		스윕의 정규화된 방향입니다.
		\param[in] distance		스윕 거리입니다. [0, inf) 범위에 있어야 하며 eASSUME_NO_INITIAL_OVERLAP이 지정된 경우 >0이어야 합니다.
								PX_MAX_SWEEP_DISTANCE로 클램핑됩니다.
		\param[out] hitCall		스윕 히트 버퍼 또는 스윕 히트를 보고하기 위해 사용되는 사용자 정의 콜백 객체입니다.
		\param[in] hitFlags		히트당 계산되고 히트 콜백을 통해 반환되어야 하는 속성을 지정합니다.
		\param[in] filterData	필터링 데이터 및 간단한 논리입니다.
		\param[in] filterCall	사용자 정의 필터 로직 (선택 사항). 해당 #PxQueryFlag 플래그가 설정된 경우에만 사용됩니다.
								NULL이면 모든 히트가 차단되었다고 가정됩니다.
		\param[in] cache		캐시된 히트 형상 (선택 사항). 스윕은 먼저 캐시된 형상과 비교됩니다. 히트가 없으면 스윕이 씬에 대해 쿼리됩니다.
								참고: 제공된 경우 캐시된 형상에 대한 필터링은 실행되지 않습니다. 대신 히트가 발생하면 항상 차단 히트로 간주됩니다.
								참고: 지난 터치 히트를 캐시로 사용하면 캐시된 히트가 항상 차단 히트로 처리되기 때문에 잘못된 동작을 일으킬 수 있습니다.
		\param[in] inflation	이 매개변수는 스윕 기하학 주위에 스킨을 생성하여 스윕 기하학의 범위를 확장합니다. 스윕은 스킨이 형상에 닿는 즉시 히트를 등록하고 해당 거리와 법선을 반환합니다.
								참고: ePRECISE_SWEEP는 팽창을 지원하지 않습니다. 따라서 스윕은 제로 팽창으로 수행됩니다.
		\param[in] queryFlags	쿼리를 제어하는 선택적 플래그.

		\return 어떤 터치 히트나 차단 히트가 발견되었거나 PxQueryFlag::eANY_HIT이 지정된 경우 어떤 히트가 발견되었으면 true를 반환합니다.

		@see PxSweepCallback PxSweepBuffer PxQueryFilterData PxQueryFilterCallback PxSweepHit PxQueryCache PxGeometryQueryFlag
		*/
		virtual bool	sweep(	const PxGeometry& geometry, const PxTransform& pose, const PxVec3& unitDir, const PxReal distance,
								PxSweepCallback& hitCall, PxHitFlags hitFlags = PxHitFlag::eDEFAULT,
								const PxQueryFilterData& filterData = PxQueryFilterData(), PxQueryFilterCallback* filterCall = NULL,
								const PxQueryCache* cache = NULL, const PxReal inflation = 0.0f, PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT) const = 0;

		/**
		\brief 씬에서 객체에 대한 겹침 테스트를 수행하고,
		결과를 PxOverlapBuffer 객체나 PxOverlapCallback에서 파생된 사용자 정의 콜백 구현을 통해 반환합니다.
		
		\note 필터링: 겹침 쿼리에 대한 사용자 필터에서 eBLOCK을 반환하면 경고가 발생합니다(#PxQueryHitType 참조).

		\param[in] geometry		겹침을 확인할 객체의 기하학(지원되는 유형은: 박스, 스피어, 캡슐, 복소체)입니다.
		\param[in] pose			객체의 포즈입니다.
		\param[out] hitCall		겹침 히트 버퍼 또는 겹침 히트를 보고하기 위해 사용되는 사용자 정의 콜백 객체입니다.
		\param[in] filterData	필터링 데이터 및 간단한 논리입니다. #PxQueryFilterData #PxQueryFilterCallback 참조
		\param[in] filterCall	사용자 정의 필터 로직 (선택 사항). 해당 #PxQueryFlag 플래그가 설정된 경우에만 사용됩니다.
								NULL이면 모든 히트가 겹친 것으로 가정됩니다.
		\param[in] cache		캐시된 히트 형상 (선택 사항). 겹침은 먼저 캐시된 형상과 비교됩니다. 히트가 없으면 겹침이 씬에 대해 쿼리됩니다.
								참고: 제공된 경우 캐시된 형상에 대한 필터링은 실행되지 않습니다. 대신 히트가 발생하면 항상 차단 히트로 간주됩니다.
								참고: 지난 터치 히트를 캐시로 사용하면 캐시된 히트가 항상 차단 히트로 처리되기 때문에 잘못된 동작을 일으킬 수 있습니다.
		\param[in] queryFlags	쿼리를 제어하는 선택적 플래그.
		참고: 제공된 경우 캐시된 형상에 대한 필터링은 실행되지 않습니다. 대신 히트가 발생하면 항상 차단 히트로 간주됩니다.
		참고: 지난 터치 히트를 캐시로 사용하면 캐시된 히트가 항상 차단 히트로 처리되기 때문에 잘못된 동작을 일으킬 수 있습니다.

		\return 어떤 터치 히트나 차단 히트가 발견되었거나 PxQueryFlag::eANY_HIT이 지정된 경우 어떤 히트가 발견되었으면 true를 반환합니다.

		\note eBLOCK을 overlap()에 대한 사용자 필터에서 반환해서는 안 됩니다. 이렇게 하면 정의되지 않은 동작이 발생하고 경고가 발생합니다.
		\note PxQueryFlag::eNO_BLOCK 플래그가 설정된 경우 eBLOCK은 자동으로 eTOUCH로 자동 변환되고 경고가 억제됩니다.

		@see PxOverlapCallback PxOverlapBuffer PxHitFlags PxQueryFilterData PxQueryFilterCallback PxGeometryQueryFlag
		*/
		virtual bool	overlap(const PxGeometry& geometry, const PxTransform& pose, PxOverlapCallback& hitCall,
								const PxQueryFilterData& filterData = PxQueryFilterData(), PxQueryFilterCallback* filterCall = NULL,
								const PxQueryCache* cache = NULL, PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT) const = 0;
		//@}
	};

	/**
	\brief Traditional SQ system for PxScene.

	Methods defined here are only available through the traditional PxScene API.
	Thus PxSceneSQSystem effectively captures the scene-query related part of the PxScene API.

	@see PxScene PxSceneQuerySystemBase
	*/
	class PxSceneSQSystem : public PxSceneQuerySystemBase
	{
		protected:
					PxSceneSQSystem()	{}
		virtual		~PxSceneSQSystem()	{}

		public:

		/** @name Scene Query
		*/
		//@{

		/**
		\brief Sets scene query update mode	

		\param[in] updateMode	Scene query update mode.

		@see PxSceneQueryUpdateMode::Enum
		*/
		PX_FORCE_INLINE	void	setSceneQueryUpdateMode(PxSceneQueryUpdateMode::Enum updateMode)	{ setUpdateMode(updateMode);	}

		/**
		\brief Gets scene query update mode	

		\return Current scene query update mode.

		@see PxSceneQueryUpdateMode::Enum
		*/
		PX_FORCE_INLINE	PxSceneQueryUpdateMode::Enum	getSceneQueryUpdateMode()	const	{ return getUpdateMode();	}

		/**
		\brief Retrieves the scene's internal scene query timestamp, increased each time a change to the
		static scene query structure is performed.

		\return scene query static timestamp
		*/
		PX_FORCE_INLINE	PxU32	getSceneQueryStaticTimestamp()	const	{ return getStaticTimestamp();	}

		/**
		\brief Flushes any changes to the scene query representation.

		@see flushUpdates
		*/
		PX_FORCE_INLINE	void	flushQueryUpdates()	{ flushUpdates();	}

		/**
		\brief Forces dynamic trees to be immediately rebuilt.

		\param[in] rebuildStaticStructure	True to rebuild the dynamic tree containing static objects
		\param[in] rebuildDynamicStructure	True to rebuild the dynamic tree containing dynamic objects

		@see PxSceneQueryDesc.dynamicTreeRebuildRateHint setDynamicTreeRebuildRateHint() getDynamicTreeRebuildRateHint()
		*/
		PX_FORCE_INLINE void	forceDynamicTreeRebuild(bool rebuildStaticStructure, bool rebuildDynamicStructure)
		{
			if(rebuildStaticStructure)
				forceRebuildDynamicTree(PX_SCENE_PRUNER_STATIC);
			if(rebuildDynamicStructure)
				forceRebuildDynamicTree(PX_SCENE_PRUNER_DYNAMIC);
		}

		/**
		\brief Return the value of PxSceneQueryDesc::staticStructure that was set when creating the scene with PxPhysics::createScene

		@see PxSceneQueryDesc::staticStructure, PxPhysics::createScene
		*/
		virtual	PxPruningStructureType::Enum getStaticStructure() const = 0;

		/**
		\brief Return the value of PxSceneQueryDesc::dynamicStructure that was set when creating the scene with PxPhysics::createScene

		@see PxSceneQueryDesc::dynamicStructure, PxPhysics::createScene
		*/
		virtual PxPruningStructureType::Enum getDynamicStructure() const = 0;

		/**
		\brief Executes scene queries update tasks.

		This function will refit dirty shapes within the pruner and will execute a task to build a new AABB tree, which is
		build on a different thread. The new AABB tree is built based on the dynamic tree rebuild hint rate. Once
		the new tree is ready it will be commited in next fetchQueries call, which must be called after.

		This function is equivalent to the following PxSceneQuerySystem calls:
		Synchronous calls:
			- PxSceneQuerySystemBase::flushUpdates()
			- handle0 = PxSceneQuerySystem::prepareSceneQueryBuildStep(PX_SCENE_PRUNER_STATIC)
			- handle1 = PxSceneQuerySystem::prepareSceneQueryBuildStep(PX_SCENE_PRUNER_DYNAMIC)
		Asynchronous calls:
			- PxSceneQuerySystem::sceneQueryBuildStep(handle0);
			- PxSceneQuerySystem::sceneQueryBuildStep(handle1);

		This function is part of the PxSceneSQSystem interface because it uses the PxScene task system under the hood. But
		it calls PxSceneQuerySystem functions, which are independent from this system and could be called in a similar
		fashion by a separate, possibly user-defined task manager.

		\note If PxSceneQueryUpdateMode::eBUILD_DISABLED_COMMIT_DISABLED is used, it is required to update the scene queries
		using this function.

		\param[in] completionTask if non-NULL, this task will have its refcount incremented in sceneQueryUpdate(), then
		decremented when the scene is ready to have fetchQueries called. So the task will not run until the
		application also calls removeReference().
		\param[in] controlSimulation if true, the scene controls its PxTaskManager simulation state. Leave
		true unless the application is calling the PxTaskManager start/stopSimulation() methods itself.

		@see PxSceneQueryUpdateMode::eBUILD_DISABLED_COMMIT_DISABLED
		*/
		virtual void	sceneQueriesUpdate(PxBaseTask* completionTask = NULL, bool controlSimulation = true)	= 0;

		/**
		\brief This checks to see if the scene queries update has completed.

		This does not cause the data available for reading to be updated with the results of the scene queries update, it is simply a status check.
		The bool will allow it to either return immediately or block waiting for the condition to be met so that it can return true
	
		\param[in] block When set to true will block until the condition is met.
		\return True if the results are available.

		@see sceneQueriesUpdate() fetchResults()
		*/
		virtual	bool	checkQueries(bool block = false) = 0;

		/**
		This method must be called after sceneQueriesUpdate. It will wait for the scene queries update to finish. If the user makes an illegal scene queries update call, 
		the SDK will issue an error	message.

		If a new AABB tree build finished, then during fetchQueries the current tree within the pruning structure is swapped with the new tree. 

		\param[in] block When set to true will block until the condition is met, which is tree built task must finish running.
		*/
		virtual	bool	fetchQueries(bool block = false)	= 0;	
		//@}
	};

	typedef PxU32	PxSQCompoundHandle;
	typedef PxU32	PxSQPrunerHandle;
	typedef void*	PxSQBuildStepHandle;

	/**
	\brief Scene-queries external sub-system for PxScene-based objects.

	The default PxScene has hardcoded support for 2 regular pruners + 1 compound pruner, but these interfaces
	should work with multiple pruners.

	Regular shapes are traditional PhysX shapes that belong to an actor. That actor can be a compound, i.e. it has
	more than one shape. *All of these go to the regular pruners*. This is important because it might be misleading:
	by default all shapes go to one of the two regular pruners, even shapes that belong to compound actors.

	For compound actors, adding all the actor's shapes individually to the SQ system can be costly, since all the
	corresponding bounds will always move together and remain close together - that can put a lot of stress on the
	code that updates the SQ spatial structures. In these cases it can be more efficient to add the compound's bounds
	(i.e. the actor's bounds) to the system, as the first level of a bounds hierarchy. The scene queries would then
	be performed against the actor's bounds first, and only visit the shapes' bounds second. This is only useful
	for actors that have more than one shape, i.e. compound actors. Such actors added to the SQ system are thus
	called "SQ compounds". These objects are managed by the "compound pruner", which is only used when an explicit
	SQ compound is added to the SQ system via the addSQCompound call. So in the end one has to distinguish between:

	- a "compound shape", which is added to regular pruners as its own individual entity.
	- an "SQ compound shape", which is added to the compound pruner as a subpart of an SQ compound actor.

	A compound shape has an invalid compound ID, since it does not belong to an SQ compound.
	An SQ compound shape has a valid compound ID, that identifies its SQ compound owner.

	@see PxScene PxSceneQuerySystemBase
	*/
	class PxSceneQuerySystem : public PxSceneQuerySystemBase
	{
		protected:
						PxSceneQuerySystem()	{}
		virtual			~PxSceneQuerySystem()	{}
		public:

		/**
		\brief Decrements the reference count of the object and releases it if the new reference count is zero.		
		*/
		virtual	void	release()	= 0;

		/**
		\brief Acquires a counted reference to this object.

		This method increases the reference count of the object by 1. Decrement the reference count by calling release()
		*/
		virtual	void	acquireReference()	= 0;

		/**
		\brief Preallocates internal arrays to minimize the amount of reallocations.

		The system does not prevent more allocations than given numbers. It is legal to not call this function at all,
		or to add more shapes to the system than the preallocated amounts.

		\param[in] prunerIndex	Index of pruner to preallocate (PX_SCENE_PRUNER_STATIC, PX_SCENE_PRUNER_DYNAMIC or PX_SCENE_COMPOUND_PRUNER when called from PxScene).
		\param[in] nbShapes		Expected number of (regular) shapes
		*/
		virtual	void	preallocate(PxU32 prunerIndex, PxU32 nbShapes)	= 0;

		/**
		\brief Frees internal memory that may not be in-use anymore.

		This is an entry point for reclaiming transient memory allocated at some point by the SQ system,
		but which wasn't been immediately freed for performance reason. Calling this function might free
		some memory, but it might also produce a new set of allocations in the next frame.
		*/
		virtual	void	flushMemory()	= 0;

		/**
		\brief Adds a shape to the SQ system.

		The same function is used to add either a regular shape, or a SQ compound shape.

		\param[in] actor				The shape's actor owner
		\param[in] shape				The shape itself
		\param[in] bounds				Shape bounds, in world-space for regular shapes, in local-space for SQ compound shapes.
		\param[in] transform			Shape transform, in world-space for regular shapes, in local-space for SQ compound shapes.
		\param[in] compoundHandle		Handle of SQ compound owner, or NULL for regular shapes.
		\param[in] hasPruningStructure	True if the shape is part of a pruning structure. The structure will be merged later, adding the objects will not invalidate the pruner.

		@see merge() PxPruningStructure
		*/
		virtual	void	addSQShape(	const PxRigidActor& actor, const PxShape& shape, const PxBounds3& bounds,
									const PxTransform& transform, const PxSQCompoundHandle* compoundHandle=NULL, bool hasPruningStructure=false)	= 0;

		/**
		\brief Removes a shape from the SQ system.

		The same function is used to remove either a regular shape, or a SQ compound shape.

		\param[in] actor	The shape's actor owner
		\param[in] shape	The shape itself
		*/
		virtual	void	removeSQShape(const PxRigidActor& actor, const PxShape& shape)	= 0;

		/**
		\brief Updates a shape in the SQ system.

		The same function is used to update either a regular shape, or a SQ compound shape.

		The transforms are eager-evaluated, but the bounds are lazy-evaluated. This means that
		the updated transform has to be passed to the update function, while the bounds are automatically
		recomputed by the system whenever needed.

		\param[in] actor		The shape's actor owner
		\param[in] shape		The shape itself
		\param[in] transform	New shape transform, in world-space for regular shapes, in local-space for SQ compound shapes.
		*/
		virtual	void	updateSQShape(const PxRigidActor& actor, const PxShape& shape, const PxTransform& transform)	= 0;

		/**
		\brief Adds a compound to the SQ system.

		\param[in] actor		The compound actor
		\param[in] shapes		The compound actor's shapes
		\param[in] bvh			BVH structure containing the compound's shapes in local space
		\param[in] transforms	Shape transforms, in local-space

		\return	SQ compound handle

		@see PxBVH PxCooking::createBVH
		*/
		virtual	PxSQCompoundHandle	addSQCompound(const PxRigidActor& actor, const PxShape** shapes, const PxBVH& bvh, const PxTransform* transforms)	= 0;

		/**
		\brief Removes a compound from the SQ system.

		\param[in] compoundHandle	SQ compound handle (returned by addSQCompound)
		*/
		virtual	void	removeSQCompound(PxSQCompoundHandle compoundHandle)	= 0;

		/**
		\brief Updates a compound in the SQ system.

		The compound structures are immediately updated when the call occurs.

		\param[in] compoundHandle		SQ compound handle (returned by addSQCompound)
		\param[in] compoundTransform	New actor/compound transform, in world-space
		*/
		virtual	void	updateSQCompound(PxSQCompoundHandle compoundHandle, const PxTransform& compoundTransform)	= 0;

		/**
		\brief Shift the data structures' origin by the specified vector.

		Please refer to the notes of the similar function in PxScene.

		\param[in] shift	Translation vector to shift the origin by.
		*/
		virtual	void	shiftOrigin(const PxVec3& shift)	= 0;

		/**
		\brief Visualizes the system's internal data-structures, for debugging purposes.

		\param[in] prunerIndex	Index of pruner to visualize (PX_SCENE_PRUNER_STATIC, PX_SCENE_PRUNER_DYNAMIC or PX_SCENE_COMPOUND_PRUNER when called from PxScene).

		\param[out] out			Filled with render output data

		@see PxRenderOutput
		*/
		virtual	void	visualize(PxU32 prunerIndex, PxRenderOutput& out)	const	= 0;

		/**
		\brief Merges a pruning structure with the SQ system's internal pruners.

		\param[in] pruningStructure		The pruning structure to merge

		@see PxPruningStructure
		*/
		virtual	void	merge(const PxPruningStructure& pruningStructure)	= 0;

		/**
		\brief Shape to SQ-pruner-handle mapping function.

		This function finds and returns the SQ pruner handle associated with a given (actor/shape) couple
		that was previously added to the system. This is needed for the sync function.

		\param[in] actor		The shape's actor owner
		\param[in] shape		The shape itself
		\param[out] prunerIndex	Index of pruner the shape belongs to

		\return Associated SQ pruner handle.
		*/
		virtual	PxSQPrunerHandle	getHandle(const PxRigidActor& actor, const PxShape& shape, PxU32& prunerIndex)	const	= 0;

		/**
		\brief Synchronizes the scene-query system with another system that references the same objects.

		This function is used when the scene-query objects also exist in another system that can also update them. For example the scene-query objects
		(used for raycast, overlap or sweep queries) might be driven by equivalent objects in an external rigid-body simulation engine. In this case
		the rigid-body simulation engine computes the new poses and transforms, and passes them to the scene-query system using this function. It is
		more efficient than calling updateSQShape on each object individually, since updateSQShape would end up recomputing the bounds already available
		in the rigid-body engine.

		\param[in] prunerIndex		Index of pruner being synched (PX_SCENE_PRUNER_DYNAMIC for regular PhysX usage)
		\param[in] handles			Handles of updated objects
		\param[in] indices			Bounds & transforms indices of updated objects, i.e. object handles[i] has bounds[indices[i]] and transforms[indices[i]]
		\param[in] bounds			Array of bounds for all objects (not only updated bounds)
		\param[in] transforms		Array of transforms for all objects (not only updated transforms)
		\param[in] count			Number of updated objects
		\param[in] ignoredIndices	Optional bitmap of ignored indices, i.e. update is skipped if ignoredIndices[indices[i]] is set.

		@see PxBounds3 PxTransform32 PxBitMap
		*/
		virtual	void	sync(PxU32 prunerIndex, const PxSQPrunerHandle* handles, const PxU32* indices, const PxBounds3* bounds, const PxTransform32* transforms, PxU32 count, const PxBitMap& ignoredIndices)	= 0;

		/**
		\brief Finalizes updates made to the SQ system.

		This function should be called after updates have been made to the SQ system, to fully reflect the changes
		inside the internal pruners. In particular it should be called:
		- after calls to updateSQShape
		- after calls to sync

		This function:
		- recomputes bounds of manually updated shapes (i.e. either regular or SQ compound shapes modified by updateSQShape)
		- updates dynamic pruners (refit operations)
		- incrementally rebuilds AABB-trees

		The amount of work performed in this function depends on PxSceneQueryUpdateMode.

		@see PxSceneQueryUpdateMode updateSQShape() sync()
		*/
		virtual	void	finalizeUpdates()	= 0;

		/**
		\brief Prepares asynchronous build step.

		This is directly called (synchronously) by PxSceneSQSystem::sceneQueriesUpdate(). See the comments there.

		This function is called to let the system execute any necessary synchronous operation before the
		asynchronous sceneQueryBuildStep() function is called.

		If there is any work to do for the specific pruner, the function returns a pruner-specific handle that
		will be passed to the corresponding, asynchronous sceneQueryBuildStep function.

		\return	A pruner-specific handle that will be sent to sceneQueryBuildStep if there is any work to do, i.e. to execute the corresponding sceneQueryBuildStep() call.

		\param[in] prunerIndex		Index of pruner being built. (PX_SCENE_PRUNER_STATIC or PX_SCENE_PRUNER_DYNAMIC when called by PxScene).

		\return	Null if there is no work to do, otherwise a pruner-specific handle.

		@see PxSceneSQSystem::sceneQueriesUpdate sceneQueryBuildStep
		*/
		virtual	PxSQBuildStepHandle	prepareSceneQueryBuildStep(PxU32 prunerIndex)	= 0;

		/**
		\brief Executes asynchronous build step.

		This is directly called (asynchronously) by PxSceneSQSystem::sceneQueriesUpdate(). See the comments there.

		This function incrementally builds the internal trees/pruners. It is called asynchronously, i.e. this can be
		called from different threads for building multiple trees at the same time.
		
		\param[in] handle	Pruner-specific handle previously returned by the prepareSceneQueryBuildStep function.

		@see PxSceneSQSystem::sceneQueriesUpdate prepareSceneQueryBuildStep
		*/
		virtual	void	sceneQueryBuildStep(PxSQBuildStepHandle handle)	= 0;
	};

#if !PX_DOXYGEN
} // namespace physx
#endif

  /** @} */
#endif
