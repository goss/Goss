outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
vulkandir = os.getenv("VULKAN_SDK")

workspace "Goss"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}
	
project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"Source",
		"" .. vulkandir .. "/include",
		"%{prj.name}/ThirdParty/glm/glm",
		"%{prj.name}/ThirdParty/spdlog/include"
	}
	
	links
	{
		"GLFW",
		"vulkan-1.lib"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"GE_PLATFORM_WINDOWS",
		"GE_BUILD_DLL"
	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox");
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
	
	
	filter "configurations:Debug"
		defines "GE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GE_RELEASE"
		optimize "On"


project "Sandbox"
	location"Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"Source",
		"" .. vulkandir .. "/include",
		"%{prj.name}/ThirdParty/glm/glm",
		"%{prj.name}/ThirdParty/spdlog/include"
	}

	links
	{
		"Engine"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"GE_PLATFORM_WINDOWS"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
	

	filter "configurations:Debug"
		defines "GE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GE_RELEASE"
		optimize "On"
