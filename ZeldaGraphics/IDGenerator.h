#pragma once

#include "GraphicsResourceID.h"

#include <rpc.h>

class IDGenerator
{
public:
	template<ResourceType type>
	static ResourceID<type> CreateID()
	{
		UUID uuid;
		RPC_STATUS result;
		ResourceID<type> resourceID;

		do
		{
			result = UuidCreate(&uuid);

			if (result != RPC_S_OK)
			{
				resourceID.ull[0] = 0ull;
				resourceID.ull[1] = 0ull;

				return resourceID;
			}
		} while (uuid.Data1 == 0ul && uuid.Data2 == 0u && uuid.Data3 == 0u &&
			uuid.Data4[0] == 0u && uuid.Data4[1] == 0u && uuid.Data4[2] == 0u && uuid.Data4[3] == 0u);

		if (result != RPC_S_OK)
		{
			resourceID.ull[0] = 0ull;
			resourceID.ull[1] = 0ull;

			return resourceID;
		}
		else
		{
			resourceID.d1 = uuid.Data1;
			resourceID.d2 = uuid.Data2;
			resourceID.d3 = uuid.Data3;
			resourceID.d4[0] = uuid.Data4[0];
			resourceID.d4[1] = uuid.Data4[1];
			resourceID.d4[2] = uuid.Data4[2];
			resourceID.d4[3] = uuid.Data4[3];

			return resourceID;
		}
	}
};

