#pragma once
#include "Serializable.h"
#include "FileManager.h"

#define PREDESERIALIZE_BASE() \
{ \
	PurahEngine::FileManager::GetInstance().SetAddress(jsonData["__Base__instanceID"], this);\
}

#define PREDESERIALIZE_VALUE(value)\
{\
	value = jsonData[#value];\
}

#define POSTDESERIALIZE_PTR(value)\
{\
	value = static_cast<decltype(value)>(PurahEngine::FileManager::GetInstance().GetAddress(jsonData[std::string("__ID__") + #value]));\
}

#define PREDESERIALIZE_VECTOR3F(value)\
{\
	value = Eigen::Vector3f(jsonData[#value]["x"], jsonData[#value]["y"], jsonData[#value]["z"]); \
}

#define PREDESERIALIZE_WSTRING(value)\
{\
	std::string text = jsonData[#value];\
	value = std::wstring(text.begin(), text.end());\
}

#define PREDESERIALIZE_QUATERNIONF(value)\
{\
	value = Eigen::Quaternionf(jsonData[#value]["w"], jsonData[#value]["x"], jsonData[#value]["y"], jsonData[#value]["z"]); \
}

/// 범용적으로 벡터를 담을 매크로함수 만들기가 좀 어렵다. 더 고민해보는중.
#define PREDESERIALIZE_VECTOR(value)\
{\
	for(int i = 0; i < jsonData[#value].size() ; i++)\
	{\
		value.push_back(jsonData[#value][i]); \
	}\
}

/// Transform children 전용 매크로 함수..뭔가 애매하다..
/// decltype(value)::value_type 으로 자료형을 가져온다.
#define POSTDESERIALIZE_VECTOR_PTR(value)\
{\
	for(int i = 0; i < jsonData[std::string("__ID__") + #value].size() ; i++)\
	{\
		value.push_back(static_cast<decltype(value)::value_type>(PurahEngine::FileManager::GetInstance().GetAddress(jsonData[std::string("__ID__") + #value][i]))); \
	}\
}
