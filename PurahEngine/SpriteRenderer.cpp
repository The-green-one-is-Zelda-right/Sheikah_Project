#include "SpriteRenderer.h"

#include "GameObject.h"
#include "Transform.h"
#include "TimeController.h"

namespace PurahEngine
{
	void SpriteRenderer::OnEnable()
	{
		elapsedTime = 0.0f;
	}

	void SpriteRenderer::Update()
	{
		float deltaTime = TimeController::GetInstance().GetDeltaTime();

		if (elapsedTime < eventTime)
		{
			elapsedTime += deltaTime;
		}
		else
		{
			elapsedTime = eventTime;
		}
	}

	void SpriteRenderer::Render(IZeldaRenderer* renderer)
	{
		if (IsRootEnable())
		{
			TextureID textureID = TextureID::ID_NULL;

			::Color color;
			if (useEnableEvent)
			{
				color.r = sr + (elapsedTime / eventTime) * (r - sr);
				color.g = sg + (elapsedTime / eventTime) * (g - sg);
				color.b = sb + (elapsedTime / eventTime) * (b - sb);
				color.a = sa + (elapsedTime / eventTime) * (a - sa);
			}
			else
			{
				color = { r, g, b, a };
			}

			if (textureName != L"")
			{
				textureID = GetTextureID(textureName);
				renderer->DrawSprite(GetGameObject()->GetTransform()->GetWorldMatrix(), textureID, color, false);
			}
		}
	}

	void SpriteRenderer::SetTexture(const std::wstring& fileName)
	{
		textureName = fileName;
	}

	void SpriteRenderer::PreSerialize(json& jsonData) const
	{
	}
	void SpriteRenderer::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_WSTRING(textureName);
		PREDESERIALIZE_VALUE(r);
		PREDESERIALIZE_VALUE(g);
		PREDESERIALIZE_VALUE(b);
		PREDESERIALIZE_VALUE(a);

		PREDESERIALIZE_VALUE(useEnableEvent);
		PREDESERIALIZE_VALUE(eventTime);
		PREDESERIALIZE_VALUE(sr);
		PREDESERIALIZE_VALUE(sg);
		PREDESERIALIZE_VALUE(sb);
		PREDESERIALIZE_VALUE(sa);
	}
	void SpriteRenderer::PostSerialize(json& jsonData) const
	{
	}
	void SpriteRenderer::PostDeserialize(const json& jsonData)
	{
	}
}