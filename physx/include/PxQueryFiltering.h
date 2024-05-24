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

#ifndef PX_QUERY_FILTERING_H
#define PX_QUERY_FILTERING_H
/** \addtogroup scenequery
@{
*/

#include "PxPhysXConfig.h"
#include "PxFiltering.h"
#include "PxQueryReport.h"
#include "PxClient.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxShape;
class PxRigidActor;
struct PxQueryHit;

/**
\brief Filtering flags for scene queries.

@see PxQueryFilterData.flags
*/
struct PxQueryFlag
{
	enum Enum
	{
		eSTATIC							= (1<<0),	//!< ���� ���¸� Ž���մϴ�.

		eDYNAMIC						= (1<<1),	//!< ���� ���¸� Ž���մϴ�.

		ePREFILTER						= (1<<2),	//!< ���� ���� �˻� ����(pre-intersection-test filter)�� �����մϴ�.
													//!< (PxQueryFilterCallback::preFilter() ����)

		ePOSTFILTER						= (1<<3),	//!< ���� ���� �˻� ����(post-intersection-test filter)�� �����մϴ�.
													//!< (PxQueryFilterCallback::postFilter() ����)

		eANY_HIT						= (1<<4),	//!< �߰ߵ� ��� ��� Ž���� �ߴ��ϰ� �ݹ��� ���� �ش� ��Ʈ�� ��ȯ�մϴ�.
													//!< ���� ������ ����ŵ�ϴ�. �� �÷��״� eTOUCH�� eBLOCK hitTypes�� ��� �ش� �÷��׷� ���� ��Ʈ�� ���ֵ˴ϴ�.

		eNO_BLOCK						= (1<<5),	//!< ��� ��Ʈ�� ����(touching)���� �����մϴ�. ����� ���Ϳ��� eBLOCK�� eTOUCH�� �����Ͽ� eBLOCK�� ��ȯ�Ǵ� ���� �������մϴ�.
													//!< �̰��� ���� ������ ����ų �� �ִ� ����ȭ ��Ʈ�Դϴ�.
		
		eBATCH_QUERY_LEGACY_BEHAVIOUR	= (1<<6),	//!< ���Ž� ��ġ ���� ���� ������ ����մϴ�.
													//!< �� �÷��׸� �ø��� �ϵ��ڵ��� ���� �������� ���õ˴ϴ�.
													//!< �̷ν� ePREFILTER�� ePOSTFILTER �÷��׿� ���� ������ PxQueryFilterCallback�� ������ ����˴ϴ�.

		eDISABLE_HARDCODED_FILTER		= (1<<6),	//!< eBATCH_QUERY_LEGACY_BEHAVIOUR�� ����������, ���� ��Ȯ�� �̸��Դϴ�.
													//!< �Ϲ�/���ġ ���������� ����� �� ������ ��������� ��Ÿ���ϴ�.

		eRESERVED						= (1<<15)	//!< ���� ����� ���� ����� �÷����Դϴ�.
	};
};
PX_COMPILE_TIME_ASSERT(PxQueryFlag::eSTATIC==(1<<0));
PX_COMPILE_TIME_ASSERT(PxQueryFlag::eDYNAMIC==(1<<1));
PX_COMPILE_TIME_ASSERT(PxQueryFlag::eBATCH_QUERY_LEGACY_BEHAVIOUR==PxQueryFlag::eDISABLE_HARDCODED_FILTER);

/**
\brief Flags typedef for the set of bits defined in PxQueryFlag.

*/
typedef PxFlags<PxQueryFlag::Enum,PxU16> PxQueryFlags;
PX_FLAGS_OPERATORS(PxQueryFlag::Enum,PxU16)

