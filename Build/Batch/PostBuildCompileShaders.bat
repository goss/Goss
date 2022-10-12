@echo off
set "TAB=	"
echo %1
cd %1
::glslc.exe or glslangValidator.exe
for /R %%I in (*.vert) do (
	if not exist "%%~pICompiled\Vulkan" mkdir %%~pICompiled\Vulkan
	if not exist "%%~pICompiled\OpenGL" mkdir %%~pICompiled\OpenGL
	echo "Compiling *.verts"
	echo %TAB% "%%~nI%%~xI"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" --target-env=vulkan "%%I" -o "%%~pICompiled\Vulkan\%%~nI%%~xI.spv"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" --target-env=opengl "%%I" -o "%%~pICompiled\OpenGL\%%~nI%%~xI.spv"
)

echo: 
for /R %%I in (*.frag) do (
	if not exist "%%~pICompiled\Vulkan" mkdir %%~pICompiled\Vulkan
	if not exist "%%~pICompiled\OpenGL" mkdir %%~pICompiled\OpenGL
	echo "Compiling *.frags"
	echo %TAB% "%%~nI%%~xI"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" --target-env=vulkan "%%I" -o "%%~pICompiled\Vulkan\%%~nI%%~xI.spv"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" --target-env=opengl "%%I" -o "%%~pICompiled\OpenGL\%%~nI%%~xI.spv"
)

echo: 
for /R %%I in (*.glsl) do (
	if not exist "%%~pICompiled\Vulkan" mkdir %%~pICompiled\Vulkan
	if not exist "%%~pICompiled\OpenGL" mkdir %%~pICompiled\OpenGL
	echo "Compiling *.glsl"
	echo %TAB% "%%~nI%%~xI"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" --target-env=vulkan "%%I" -o "%%~pICompiled\Vulkan\%%~nI%%~xI.spv"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" --target-env=opengl "%%I" -o "%%~pICompiled\OpenGL\%%~nI%%~xI.spv"
)
pause