-- premake5.lua
-- C:\DEV\dev2021\RigMaster\RigMaster>..\premake5.exe vs2019

-- GLFW
-- ENGINE
-- LUA
-- LUA WRAPPER
-- MATH
-- CORE
-- MAYA
-- SANDBOX
include "Dependencies.lua"


workspace "RigMaster"
	architecture 	"x64"	
	configurations 	{ "Debug","Release"}
	platforms 		{ "Maya2018", "Maya2019", "Maya2020" ,"Maya2022","vufApp19" }
	
outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.platform}_%{cfg.architecture}"

filter { "system:windows" }
	defines
	{
		"VF_PLATFORM_WINDOWS",		 
		"_CRT_SECURE_NO_WARNINGS"
	}
	
filter {"system:windows","configurations:Debug" }
	symbols 		"On" 
	staticruntime 	"On"	
	defines 		"DEBUG"
		
filter { "system:windows", "configurations:Release" }
	staticruntime 	"On"
	buildoptions 	"/MD"
	staticruntime 	"off"
	optimize 		"On"

filter { "system:windows", "platforms:Maya2018" }
	toolset 		"msc-v140"
	systemversion 	"10.0.18362.0"
	
filter { "system:windows", "platforms:Maya2019" }
	toolset 		"msc-v140"
	systemversion 	"10.0.18362.0"

filter { "system:windows", "platforms:Maya2020" }
	toolset 		"msc-v141"
	systemversion 	"10.0.18362.0"
	
filter { "system:windows", "platforms:Maya2022" }
	cppdialect 		"C++17"
	toolset 		"msc-v142"
	systemversion 	"latest"

filter { "system:windows", "platforms:vufApp19" }
	toolset 		"msc-v142"
	systemversion 	"latest"

--------------------------------------------------------------------------------------------	
-- 									    PROJECT OPENCL
--------------------------------------------------------------------------------------------
project "OpenCL"
	location 	"OpenCL"
	kind 		"ConsoleApp"
	language 	"C++"
	
	targetdir 	("bin/" .. outputdir .. "/%{prj.name}")
	objdir 	("bin-int/" .. outputdir .. "/%{prj.name}")
	includedirs 
	{
		"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.2/include",
		"Core/include/",
		"Math/include/",
		"%{prj.name}/include"
	}
	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/**.cpp",
		"%{prj.name}/include/**.h"		
	}
	libdirs
	{
		"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.2/lib/x64",
		"bin/" .. outputdir .. "/Core"
	}
	links
	{
		"openCL.lib",
		"Core.lib"
	}







include "Core"
include "Lua"
include "Math"
include "LuaWrapper"
include "Sandbox"
include "Maya"
include "GLFW"
include "GraphicEngine"
