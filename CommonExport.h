#ifdef COMMON_EXPORTS
#define COMMON_DLL_API __declspec(dllexport)
#else
#define COMMON_DLL_API __declspec(dllimport)
#endif
