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
		ePOSITION = (1 << 0),					//!< #PxQueryHit�� "position" ����� ��ȿ�մϴ�.
		eNORMAL = (1 << 1),						//!< #PxQueryHit�� "normal" ����� ��ȿ�մϴ�.
		eUV = (1 << 3),							//!< #PxQueryHit�� "u" �� "v" �ٸ���Ʈ�� ��ǥ�� ��ȿ�մϴ�. ���� �������� �ش���� �ʽ��ϴ�.
		eASSUME_NO_INITIAL_OVERLAP = (1 << 4),  //!< �ʱ� ��ħ�� ���ٴ� ���� �˷��� ��� ������ ���� ��Ʈ �÷����Դϴ�.
												//!< ����: �� �÷��׸� ����ϸ� ������ �ʱ⿡ ��ħ�ϴ� ��� ���ǵ��� ���� ����� �߻��� �� �ֽ��ϴ�.
		eANY_HIT = (1 << 5),					//!< � ù ��° ��Ʈ�� �����մϴ�. �ϳ� �̻��� �⺻ü�� �����ϴ� ������Ʈ���� ���˴ϴ�. 
												//!< �޽��� ��� eMESH_MULTIPLE �Ǵ� eANY_HIT�� �������� ���� ��� ���� ����� ��Ʈ �ϳ��� ����˴ϴ�.
		eMESH_MULTIPLE = (1 << 6),				//!< �޽��� ��� ��Ʈ�� ���� �ϳ� �̻��� �⺻ü�� �����ϴ� ������Ʈ���� ���˴ϴ�. ���� �������� �ش���� �ʽ��ϴ�.
		eMESH_ANY = eANY_HIT,					//!< @deprecated ������, ��� eANY_HIT�� ����ϼ���.
		eMESH_BOTH_SIDES = (1 << 7),			//!< �޽� �ﰢ���� �޸���� ��Ʈ�� ���� �޽� ǥ�鿡�� ���۵ǰ� ǥ�� �������� �־����� ����ĳ��Ʈ�� ��Ʈ�� �����մϴ�. 
												//!< ���� �������� �ش���� �ʽ��ϴ�. ���� �ʵ忡 ���� Ư�� �������� ���ؼ��� ����� ���̵带 �����ϼ���.
		ePRECISE_SWEEP = (1 << 8),				//!< �� ��Ȯ������ ���� �׾������� ���� �׽�Ʈ�� ����մϴ�. 
												//!< PhysX 3.2 ���� ���۰� �� ���� ȣȯ���� ������ �� �ֽ��ϴ�.
		eMTD = (1 << 9),						//!< �ּ� �����̵� ����, ���� �� �������� �����մϴ�.
		eFACE_INDEX = (1 << 10),				//!< #PxQueryHit�� "face index" ����� ��ȿ�մϴ�.

		eDEFAULT = ePOSITION | eNORMAL | eFACE_INDEX,

		/** \brief ���� ���Ϳ� ���� ���� ������ �÷����� �� ���� ���ո� ������ �� �ֽ��ϴ�. �ٸ� ������ �������ϴ�. */
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
	�ﰢ�� �޽�, ���� �޽� �� ���� �ʵ��� ��ġ�� �ﰢ���� �� �ε����Դϴ�.

	\note	��ħ ������ ��� �� �ε����� �⺻������ 0xFFFFffff ���� �˴ϴ�.
	\note	���� ������ ���� �ڼ��� ������ ����� ���̵带 �����ϼ���.
	\note	�� �ε����� �޽� ��ŷ�� ���� �ٽ� ���ε˴ϴ�.
			���� �޽� �ε����� ��ȯ�Ϸ��� #PxTriangleMesh::getTrianglesRemap()�� ����ϼ���.
	\note	���� �޽��� ��� #PxConvexMesh::getPolygonData()�� ����Ͽ� ��ġ�� ������ �����͸� �˻��ϼ���.
	*/
	PxU32				faceIndex;
};

/**
\brief ����ĳ��Ʈ �� ������ ��Ʈ ��ġ �� ���� ������ ���� �� ���� ��Ʈ �����Դϴ�.

::PxHitFlag �÷��״� �� ���� �Լ��� ���޵� �� ������
����ȭ�� ���� SDK�� �� ����ü�� Ư�� ����� �����ϵ��� ������ �� �ֽ��ϴ�.
*/
struct PxLocationHit : PxQueryHit
{
	PX_INLINE			PxLocationHit() : flags(0), position(PxVec3(0)), normal(PxVec3(0)), distance(PX_MAX_REAL)	{}

	/**
	\note	����ĳ��Ʈ ��Ʈ�� ���: ����ĳ��Ʈ ������ ��ġ�� �����̸� true�Դϴ�.
	\note	���� ��Ʈ�� ���: ���� ���� �Ÿ����� ��ġ�� �����̸� true�Դϴ�.

	@see PxRaycastHit PxSweepHit
	*/
	PX_INLINE bool		hadInitialOverlap() const { return (distance <= 0.0f); }

	// ���� �ʵ���� #PxHitFlags�� ���� �����˴ϴ�.
	PxHitFlags flags;   //!< ��ȿ�� ���� �����ϴ� ����� �����ϴ� ��Ʈ �÷���.
	PxVec3 position;    //!< ���� ���� ��Ʈ ��ġ (�÷���: #PxHitFlag::ePOSITION)
	PxVec3 normal;      //!< ���� ���� ��Ʈ ���� (�÷���: #PxHitFlag::eNORMAL)

	/**
	\brief ��Ʈ������ �Ÿ�.
	\note	eMTD �÷��׸� ����ϴ� ��� ������ ��ġ�� ��� ���� ������ �Ÿ��� ǥ�õǸ�, �̴� ħ�� ���̸� ��Ÿ���ϴ�.
	\note	�׷��� ���� ��� �� ���� >= 0�� ���Դϴ�.
	*/
	PxF32				distance;
};

/**
\brief ����ĳ��Ʈ ������ ����� �����մϴ�.

����ĳ��Ʈ �Լ����� ::PxHitFlag �÷��װ� ���޵� �� ������,
����ȭ�� ���� SDK�� �� ����ü�� Ư�� ����� ����ϵ��� ������ �� �ֽ��ϴ�.

�ٸ���Ʈ�� ��ǥ�� ���� �Ϻ� ����� ���� �ﰢ�� �޽� �� ���� �ʵ忡 ���ؼ��� ��������,
���� ���������� �ٸ� ��쿡�� ������ �� �ֽ��ϴ�.
Ŭ���̾�Ʈ �ڵ�� #flags�� Ȯ���Ͽ� ��ȯ�� ���� ��ȿ���� Ȯ���ؾ� �մϴ�.

@see PxScene.raycast
*/
struct PxGeomRaycastHit : PxLocationHit
{
	PX_INLINE			PxGeomRaycastHit() : u(0.0f), v(0.0f)	{}

	// the following fields are set in accordance with the #PxHitFlags

	PxReal	u, v;			//!< ��Ʈ ������ �ﰢ�� �޽� �� ���� �ʵ忡 ���� �ٸ���Ʈ�� ��ǥ (�÷���: #PxHitFlag::eUV)
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
