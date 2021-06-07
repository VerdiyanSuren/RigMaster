--------------------------------------------------------------------------------------------	
-- 									    PROJECT ENGINE
--------------------------------------------------------------------------------------------
project "GraphicEngine"
	--location 	"GraphicEngine"
	kind 		"SharedLib"
	language 	"C++"
	
	targetdir 	("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir 		("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs 
	{
		IncludeDir["Lua"],
		IncludeDir["Core"],
		IncludeDir["Math"],
		IncludeDir["GLFW"],
		IncludeDir["GraphicEngine"]
	}
	files
	{
		"src/**.cpp",
		"**.cpp",
		"include/**.h"
	}	
	libdirs
	{
		"../bin/" .. outputdir .. "/Lua",
		"../bin/" .. outputdir .. "/Core",
		"../bin/" .. outputdir .. "/GLFW"		
	}	
	links
	{
		"Core.lib",
		"GLFW.lib",
		"Lua.lib"
	}	
	defines
	{
		"VF_BUILD_DLL",
		"VF_CONSOLE_USE_COLORS"
	}
	
	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir .. "/Sandbox")
	}
	