/**
\brief �� ���� ��Ʈ(����)�� �з�.

	- eNONE: �� ��Ʈ ������ ��ȯ�ϸ� �ش� ��Ʈ�� �������� �ʾƾ� ���� �ǹ��մϴ�.
	- eBLOCK: ��� ����ĳ��Ʈ, ���� �� ������ ������ ���� ���� ����� eBLOCK ���� ��Ʈ�� �׻� PxHitCallback::block ����� ��ȯ�˴ϴ�.
	- eTOUCH: ����ĳ��Ʈ, ���� �Ǵ� ������ ������ PxHitCallback::nbTouches �� PxHitCallback::touches �Ű������� ȣ��� ���, ���������� ���� ����� eBLOCK ���� ��Ʈ�� �Ÿ��� ���ų� �� �����(eTOUCH hits that are closer or same distance) eTOUCH ���� ��Ʈ�� ����˴ϴ�. (touchDistance <= blockDistance ����)
	- ���� ���, ����ĳ��Ʈ ������ ��� ��Ʈ�� ����Ϸ��� �׻� eTOUCH�� ��ȯ�ϼ���.

������() ������ ��� ��Ʈ�� ���� �Ÿ��� 0���� ó���˴ϴ�.
�̴� ��Ʈ�� ���ĵ��� ������ ��� eTOUCH ��Ʈ�� �ݹ鿡 ��ϵǸ� eBLOCK ������ ��Ʈ�� �������� ��� eTOUCH ��Ʈ�� ��ϵ˴ϴ�.
��� ������() ���� ��Ʈ�� ���̰� 0������, �ϳ� (������)�� eBLOCK ������ ��Ʈ�� PxHitCallback::block�� ��ϵ˴ϴ�.
��� ������() eTOUCH ���� ��Ʈ�� ����˴ϴ�. (zero touchDistance <= zero blockDistance condition)

PxHitCallback::nbTouches ��� �Ǵ� ���� ��ġ ���۷� ����ĳ��Ʈ/����/������ ȣ���� ���,
eBLOCK ������ ���� ����� ��Ʈ�� ��ȯ�˴ϴ�. ��� eTOUCH ��Ʈ�� �����˴ϴ�.

@see PxQueryFilterCallback.preFilter PxQueryFilterCallback.postFilter PxScene.raycast PxScene.sweep PxScene.overlap
*/
struct PxQueryHitType
{
	enum Enum
	{
		eNONE	= 0,	//!< ������ �� ���¸� �����ؾ� �մϴ�.
		eTOUCH	= 1,	//!< ���¿� ���� ��Ʈ�� ������ ���� ������Ʈ���� ���������� �������� �ʽ��ϴ�.
		eBLOCK	= 2		//!< ���¿� ���� ��Ʈ�� ������ �����մϴ�. (��ħ ������ ���������� ����)
	};
};

/**
\brief �� ���� ���͸� ������.

�� ������ ���¿� �����Ǵ� ���, ���͸��� ������ ���� ������ ����˴ϴ�:

\li ��ġ ������ �ƴ� ��츸: <br>���� ������ �ʵ尡 0�� �ƴϰ�,
	�����Ϳ� ������ queryFilterData�� ��Ʈ AND ���� 0�̸�, �ش� ���´� �ǳʶݴϴ�.

\li �÷��� �ʵ忡�� ���� �ݹ��� Ȱ��ȭ�� ��� (PxQueryFlags ����), �ش�Ǵ� �ݹ��� ȣ��˴ϴ�.

\li #PxQueryFlag::ePREFILTER �Ǵ� #PxQueryFlag::ePOSTFILTER �� �ϳ��� �����Ǿ� ���� ������,
	PxHitCallback::nbTouches�� ���� 0�̰� PxHitCallback::nbTouches�� ������ ������ ������,
	��Ʈ�� �⺻������ #PxQueryHitType::eBLOCK �������� �����˴ϴ�.
	�׷��� ������ #PxQueryHitType::eTOUCH �������� �����˴ϴ�.

@see PxScene.raycast PxScene.sweep PxScene.overlap PxQueryFlag::eANY_HIT
*/
struct PxQueryFilterData
{
	/** \brief default constructor */
	explicit PX_INLINE PxQueryFilterData() : flags(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC)		{}

