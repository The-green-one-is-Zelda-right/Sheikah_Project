#include "PointLight.h"

namespace PurahEngine
{
	void PointLight::Initialize()
	{
		CreatePointLight(1.0f);
	}
	PointLight::~PointLight()
	{
		ReleaseLight();
	}
}