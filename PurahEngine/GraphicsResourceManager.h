#pragma once

#include <unordered_map>
#include <unordered_set>

#include "GraphicsResourceID.h"

#ifdef YH_RENDERER
#include "../Floater/ZeldaGraphicsAdapter/include/IZeldaRendererAdapter.h"
#else
#include "IZeldaRenderer.h"
#endif

namespace PurahEngine
{
	class GraphicsResourceManager
	{
	public:
		GraphicsResourceManager(IZeldaRenderer* zeldaRenderer);
		~GraphicsResourceManager();

		// 여러개 사용될 수 있는 것
		TextureID GetTextureID(std::wstring textureName);
		ModelID GetModelID(std::wstring modelName);


		// 하나만 사용되는 것
		LightID CreateDirectionalLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction);
		LightID CreatePointLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& position, float range);
		LightID CreateSpotLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle);

		void ReleaseLight(LightID lightID);

		CameraID CreateCamera();

		void ReleaseCamera(CameraID cameraID);

	private:
		std::unordered_map<std::wstring, TextureID> textureTable;
		std::unordered_map<std::wstring, ModelID> modelTable;
		std::unordered_map<std::wstring, MeshID> meshTable;

		std::unordered_set<LightID> lightSet;
		std::unordered_set<CameraID> cameraSet;

	private:
		IZeldaRenderer* graphicsModule;
	};
}