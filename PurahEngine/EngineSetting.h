#pragma once
#include <queue>

#include "PurahEngineAPI.h"
#include "SerializableDefine.h"
#include "ZnMaterial.h"
#include "ZnResourceID.h"

#include <unordered_map>
#include <tuple>

namespace PurahEngine
{
	class GameObject;

	class EngineSetting : public Serializable
	{
	public:
		void Initialize();

		std::wstring TagIDtoTag(int tagid) const;
		int TagtoTagID(const std::wstring& tag) const;
		int TagSize();

		std::wstring LayerIDtoLayer(int layer) const;
		int LayertoLayerID(const std::wstring& layer) const;
		int LayerSize();

		float GetShadowAreaRange();
		float GetShadowAreaOffset();
		int GetShadowMapSize();
		float GetShadowMapDepthBias();
		float GetPointLightDepthBias();
		float GetGravity();

		std::wstring GetScene(int sceneNumber);
		int GetSceneCount();

		std::vector<std::wstring> GetPreLoadModels();
		std::vector<std::vector<bool>> GetCollsionSetting();

		const std::vector<std::tuple<std::wstring, float, float, float, int, int>>& GetPhysicsMaterials() const;

	private:
		// 게임 실행전에 완료되어야 하는 설정
		std::unordered_map<std::wstring, int> tagTable;
		std::unordered_map<int, std::wstring> tagIDTable;
		unsigned int tagSize;

		std::unordered_map<std::wstring, int> layerTable;
		std::unordered_map<int, std::wstring> layerIDTable;
		unsigned int layerSize;

		std::vector<std::tuple<std::wstring, float, float, float, int, int>> physicsMaterials;
		unsigned int materialsSize;

		float shadowAreaRange;
		float shadowAreaOffset;
		int shadowMapSize;

		std::vector<std::wstring> preLoadModels;
		std::vector<std::wstring> scenes;

		// 게임 실행 후에도 변경될 수 있는 설정
		std::vector<std::vector<bool>> collisionSetting;

		float shadowMapDepthBias;
		float pointLightDepthBias;
		float gravity;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	public:
		static EngineSetting& GetInstance();

	private:
		EngineSetting();
		~EngineSetting();
		EngineSetting(const EngineSetting& ref) = delete;
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		EngineSetting& operator=(const EngineSetting& ref) = delete;
	};
}

