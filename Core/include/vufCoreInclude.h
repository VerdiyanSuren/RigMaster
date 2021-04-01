#ifndef VF_CORE_INCLUDE_H
#define VF_CORE_INCLUDE_H

#include <string>

// specify dll_export dll_import for windows platforms
#ifdef VF_PLATFORM_WINDOWS
	#ifdef VF_EXPORT_DLL
		#define VF_API _declspec(dllexport) 
	#else
		#ifdef VF_IMPORT_DLL
			#define VF_API _declspec(dllimport) 
		#else  
			#define VF_API
		#endif // VF_IMPORT_DLL
	#endif
#else 
	#define VF_API
#endif

// specify string (std::string or custom)
#define VF_STRING std::string
#define VF_CHAR	  char
#define VF_PADDING_MAX 70

#endif // !VF_CORE_INCLUDE_H