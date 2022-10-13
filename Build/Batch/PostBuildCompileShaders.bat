@echo off
set "TAB=	"
echo %1
cd %1
::glslc.exe or glslangValidator.exe
for /R %%I in (*.vert) do (
	if not exist "%%~pICompiled" mkdir %%~pICompiled
	echo "Compiling *.verts"
	echo %TAB% "%%~nI%%~xI"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" "%%I" -o "%%~pICompiled\%%~nI%%~xI.spv"
)

echo: 
for /R %%I in (*.frag) do (
	if not exist "%%~pICompiled" mkdir %%~pICompiled
	echo "Compiling *.frags"
	echo %TAB% "%%~nI%%~xI"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" "%%I" -o "%%~pICompiled\%%~nI%%~xI.spv"
)

echo: 
for /R %%I in (*.glsl) do (
	if not exist "%%~pICompiled" mkdir %%~pICompiled
	echo "Compiling *.glsl"
	echo %TAB% "%%~nI%%~xI"
	"C:\VulkanSDK\1.3.224.1\Bin\glslc.exe" "%%I" -o "%%~pICompiled\%%~nI%%~xI.spv"
)
pause