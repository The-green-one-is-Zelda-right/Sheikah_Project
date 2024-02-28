#pragma once
#include "PurahEngineAPI.h"
#include <string>
#include <map>
#include <functional>

namespace PurahEngine
{
	class GameObject;
	class Component;
	class PURAHENGINE_API ComponentFactory
	{
	public:

		template<typename T>
		void RegisterComponent(const std::string componentName)
		{
			// ¿Ã∏ß, ∞¥√º
			componentFactory[componentName] = []()
				{
					T* component = new T;
					return component;
				};
		}


	private:
		std::map<std::string, std::function<Component* ()>> componentFactory;

	public:
		static ComponentFactory& GetInstance()
		{
			static ComponentFactory instance;
			return instance;
		}

		friend GameObject;
	};
}
