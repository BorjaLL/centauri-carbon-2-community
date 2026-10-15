@echo off
cd /d "%~dp0"
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Build-Trial.ps1"
if errorlevel 1 echo BUILD FAILED. Paste the first error.
pause
