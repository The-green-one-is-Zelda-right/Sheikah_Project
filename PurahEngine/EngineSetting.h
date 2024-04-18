#pragma once
#include "PurahEngineAPI.h"
#include "SerializableDefine.h"

#include <unordered_map>

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
		float GetGravity();

		std::wstring GetScene(int sceneNumber);
		int GetSceneCount();

		std::vector<std::wstring> GetPreLoadModels();
		std::vector<std::vector<bool>> GetCollsionSetting();

	private:
		// ���� �������� �Ϸ�Ǿ�� �ϴ� ����
		std::unordered_map<std::wstring, int> tagTable;
		std::unordered_map<int, std::wstring> tagIDTable;
		unsigned int tagSize;

		std::unordered_map<std::wstring, int> layerTable;
		std::unordered_map<int, std::wstring> layerIDTable;
		unsigned int layerSize;

		float shadowAreaRange;
		float shadowAreaOffset;
		int shadowMapSize;

		std::vector<std::wstring> preLoadModels;
		std::vector<std::wstring> scenes;

		// ���� ���� �Ŀ��� ����� �� �ִ� ����
		std::vector<std::vector<bool>> collisionSetting;

		float shadowMapDepthBias;
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
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ��������� delete�� ����Ͽ� ����� ���´�.
		EngineSetting& operator=(const EngineSetting& ref) = delete;
	};
}

