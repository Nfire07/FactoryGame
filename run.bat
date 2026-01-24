@echo off
echo Starting raylib Docker container on Windows

where docker >nul 2>nul
if errorlevel 1 (
    echo Docker is not installed
    exit /b 1
)

docker info >nul 2>nul
if errorlevel 1 (
    echo Docker Desktop is not running
    exit /b 1
)

docker compose version >nul 2>nul
if errorlevel 1 (
    echo docker compose is not available
    exit /b 1
)

where wsl >nul 2>nul
if errorlevel 1 (
    echo WSL is not installed
    exit /b 1
)

if "%DISPLAY%"=="" (
    echo DISPLAY is not set
)

tasklist | findstr /I vcxsrv.exe >nul
if errorlevel 1 (
    echo X server not detected. If not using WSLg, start VcXsrv
)

echo Launching container
docker compose up --build -d

echo Running dev-mode.bat
call dev-mode.bat

