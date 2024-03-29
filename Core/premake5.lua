--------------------------------------------------------------------------------------------	
-- 										Project CORE
--------------------------------------------------------------------------------------------
project "Core"
	--location 	"Core"
	--kind 		"ConsoleApp"
	kind 		"StaticLib"
	language 	"C++"
	
	defines
	{
		"VF_CONSOLE_USE_COLORS"
	}
	includedirs 
	{
		"%{IncludeDir.Core}",
		"%{IncludeDir.Lua}",
		"%{IncludeDir.Math}"
	}	
	files
	{
		"src/**.cpp",
		"**.cpp",
		"include/**.h"
	}
	--defines
	--{
	--	"VF_EXPORT_DLL"
	--}
	targetdir 	("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir 		("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	filter { "system:windows", "platforms:Maya2018" }
	includedirs 
	{
		"C:/local/boost_1_76_0_vs_14_0"
	}
	libdirs
	{
		"C:/local/boost_1_76_0_vs_14_0/lib64-msvc-14.0"
	}
	---------------------------------------------------
	filter { "system:windows", "platforms:Maya2019" }
	includedirs 
	{
		"C:/local/boost_1_76_0_vs_14_0"
	}
	libdirs
	{
		"C:/local/boost_1_76_0_vs_14_0/lib64-msvc-14.0"
	}
	---------------------------------------------------
	filter { "system:windows", "platforms:Maya2020" }
	includedirs 
	{
		"C:/local/boost_1_76_0_vs_14_1"
	}
	libdirs
	{
		"C:/local/boost_1_76_0_vs_14_1/lib64-msvc-14.1"
	}
	---------------------------------------------------
	filter { "system:windows", "platforms:Maya2022" }
	includedirs 
	{
		"C:/local/boost_1_76_0_vs_14_2"
	}
	libdirs
	{
		"C:/local/boost_1_76_0_vs_14_2/lib64-msvc-14.2"
	}
	---------------------------------------------------
	--postbuildcommands
	--{
	--	("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir .. "/Sandbox"),
	--	("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir .. "/Math"),
	--	("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir .. "/Maya")
	--}
