#pragma once

#include <unordered_map>

enum class ResourceType : unsigned long long {
	Camera = 1,
	Texture = 2,
	Model = 3,
	Mesh = 4,
	Shader = 5,
	Light = 6
};

template <ResourceType type>
struct ResourceID
{
	union
	{
		struct
		{
			unsigned long d1;
			unsigned short d2;
			unsigned short d3;
			unsigned char d4[4];
		};

		struct
		{
			unsigned long long ull1;
			unsigned long long ull2;
		};

		unsigned long long ull[2];

		unsigned int ui[4];

		unsigned short us[8];

		unsigned char uc[16];
	};

	static constexpr ResourceType resourceType = type;
	const static ResourceID ID_NULL;

	bool operator==(const ResourceID& right) const
	{
		return ull[0] == right.ull[0] && ull[1] == right.ull[1] && resourceType == right.resourceType;
	}

	bool operator!=(const ResourceID& right) const
	{
		return (ull[0] != right.ull[0]) || (ull[1] != right.ull[1]) || (resourceType != right.resourceType);
	}
};

template <ResourceType type>
const ResourceID<type> ResourceID<type>::ID_NULL = { 0llu, 0llu };

using CameraID = ResourceID<ResourceType::Camera>;
using TextureID = ResourceID<ResourceType::Texture>;
using ModelID = ResourceID<ResourceType::Model>;
using MeshID = ResourceID<ResourceType::Mesh>;
using ShaderID = ResourceID<ResourceType::Shader>;
using LightID = ResourceID<ResourceType::Light>;

namespace std {
	template <>
	struct hash<CameraID> {
		std::size_t operator()(const CameraID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

namespace std {
	template <>
	struct hash<TextureID> {
		std::size_t operator()(const TextureID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

namespace std {
	template <>
	struct hash<ModelID> {
		std::size_t operator()(const ModelID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

namespace std {
	template <>
	struct hash<MeshID> {
		std::size_t operator()(const MeshID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

namespace std {
	template <>
	struct hash<ShaderID> {
		std::size_t operator()(const ShaderID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

namespace std {
	template <>
	struct hash<LightID> {
		std::size_t operator()(const LightID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

namespace std {
	template <>
	struct hash<std::pair<MeshID, TextureID>> {
		size_t operator()(const std::pair<MeshID, TextureID>& obj) const {
			return std::hash<MeshID>{}(obj.first) ^ std::hash<TextureID>{}(obj.second);
		}
	};
}
