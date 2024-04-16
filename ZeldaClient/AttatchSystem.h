#pragma once
#include <queue>

#include "Singleton.h"

namespace PurahEngine
{
	class RigidBody;
}

namespace Phyzzle
{
	class AttatchSystem final : public Singleton<AttatchSystem>
	{
	public:
		~AttatchSystem() override;

	public:
		void Update();

		bool HasSelectedBody() const;
		void SelectBody(PurahEngine::RigidBody* _body);
		void Cancel();
		void XRotate(float _degree);
		void YRotate(float _degree);
		void Attatch(PurahEngine::RigidBody* _base, PurahEngine::RigidBody* _target);


	private:
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