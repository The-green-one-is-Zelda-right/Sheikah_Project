#include "EngineSetting.h"

#include "FileManager.h"
#include "Physics.h"
#include "PhysicsSystem.h"

void PurahEngine::EngineSetting::Initialize()
{
	auto& fManager = PurahEngine::FileManager::GetInstance();
	fManager.clear();

	json settingData = fManager.LoadData(L"EngineSettings.json");
	Deserialize(settingData);
}

std::wstring PurahEngine::EngineSetting::TagIDtoTag(int tagid) const
{
	auto iter = tagIDTable.find(tagid);

	if (tagIDTable.end() != iter)
	{
		return iter->second;
	}
	else
	{
		assert(0);
		return nullptr;
	}
}

int PurahEngine::EngineSetting::TagtoTagID(const std::wstring& tag) const
{
	auto iter = tagTable.find(tag);

	if (tagTable.end() != iter)
	{
		return iter->second;
	}
	else
	{
		assert(0);
		return -1;
	}
}

int PurahEngine::EngineSetting::TagSize()
{
	return tagSize;
}

std::wstring PurahEngine::EngineSetting::LayerIDtoLayer(int layer) const
{
	auto iter = layerIDTable.find(layer);

	if (layerIDTable.end() != iter)
	{
		return iter->second;
	}
	else
	{
		assert(0);
		return nullptr;
	}
}

int PurahEngine::EngineSetting::LayertoLayerID(const std::wstring& layer) const
{
	auto iter = layerTable.find(layer);

	if (layerTable.end() != iter)
	{
		return iter->second;
	}
	else
	{
		assert(0);
		return -1;
	}
}

int PurahEngine::EngineSetting::LayerSize()
{
	return layerSize;
}

float PurahEngine::EngineSetting::GetShadowAreaRange()
{
	return shadowAreaRange;
}

float PurahEngine::EngineSetting::GetShadowAreaOffset()
{
	return shadowAreaOffset;
}

int PurahEngine::EngineSetting::GetShadowMapSize()
{
	return shadowMapSize;
}

float PurahEngine::EngineSetting::GetShadowMapDepthBias()
{
	return shadowMapDepthBias;
}

float PurahEngine::EngineSetting::GetPointLightDepthBias()
{
	return pointLightDepthBias;
}

float PurahEngine::EngineSetting::GetGravity()
{
	return gravity;
}

std::wstring PurahEngine::EngineSetting::GetScene(int sceneNumber)
{
	assert(0 <= sceneNumber && sceneNumber < scenes.size());
	
	if (sceneNumber < 0 || scenes.size() <= sceneNumber)
	{
		return L"";
	}
	else
	{
		return scenes[sceneNumber];
	}
}

int PurahEngine::EngineSetting::GetSceneCount()
{
	return 0;
}

std::vector<std::wstring> PurahEngine::EngineSetting::GetPreLoadModels()
{
	return preLoadModels;
}

std::vector<std::vector<bool>> PurahEngine::EngineSetting::GetCollsionSetting()
{
	return collisionSetting;
}

const std::vector<std::tuple<std::wstring, float, float, float, int, int>>& PurahEngine::EngineSetting::GetPhysicsMaterials() const
{
	return physicsMaterials;
}

void PurahEngine::EngineSetting::PreSerialize(json& jsonData) const
{

}

void PurahEngine::EngineSetting::PreDeserialize(const json& jsonData)
{
	tagTable.clear();
	tagIDTable.clear();

	const json& tags = jsonData["tag"];
	tagSize = tags.size();
	for (int tagIDCount = 0; tagIDCount < tags.size(); tagIDCount++)
	{
		std::string tagName = tags[tagIDCount];
		std::wstring wTagName(tagName.begin(), tagName.end());

		tagTable[wTagName] = tagIDCount;
		tagIDTable[tagIDCount] = wTagName;
	}

	layerTable.clear();
	layerIDTable.clear();

	const json& layers = jsonData["layer"];
	layerSize = layers.size();
	for (int layerIDCount = 0; layerIDCount < layers.size(); layerIDCount++)
	{
		std::string layerName = layers[layerIDCount];
		std::wstring wLayerName(layerName.begin(), layerName.end());

		layerTable[wLayerName] = layerIDCount;
		layerIDTable[layerIDCount] = wLayerName;
	}

	physicsMaterials.clear();
	const json& materials = jsonData["physicsMaterials"];
	materialsSize = materials.size();
	for (int materialIDCount = 0; materialIDCount < materials.size(); materialIDCount++)
	{
		std::string materialName = materials[materialIDCount]["name"];
		float		staticFriction = materials[materialIDCount]["staticFriction"];
		float		dynamicFriction = materials[materialIDCount]["dynamicFriction"];
		float		restitution = materials[materialIDCount]["restitution"];
		int			eFriction = materials[materialIDCount]["eFriction"];
		int			eRestitution = materials[materialIDCount]["eRestitution"];

		std::wstring wMaterialName(materialName.begin(), materialName.end());

		physicsMaterials.push_back(
			{ 
				wMaterialName, 
				staticFriction, dynamicFriction, 
				restitution, eFriction, eRestitution 
			}
		);
	}

	const json& collisionInfo = jsonData["collisionSetting"];
	collisionSetting.resize(collisionInfo.size(), std::vector<bool>(collisionInfo.size(), false));
	for (int i = 0; i < collisionInfo.size(); i++)
	{
		for (int j = 0; j < collisionInfo.size(); j++)
		{
			bool value = collisionInfo[i][j];
			collisionSetting[i][j] = value;
			collisionSetting[j][i] = value;
		}
	}

	const json& preloadmodels = jsonData["preLoadModels"];
	preLoadModels.resize(preloadmodels.size());
	for (int i = 0; i < preloadmodels.size(); i++)
	{
		std::string modelPath = preloadmodels[i];
		std::wstring wmodelPath(modelPath.begin(), modelPath.end());
		preLoadModels[i] = wmodelPath;
	}

	const json& scenesdata = jsonData["scenes"];
	scenes.resize(scenesdata.size());
	for (int i = 0; i < scenes.size(); i++)
	{
		std::string path = scenesdata[i];
		std::wstring wPath(path.begin(), path.end());
		scenes[i] = wPath;
	}

	shadowAreaRange = jsonData["shadowAreaRange"];
	shadowAreaOffset = jsonData["shadowAreaOffset"];
	shadowMapSize = jsonData["shadowMapSize"];
	shadowMapDepthBias = jsonData["shadowMapDepthBias"];
	pointLightDepthBias = jsonData["pointLightDepthBias"];
	gravity = jsonData["gravity"];
}

void PurahEngine::EngineSetting::PostSerialize(json& jsonData) const
{

}

void PurahEngine::EngineSetting::PostDeserialize(const json& jsonData)
{

}

PurahEngine::EngineSetting& PurahEngine::EngineSetting::GetInstance()
{
	static EngineSetting instance;
	return instance;
}

PurahEngine::EngineSetting::EngineSetting() :
	shadowAreaRange(1024.0f),
	shadowAreaOffset(0.0f),
	shadowMapSize(16384u),
	shadowMapDepthBias(0.01f)
{
}

PurahEngine::EngineSetting::~EngineSetting()
{
}
