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

#ifndef PX_GEOMETRY_QUERY_H
#define PX_GEOMETRY_QUERY_H

/**
\brief Maximum sweep distance for scene sweeps. The distance parameter for sweep functions will be clamped to this value.
The reason for this is GJK support cannot be evaluated near infinity. A viable alternative can be a sweep followed by an infinite raycast.

@see PxScene
*/
#define PX_MAX_SWEEP_DISTANCE 1e8f

/** \addtogroup geomutils
  @{
*/

#include "common/PxPhysXCommonConfig.h"
#include "geometry/PxGeometryHit.h"
#include "geometry/PxGeometryQueryFlags.h"
#include "geometry/PxGeometryQueryContext.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxGeometry;
class PxContactBuffer;

/**
\brief Collection of geometry object queries (sweeps, raycasts, overlaps, ...).
*/
class PxGeometryQuery
{
public:

	/**
	\brief ���� ��ü�� ���� ����ĳ��Ʈ �׽�Ʈ.

	��� ���� Ÿ���� ����������, PxParticleSystemGeometry, PxTetrahedronMeshGeometry �� PxHairSystemGeometry�� ���ܵ˴ϴ�.

	\param[in] origin            �׽�Ʈ�� ������ ������
	\param[in] unitDir           �׽�Ʈ�� ������ ����ȭ�� ����
	\param[in] geom              ���̸� �׽�Ʈ�� ���� ��ü
	\param[in] pose              ���� ��ü�� ����
	\param[in] maxDist           ������ �ִ� ����, [0, ����) ���� ���� �־�� ��
	\param[in] hitFlags          �浹 �� ������ ������ ������ ����. #PxHitFlag �÷����� ����
	\param[in] maxHits           ��ȯ�Ǵ� �ִ� �浹 �� = 'rayHits' ������ ũ��
	\param[out] rayHits          ����ĳ��Ʈ �浹 ����
	\param[in] stride            rayHits �迭�� ��Ʈ���̵� �� (����Ʈ ����). �Ϲ������� ��ŷ�� �迭�� ��� sizeof(PxGeomRaycastHit).
	\param[in] queryFlags        ������ �����ϴ� ������ �÷���
	\param[in] threadContext     ������ ����� ���� �����庰 ���ؽ�Ʈ

	\return ���̿� ���� ��ü ������ �浹 ��

	@see PxGeomRaycastHit PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static PxU32 raycast(	const PxVec3& origin, const PxVec3& unitDir,
												const PxGeometry& geom, const PxTransform& pose,
												PxReal maxDist, PxHitFlags hitFlags,
												PxU32 maxHits, PxGeomRaycastHit* PX_RESTRICT rayHits, PxU32 stride = sizeof(PxGeomRaycastHit), PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT,
												PxRaycastThreadContext* threadContext = NULL);

	/**
	\brief �� ���� ��ü�� ���� ��ħ �׽�Ʈ.

	���� ������ �������� �ʽ��ϴ�:
	\li PxPlaneGeometry vs. {PxPlaneGeometry, PxTriangleMeshGeometry, PxHeightFieldGeometry}
	\li PxTriangleMeshGeometry vs. PxHeightFieldGeometry
	\li PxHeightFieldGeometry vs. PxHeightFieldGeometry
	\li PxParticleSystemGeometry, PxTetrahedronMeshGeometry �Ǵ� PxHairSystemGeometry�� ���Ե� ��� ����

	\param[in] geom0            ù ��° ���� ��ü
	\param[in] pose0            ù ��° ���� ��ü�� ����
	\param[in] geom1            �� ��° ���� ��ü
	\param[in] pose1            �� ��° ���� ��ü�� ����
	\param[in] queryFlags       ������ �����ϴ� ������ �÷���
	\param[in] threadContext    ������ ����� ���� �����庰 ���ؽ�Ʈ

	\return �� ���� ��ü�� ��ġ�� ��� true�� ��ȯ�մϴ�.

	@see PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static bool overlap(const PxGeometry& geom0, const PxTransform& pose0,
											const PxGeometry& geom1, const PxTransform& pose1,
											PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT, PxOverlapThreadContext* threadContext=NULL);

	/**
	\brief ������ ������Ʈ�� ��ü�� �������� �������ϰ� �־��� ��ü�� �浹�� �׽�Ʈ�մϴ�.

	���� ������ �����˴ϴ�.

	\li PxSphereGeometry vs. {PxSphereGeometry, PxPlaneGeometry, PxCapsuleGeometry, PxBoxGeometry, PxConvexMeshGeometry, PxTriangleMeshGeometry, PxHeightFieldGeometry}
	\li PxCapsuleGeometry vs. {PxSphereGeometry, PxPlaneGeometry, PxCapsuleGeometry, PxBoxGeometry, PxConvexMeshGeometry, PxTriangleMeshGeometry, PxHeightFieldGeometry}
	\li PxBoxGeometry vs. {PxSphereGeometry, PxPlaneGeometry, PxCapsuleGeometry, PxBoxGeometry, PxConvexMeshGeometry, PxTriangleMeshGeometry, PxHeightFieldGeometry}
	\li PxConvexMeshGeometry vs. {PxSphereGeometry, PxPlaneGeometry, PxCapsuleGeometry, PxBoxGeometry, PxConvexMeshGeometry, PxTriangleMeshGeometry, PxHeightFieldGeometry}

	\param[in] unitDir ������Ʈ�� geom0�� �������� ����ȭ�� ����
	\param[in] maxDist �ִ� ������ �Ÿ�. [0, inf) �������� �մϴ�.
	\param[in] geom0 �������� ������Ʈ�� ��ü. �����Ǵ� ������Ʈ���� #PxSphereGeometry, #PxCapsuleGeometry, #PxBoxGeometry, #PxConvexMeshGeometry�Դϴ�.
	\param[in] pose0 �������� ������Ʈ�� ��ü�� ����
	\param[in] geom1 ������ ������� �׽�Ʈ�� ������Ʈ�� ��ü
	\param[in] pose1 ������ ��� ������Ʈ�� ��ü�� ����
	\param[out] sweepHit ������ �浹 ����. �� �޼��尡 true�� ��ȯ�� ���� ��ȿ�մϴ�.
	\param[in] hitFlags �� �浹�� ���� ����ϰ� ��� �浹 �迭�� �ۼ��� �Ӽ��� �����մϴ�. #PxHitFlag �÷����� ����
	\param[in] inflation �����ε� ����� ǥ���� ���� �������� �߰������� Ȯ��˴ϴ�. �𼭸��� �������� �ձ۰� ����ϴ�.
	\param[in] queryFlags ������ �����ϴ� ������ �÷���
	\param[in] threadContext ������ ����� ���� �����庰 ���ؽ�Ʈ

	\return �����ε� ������Ʈ�� ��ü geom0�� ������Ʈ�� ��ü geom1�� �浹�ϴ� ��� true�� ��ȯ�մϴ�.

	@see PxGeomSweepHit PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static bool sweep(	const PxVec3& unitDir, const PxReal maxDist,
											const PxGeometry& geom0, const PxTransform& pose0,
											const PxGeometry& geom1, const PxTransform& pose1,
											PxGeomSweepHit& sweepHit, PxHitFlags hitFlags = PxHitFlag::eDEFAULT,
											const PxReal inflation = 0.0f, PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT,
											PxSweepThreadContext* threadContext = NULL);

	/**
	\brief �� ���� ��ü ���� �ּ� ���� �̵� �Ÿ�(MTD)�� ����մϴ�.

	���� ���� ��ü ������ �������� �ʽ��ϴ�:
	- plane/plane (���/���)
	- plane/mesh (���/�޽�)
	- plane/heightfield (���/���� �ʵ�)
	- mesh/mesh (�޽�/�޽�)
	- mesh/heightfield (�޽�/���� �ʵ�)
	- heightfield/heightfield (���� �ʵ�/���� �ʵ�)
	- PxParticleSystemGeometry, PxTetrahedronMeshGeometry �Ǵ� PxHairSystemGeometry�� ���Ե� ��� ����

	�� �Լ��� ���� ����('direction')�� ���� ����('depth')�� ��ȯ�մϴ�.

	ù ��° ��ü�� �� ��° ��ü���� ����� �ϱ� ����, ���� �̵� ���� D = direction * depth�� ù ��° ��ü�� �����ؾ� �մϴ�.

	��ȯ�� ���̴� �׻� ��� �Ǵ� 0�̾�� �մϴ�.

	��ü���� ��ġ�� �ʴ� ���, �� �Լ��� MTD�� ����� �� ������ false�� ��ȯ�մϴ�.

	\param[out] direction    ���� MTD ���� ����
	\param[out] depth        ���� ����. �׻� ��� �Ǵ� 0.
	\param[in] geom0         ù ��° ���� ��ü
	\param[in] pose0         ù ��° ���� ��ü�� ����
	\param[in] geom1         �� ��° ���� ��ü
	\param[in] pose1         �� ��° ���� ��ü�� ����
	\param[in] queryFlags    ������ �����ϴ� ������ �÷���.
	\return ��ü���� ��ġ�� ���, �� MTD�� ���������� ���� ��� true�� ��ȯ�մϴ�.

	@see PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static bool	computePenetration(	PxVec3& direction, PxF32& depth,
														const PxGeometry& geom0, const PxTransform& pose0,
														const PxGeometry& geom1, const PxTransform& pose1,
														PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT);

	/**
	\brief ���� ���� ��ü ���� �Ÿ��� ����մϴ�.

	���� �����Ǵ� ���� ��ü: ����, ��, ĸ��, ���� �ٰ���, �޽�.

	\note �޽��� ���, BVH34 �߰� �ܰ� ������ ������ �����˴ϴ�.

	\param[in] point            �� P
	\param[in] geom             ���� ��ü
	\param[in] pose             ���� ��ü�� ����
	\param[out] closestPoint    ���� ��ü���� �� P�� ���� ����� ���� ���������� ��ȯ�մϴ�. ��ȯ�� �Ÿ��� ������ ����� ��쿡�� ��ȿ�մϴ�.
	\param[out] closestIndex    ���������� ���� ����� (�ﰢ��) �ε����� ��ȯ�մϴ�. �ﰢ�� �޽����� ��ȿ�մϴ�.
	\param[in] queryFlags       ������ �����ϴ� ������ �÷���.
	\return ���� ���� ��ü ���� ���� �Ÿ�, ���� ��ü ���ο� �ִ� ��� 0.0, ������ �߻��� ��� (�������� �ʴ� ���� Ÿ�� �Ǵ� �߸��� ����) -1.0�� ��ȯ�մϴ�.

	@see PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static PxReal pointDistance(const PxVec3& point, const PxGeometry& geom, const PxTransform& pose,
													PxVec3* closestPoint=NULL, PxU32* closestIndex=NULL,
													PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT);

	/**
	\brief ������Ʈ�� ��ü�� ��踦 ����մϴ�.

	\param[out] bounds		���� ��踦 ��ȯ�մϴ�.
	\param[in] geom			������Ʈ�� ��ü
	\param[in] pose			������Ʈ�� ��ü�� ����(��ġ�� ȸ��)
	\param[in] offset		���� ��迡 �߰��� ������.
							�� ���� ������Ʈ���� ����(Extents)�� �߰��˴ϴ�.
	\param[in] inflation	���� ��迡 ���� ������ ����.
							������Ʈ���� ����(Extents)�� �� ���� ���մϴ�.
	\param[in] queryFlags	�������� �÷��׷� ������ �����մϴ�.

	@see PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static void	computeGeomBounds(PxBounds3& bounds, const PxGeometry& geom, const PxTransform& pose, float offset=0.0f, float inflation=1.0f, PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT);

	/**
	\brief ���� ���� ��ü�� ���� �ﰢ�� ���� �浹 ������ �����մϴ�.

	\param[in] geom                 ���� ��ü. ĸ��, ���� �Ǵ� ���� �޽��� �� �ֽ��ϴ�.
	\param[in] pose                 ���� ��ü�� ����
	\param[in] triangleVertices     �ﰢ���� ���� ���� �� ������
	\param[in] triangleIndex        �ﰢ�� �ε���
	\param[in] contactDistance      �� ���� ������ �����Ǳ� �����ϴ� �Ÿ�
	\param[in] meshContactMargin    �޽� ���� ����
	\param[in] toleranceLength      toleranceLength. �Ÿ� ��� �Ӱ谪�� ���������� �����ϸ��Ͽ� �پ��� ������ �ùķ��̼ǿ� ������ ����� �����ϴ� �� ���˴ϴ�.
	\param[out] contactBuffer       ���� ������ ����� ����

	\return �浹�� �־����� ���θ� ��ȯ�մϴ�.
	*/
	PX_PHYSX_COMMON_API static bool generateTriangleContacts(const PxGeometry& geom, const PxTransform& pose, const PxVec3 triangleVertices[3], PxU32 triangleIndex, PxReal contactDistance, PxReal meshContactMargin, PxReal toleranceLength, PxContactBuffer& contactBuffer);

	/**
	\brief Checks if provided geometry is valid.

	\param[in] geom	The geometry object.
	\return True if geometry is valid.

	@see PxGeometry
	*/
	PX_PHYSX_COMMON_API static bool isValid(const PxGeometry& geom);
};

#if !PX_DOXYGEN
}
#endif

/** @} */
#endif
