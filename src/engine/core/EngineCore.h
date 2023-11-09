#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

ENGINE_API void initializeEngine();
ENGINE_API void updateEngine();

#endif  // ENGINE_CORE_H