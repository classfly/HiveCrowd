#ifdef LOG_DLL_EXPORT
#define LOG_DLL_API __declspec(dllexport)
#else
#define LOG_DLL_API __declspec(dllimport)
#endif
