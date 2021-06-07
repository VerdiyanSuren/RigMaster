#ifndef VF_ENGINE_INCLUDE_H
#define VF_ENGINE_INCLUDE_H

#ifdef VF_PLATFORM_WINDOWS
	#ifdef VF_BUILD_DLL
		#define VF_ENGINE_API _declspec(dllexport) 
	#else
		#define VF_ENGINE_API _declspec(dllimport) 
	#endif
#else 
	#error Graphics only support Windows
#endif


#endif // !VF_ENGINE_INCLUDE_H
