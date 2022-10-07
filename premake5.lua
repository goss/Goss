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
	
	flags
	{
		"MultiProcessorCompile"
	}
	
IncludeDir = {}
IncludeDir["EngineSource"] = 			"%{wks.location}/Engine/Source/"
IncludeDir["EngineCore"] = 				"%{wks.location}/Engine/Source/Runtime/Core/Public"
IncludeDir["EngineEvents"] = 			"%{wks.location}/Engine/Source/Runtime/Events/Public"
IncludeDir["EngineGameframework"] = 	"%{wks.location}/Engine/Source/Runtime/GameFramework/Public"
IncludeDir["EnginePlatformWindows"] = 	"%{wks.location}/Engine/Source/Runtime/Platform/Windows/Public"
IncludeDir["EngineInterface"] = 		"%{wks.location}/Engine/Source/Runtime/RenderAPI/Interface/Public"
IncludeDir["EngineOpenGL"] = 			"%{wks.location}/Engine/Source/Runtime/RenderAPI/OpenGL/Public"
IncludeDir["EngineVulkan"] = 			"%{wks.location}/Engine/Source/Runtime/RenderAPI/Vulkan/Public"

IncludeDir["VulkanSDK"] = 				"%{VULKAN_SDK}/Include"
IncludeDir["GLAD"] = 					"%{wks.location}/Engine/ThirdParty/glad/include"
IncludeDir["GLFW"] = 					"%{wks.location}/Engine/ThirdParty/glfw/include"
IncludeDir["GLM"] = 					"%{wks.location}/Engine/ThirdParty/glm/glm"
IncludeDir["SPDLOG"] = 					"%{wks.location}/Engine/ThirdParty/spdlog/include"
IncludeDir["STB_IMAGE"] = 				"%{wks.location}/Engine/ThirdParty/stb_image"

LibraryDir = {}
LibraryDir["VulkanSDK"] = 				"%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = 					"%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = 				"%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

--Debug
Library["ShaderC_Debug"] = 				"%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = 			"%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = 	"%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = 			"%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

--Release
Library["ShaderC_Release"] = 			"%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = 		"%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = 	"%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

BuildCommands = {}
BuildCommands["CompileShaders"] =		("call $(SolutionDir)Build/Batch/CompileShaders.bat -$(ProjectDir)/Assets/Shaders");

--include GLFW premade5.lua 
--new projects should copy the glfw.lua script to Engine/ThirdParty/glfw/ folder
--rename file to premake5.lua
include "Engine/ThirdParty/glfw/" 
include "Engine/ThirdParty/glad/" 

project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "gepch.h"
	pchsource "Engine/Source/gepch.cpp"

	files
	{
		--Premake
		"%{wks.location}/premake5.lua",
		--Batch
		"%{wks.location}/Build/Batch/**.bat",
	
		--Engine
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		
		--Shaders
		"%{prj.name}/Assets/Shaders/**.glsl",
		"%{prj.name}/Assets/Shaders/**.vert",
		"%{prj.name}/Assets/Shaders/**.frag",

		
		--stb_image
		"%{prj.name}/ThirdParty/stb_image/**.h",
		"%{prj.name}/ThirdParty/stb_image/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.EngineSource}",
		"%{IncludeDir.EngineCore}",
		"%{IncludeDir.EngineEvents}",
		"%{IncludeDir.EngineGameframework}",
		"%{IncludeDir.EnginePlatformWindows}",
		"%{IncludeDir.EngineInterface}",
		"%{IncludeDir.EngineOpenGL}",
		"%{IncludeDir.EngineVulkan}",

		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.STB_IMAGE}",
	}
	
	links
	{
		--"opengl32.lib",
		"%{Library.Vulkan}",
		"GLFW", --project ThirdParty/glfw
		"GLAD", --project ThirdParty/glad
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	postbuildcommands
	{
		("call $(SolutionDir)Build/Batch/CompileShaders.bat -$(ProjectDir)/Assets/Shaders");
		("{COPY} %{cfg.buildtarget.relpath} ../Bin/" .. outputdir .. "/Sandbox");
	}

	--glslangValidator.exe

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "GE_DEBUG"
		symbols "On"
		runtime "Debug"
		
		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "GE_RELEASE"
		runtime "Release"
		optimize "On"
		
		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}


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
		--Sandbox
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		
		--Shaders
		"%{prj.name}/Assets/Shaders/**.glsl",
		"%{prj.name}/Assets/Shaders/**.vert",
		"%{prj.name}/Assets/Shaders/**.frag",
	}

	includedirs
	{
		"%{IncludeDir.EngineSource}",
		"%{IncludeDir.EngineCore}",
		"%{IncludeDir.EngineEvents}",
		"%{IncludeDir.EngineGameframework}",
		"%{IncludeDir.EngineInterface}",

		--"%{IncludeDir.VulkanSDK}",
		--"%{IncludeDir.GLAD}",
		--"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.STB_IMAGE}",
	}

	links
	{
		"Engine" --project Engine
	}

	postbuildcommands
	{
		("call $(SolutionDir)Build/Batch/CompileShaders.bat -$(ProjectDir)/Assets/Shaders");
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