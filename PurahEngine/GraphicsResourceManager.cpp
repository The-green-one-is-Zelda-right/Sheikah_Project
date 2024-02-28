#include "GraphicsResourceManager.h"

namespace PurahEngine
{
	GraphicsResourceManager::GraphicsResourceManager(IZeldaRenderer* zeldaRenderer)
	{
		graphicsModule = zeldaRenderer;
	}

	GraphicsResourceManager::~GraphicsResourceManager()
	{
		for (auto iter = textureTable.begin(); iter != textureTable.end(); iter++)
		{
			graphicsModule->ReleaseTexture(iter->second);
		}

		for (auto iter = modelTable.begin(); iter != modelTable.end(); iter++)
		{
			graphicsModule->ReleaseModel(iter->second);
		}

		for (auto iter = lightSet.begin(); iter != lightSet.end(); iter++)
		{
			graphicsModule->ReleaseLight(*iter);
		}
	}

	TextureID GraphicsResourceManager::GetTextureID(std::wstring textureName)
	{
		auto iter = textureTable.find(textureName);
		
		// textureName이 textureTable에 존재하지 않음
		if (iter == textureTable.end())
		{
			TextureID id = graphicsModule->CreateTexture(textureName);
			textureTable[textureName] = id;
			return id;
		}
		else
		{
			return iter->second;
		}
	}

	ModelID GraphicsResourceManager::GetModelID(std::wstring modelName)
	{
		auto iter = modelTable.find(modelName);

		// modelName이 modelTable에 존재하지 않음
		if (iter == modelTable.end())
		{
			ModelID id = graphicsModule->CreateModel(modelName);
			modelTable[modelName] = id;
			return id;
		}
		else
		{
			return iter->second;
		}
	}

	LightID GraphicsResourceManager::CreateDirectionalLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction)
	{
		LightID lightID = graphicsModule->CreateDirectionalLight(ambient, diffuse, specular, direction);
		lightSet.insert(lightID);

		return lightID;
	}

	LightID GraphicsResourceManager::CreatePointLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& position, float range)
	{
		LightID lightID = graphicsModule->CreatePointLight(ambient, diffuse, specular, position, range);
		lightSet.insert(lightID);

		return lightID;
	}

	LightID GraphicsResourceManager::CreateSpotLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle)
	{
		LightID lightID = graphicsModule->CreateSpotLight(ambient, diffuse, specular, direction, position, range, angle);
		lightSet.insert(lightID);

		return lightID;
	}

	void GraphicsResourceManager::ReleaseLight(LightID lightID)
	{
		graphicsModule->ReleaseLight(lightID);

		lightSet.erase(lightID);
	}

	CameraID GraphicsResourceManager::CreateCamera()
	{
		CameraID cameraID = graphicsModule->CreateCamera();
		cameraSet.insert(cameraID);

		return cameraID;
	}

	void GraphicsResourceManager::ReleaseCamera(CameraID cameraID)
	{
		graphicsModule->ReleaseCamera(cameraID);

		cameraSet.erase(cameraID);
	}
}