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

#ifndef PX_CONVEX_MESH_DESC_H
#define PX_CONVEX_MESH_DESC_H
/** \addtogroup cooking
@{
*/

#include "foundation/PxVec3.h"
#include "foundation/PxFlags.h"
#include "common/PxCoreUtilityTypes.h"
#include "geometry/PxConvexMesh.h"
#include "PxSDFDesc.h"

#if !PX_DOXYGEN
namespace physx
{
#endif
/**
\brief Flags which describe the format and behavior of a convex mesh.
*/
struct PxConvexFlag
{
	enum Enum
	{
		/**
		PxConvexMeshDesc::triangles �Ǵ� PxConvexMeshDesc::polygons���� 16��Ʈ ���� �ε����� ����� ��Ÿ���ϴ�.
		(�׷��� ������ 32��Ʈ �ε����� �����˴ϴ�)
		@see #PxConvexMeshDesc.indices
		*/
		e16_BIT_INDICES		=	(1<<0),

		/**
		�������� ���� �ٰ����� �ڵ����� �ٽ� ����մϴ�. �� �÷��װ� �������� ���� ��� ��ü ���� ������ �������� �����ؾ� �մϴ�.

		\note ���� ���� �ٰ��� ��꿡�� �� ���� �ٸ� �˰����� �ֽ��ϴ�. PxConvexMeshCookingType�� �����Ͻʽÿ�.

		@see PxConvexMeshCookingType
		*/
		eCOMPUTE_CONVEX		=	(1<<1),	

		/**
		���� ���� �ٰ��� ��� �߿� ���� ���� ���� �ﰢ���� Ȯ���ϰ� �����մϴ�.
		�źε� ���� ũ��� PxCookingParams::areaTestEpsilon���� �����˴ϴ�.

		\note �� �÷��״� eCOMPUTE_CONVEX�� �Բ��� ���˴ϴ�.

		@see PxCookingParams PxCookingParams::areaTestEpsilon
		*/
		eCHECK_ZERO_AREA_TRIANGLES		=	(1<<2),

		/**
		�Է� ������ k-��� Ŭ�����͸��� ����Ͽ� ����ȭ�մϴ�.

		\note �Է� ������ PxConvexMeshDesc::quantizedCount�� ����ȭ�˴ϴ�.
		http://en.wikipedia.org/wiki/K-means_clustering�� �����Ͻʽÿ�.

		*/
		eQUANTIZE_INPUT = (1 << 3),

		/**
		���� �޽� ��ȿ�� �˻縦 ��Ȱ��ȭ�Ͽ� �� ���� �ӵ��� ���Դϴ�. 
		����� ���忡�� ������ ��ȿ�� �˻� �Լ��� ����Ͻʽÿ�.

		������ ��ȿ�� �˻縦 �������� �ʰ� ��ȿ���� ���� 
		�Է� �����ͷ� ���� �޽��� �����ϸ� ���ǵ��� ���� ������ �߻��� �� �ֽ��ϴ�.

		@see PxCooking::validateConvexMesh
		*/
		eDISABLE_MESH_VALIDATION = (1 << 4),

		/**
		���� ���� �˰��� �÷��� �������� Ȱ��ȭ�մϴ�.

		�÷��� �������� ���� ���� ������ ���� ���Ѻ��� ���� ������ ������ �ִ� ��쿡 ���� ��� �˰����Դϴ�.

		�⺻ �˰����� ��ü ��� �Է� ���� ������ OBB�� ����մϴ�. 
		�� OBB�� �� �� �� �÷��ΰ� �����̽��˴ϴ�
		���� ������ ���޵� ������. 
		�⺻ �˰����� ���� ������ ��� 8�� �����ؾ� �ϸ� �Ϲ������� �÷��� �����ú��� �ξ�
		ǰ���� ���� ����� �����մϴ�.

		�÷��� �������� Ȱ��ȭ�Ǹ� �� ����� ���� ���ѿ� ������ �� �����˴ϴ�. 
		�׷� ���� �� �÷����� ��� �Է� ������ �����ϵ��� �̵��Ǹ�
		���ο� ��� �������� ������ ���� �������� ���� ���� �����ϱ� ���� ���˴ϴ�. 
		�÷��� �������� �Է� Ŭ���忡�� �ſ� �� ������ ����
		��ī�ο� �𼭸��� ������ �� ������ ������� �� ���� ��� �Է� ������ ���Ե��� ���� �� �ֽ��ϴ�. 
		�׷��� ���� ������ 4�� ������ �� �ֽ��ϴ�.
		*/
		ePLANE_SHIFTING = (1 << 5),

