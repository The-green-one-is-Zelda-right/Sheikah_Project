#pragma once

#include "IZeldaRenderer.h"

namespace PurahEngine
{
	struct Color
	{
		float r, g, b, a;
	};

	constexpr Color Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	constexpr Color Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	constexpr Color Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	constexpr Color Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	constexpr Color White = { 1.0f, 1.0f, 1.0f, 1.0f };

	class IRenderer
	{
	public:
		virtual void Render(IZeldaRenderer* renderer) abstract;
	};
}