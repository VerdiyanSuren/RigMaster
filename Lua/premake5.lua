--------------------------------------------------------------------------------------------	
-- 										Project LUA
--------------------------------------------------------------------------------------------	
project "Lua"
	--location 	"Lua"
	kind 		"SharedLib"
	language 	"C++"
	
	targetdir 	("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir 		("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs 
	{
		"%{IncludeDir.Lua}"
		--"%{prj.name}/include"
	}
	files
	{		
		"src/lapi.c",
		"src/lcode.c",
		"src/lctype.c",
		"src/ldebug.c",
		"src/ldo.c",
		"src/ldump.c",
		"src/lfunc.c",
		"src/lgc.c",
		"src/llex.c",
		"src/lmem.c",
		"src/lobject.c",
		"src/lopcodes.c",
		"src/lparser.c",
		"src/lstate.c",
		"src/lstring.c",
		"src/ltable.c",
		"src/ltm.c",
		"src/lundump.c",
		"src/lvm.c",
		"src/lzio.c",
		"src/lauxlib.c",
		"src/lbaselib.c",
		"src/lcorolib.c",
		"src/ldblib.c",
		"src/liolib.c",
		"src/lmathlib.c",
		"src/loadlib.c",
		"src/loslib.c",
		"src/lstrlib.c",
		"src/ltablib.c",
		"src/lutf8lib.c",
		"src/linit.c",		
		"src/**.h"
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
	
	filter { "system:windows", "configurations:Release" }
		buildoptions 	"/MD"
		staticruntime 	"off"
