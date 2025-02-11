#pragma once

namespace Phyzzle
{
	class Player;

	class AttachSystem2
	{
	public:
		explicit AttachSystem2(Player* player);
		bool TryAttach();   // 부착 시도
		void CancelAttach(); // 부착 취소

	private:
		Player* player;
	};
}
