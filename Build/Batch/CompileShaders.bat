@echo off
set "TAB=	"
cd ..\..\Assets\Shaders\

for /R %%I in (*.vert) do (
	if not exist "%%~pICompiled\Vulkan\" mkdir %%~pICompiled\Vulkan
	echo "Compiling *.verts"
	echo %TAB% "%%~nI%%~xI"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" "%%I" -o "%%~pICompiled\Vulkan\%%~nI%%~xI.spv"
)

echo: 
for /R %%I in (*.frag) do (
	if not exist "%%~pICompiled\Vulkan\" mkdir %%~pICompiled\Vulkan
	echo "Compiling *.frags"
	echo %TAB% "%%~nI%%~xI"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" "%%I" -o "%%~pICompiled\Vulkan\%%~nI%%~xI.spv"
)

echo: 
for /R %%I in (*.glsl) do (
	if not exist "%%~pICompiled\OpenGL\" mkdir %%~pICompiled\OpenGL
	echo "Compiling *.glsl"
	echo %TAB% "%%~nI%%~xI"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" "%%I" -o "%%~pICompiled\OpenGL\%%~nI%%~xI.spv"
)
pause