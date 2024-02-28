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

#ifndef PX_GEOMETRY_HIT_H
#define PX_GEOMETRY_HIT_H
/** \addtogroup scenequery
@{
*/
#include "foundation/PxVec3.h"
#include "foundation/PxFlags.h"
#include "common/PxPhysXCommonConfig.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

/**
\brief Scene query and geometry query behavior flags.

PxHitFlags are used for 3 different purposes:

1) To request hit fields to be filled in by scene queries (such as hit position, normal, face index or UVs).
2) Once query is completed, to indicate which fields are valid (note that a query may produce more valid fields than requested).
3) To specify additional options for the narrow phase and mid-phase intersection routines.

All these flags apply to both scene queries and geometry queries (PxGeometryQuery).

@see PxRaycastHit PxSweepHit PxOverlapHit PxScene.raycast PxScene.sweep PxScene.overlap PxGeometryQuery PxFindFaceIndex
*/
struct PxHitFlag
{
	enum Enum
	{
		ePOSITION = (1 << 0),					//!< #PxQueryHit의 "position" 멤버가 유효합니다.
		eNORMAL = (1 << 1),						//!< #PxQueryHit의 "normal" 멤버가 유효합니다.
		eUV = (1 << 3),							//!< #PxQueryHit의 "u" 및 "v" 바리센트릭 좌표가 유효합니다. 스윕 쿼리에는 해당되지 않습니다.
		eASSUME_NO_INITIAL_OVERLAP = (1 << 4),  //!< 초기 겹침이 없다는 것이 알려진 경우 스윕의 성능 힌트 플래그입니다.
												//!< 참고: 이 플래그를 사용하면 형상이 초기에 겹침하는 경우 정의되지 않은 결과가 발생할 수 있습니다.
		eANY_HIT = (1 << 5),					//!< 어떤 첫 번째 히트든 보고합니다. 하나 이상의 기본체를 포함하는 지오메트리에 사용됩니다. 
												//!< 메쉬의 경우 eMESH_MULTIPLE 또는 eANY_HIT이 지정되지 않은 경우 가장 가까운 히트 하나만 보고됩니다.
		eMESH_MULTIPLE = (1 << 6),				//!< 메쉬의 모든 히트를 보고 하나 이상의 기본체를 포함하는 지오메트리에 사용됩니다. 스윕 쿼리에는 해당되지 않습니다.
		eMESH_ANY = eANY_HIT,					//!< @deprecated 폐지됨, 대신 eANY_HIT을 사용하세요.
		eMESH_BOTH_SIDES = (1 << 7),			//!< 메쉬 삼각형의 뒷면과의 히트를 보고 메쉬 표면에서 시작되고 표면 법선에서 멀어지는 레이캐스트의 히트를 보고합니다. 
												//!< 스윕 쿼리에는 해당되지 않습니다. 높이 필드에 대한 특정 차이점에 대해서는 사용자 가이드를 참조하세요.
		ePRECISE_SWEEP = (1 << 8),				//!< 더 정확하지만 느린 네어페이즈 스윕 테스트를 사용합니다. 
												//!< PhysX 3.2 스윕 동작과 더 좋은 호환성을 제공할 수 있습니다.
		eMTD = (1 << 9),						//!< 최소 평행이동 깊이, 법선 및 접촉점을 보고합니다.
		eFACE_INDEX = (1 << 10),				//!< #PxQueryHit의 "face index" 멤버가 유효합니다.

		eDEFAULT = ePOSITION | eNORMAL | eFACE_INDEX,

		/** \brief 사전 필터에 의해 수정 가능한 플래그의 이 하위 집합만 수정할 수 있습니다. 다른 수정은 버려집니다. */
		eMODIFIABLE_FLAGS = eMESH_MULTIPLE | eMESH_BOTH_SIDES | eASSUME_NO_INITIAL_OVERLAP | ePRECISE_SWEEP
	};
};

/**
\brief collection of set bits defined in PxHitFlag.

@see PxHitFlag
*/
PX_FLAGS_TYPEDEF(PxHitFlag, PxU16)

/**
\brief Scene query hit information.
*/
struct PxQueryHit
{
	PX_INLINE			PxQueryHit() : faceIndex(0xFFFFffff) {}

