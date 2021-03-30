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

workspace "RigMaster"
	architecture 	"x64"	
	configurations 	{ "Debug","Release"}
	platforms 		{ "Maya2018", "Maya2019", "Maya2020" ,"vufApp19" }
	
outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.platform}_%{cfg.architecture}"

filter { "system:windows" }
	defines
	{
		"VF_PLATFORM_WINDOWS"
	}
	
filter {"system:windows","configurations:Debug" }
	symbols 		"On" 
	staticruntime 	"On"	
		
filter { "system:windows", "configurations:Release" }
	staticruntime 	"On"
	buildoptions 	"/MT"
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

filter { "system:windows", "platforms:vufApp19" }
	toolset 		"msc-v142"
	systemversion 	"latest"

--------------------------------------------------------------------------------------------	
-- 									    PROJECT GLFW
--------------------------------------------------------------------------------------------
project "GLFW"
	location 	"Glfw"
	kind 		"SharedLib"
	language 	"C++"
--------------------------------------------------------------------------------------------	
-- 									    PROJECT ENGINE
--------------------------------------------------------------------------------------------
project "GraphicEngine"
	location 	"GraphicEngine"
	kind 		"SharedLib"
	language 	"C++"
--------------------------------------------------------------------------------------------	
-- 										Project LUA
--------------------------------------------------------------------------------------------	
project "Lua"
	location 	"Lua"
	kind 		"SharedLib"
	language 	"C++"
	
	targetdir 	("bin/" .. outputdir .. "/%{prj.name}")
	objdir 	("bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs 
	{
		"%{prj.name}/include"
	}
	files
	{		
		"%{prj.name}/src/lapi.c",
		"%{prj.name}/src/lcode.c",
		"%{prj.name}/src/lctype.c",
		"%{prj.name}/src/ldebug.c",
		"%{prj.name}/src/ldo.c",
		"%{prj.name}/src/ldump.c",
		"%{prj.name}/src/lfunc.c",
		"%{prj.name}/src/lgc.c",
		"%{prj.name}/src/llex.c",
		"%{prj.name}/src/lmem.c",
		"%{prj.name}/src/lobject.c",
		"%{prj.name}/src/lopcodes.c",
		"%{prj.name}/src/lparser.c",
		"%{prj.name}/src/lstate.c",
		"%{prj.name}/src/lstring.c",
		"%{prj.name}/src/ltable.c",
		"%{prj.name}/src/ltm.c",
		"%{prj.name}/src/lundump.c",
		"%{prj.name}/src/lvm.c",
		"%{prj.name}/src/lzio.c",
		"%{prj.name}/src/lauxlib.c",
		"%{prj.name}/src/lbaselib.c",
		"%{prj.name}/src/lcorolib.c",
		"%{prj.name}/src/ldblib.c",
		"%{prj.name}/src/liolib.c",
		"%{prj.name}/src/lmathlib.c",
		"%{prj.name}/src/loadlib.c",
		"%{prj.name}/src/loslib.c",
		"%{prj.name}/src/lstrlib.c",
		"%{prj.name}/src/ltablib.c",
		"%{prj.name}/src/lutf8lib.c",
		"%{prj.name}/src/linit.c",		
		"%{prj.name}/src/**.h"
	}
	
	defines
	{
		"LUA_BUILD_AS_DLL"
	}		
	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir .. "/LuaWrapper"),
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir .. "/Core"),
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir .. "/Math"),
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir .. "/Maya")
	}
--------------------------------------------------------------------------------------------	
-- 										Project LUA WRAPPER
--------------------------------------------------------------------------------------------
project "LuaWrapper"
	location 	"LuaWrapper"
	kind 		"ConsoleApp"
	language 	"C++"

	targetdir 	("bin/" .. outputdir .. "/%{prj.name}")
	objdir 	("bin-int/" .. outputdir .. "/%{prj.name}")
	includedirs 
	{
		"Lua/src/",
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
		"bin/" .. outputdir .. "/Lua"
	}
	links
	{
		"Lua.lib"
	}
