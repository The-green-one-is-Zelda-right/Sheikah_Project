#include "ResourceManager.h"

#include "JointHelper.h"
#include "FBXData.h"
#include "FBXLoader.h"

namespace ZonaiPhysics
{
	std::unordered_map<ResourceID, FBXLoader::Model*> ResourceManager::loadedResources{};

	ResourceID ResourceManager::LoadFBXFile(const std::wstring& _path)
	{
		constexpr std::hash<std::wstring> hash;
		const ResourceID resourceID = hash(_path);

		if (loadedResources.contains(resourceID))
		{
			return resourceID;
		}

		FBXLoader::FBXLoader loader;
		FBXLoader::Model* model = loader.CreateModelFromFBX(_path);
		loadedResources.insert(std::make_pair(resourceID, model));

		return resourceID;
	}

	FBXLoader::Model* ResourceManager::GetModel(ResourceID resourceID)
	{
		return loadedResources[resourceID];
	}

	void ResourceManager::UnloadFBXFile(ResourceID resourceID)
	{
		FBXLoader::FBXLoader loader;
		FBXLoader::Model* model = loadedResources[resourceID];
		loader.ReleaseModel(model);
	}
}
