--------------------------------------------------------------------------------------------	
-- 									    PROJECT SANDBOX
--------------------------------------------------------------------------------------------
project "Sandbox"
	--location 	"Sandbox"
	kind 		"ConsoleApp"
	language 	"C++"
	
	includedirs 
	{
		"%{IncludeDir.Core}",
		"%{IncludeDir.Lua}",
		"%{IncludeDir.LuaWrapper}",
		"%{IncludeDir.Math}",
		"%{IncludeDir.Sandbox}",
		"%{IncludeDir.GraphicEngine}",
		
	}
	libdirs
	{
		"../bin/" .. outputdir .. "/Lua",
		"../bin/" .. outputdir .. "/Core",
		"../bin/" .. outputdir .. "/GraphicEngine"
	}
	links
	{
		"Lua.lib",
		"Core.lib",
		"GraphicEngine.lib"
	}
	files
	{
		"src/**.cpp",
		"**.cpp",
		"include/**.h"
	}
	targetdir 	("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir 		("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	filter { "system:windows", "platforms:Maya2018" }
	libdirs
	{
		"C:/local/boost_1_69_0_vs_14_0/lib64-msvc-14.0"
	}

	filter { "system:windows", "platforms:Maya2019" }
	libdirs
	{
		"C:/local/boost_1_69_0_vs_14_0/lib64-msvc-14.0"
	}