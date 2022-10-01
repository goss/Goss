outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"{prj.name}/Source/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/ThirdParty/spdlog/include"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"GE_PLATFORM_WINDOWS",
		"GE_BUILD_DLL"
	}

	filter "system:windows"
	{
		cppdialect "C++17"
		staticruntime "On"
		systemverison "Latest"
	}

	postbuildscomands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox");
	}

	filter "configurations:Debug"
		defines "GE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GE_RELEASE"
		optmize "On"


project "Sandbox"
	location"Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"{prj.name}/Source/**.cpp",
	}

	includedirs
	{
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
	{
		cppdialect "C++17"
		staticruntime "On"
		systemverison "Latest"
	}

	filter "configurations:Debug"
		defines "GE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GE_RELEASE"
		optmize "On"
