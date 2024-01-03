#pragma once
#include "RawTexture.h"
#include <string>

namespace flt
{
	struct RawMaterial
	{
		static constexpr int MAX_TEXTURES = 10;

		enum TextureType
		{
			ALBEDO = 0,
			NORMAL,
			METALLIC,
			ROUGHNESS,
			AO,
			EMISSIVE,
			HEIGHT,
			OPACITY,
			UNKNOWN
		};

		RawMaterial() :
			name(),
			textures{},
			baseColor{ 0.0f, 0.0f, 0.0f }, specular{ 0.0f, 0.0f, 0.0f }, emissiveColor{ 0.0f, 0.0f, 0.0f }, ambient{0.0f, 0.0f, 0.0f}, metallic(0.0f), roughness(0.0f), opacity(0.0f) 
		{
			for (int i = 0; i < MAX_TEXTURES; ++i)
			{
				textures[i] = new(std::nothrow) RawTexture();
			}
		}
		~RawMaterial()
		{
			for (int i = 0; i < MAX_TEXTURES; ++i)
			{
				delete textures[i];
			}
		}


		std::wstring name;

		RawTexture* textures[MAX_TEXTURES];

		float baseColor[3];
		float specular[3];
		float emissiveColor[3];
		float ambient[3];
		float metallic;
		float roughness;
		float opacity;
	};
}
