#pragma once
#include <queue>

#include "Singleton.h"

namespace Phyzzle
{
	class AttachIsland;
}

namespace Phyzzle
{
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

	public:
		void Update();

		bool HasSelectedBody() const;
		void SelectBody(PurahEngine::RigidBody* _body);
		void Cancel();
		void XRotate(float _degree);
		void YRotate(float _degree);
		void Attatch(PurahEngine::RigidBody* _base, PurahEngine::RigidBody* _target);


	private:
		std::vector<AttachIsland*> attachIsland;

		PurahEngine::RigidBody* selectBody = nullptr;
		bool gravity = true;
		float mass = 1.f;
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