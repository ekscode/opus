@echo off
setlocal

REM ------------------------------------------------------------
REM Project paths (derived from this script location)
REM ------------------------------------------------------------
set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%.."
for %%I in ("%PROJECT_ROOT%") do set "PROJECT_ROOT=%%~fI"

set "INC_ROOT=%PROJECT_ROOT%\include"

REM ------------------------------------------------------------
REM Locate latest Visual Studio with MSVC x64 tools
REM ------------------------------------------------------------
for /f "usebackq tokens=* delims=" %%i in (`
  "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" ^
    -latest ^
    -products * ^
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
    -property installationPath
`) do set "VSINSTALL=%%i"

if "%VSINSTALL%"=="" (
  echo ERROR: Visual Studio with MSVC x64 tools not found.
  exit /b 1
)

REM ------------------------------------------------------------
REM Initialize MSVC environment (x64)
REM ------------------------------------------------------------
call "%VSINSTALL%\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b 1

REM ------------------------------------------------------------
REM Ensure output directories exist
REM ------------------------------------------------------------
if not exist build\x64\Debug mkdir build\x64\Debug
if not exist build\x64\Debug\obj mkdir build\x64\Debug\obj

REM ------------------------------------------------------------
REM Compile + link Opus libretro core (Debug)
REM ------------------------------------------------------------
echo.
echo [Opus Debug Build]
echo   PROJECT_ROOT = %PROJECT_ROOT%
echo   INC_ROOT     = %INC_ROOT%
echo.

cl /nologo /std:c++20 /EHsc /Zi /Od /MDd ^
  /I "%INC_ROOT%" ^
  /Fo:build\x64\Debug\obj\ ^
  src\opus_libretro.cpp ^
  /link /DLL ^
  /OUT:build\x64\Debug\opus_libretro.dll ^
  /IMPLIB:build\x64\Debug\opus_libretro.lib ^
  /PDB:build\x64\Debug\opus_libretro.pdb

exit /b %ERRORLEVEL%
