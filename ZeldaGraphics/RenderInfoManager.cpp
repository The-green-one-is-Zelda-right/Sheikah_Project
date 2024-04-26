#include "RenderInfoManager.h"

void RenderInfoManager::ClearRenderInfo()
{
	renderInfoList.clear();
}

void RenderInfoManager::SortRenderInfo()
{
	// ���ĵ� RenderInfo�� �����ϴ� �����̳ʵ� �ʱ�ȭ
	deferredRenderInfo.clear();
	forwardRenderInfo.clear();
	spriteRenderInfo.clear();
	lightRenderInfo.clear();

	// renderInfoList�� ��ȸ�ϸ� �����Ѵ�.
	for (int i = 0; i < renderInfoList.size(); i++)
	{
		RenderInfo& renderInfo = renderInfoList[i];

		RenderType renderType = renderInfo.renderType;

		switch (renderType)
		{
			case RenderType::Deferred_Mesh:
			case RenderType::Deferred_Model:
			case RenderType::Deferred_BlendingAnimation:
			{
				SortRenderInfo(&renderInfo, deferredRenderInfo);
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
				SortRenderInfo(&renderInfo, spriteRenderInfo);
				break;
			}
			case RenderType::String:
			{

				break;
			}
			case RenderType::Light:
			{
				//lightRenderInfo.insert(renderInfo.instancingKey);
				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}

	int drawIDCounter = 1;



}

void RenderInfoManager::SortRenderInfo(RenderInfo* renderInfo, std::unordered_map<InstancingKey, std::vector<RenderInfo*>>& targetContainer)
{
	//auto iter = targetContainer.find(renderInfo->instancingKey);

	//// ������ instancingKey�� �׸����� ����
	//if (iter != targetContainer.end())
	//{
	//	auto& infoVector = iter->second;
	//	infoVector.push_back(renderInfo);
	//}
	//// ������ instancingKey�� �׸����� ����
	//else
	//{
	//	targetContainer.insert({ renderInfo->instancingKey, std::vector<RenderInfo*>(1, renderInfo) });
	//}
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
