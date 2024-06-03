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
		// ID ���� ����
		IslandID CreateIslandID();
		void RemoveIslandID(IslandID _id);

		// �� ���� ����
		IslandID CreateIsland(const std::vector<PzObject*>& _arr);
		void RemoveIsland(IslandID _id);

	public:
		void						Clear();

		void						SelectBody(PzObject* _body);
		void						DeselectBody(PzObject* _body);

		void						ApplyOutlineSettings(PzObject* obj, const Eigen::Vector4f& color, bool value);
		void						EnableOutline(PzObject*, bool, const Eigen::Vector4f& _targetColor = PurahColor::BlueViolet, const Eigen::Vector4f& _subColor = PurahColor::Yellow);


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