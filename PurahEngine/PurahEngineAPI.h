#pragma once

#ifdef PURAHENGINE_EXPORTS
	#define PURAHENGINE_API __declspec(dllexport)
#else
	#define PURAHENGINE_API __declspec(dllimport)
#endif