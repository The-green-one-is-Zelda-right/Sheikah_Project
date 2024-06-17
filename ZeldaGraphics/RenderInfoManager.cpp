#include "RenderInfoManager.h"

void RenderInfoManager::ClearRenderInfo()
{
	renderInfoList.clear();
}

void RenderInfoManager::SortRenderInfo(DirectX::XMMATRIX viewMatrix)
{
	// ���ĵ� RenderInfo�� �����ϴ� �����̳ʵ� �ʱ�ȭ
	deferredRenderInfo.clear();
	forwardRenderInfo.clear();
	spriteRenderInfo.clear();
	lightRenderInfo.clear();
	stringRenderInfo.clear();
	billBoardRenderInfo.clear();
	cubeMapRenderInfo = nullptr;
	imageRenderInfo.clear();
	billBoardParticleRenderInfo.clear();

	shadowRenderInfo.clear();
	fastOutLineRenderInfo.clear();
	outLineRenderInfo.clear();

	// renderInfoList�� ��ȸ�ϸ� �����Ѵ�.
	for (int i = 0; i < renderInfoList.size(); i++)
	{
		RenderInfo& renderInfo = renderInfoList[i];

		RenderType renderType = renderInfo.renderType;
		RenderOption renderOption = renderInfo.renderOption;

		switch (renderType)
		{
			case RenderType::Deferred_Mesh:
			case RenderType::Deferred_Model:
			case RenderType::Deferred_BlendingAnimation:
			{
				SortRenderInfo(&renderInfo, deferredRenderInfo);

				// Option
				if ((renderOption & RenderInfoOption::OutLine) > 0u)
				{
					outLineRenderInfo.push_back(&renderInfo);
				}
				// FastOutLine�� OutLine�� �׸��ٸ� �������� �ʴ´�.
				else if ((renderOption & RenderInfoOption::FastOutLine) > 0u)
				{
					SortRenderInfo(&renderInfo, fastOutLineRenderInfo);
				}

				if ((renderOption & RenderInfoOption::Shadow) > 0u)
				{
					SortRenderInfo(&renderInfo, shadowRenderInfo);
				}

				break;
			}
			case RenderType::Forward_Mesh:
			case RenderType::Forward_Model:
			case RenderType::Forward_BlendingAnimation:
			{
				SortRenderInfo(&renderInfo, forwardRenderInfo);
				break;
			}
			case RenderType::Sprite:
			{
				DirectX::XMMATRIX wv = renderInfo.instancingValue.worldMatrix * viewMatrix;
				float depth = wv.r[3].m128_f32[2];
				spriteRenderInfo.insert({ depth, &renderInfo });
				break;
			}
			case RenderType::String:
			{
				stringRenderInfo.push_back(&renderInfo);
				break;
			}
			case RenderType::Light:
			{
				lightRenderInfo.insert({ renderInfo.instancingKey, &renderInfo });
				break;
			}
			case RenderType::CubeMap:
			{
				cubeMapRenderInfo = &renderInfo;
				break;
			}
			case RenderType::BillBoard:
			{
				DirectX::XMMATRIX wv = renderInfo.instancingValue.worldMatrix * viewMatrix;
				float depth = wv.r[3].m128_f32[2];
				billBoardRenderInfo.insert({ depth, &renderInfo });
				break;
			}
			case RenderType::Image:
			{
				imageRenderInfo.insert({ renderInfo.instancingValue.layer, &renderInfo });
				break;
			}
			case RenderType::BillBoardParticle:
			{
				DirectX::XMMATRIX wv = renderInfo.instancingValue.worldMatrix * viewMatrix;
				float depth = wv.r[3].m128_f32[2];

				DirectX::XMMATRIX pv = renderInfo.instancingValue.particleMatrix * viewMatrix;
				float pdepth = pv.r[3].m128_f32[2];
				billBoardParticleRenderInfo.insert({ std::make_pair(depth, std::make_pair(renderInfo.instancingValue.layer, std::make_pair(pdepth, renderInfo.instancingValue.particleID))), &renderInfo });
				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}

	// sprite�� billboard�� depth������ �ѹ� �� �����Ѵ�.

	// ID�� �ο��Ѵ�.
	int drawIDCounter = 1;

	// 1
	for (auto& [key, value] : deferredRenderInfo)
	{
		for (auto& renderInfo : value)
		{
			renderInfo->drawID = drawIDCounter;
			drawIDCounter += 1;
		}
	}

	// 2
	for (auto& [key, value] : forwardRenderInfo)
	{
		for (auto& renderInfo : value)
		{
			renderInfo->drawID = drawIDCounter;
			drawIDCounter += 1;
		}
	}

	// 3
	for (auto& [key, value] : spriteRenderInfo)
	{
		value->drawID = drawIDCounter;
		drawIDCounter += 1;
	}

	// 4
	for (auto& [key, value] : lightRenderInfo)
	{
		value->drawID = drawIDCounter;
		drawIDCounter += 1;
	}

	// 5
	for (auto& iter : stringRenderInfo)
	{
		iter->drawID = drawIDCounter;
		drawIDCounter += 1;
	}

	// 6
	for (auto& [key, value] : billBoardRenderInfo)
	{
		value->drawID = drawIDCounter;
		drawIDCounter += 1;
	}

	// 7
	if (cubeMapRenderInfo != nullptr)
	{
		cubeMapRenderInfo->drawID = drawIDCounter;
		drawIDCounter += 1;
	}

	// 8
	for (auto& [key, value] : imageRenderInfo)
	{
		value->drawID = drawIDCounter;
		drawIDCounter += 1;
	}

	// 9
	for (auto& [key, value] : billBoardParticleRenderInfo)
	{
		value->drawID = drawIDCounter;
		drawIDCounter += 1;
	}
}

void RenderInfoManager::RegisterRenderInfo(RenderType renderType, RenderOption renderOption, InstancingKey instancingKey, InstancingValue instancingValue)
{
	RenderInfo renderInfo;
	renderInfo.renderType = renderType;
	renderInfo.renderOption = renderOption;
	renderInfo.instancingKey = instancingKey;
	renderInfo.instancingValue = instancingValue;

	renderInfoList.push_back(renderInfo);
}

const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& RenderInfoManager::GetDeferredRenderInfo() const
{
	return deferredRenderInfo;
}

const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& RenderInfoManager::GetForwardRenderInfo() const
{
	return forwardRenderInfo;
}

const std::multimap<float, RenderInfo*, std::greater<float>>& RenderInfoManager::GetSpriteRenderInfo() const
{
	return spriteRenderInfo;
}

const std::unordered_map<InstancingKey, RenderInfo*>& RenderInfoManager::GetLightRenderInfo() const
{
	return lightRenderInfo;
}

const std::vector<RenderInfo*>& RenderInfoManager::GetStringRenderInfo() const
{
	return stringRenderInfo;
}

const std::multimap<float, RenderInfo*, std::greater<float>>& RenderInfoManager::GetBillBoardRenderInfo() const
{
	return billBoardRenderInfo;
}

const RenderInfo* RenderInfoManager::GetCubeMapRenderInfo()
{
	return cubeMapRenderInfo;
}

const std::multimap<unsigned int, RenderInfo*, std::greater<unsigned int>>& RenderInfoManager::GetImageRenderInfo()
{
	return imageRenderInfo;
}

const std::multimap<std::pair<float, std::pair<unsigned int, std::pair<float, unsigned int>>>, RenderInfo*, std::greater<std::pair<float, std::pair<unsigned int, std::pair<float, unsigned int>>>>>& RenderInfoManager::GetBillBoardParticleRenderInfo() const
{
	return billBoardParticleRenderInfo;
}

const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& RenderInfoManager::GetShadowRenderInfo() const
{
	return shadowRenderInfo;
}

const std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& RenderInfoManager::GetFastOutLineRenderInfo() const
{
	return fastOutLineRenderInfo;
}

const std::vector<RenderInfo*>& RenderInfoManager::GetOutLineRenderInfo() const
{
	return outLineRenderInfo;
}

void RenderInfoManager::SortRenderInfo(RenderInfo* renderInfo, std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& targetContainer)
{
	auto iter = targetContainer.find(renderInfo->instancingKey);

	// ������ instancingKey�� �׸����� ����
	if (iter != targetContainer.end())
	{
		auto& infoVector = iter->second;
		infoVector.push_back(renderInfo);
	}
	// ������ instancingKey�� �׸����� ����
	else
	{
		targetContainer.insert({ renderInfo->instancingKey, std::vector<RenderInfo*>(1, renderInfo) });
	}
}

RenderInfoManager& RenderInfoManager::GetInstance()
{
	static RenderInfoManager instance;
	return instance;
}

RenderInfoManager::RenderInfoManager()
{
}

RenderInfoManager::~RenderInfoManager()
{
}
