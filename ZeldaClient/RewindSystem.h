#pragma once
#include <queue>
#include "Singleton.h"

namespace Phyzzle
{
	struct Snapshot;
	class Rewindable;

	class RewindSystem : public Singleton<RewindSystem>
	{
	public:
		void Update();						// ������Ʈ
		void Rewind();						// �ǵ����� ����
		void Select(Rewindable* _target);	// Ÿ���� ����
		void Cancel();						// �ɷ��� �����

		void Active();
		void Deactive();
		void Store();

	private:
		static Rewindable* target;

	private:
		std::vector<Rewindable*> activatedObject;
		std::queue<Rewindable*> activateObject;
		std::queue<Rewindable*> deactivateObject;

	private:
		std::unordered_map<Rewindable*, std::list<Snapshot*>> history;
	};
}
