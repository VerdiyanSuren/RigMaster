--------------------------------------------------------------------------------------------	
-- 										Project MAYA
--------------------------------------------------------------------------------------------		
project "Maya"
	--location 	"Maya"
	kind 		"SharedLib"
	language 	"C++"
	
	includedirs 
	{
		"%{IncludeDir.Core}",
		"%{IncludeDir.Lua}",
		"%{IncludeDir.LuaWrapper}",
		"%{IncludeDir.Math}",
		"%{IncludeDir.Maya}"
	}
	libdirs
	{
		"../bin/" .. outputdir .. "/Lua",
		"../bin/" .. outputdir .. "/Core"		
	}
	files
	{
		"src/**.cpp",
		"**.cpp",
		"include/**.h"
	}
	links
	{
		"OpenMaya.lib",
		"OpenMayaAnim.lib",
		"OpenMayaRender.lib",
		"OpenMayaUI.lib",
		"Foundation.lib",
		"Qt5Gui.lib",
		"Qt5Core.lib",
		"Qt5Widgets.lib",
		"Lua.lib",
		"Core.lib"
	}
	targetextension ".mll"
	targetname  	"vufRigMaster"
	
	targetdir 	("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir 		("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")


	filter { "system:windows", "configurations:Release" }
		buildoptions 	"/MD"
		staticruntime 	"off"

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
			"C:/Program Files/Autodesk/Maya2018/lib",
			"C:/local/boost_1_76_0_vs_14_0/lib64-msvc-14.0"
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
			"C:/Program Files/Autodesk/Maya2019/lib",
			"C:/local/boost_1_76_0_vs_14_0/lib64-msvc-14.0"
		}
		prebuildcommands 
		{
			("\"C:/Program Files/Autodesk/Maya2019/bin/moc.exe\" .%{prj.directory}/include/expressions/ui/vuLuafExprWindow.h -o .%{prj.directory}/src/expressions/ui/vuLuafExprWindow_moc.cpp")
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
			"C:/Program Files/Autodesk/Maya2020/lib",
			"C:/local/boost_1_76_0_vs_14_1/lib64-msvc-14.1"
		}
		postbuildcommands 
		{
			("copy %{cfg.buildtarget.directory}%{cfg.targetname}.mll \"C:/Program Files/Autodesk/Maya2020/bin/plug-ins/vufRigMaster.mll\""),
			("copy %{cfg.buildtarget.directory}Lua.dll \"C:/Program Files/Autodesk/Maya2020/bin/Lua.dll\"")
		}
	
	-- Maya 2022 specialization
	filter { "system:windows", "platforms:Maya2022" }
		includedirs 
		{
			"C:/Program Files/Autodesk/Maya2022/include",
			"C:/Program Files/Autodesk/Maya2022/include/qt"
		}
		libdirs
		{
			"bin/" .. outputdir .. "/Lua",
			"C:/Program Files/Autodesk/Maya2022/lib",
			"C:/local/boost_1_76_0_vs_14_2/lib64-msvc-14.2"
		}
		postbuildcommands 
		{
			("copy %{cfg.buildtarget.directory}%{cfg.targetname}.mll \"C:/Program Files/Autodesk/Maya2022/bin/plug-ins/vufRigMaster.mll\""),
			("copy %{cfg.buildtarget.directory}Lua.dll \"C:/Program Files/Autodesk/Maya2022/bin/Lua.dll\"")
		}


