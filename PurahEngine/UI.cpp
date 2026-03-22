#include "UI.h"

#include "GameObject.h"
#include "Transform.h"

namespace PurahEngine
{
	void UI::Awake()
	{
		if (parent == nullptr)
		{
			parent = FindParent();
		}
	}

	Eigen::Vector2f UI::GetScreenPosition()
	{
		if (parent == nullptr)
		{
			return position;
		}
		else
		{
			return position + parent->GetScreenPosition();
		}
	}

	void UI::SetLocalPosition(const Eigen::Vector2f& value)
	{
		position = value;
	}

	void UI::SetSize(const Eigen::Vector2f& value)
	{
		size = value;
	}

	Eigen::Vector2f UI::GetLocalPosition()
	{
		return position;
	}

	Eigen::Vector2f UI::GetSize()
	{
		return size;
	}

	UI* UI::FindParent()
	{
		Transform* transformParent = GetGameObject()->GetTransform()->GetParent();
		UI* uiParent = transformParent->GetGameObject()->GetComponent<UI>();

		return uiParent;
	}

	void UI::PreSerialize(json& jsonData) const
	{
	}

	void UI::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VECTOR2F(position);
		PREDESERIALIZE_VECTOR2F(size);
	}

	void UI::PostSerialize(json& jsonData) const
	{
	}

	void UI::PostDeserialize(const json& jsonData)
	{
	}
}
