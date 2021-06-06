--------------------------------------------------------------------------------------------	
-- 										Project MATH
--------------------------------------------------------------------------------------------		
project "Math"
	--location 	"Math"
	kind 		"ConsoleApp"
	language 	"C++"

	targetdir 	("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir 		("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs 
	{
		IncludeDir["Lua"],
		IncludeDir["Core"],
		IncludeDir["Math"],
	}
	files
	{
		"src/**.cpp",
		"**.cpp",
		"include/**.h"
	}
	libdirs
	{
		"../bin/" .. outputdir .. "/Core"		
	}
	links
	{
		"Core.lib"
	}