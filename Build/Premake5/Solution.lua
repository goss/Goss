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
IncludeDir["Engine"] = "%{wks.location}/Engine/Source/Runtime"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["GLFW"] = "%{wks.location}/Engine/ThirdParty/glfw/include"
IncludeDir["GLM"] = "%{wks.location}/Engine/ThirdParty/glm/glm"
IncludeDir["SPDLOG"] = "%{wks.location}/Engine/ThirdParty/spdlog/include"

Library = {}
Library["Vulkan"] = "%{VULKAN_SDK}/Lib/vulkan-1.lib"
Library["VulkanUtils"] = "%{VULKAN_SDK}/VkLayer_utils.lib"

include "Engine/ThirdParty/glfw/" --include GLFW premade5.lua

project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "gepch.h"
	pchsource "Engine/Source/Runtime/gepch.cpp"

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
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
		"%{IncludeDir.Engine}",
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