	/**
	삼각형 메쉬, 복소 메쉬 및 높이 필드의 터치된 삼각형의 면 인덱스입니다.

	\note	겹침 쿼리의 경우 이 인덱스는 기본값으로 0xFFFFffff 값이 됩니다.
	\note	스윕 쿼리에 대한 자세한 내용은 사용자 가이드를 참조하세요.
	\note	이 인덱스는 메쉬 쿠킹에 의해 다시 매핑됩니다.
			원래 메쉬 인덱스로 변환하려면 #PxTriangleMesh::getTrianglesRemap()를 사용하세요.
	\note	복소 메쉬의 경우 #PxConvexMesh::getPolygonData()를 사용하여 터치된 폴리곤 데이터를 검색하세요.
	*/
	PxU32				faceIndex;
};

/**
\brief 레이캐스트 및 스윕의 히트 위치 및 법선 정보에 대한 씬 쿼리 히트 정보입니다.

::PxHitFlag 플래그는 씬 쿼리 함수에 전달될 수 있으며
최적화를 위해 SDK가 이 구조체의 특정 멤버만 생성하도록 지정할 수 있습니다.
*/
struct PxLocationHit : PxQueryHit
{
	PX_INLINE			PxLocationHit() : flags(0), position(PxVec3(0)), normal(PxVec3(0)), distance(PX_MAX_REAL)	{}

	/**
	\note	레이캐스트 히트의 경우: 레이캐스트 원점과 겹치는 형상이면 true입니다.
	\note	스윕 히트의 경우: 제로 스윕 거리에서 겹치는 형상이면 true입니다.

	@see PxRaycastHit PxSweepHit
	*/
	PX_INLINE bool		hadInitialOverlap() const { return (distance <= 0.0f); }

	// 다음 필드들은 #PxHitFlags에 따라 설정됩니다.
	PxHitFlags flags;   //!< 유효한 값을 포함하는 멤버를 지정하는 히트 플래그.
	PxVec3 position;    //!< 월드 공간 히트 위치 (플래그: #PxHitFlag::ePOSITION)
	PxVec3 normal;      //!< 월드 공간 히트 법선 (플래그: #PxHitFlag::eNORMAL)

	/**
	\brief 히트까지의 거리.
	\note	eMTD 플래그를 사용하는 경우 형상이 겹치는 경우 음수 값으로 거리가 표시되며, 이는 침투 깊이를 나타냅니다.
	\note	그렇지 않은 경우 이 값은 >= 0일 것입니다.
	*/
	PxF32				distance;
};

/**
\brief 레이캐스트 쿼리의 결과를 저장합니다.

레이캐스트 함수에는 ::PxHitFlag 플래그가 전달될 수 있으며,
최적화를 위해 SDK가 이 구조체의 특정 멤버만 계산하도록 지정할 수 있습니다.

바리센트릭 좌표와 같은 일부 멤버는 현재 삼각형 메쉬 및 높이 필드에 대해서만 계산되지만,
향후 버전에서는 다른 경우에도 제공될 수 있습니다.
클라이언트 코드는 #flags를 확인하여 반환된 값이 유효한지 확인해야 합니다.

@see PxScene.raycast
*/
struct PxGeomRaycastHit : PxLocationHit
{
	PX_INLINE			PxGeomRaycastHit() : u(0.0f), v(0.0f)	{}

	// the following fields are set in accordance with the #PxHitFlags

	PxReal	u, v;			//!< 히트 지점의 삼각형 메쉬 및 높이 필드에 대한 바리센트릭 좌표 (플래그: #PxHitFlag::eUV)
};

/**
\brief Stores results of overlap queries.

@see PxScene.overlap 
*/
struct PxGeomOverlapHit : PxQueryHit
{
	PX_INLINE			PxGeomOverlapHit() {}
};

/**
\brief Stores results of sweep queries.

@see PxScene.sweep
*/
struct PxGeomSweepHit : PxLocationHit
{
	PX_INLINE			PxGeomSweepHit() {}
};

/**
\brief Pair of indices, typically either object or triangle indices.
*/
struct PxGeomIndexPair
{
    PX_FORCE_INLINE PxGeomIndexPair()												{}
    PX_FORCE_INLINE PxGeomIndexPair(PxU32 _id0, PxU32 _id1) : id0(_id0), id1(_id1)	{}

	PxU32	id0, id1;
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
