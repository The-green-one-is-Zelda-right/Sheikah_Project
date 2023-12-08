#include "./include/ResourceMgr.h"
#include "./include/Resource.h"
#include "./include/IBuilder.h"


namespace flt
{
	namespace global
	{
		ResourceMgr g_resourceMgr;
	}
}

void* flt::ResourceMgr::GetResource(ResourceBase* resource, const IBuilderBase& builder)
{
	void* data = nullptr;

	std::lock_guard<std::recursive_mutex> lock(resourceMutex);
	resource->_key = builder.key;
	if (resources.find(builder.key) == resources.end())
	{
		// 관리하지 않는 데이터일 경우 생성
		std::wstring typeName;
		data = builder(&typeName);
		// 생성 실패 시 nullptr 반환
		if (data == nullptr)
		{
			return nullptr;
		}

		resources[builder.key] = { data, typeName };
		//auto[iter, ret] = resources.emplace(builder.key, data, typeName);
	}
	else
	{
		// 관리중일 데이터일 경우 참조 카운트 증가
		data = resources[builder.key].GetData();
	}


	return data;
}

bool flt::ResourceMgr::ReleaseResource(ResourceBase* resource)
{
	std::lock_guard<std::recursive_mutex> lock(resourceMutex);
	if (resources.find(resource->_key) != resources.end())
	{
		if (resources[resource->_key].Release())
		{
			resources.erase(resource->_key);
			//delete resource;
			return true;
			// 빌더의 Release 를 호출해야함.
			// Resource의 소멸자가 virtual이기 때문에 해당 객체가 소멸할 때 소멸자에서 Release를 호출해줌.
		}
	}

	return false;
}
