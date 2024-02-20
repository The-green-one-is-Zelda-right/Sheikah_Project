#include "DirectionalLight.h"

namespace PurahEngine
{
	void DirectionalLight::Initialize()
	{
		CreateDirectionalLight();
	}

	DirectionalLight::~DirectionalLight()
	{
		ReleaseLight();
	}


}