		/**
		SIMD �ڵ带 ����Ͽ� ���� �ټ��� ����մϴ�. 
		�׷��� ���е��� ���� �ſ� ���� ���� ��� �߸��� ������ �ʷ��� �� �ֽ��ϴ�.
		*/
		eFAST_INERTIA_COMPUTATION = (1 << 6),

		/**
		GPU �ùķ��̼� ������ ����Ͽ� ���� ���� �����մϴ�.
		���� ���� �� �ٰ��� ������ 64�� �����ǰ�
		�� ���� ���� ������ ���������� 32�� �����˴ϴ�.
		\note eCOMPUTE_CONVEX �÷��׿� �Բ��� ����� �� �ֽ��ϴ�.

		@deprecated PhysX 5.2 ����. #PxCookingParams::buildGPUData�� true�� �����ϸ� �׻� GPU ȣȯ �޽��� ���մϴ�.
		*/
		eGPU_COMPATIBLE PX_DEPRECATED = (1 << 7),

		/**
		�Է� ������ �� ���� ��� �������� �����ϱ� ���� ���� �ֺ����� �̵��մϴ�.
		���� �ֺ��� �Է� ������ �����ϴ� ���� �����ϴ�.
		�׷��� ������ �� �÷��׸� ����Ͽ�
		��ġ�� �������� ����ŵ�ϴ�.
		\note eCOMPUTE_CONVEX �÷��׿� �Բ��� ���˴ϴ�.
		*/
		eSHIFT_VERTICES = (1 << 8)
	};
};

/**
\brief collection of set bits defined in PxConvexFlag.

@see PxConvexFlag
*/
typedef PxFlags<PxConvexFlag::Enum,PxU16> PxConvexFlags;
PX_FLAGS_OPERATORS(PxConvexFlag::Enum,PxU16)

/**
\brief Descriptor class for #PxConvexMesh.
\note The number of vertices and the number of convex polygons in a cooked convex mesh is limited to 256.
\note The number of vertices and the number of convex polygons in a GPU compatible convex mesh is limited to 64,
and the number of faces per vertex is limited to 32.

@see PxConvexMesh PxConvexMeshGeometry PxShape PxPhysics.createConvexMesh()

*/
class PxConvexMeshDesc
{
public:

	/**
	\brief Vertex positions data in PxBoundedData format.

	<b>Default:</b> NULL
	*/
	PxBoundedData points;

	/**
	\brief Polygons data in PxBoundedData format.
	<p>Pointer to first polygon. </p>

	<b>Default:</b> NULL	

	@see PxHullPolygon
	*/
	PxBoundedData polygons;

	/**
	\brief Polygon indices data in PxBoundedData format.
	<p>Pointer to first index.</p>

	<b>Default:</b> NULL	

	<p>This is declared as a void pointer because it is actually either an PxU16 or a PxU32 pointer.</p>

	@see PxHullPolygon PxConvexFlag::e16_BIT_INDICES
	*/
	PxBoundedData indices;

	/**
	\brief Flags bits, combined from values of the enum ::PxConvexFlag

	<b>Default:</b> 0
	*/
	PxConvexFlags flags;

