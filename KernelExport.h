#ifdef KERNEL_DLL_EXPORT
#define KERNEL_DLL_API __declspec(dllexport)
#else
#define KERNEL_DLL_API __declspec(dllimport)
#endif
