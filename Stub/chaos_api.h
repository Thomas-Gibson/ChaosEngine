#pragma once

#ifdef CHAOS_EXPORT
#define CHAOS_API __declspec(dllexport)
#else
#define CHAOS_API __declspec(dllimport)
#endif // CHAOS_EXPORT
