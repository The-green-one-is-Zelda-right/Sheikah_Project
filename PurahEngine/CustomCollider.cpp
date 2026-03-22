#include "CustomCollider.h"


namespace PurahEngine
{
	CustomCollider::~CustomCollider()
	{
	}

	void CustomCollider::PreDeserialize(const json& jsonData)
	{
		Collider::PreDeserialize(jsonData);

		PREDESERIALIZE_WSTRING(modelName);
		PREDESERIALIZE_QUATERNIONF(rotationOffset);
		PREDESERIALIZE_VECTOR3F(scaleOffset);
	}
}