#pragma once
#include "Component.h"

namespace PurahEngine
{
	class Transform : public Component
	{
	public:
		float position[3];
		float rotation[4];       // ���Ϸ�? ���ʹϾ�?
		float scale[3];


	private:

	};
}
