#pragma once

namespace Phyzzle
{
	class Player;

	class RewindSystem2
	{
	public:
		explicit RewindSystem2(Player* player);
		bool StartRewind(); // 되감기 시작
		void StopRewind();  // 되감기 종료

	private:
		Player* player;
	};
}