	/** \brief constructor to set both filter data and filter flags */
	explicit PX_INLINE PxQueryFilterData(const PxFilterData& fd, PxQueryFlags f) : data(fd), flags(f)	{}

	/** \brief constructor to set filter flags only */
	explicit PX_INLINE PxQueryFilterData(PxQueryFlags f) : flags(f)										{}

	PxFilterData	data;		//!< Filter data associated with the scene query
	PxQueryFlags	flags;		//!< Filter flags (see #PxQueryFlags)
};

/**
\brief �� ���� ���͸� �ݹ��Դϴ�.

�� ���� ���� �ĺ��� ���� ����� ���� ���͸� �����Դϴ�. ���� �ĺ� ��ü�� ������ ��� ����(#PxQueryFilterData ����)�� ����ϸ�,
���͸� �ݹ��� ��û�� ���(#PxQueryFilterData.flags ����),

\li #PxQueryFlag::ePREFILTER�� ������ ���,
	preFilter �Լ��� ��Ȯ�� ���� �׽�Ʈ ������ ����˴ϴ�.
	�� �Լ��� #PxQueryHitType::eTOUCH �Ǵ� #PxQueryHitType::eBLOCK�� ��ȯ�ϸ�
	��Ȯ�� �׽�Ʈ�� ����Ǿ� ���� ��ġ�� �����˴ϴ�.

preFilter �Լ��� �μ��� ���޹��� queryFlags�� �纻�� ��� �� �־,
#PxHitFlag::eMODIFIABLE_FLAGS �� �Ϻθ� ���� ��� �������� ������ �� �ֽ��ϴ�.
���� ������ ���͸��� ��翡�� ����Ǹ�, �ٸ� �÷��׿� ���� ������ ���õ˴ϴ�.

\li #PxQueryFlag::ePREFILTER�� �������� ���� ���,
	���� ������ filterData.flags�� ����Ͽ� ��Ȯ�� ���� �׽�Ʈ�� ����˴ϴ�.

\li #PxQueryFlag::ePOSTFILTER�� ������ ���,
	�� ������ ���� postFilter �Լ��� ȣ��Ǿ� ��ġ/��� ���¸� �����մϴ�.
	�̴� �� ��翡 ���� ������ preFilter �Լ����� ��ȯ�� ��ġ/��� ���¸� �������մϴ�.

���͸� ȣ���� �����̳� ������ ������ ���� ���ĵ��� �ʽ��ϴ�.

@see PxScene.raycast PxScene.sweep PxScene.overlap PxQueryFlags PxHitFlags
*/
class PxQueryFilterCallback
{
public:

	/**
	\brief This filter callback is executed before the exact intersection test if PxQueryFlag::ePREFILTER flag was set.

	\param[in] filterData		custom filter data specified as the query's filterData.data parameter.
	\param[in] shape			A shape that has not yet passed the exact intersection test.
	\param[in] actor			The shape's actor.
	\param[in,out] queryFlags	scene query flags from the query's function call (only flags from PxHitFlag::eMODIFIABLE_FLAGS bitmask can be modified)
	\return the updated type for this hit  (see #PxQueryHitType)
	*/
	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) = 0;

	/**
	\brief This filter callback is executed if the exact intersection test returned true and PxQueryFlag::ePOSTFILTER flag was set.

	\param[in] filterData	custom filter data of the query
	\param[in] hit			Scene query hit information. faceIndex member is not valid for overlap queries. For sweep and raycast queries the hit information can be cast to #PxSweepHit and #PxRaycastHit respectively.
	\param[in] shape		Hit shape
	\param[in] actor		Hit actor
	\return the updated hit type for this hit  (see #PxQueryHitType)
	*/
	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor) = 0;

	/**
	\brief virtual destructor
	*/
	virtual ~PxQueryFilterCallback() {}
};


#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
