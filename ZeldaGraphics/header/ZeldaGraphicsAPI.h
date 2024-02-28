#pragma once

#ifdef ZELDAGRAPHICS_EXPORTS
#define ZELDAGRAPHICS_API __declspec(dllexport)
#else
#define ZELDAGRAPHICS_API __declspec(dllimport)
#endif