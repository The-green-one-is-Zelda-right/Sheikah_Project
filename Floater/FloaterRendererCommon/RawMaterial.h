#pragma once
#include "RawTexture.h"
#include <string>

namespace flt
{
	struct Material
	{
		std::wstring name;
		
		RawTexture* albedo;
		RawTexture* normal;
		RawTexture* metallic;
		RawTexture* roughness;
		RawTexture* ao;
		RawTexture* emissive;
		RawTexture* height;
		RawTexture* opacity;

		float baseColor[3];
		float specular[3];
		float emissiveColor[3];
		float metallic;
		float roughness;
		float opacity;
	};
}