	/**
	\brief Limits the number of vertices of the result convex mesh. Hard maximum limit is 255
	and minimum limit is 4 if PxConvexFlag::ePLANE_SHIFTING is used, otherwise the minimum
	limit is 8.

	\note The please see PxConvexFlag::ePLANE_SHIFTING for algorithm explanation
	\note The maximum limit for GPU compatible convex meshes is 64.

	@see PxConvexFlag::ePLANE_SHIFTING

	<b>Range:</b> [4, 255]<br>
	<b>Default:</b> 255
	*/
	PxU16 vertexLimit;

	/**
	\brief Limits the number of polygons of the result convex mesh. Hard maximum limit is 255
	and minimum limit is 4.

	\note The maximum limit for GPU compatible convex meshes is 64.

	<b>Range:</b> [4, 255]<br>
	<b>Default:</b> 255
	 */
	PxU16 polygonLimit;

	/**
	\brief Maximum number of vertices after quantization. The quantization is done during the vertex cleaning phase. 
	The quantization is applied when PxConvexFlag::eQUANTIZE_INPUT is specified.

	@see PxConvexFlag::eQUANTIZE_INPUT

	<b>Range:</b> [4, 65535]<br>
	<b>Default:</b> 255
	*/
	PxU16 quantizedCount;

	/**
	\brief SDF descriptor. When this descriptor is set, signed distance field is calculated for this convex mesh.

	<b>Default:</b> NULL
	*/
	PxSDFDesc* sdfDesc;

	/**
	\brief constructor sets to default.
	*/
	PX_INLINE PxConvexMeshDesc();
	/**
	\brief (re)sets the structure to the default.	
	*/
	PX_INLINE void setToDefault();

	
	/**
	\brief Returns true if the descriptor is valid.

	\return True if the current settings are valid
	*/
	PX_INLINE bool isValid() const;
};

PX_INLINE PxConvexMeshDesc::PxConvexMeshDesc()	//constructor sets to default
: vertexLimit(255), polygonLimit(255), quantizedCount(255), sdfDesc(NULL)
{

}

PX_INLINE void PxConvexMeshDesc::setToDefault()
{
	*this = PxConvexMeshDesc();
}

PX_INLINE bool PxConvexMeshDesc::isValid() const
{
	// Check geometry
	if(points.count < 3 ||	//at least 1 trig's worth of points
		(points.count > 0xffff && flags & PxConvexFlag::e16_BIT_INDICES))
		return false;
	if(!points.data)
		return false;
	if(points.stride < sizeof(PxVec3))	//should be at least one point's worth of data
		return false;
	if (quantizedCount < 4)
		return false;

	// Check topology
	if(polygons.data)
	{
		if(polygons.count < 4) // we require 2 neighbors for each vertex - 4 polygons at least
			return false;

		if(!indices.data) // indices must be provided together with polygons
			return false;

		PxU32 limit = (flags & PxConvexFlag::e16_BIT_INDICES) ? sizeof(PxU16) : sizeof(PxU32);
		if(indices.stride < limit) 
			return false;

		limit = sizeof(PxHullPolygon);
		if(polygons.stride < limit) 
			return false;
	}
	else
	{
		// We can compute the hull from the vertices
		if(!(flags & PxConvexFlag::eCOMPUTE_CONVEX))
			return false;	// If the mesh is convex and we're not allowed to compute the hull,
							// you have to provide it completely (geometry & topology).
	}

	if((flags & PxConvexFlag::ePLANE_SHIFTING) &&  vertexLimit < 4)
	{
		return false;
	}

	if (!(flags & PxConvexFlag::ePLANE_SHIFTING) && vertexLimit < 8)
	{
		return false;
	}

	if(vertexLimit > 255)
	{
		return false;
	}

	if (polygonLimit < 4)
	{
		return false;
	}

	if (polygonLimit > 255)
	{
		return false;
	}

	if (sdfDesc && !sdfDesc->isValid())
	{
		return false;
	}

	return true;
}

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
