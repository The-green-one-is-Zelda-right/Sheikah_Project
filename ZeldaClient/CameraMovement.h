#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class GameObject;

	class CameraMovement : public Component
	{
	public:
		CameraMovement();
		~CameraMovement();

		void Update();

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}

