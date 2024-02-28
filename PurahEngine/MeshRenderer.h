#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class PURAHENGINE_API MeshRenderer final : public Renderer
	{
	public:
		MeshRenderer();

		enum class MeshType
		{
			None,
			Cube
		};

		// Renderer을(를) 통해 상속됨
		void Render(IZeldaRenderer* renderer) override;

		void SetTexture(const std::wstring& textureName);
		void SetMesh(MeshType type);
		void SetWireFrame(bool value);
		void SetColor(float r, float g, float b, float a);

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		std::wstring textureName;
		MeshType meshType;
		bool wireFrame;
		float r;
		float g;
		float b;
		float a;
	};
}