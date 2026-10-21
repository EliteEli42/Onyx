@echo off
setlocal

:: Switch to the repository root.
cd /d "%~dp0.."

:: Create the working directory if it does not exist.
if not exist "DebugDir" mkdir "DebugDir"

:: Generate Visual Studio 2026 project files.
"Vendor\premake\bin\premake5.exe" --build-dir=VS2026 vs2026

if errorlevel 1 (
    echo.
    echo [ERROR] Premake failed!
) else (
    echo.
    echo [SUCCESS] Visual Studio 2026 project files generated.
)

pause
endlocal