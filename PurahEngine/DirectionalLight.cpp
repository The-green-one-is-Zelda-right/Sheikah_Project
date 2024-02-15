#include "DirectionalLight.h"

namespace PurahEngine
{
	DirectionalLight::DirectionalLight()
	{
		CreateDirectionalLight();
	}

	DirectionalLight::~DirectionalLight()
	{
		ReleaseLight();
	}
}