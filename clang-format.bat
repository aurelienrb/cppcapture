@echo off

where clang-format.exe || exit /b 1

set SRC=%~dp0src

for /r "%SRC%" %%f in ("*.cpp" "*.h") do (
    echo - %%f
    clang-format.exe -i %%f
)
