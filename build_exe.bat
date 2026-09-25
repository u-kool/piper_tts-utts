@echo off
rem Build libpiper (piper.dll) and piper_exe (piper.exe) with MSVC + Ninja
rem Usage: build_exe.bat [configure|build]
setlocal
set "ROOT=%~dp0"
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if /i "%~1"=="configure" (
  cmake -G Ninja -S "%ROOT%libpiper" -B "%ROOT%build" -DCMAKE_BUILD_TYPE=Release
) else (
  cmake --build "%ROOT%build" --target piper_exe
)
endlocal
