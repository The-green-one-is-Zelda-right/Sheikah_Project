#pragma once

#include "RenderInfo.h"

#include <unordered_set>
#include <unordered_map>
#include <map>

class RenderInfoManager final
{
public:
	void ClearRenderInfo();

	// RenderInfo를 정렬한다. (DrawID가 정해진다.)
	void SortRenderInfo(DirectX::XMMATRIX viewMatrix);

	void RegisterRenderInfo(RenderType renderType, RenderOption renderOption, InstancingKey instancingKey, InstancingValue instancingValue);

	const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& GetDeferredRenderInfo() const;
	const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& GetForwardRenderInfo() const;
	const std::multimap<float, RenderInfo*, std::greater<float>>& GetSpriteRenderInfo() const;
	const std::unordered_map<InstancingKey, RenderInfo*>& GetLightRenderInfo() const;
	const std::vector<RenderInfo*>& GetStringRenderInfo() const;
	const std::multimap<float, RenderInfo*, std::greater<float>>& GetBillBoardRenderInfo() const;
	const RenderInfo* GetCubeMapRenderInfo();
	const std::multimap<unsigned int, RenderInfo*, std::greater<unsigned int>>& GetImageRenderInfo();
	const std::multimap<std::pair<float, std::pair<unsigned int, std::pair<float, unsigned int>>>, RenderInfo*, std::greater<std::pair<float, std::pair<unsigned int, std::pair<float, unsigned int>>>>>& GetBillBoardParticleRenderInfo() const;

	const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& GetDirectionalShadowRenderInfo() const;
	const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& GetPointShadowRenderInfo() const;
	const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& GetFastOutLineRenderInfo() const;
	const std::vector<RenderInfo*>& GetOutLineRenderInfo() const;

private:
	void SortRenderInfo(RenderInfo* renderInfo, std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& targetContainer);

private:
	// Render Info
	std::vector<RenderInfo> renderInfoList;

	// Sorted RenderInfo
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> deferredRenderInfo;					// ID 1
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> forwardRenderInfo;					// ID 2
	std::multimap<float, RenderInfo*, std::greater<float>> spriteRenderInfo;						// ID 3
	std::unordered_map<InstancingKey, RenderInfo*> lightRenderInfo;									// ID 4
	std::vector<RenderInfo*> stringRenderInfo;														// ID 5
	std::multimap<float, RenderInfo*, std::greater<float>> billBoardRenderInfo;						// ID 6
	RenderInfo* cubeMapRenderInfo;																	// ID 7
	std::multimap<unsigned int, RenderInfo*, std::greater<unsigned int>> imageRenderInfo;			// ID 8
	// key: <worldDepth, layer, <particleDepth, particleID>>>
	std::multimap<std::pair<float, std::pair<unsigned int, std::pair<float, unsigned int>>>, RenderInfo*, std::greater<std::pair<float, std::pair<unsigned int, std::pair<float, unsigned int>>>>> billBoardParticleRenderInfo;	// ID 9
	
	// Option RenderInfo
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> directionalShadowRenderInfo;
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> pointShadowRenderInfo;
	std::unordered_map<InstancingKey, std::vector<RenderInfo*>> fastOutLineRenderInfo;
	std::vector<RenderInfo*> outLineRenderInfo;



	//singleton
public:
	static RenderInfoManager& GetInstance();

private:
	RenderInfoManager();
	~RenderInfoManager();
	RenderInfoManager(const RenderInfoManager& ref) = delete;
	RenderInfoManager& operator=(const RenderInfoManager& ref) = delete;
};