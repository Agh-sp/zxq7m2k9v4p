@echo off
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
cd /d F:\AC-master\source\vcpp
msbuild assaultcube.vcxproj /p:Configuration=Release /p:Platform=Win32 /p:PlatformToolset=v145 /m