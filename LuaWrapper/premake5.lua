--------------------------------------------------------------------------------------------	
-- 										Project LUA WRAPPER
--------------------------------------------------------------------------------------------
project "LuaWrapper"
	--location 	"LuaWrapper"
	kind 		"ConsoleApp"
	language 	"C++"

	targetdir 	("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir 		("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs 
	{
		"%{IncludeDir.Core}",
		"%{IncludeDir.Lua}",
		"%{IncludeDir.LuaWrapper}",
		"%{IncludeDir.Math}",
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
		"../bin/" .. outputdir .. "/Core"
	}
	links
	{
		"Lua.lib",
		"Core.lib"
	}

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
