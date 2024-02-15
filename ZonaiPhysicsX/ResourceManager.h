#pragma once
#include <string>
#include <unordered_map>

namespace FBXLoader
{
	struct Model;
}

namespace ZonaiPhysics
{
	using ResourceID = std::size_t;

	class ResourceManager
	{
	private:
		static std::unordered_map<ResourceID, FBXLoader::Model*>  loadedResources;

	public:
		static ResourceID LoadFBXFile(const std::wstring& _path);
		static FBXLoader::Model* GetModel(ResourceID resourceID);
		static void UnloadFBXFile(ResourceID resourceID);
	};
}
