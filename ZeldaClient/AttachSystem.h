#pragma once
#include <queue>
#include <unordered_map>
#include <Eigen/Dense>

#include "IslandID.h"
#include "PurahColor.h"
#include "Singleton.h"

namespace Phyzzle
{
	class AttachIsland;
	class PzObject;
}

namespace ZonaiPhysics
{
	struct ZnBound3;
}

namespace PurahEngine
{
	class FixedJoint;
	class RigidBody;
}

namespace Phyzzle
{
	class AttachSystem final : public Singleton<AttachSystem>
	{
	public:
		~AttachSystem() override;

	private:
		using AttachIsland = std::vector<PzObject*>;
		std::unordered_map<IslandID, AttachIsland> attachIsland;
		std::queue<IslandID> removedIndex;

	private:
		// ID 생성 삭제
		IslandID					CreateIslandID();
		void						RemoveIslandID(IslandID _id);

		// 섬 생성 삭제
		IslandID					CreateIsland(const std::vector<PzObject*>& _arr);
		void						RemoveIsland(IslandID _id);

	public:
		void						Clear();

		void						SelectBody(PzObject* _body);
		void						DeselectBody(PzObject* _body);

		void						SetOutlineColor(Eigen::Vector4f* const _color0, Eigen::Vector4f* const _color1, Eigen::Vector4f* const _color2);
		void						ApplyOutlineSettings(PzObject* obj, bool value, Eigen::Vector4f* const color);
		void						ApplyDShadowSettings(PzObject* obj, bool value);
		void						EnableOutline(PzObject*, Eigen::Vector4f* const _targetColor, Eigen::Vector4f* const _subColor = nullptr);
		void						DisableOutline(PzObject*);
		void						EnableDShadow(PzObject* _obj);
		void						DisableDShadow(PzObject* _obj);

		bool						IsTouching(PzObject* _base);
		bool						TryAttach(PzObject* _base);
		bool						Attach(PzObject* _base, PzObject* _other);
		bool						Dettach(PzObject* _base);

		void						ConnectNode(PzObject* _base, PzObject* _other, PurahEngine::FixedJoint* _joint);
		void						DisconnectNode(PzObject* _base, PzObject* _other);

		PurahEngine::FixedJoint*	CreateJoint(PzObject* _base, PzObject* _other);
		void						BreakJoint(PzObject* _base, PzObject* _other);

		bool						HasAttachIsland(const IslandID& _id, AttachIsland& _island);
		void						RebuildIsland(PzObject* _base, PzObject* _other);

		uint32_t					GetCountInIsland(PzObject* _obj);

		void						CalculateLocalAnchor(
										const Eigen::Vector3f& _anchorP, 
										const Eigen::Quaternionf& _anchorQ,
										const PzObject* _base,
										Eigen::Vector3f& _outP, 
										Eigen::Quaternionf& _outQ
									);

		/// <summary>
		/// 섬의 바운딩 박스를 구하는 함수
		/// </summary>
		/// <param name="_base">attachable pointer</param>
		/// <param name="_mat">world matrix</param>
		/// <returns></returns>
		ZonaiPhysics::ZnBound3		ComputeBoundingBox(PzObject* const _base, const Eigen::Matrix4f& _mat);
		ZonaiPhysics::ZnBound3		ComputeBoundingBoxAtTransform(PzObject* const _base, const Eigen::Matrix4f& _mat);

	public:
		Eigen::Vector4f color0;
		Eigen::Vector4f color1;
		Eigen::Vector4f color2;
	};
}

/*
 * 1.	플레이어가 울트라핸드로 오브젝트 하나를 선택
 *
 * 2.	오브젝트와 부착할 수 있는 오브젝트를 탐색
 *
 * 3.	일정 거리에 다다르면 Attatch가 가능.
 *
 * 4.	Attatch를 하면 두 오브젝트는 일정한 속력으로 서로에게 가까워짐
 *
 * 5.	두 물체가 충돌을 감지하면 충돌한 위치를 Anchor 위치로 하고 FixedJoint를 연결함.
 *
 * 6.	Anchor의 좌표는 월드로 나오기 때문에 로컬로 계산해서 조인트를 만들 필요가 있음.
*/