#pragma once
#include <queue>
#include <unordered_map>
#include <Eigen/Dense>

#include "IslandID.h"
#include "Singleton.h"

namespace Phyzzle
{
	class AttachIsland;
	class Attachable;
}

namespace PurahEngine
{
	class RigidBody;
}

namespace Phyzzle
{
	class AttachSystem final : public Singleton<AttachSystem>
	{
	public:
		~AttachSystem() override;

	private:
		using AttachIsland = std::vector<Attachable*>;
		std::unordered_map<IslandID, AttachIsland> attachIsland;
		std::queue<IslandID> removedIndex;

		IslandID islandID;
		bool gravity = true;
		float mass = 1.f;

	private:
		// ID ���� ����
		IslandID CreateIslandID();
		void RemoveIslandID(const IslandID& _id);

		// �� ���� ����
		IslandID CreateIsland(const std::vector<Attachable*>& _arr);
		void RemoveIsland(const IslandID& _id);

	public:
		void SelectBody(Attachable* _body);
		void DeselectBody(Attachable* _body);
		bool Attach(Attachable* _base);
		bool Dettach(Attachable* _base);

		void ConnectNode(Attachable* _base, Attachable* _other);
		void ConnectJoint(Attachable* _base, Attachable* _other);

		bool HasAttachIsland(const IslandID& _id, AttachIsland& _island);

		void CalculateLocalAnchor(
			const Eigen::Vector3f& _anchorP, const Eigen::Quaternionf& _anchorQ,
			const Attachable* _base,
			Eigen::Vector3f& _outP, Eigen::Quaternionf& _outQ
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