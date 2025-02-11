#pragma once
#include <utility>

namespace Phyzzle
{
	struct PlayerInput
	{
		struct StickData
		{
			float X;
			float Y;
			float Size;

			StickData() = default;
			StickData(const StickData& stickData) 
				: X(stickData.X), Y(stickData.Y), Size(stickData.Size)
			{}
			StickData(StickData&& stickData) noexcept
				: X(std::move(stickData.X)), 
				Y(std::move(stickData.Y)),
				Size(std::move(stickData.Size))
			{}
			StickData& operator=(const StickData& _other)
			{
				if (this != &_other)
				{
					X = _other.X;
					Y = _other.Y;
					Size = _other.Size;
				}

				return *this;
			}
			StickData& operator=(StickData&& _other) noexcept
			{
				if (this != &_other)
				{
					X = std::move(_other.X);
					Y = std::move(_other.Y);
					Size = std::move(_other.Size);
				}

				return *this;
			}
		};

		StickData Lstick;
		StickData Rstick;
		float LTrigger;
		float RTrigger;

		PlayerInput() = default;
		PlayerInput(const PlayerInput& playerData)
			: Lstick(playerData.Lstick), Rstick(playerData.Rstick), 
			LTrigger(playerData.LTrigger), RTrigger(playerData.RTrigger) 
		{}
		PlayerInput(PlayerInput&& playerData) noexcept
			: Lstick(std::move(playerData.Lstick)), Rstick(std::move(playerData.Rstick)),
			LTrigger(std::move(playerData.LTrigger)), RTrigger(std::move(playerData.RTrigger))
		{}
		PlayerInput& operator=(const PlayerInput& _other)
		{
			if (this != &_other)
			{

				Lstick = _other.Lstick;
				Rstick = _other.Rstick;

				LTrigger = _other.LTrigger;
				RTrigger = _other.RTrigger;
			}

			return *this;
		}
		PlayerInput& operator=(PlayerInput&& _other) noexcept
		{
			if (this != &_other)
			{
				Lstick = std::move(_other.Lstick);
				Rstick = std::move(_other.Rstick);

				LTrigger = std::move(_other.LTrigger);
				RTrigger = std::move(_other.RTrigger);
			}

			return *this;
		}
	};
}