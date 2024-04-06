#pragma once
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

		void Store();

	private:
		static Rewindable* target;

		std::unordered_map<Rewindable*, std::list<Snapshot*>> history;
	};
}
