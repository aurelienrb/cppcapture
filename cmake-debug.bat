@echo off
where cmake || exit /B 1

set BUILDDIR=%~dp0cmake-build-debug
if exist %BUILDDIR% (
    rmdir /s/q %BUILDDIR% || exit /b 1
)

mkdir %BUILDDIR% || goto:eof
compact /c /q %BUILDDIR% > nul
cd %BUILDDIR%

cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=Debug -DLOG_DEBUG_MESSAGES=ON -DBUILD_TESTS=ON .. || pause