--------------------------------------------------------------------------------------------	
-- 										Project MATH
--------------------------------------------------------------------------------------------		
project "Math"
	location 	"Math"
	kind 		"ConsoleApp"
	language 	"C++"

	targetdir 	("bin/" .. outputdir .. "/%{prj.name}")
	objdir 	("bin-int/" .. outputdir .. "/%{prj.name}")
	includedirs 
	{
		"Lua/src/",
		"%{prj.name}/include"
	}
	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/**.cpp",
		"%{prj.name}/include/**.h"
	}
--------------------------------------------------------------------------------------------	
-- 										Project CORE
--------------------------------------------------------------------------------------------		
project "Core"
	location 	"Core"
	kind 		"ConsoleApp"
	language 	"C++"
	
	defines
	{
		"VF_CONSOLE_USE_COLORS"
	}
	includedirs 
	{
		"%{prj.name}/include",
		"Lua/src/",		
		"Math/include"
	}
	libdirs
	{
		"bin/" .. outputdir .. "/Lua"
	}
	links
	{
		"Lua.lib"
	}
	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/**.cpp",
		"%{prj.name}/include/**.h"
	}
	--defines
	--{
	--	"VF_EXPORT_DLL"
	--}
	targetdir 	("bin/" .. outputdir .. "/%{prj.name}")
	objdir 		("bin-int/" .. outputdir .. "/%{prj.name}")
--------------------------------------------------------------------------------------------	
-- 										Project MAYA
--------------------------------------------------------------------------------------------		
project "Maya"
	location 	"Maya"
	kind 		"SharedLib"
	language 	"C++"
	
	includedirs 
	{
		"Lua/src/",
		"%{prj.name}/include",
		"Math/include",
		"Core/include",
		"LuaWrapper/include"
	}
	libdirs
	{
		"bin/" .. outputdir .. "/Lua"
	}
	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/**.cpp",
		"%{prj.name}/include/**.h"
	}
	links
	{
		"OpenMaya.lib",
		"OpenMayaAnim.lib",
		"OpenMayaRender.lib",
		"OpenMayaUI.lib",
		"Foundation.lib",
		"Lua.lib"
	}
	targetextension ".mll"
	targetname  	"vufRigMaster"
	targetdir 		("bin/" .. outputdir .. "/%{prj.name}")
	objdir 			("bin-int/" .. outputdir .. "/%{prj.name}")


	-- Maya 2018 specialization
	filter { "system:windows", "platforms:Maya2018" }
		includedirs 
		{
			"C:/Program Files/Autodesk/Maya2018/include",
			"C:/Program Files/Autodesk/Maya2018/include/qt"
		}
		libdirs
		{
			"bin/" .. outputdir .. "/Lua",
			"C:/Program Files/Autodesk/Maya2018/lib"
		}
		postbuildcommands 
		{
			("copy %{cfg.buildtarget.directory}%{cfg.targetname}.mll \"C:/Program Files/Autodesk/Maya2018/bin/plug-ins/vufRigMaster.mll\"")
			--("{COPY} %{cfg.targetdir}/%{cfg.targetname}.mll  C:/Program Files/Autodesk/Maya2018/bin/plug-ins/vufRigMaster.mll")
		}
		
		
	-- Maya 2019 specializaton
	filter { "system:windows", "platforms:Maya2019" }
		includedirs 
		{
			"C:/Program Files/Autodesk/Maya2019/include",
			"C:/Program Files/Autodesk/Maya2019/include/qt"
		}
		libdirs
		{
			"bin/" .. outputdir .. "/Lua",
			"C:/Program Files/Autodesk/Maya2019/lib"
		}
		
		postbuildcommands 
		{		
			("copy %{cfg.buildtarget.directory}%{cfg.targetname}.mll \"C:/Program Files/Autodesk/Maya2019/bin/plug-ins/vufRigMaster.mll\""),
			("copy %{cfg.buildtarget.directory}Lua.dll \"C:/Program Files/Autodesk/Maya2019/bin/Lua.dll\"")

			--("{COPY} %{cfg.targetdir}/%{cfg.targetname}.mll  C:/Program Files/Autodesk/Maya2019/bin/plug-ins/vufRigMaster.mll")
		}
		
		
	-- Maya 2020 specialization
	filter { "system:windows", "platforms:Maya2020" }
		includedirs 
		{
			"C:/Program Files/Autodesk/Maya2020/include",
			"C:/Program Files/Autodesk/Maya2020/include/qt"
		}
		libdirs
		{
			"bin/" .. outputdir .. "/Lua",
			"C:/Program Files/Autodesk/Maya2020/lib"
		}
		postbuildcommands 
		{
			("copy %{cfg.buildtarget.directory}%{cfg.targetname}.mll \"C:/Program Files/Autodesk/Maya2020/bin/plug-ins/vufRigMaster.mll\"")
			--("{COPY} %{cfg.targetdir}/%{cfg.targetname}.mll  C:/Program Files/Autodesk/Maya2020/bin/plug-ins/vufRigMaster.mll")
		}
--------------------------------------------------------------------------------------------	
-- 									    PROJECT SANDBOX
--------------------------------------------------------------------------------------------
project "Sandbox"
	location 	"Sandbox"
	kind 		"ConsoleApp"
	language 	"C++"
	
	includedirs 
	{
		"Lua/src/",
		"%{prj.name}/include",
		"Math/include"
	}
	libdirs
	{
		"bin/" .. outputdir .. "/Lua"
	}
	links
	{
		"Lua.lib"
	}
	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/**.cpp",
		"%{prj.name}/include/**.h"
	}
	targetdir 	("bin/" .. outputdir .. "/%{prj.name}")
	objdir 		("bin-int/" .. outputdir .. "/%{prj.name}")