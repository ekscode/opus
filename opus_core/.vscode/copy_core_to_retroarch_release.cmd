@echo off
setlocal

set SRC=%~dp0..\build\x64\Release\opus_libretro.dll
set DST=C:\RetroArch-Win64\cores\opus_libretro.dll

if not exist "%SRC%" (
  echo ERROR: Release core not found:
  echo %SRC%
  exit /b 1
)

if not exist "C:\RetroArch-Win64\cores" (
  echo ERROR: RetroArch cores folder not found:
  echo C:\RetroArch-Win64\cores
  exit /b 1
)

copy /y "%SRC%" "%DST%"
if errorlevel 1 exit /b 1

echo Copied:
echo   %SRC%
echo to:
echo   %DST%
exit /b 0
