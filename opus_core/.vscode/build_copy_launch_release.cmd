@echo off
setlocal

echo === Opus: Build (Release) ===
call "%~dp0build_libretro_release.cmd"
if errorlevel 1 (
  echo ERROR: Build failed.
  exit /b 1
)

echo === Opus: Copy core to RetroArch ===
call "%~dp0copy_core_to_retroarch_release.cmd"
if errorlevel 1 (
  echo ERROR: Copy failed.
  exit /b 1
)

echo === Opus: Restart RetroArch ===
call "%~dp0launch_retroarch.cmd"
if errorlevel 1 (
  echo ERROR: Launch failed.
  exit /b 1
)

echo === Done ===
exit /b 0
