#pragma once
#include "Transform.h"

namespace flt
{
	struct TransformOwner
	{
	public:
		TransformOwner() : transform() { transform._pOwner = this; }
		TransformOwner(const TransformOwner& other) : transform()
		{
			transform._position = other.transform._position;
			transform._rotation = other.transform._rotation;
			transform._scale = other.transform._scale;

			transform.SetParent(other.transform.GetParent());

			transform._pOwner = this;
		}

		virtual ~TransformOwner() = default;

		Transform transform;
	};
}
