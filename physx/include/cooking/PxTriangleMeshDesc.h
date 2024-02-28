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

#ifndef PX_TRIANGLE_MESH_DESC_H
#define PX_TRIANGLE_MESH_DESC_H
/** \addtogroup cooking
@{
*/

#include "PxPhysXConfig.h"
#include "geometry/PxSimpleTriangleMesh.h"
#include "PxSDFDesc.h"

#if !PX_DOXYGEN
namespace physx
{
#endif


/**
\brief #PxTriangleMesh에 대한 설명자 클래스입니다.

이 클래스는 기본 메시를 설명하는 멤버를 포함하는 PxSimpleTriangleMesh에서 파생됩니다. 
이 설명자로부터 PxTriangleMesh 객체가 생성될 때 메시 데이터가 복사됩니다. 
호출 후에는 사용자가 삼각형 데이터를 폐기할 수 있습니다.

@see PxTriangleMesh PxTriangleMeshGeometry PxShape
*/
class PxTriangleMeshDesc : public PxSimpleTriangleMesh
{
public:

	/**
	옵션으로 첫 번째 재료 인덱스를 가리키는 포인터이며, NULL일 수 있습니다.
	총 PxSimpleTriangleMesh::numTriangles 개의 인덱스가 있습니다.
	호출자는 materialIndexStride 바이트를 포인터에 추가하여 다음 삼각형에 액세스할 수 있습니다.

	삼각형 메시가 다른 객체와 충돌할 때 충돌 지점에 재료가 필요합니다.
	materialIndices가 NULL이면 PxShape 인스턴스의 재료가 사용됩니다.
	그렇지 않으면, 접촉 지점이 인덱스 i의 삼각형에 있을 때,
	재료 인덱스는 다음과 같이 결정됩니다:
	PxMaterialTableIndex index = *(PxMaterialTableIndex )(((PxU8)materialIndices) + materialIndexStride * i);

	접촉 지점이 버텍스 또는 엣지에 있는 경우,
	버텍스 또는 엣지에 인접한 삼각형이 선택되고 해당 인덱스를 사용하여 재료를 조회합니다.
	이 선택은 임의적이지만 시간이 지나도 일관적입니다.

	<b>기본값:</b> NULL

	@see materialIndexStride
	*/
	PxTypedStridedData<PxMaterialTableIndex> materialIndices;

	/**
	\brief SDF descriptor. When this descriptor is set, signed distance field is calculated for this convex mesh.

	<b>Default:</b> NULL
	*/
	PxSDFDesc* sdfDesc;

	/**
	\brief Constructor sets to default.
	*/
	PX_INLINE PxTriangleMeshDesc();

	/**
	\brief (re)sets the structure to the default.	
	*/
	PX_INLINE void setToDefault();

	/**
	\brief Returns true if the descriptor is valid.
	\return true if the current settings are valid
	*/
	PX_INLINE bool isValid() const;
};


PX_INLINE PxTriangleMeshDesc::PxTriangleMeshDesc()	//constructor sets to default
{
	PxSimpleTriangleMesh::setToDefault();
	sdfDesc = NULL;
}

PX_INLINE void PxTriangleMeshDesc::setToDefault()
{
	*this = PxTriangleMeshDesc();
}

PX_INLINE bool PxTriangleMeshDesc::isValid() const
{
	if(points.count < 3) 	//at least 1 trig's worth of points
		return false;
	if ((!triangles.data) && (points.count%3))		// Non-indexed mesh => we must ensure the geometry defines an implicit number of triangles // i.e. numVertices can't be divided by 3
		return false;
	//add more validity checks here
	if (materialIndices.data && materialIndices.stride < sizeof(PxMaterialTableIndex))
		return false;

	if (sdfDesc && !sdfDesc->isValid())
			return false;
	
	return PxSimpleTriangleMesh::isValid();
}


#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
