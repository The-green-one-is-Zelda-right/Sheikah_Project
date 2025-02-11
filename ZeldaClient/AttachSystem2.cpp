#include "AttachSystem2.h"
#include "Player.h"

namespace Phyzzle
{
	AttachSystem2::AttachSystem2(Player* player) 
		: player(player) 
	{}

	bool AttachSystem2::TryAttach()
	{
		// 부착 가능 여부 확인
		if (player->CheckAttachableObject())
		{
			player->AttachToObject();
			return true;
		}
		return false;
	}

	void AttachSystem2::CancelAttach()
	{
		player->DetachObject();
	}
}
