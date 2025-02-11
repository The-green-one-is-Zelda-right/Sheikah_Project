#include "RewindSystem2.h"
#include "Player.h"

namespace Phyzzle
{
	RewindSystem2::RewindSystem2(Player* player) 
		: player(player) 
	{}

	bool RewindSystem2::StartRewind()
	{
		if (player->CanRewind())
		{
			player->BeginRewind();
			return true;
		}
		return false;
	}

	void RewindSystem2::StopRewind()
	{
		player->EndRewind();
	}
}
