@echo off
setlocal

REM ------------------------------------------------------------
REM RetroArch executable path
REM ------------------------------------------------------------
set RA_EXE=C:\RetroArch-Win64\retroarch.exe

if not exist "%RA_EXE%" (
  echo ERROR: RetroArch executable not found:
  echo %RA_EXE%
  exit /b 1
)

REM ------------------------------------------------------------
REM Kill RetroArch if it is already running (ignore errors)
REM ------------------------------------------------------------
taskkill /IM retroarch.exe /F >nul 2>&1

REM ------------------------------------------------------------
REM Launch RetroArch
REM ------------------------------------------------------------
start "" "%RA_EXE%"

exit /b 0
