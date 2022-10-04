outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
VULKAN_SDK = os.getenv("VULKAN_SDK")

workspace "Goss"
	architecture "x86_64"
	startproject "Sandbox"
	
	configurations
	{
		"Debug",
		"Release"
	}
	
IncludeDir = {}
IncludeDir["EngineSource"] = "%{wks.location}/Engine/Source/"
IncludeDir["EngineRendering"] = "%{wks.location}/Engine/Source/Runtime/Rendering/Public"
IncludeDir["EngineLogging"] = "%{wks.location}/Engine/Source/Runtime/Core/Logging/Public"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["GLFW"] = "%{wks.location}/Engine/ThirdParty/glfw/include"
IncludeDir["GLM"] = "%{wks.location}/Engine/ThirdParty/glm/glm"
IncludeDir["SPDLOG"] = "%{wks.location}/Engine/ThirdParty/spdlog/include"

Library = {}
Library["Vulkan"] = "%{VULKAN_SDK}/Lib/vulkan-1.lib"
Library["VulkanUtils"] = "%{VULKAN_SDK}/VkLayer_utils.lib"

--include GLFW premade5.lua 
--new projects should copy the glfw.lua script to Engine/ThirdParty/glfw/ folder
--rename file to premake5.lua
include "Engine/ThirdParty/glfw/" 

project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "gepch.h"
	pchsource "Engine/Source/gepch.cpp"

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.EngineSource}",
		"%{IncludeDir.EngineLogging}",
		"%{IncludeDir.EngineRendering}",

		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.SPDLOG}",
	}
	
	links
	{
		"%{Library.Vulkan}",
		"GLFW" --project GLFW
	}

	defines
	{
		"GE_PLATFORM_WINDOWS",
		"GE_BUILD_DLL"
	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../Bin/" .. outputdir .. "/Sandbox");
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
	
	
	filter "configurations:Debug"
		defines "GE_DEBUG"
		runtime "Debug"
		symbols "On"
		staticruntime "off"

	filter "configurations:Release"
		defines "GE_RELEASE"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location"Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.EngineSource}",
		"%{IncludeDir.EngineLogging}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.SPDLOG}",
	}

	links
	{
		"Engine" --project Engine
	}

	defines
	{
		"GE_PLATFORM_WINDOWS"
	}

	filter "system:windows"
		systemversion "latest"
	
	filter "configurations:Debug"
		defines "GE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "GE_RELEASE"
		runtime "Release"
		optimize "On"