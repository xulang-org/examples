@echo off
where pwsh >nul 2>nul
if errorlevel 1 (
    powershell -NoProfile -File "%~dp0build_and_test.ps1" %*
) else (
    pwsh -NoProfile -File "%~dp0build_and_test.ps1" %*
)
exit /b %errorlevel%
