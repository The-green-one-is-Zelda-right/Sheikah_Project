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

	public:
		Eigen::Vector4f color0;
		Eigen::Vector4f color1;
		Eigen::Vector4f color2;

	private:
		// ID ���� ����
		IslandID					CreateIslandID();
		void						RemoveIslandID(IslandID _id);

		// �� ���� ����
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

		void						CalculateLocalAnchor(
										const Eigen::Vector3f& _anchorP, 
										const Eigen::Quaternionf& _anchorQ,
										const PzObject* _base,
										Eigen::Vector3f& _outP, 
										Eigen::Quaternionf& _outQ
									);

		/// <summary>
		/// ���� �ٿ�� �ڽ��� ���ϴ� �Լ�
		/// </summary>
		/// <param name="_base">attachable pointer</param>
		/// <param name="_mat">world matrix</param>
		/// <returns></returns>
		ZonaiPhysics::ZnBound3		ComputeBoundingBox(PzObject* const _base, const Eigen::Matrix4f& _mat);
		ZonaiPhysics::ZnBound3		ComputeBoundingBoxAtTransform(PzObject* const _base, const Eigen::Matrix4f& _mat);
	};
}

/*
 * 1.	�÷��̾ ��Ʈ���ڵ�� ������Ʈ �ϳ��� ����
 *
 * 2.	������Ʈ�� ������ �� �ִ� ������Ʈ�� Ž��
 *
 * 3.	���� �Ÿ��� �ٴٸ��� Attatch�� ����.
 *
 * 4.	Attatch�� �ϸ� �� ������Ʈ�� ������ �ӷ����� ���ο��� �������
 *
 * 5.	�� ��ü�� �浹�� �����ϸ� �浹�� ��ġ�� Anchor ��ġ�� �ϰ� FixedJoint�� ������.
 *
 * 6.	Anchor�� ��ǥ�� ����� ������ ������ ���÷� ����ؼ� ����Ʈ�� ���� �ʿ䰡 ����.
*/