#include "Image.h"

#include "GameObject.h"
#include "UI.h"

#include "UnifiedInputManager.h"
#include "GraphicsManager.h"
#include "TimeController.h"

namespace PurahEngine
{
	void Image::OnEnable()
	{
		elapsedTime = 0.0f;
	}

	void Image::Update()
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

	void Image::Awake()
	{
		ui = GetGameObject()->GetComponent<UI>();
	}

	void Image::Render(IZeldaRenderer* renderer)
	{
		Eigen::Vector2f position = ui->GetScreenPosition();
		Eigen::Vector2f size = ui->GetSize();

		TextureID textureID = TextureID::ID_NULL;
		if (textureName != L"")
		{
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

			textureID = GetTextureID(textureName);
			renderer->DrawImage(position, size, textureID, color, layer);
		}
	}

	void Image::SetTexture(const std::wstring& fileName)
	{
		textureName = fileName;
	}

	void Image::PreSerialize(json& jsonData) const
	{
	}

	void Image::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_WSTRING(textureName);
		PREDESERIALIZE_VALUE(layer);

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

	void Image::PostSerialize(json& jsonData) const
	{
	}

	void Image::PostDeserialize(const json& jsonData)
	{
	}
}
