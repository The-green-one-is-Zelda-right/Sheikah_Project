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

		void Active();
		void Deactive();
		void Store(Rewindable* _object, Snapshot* _snapshot);
		bool Check(Snapshot* _last, Snapshot* _newSnapshot);

	private:
		static Rewindable* target;

	private:
		std::unordered_map<Rewindable*, std::list<Snapshot*>> history;
	};
}
