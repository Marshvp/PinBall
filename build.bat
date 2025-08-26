@echo off
setlocal

REM === Config: edit these if your install paths differ ===
set "MSYS=C:\msys64"
REM If you use UCRT64 instead, change mingw64 -> ucrt64 on the next line:
set "MINGW=%MSYS%\mingw64"

set "GPP=%MINGW%\bin\g++.exe"
set "RAYLIB_ROOT=C:\raylib-5.5\raylib-5.5"
set "RAYLIB_INC=%RAYLIB_ROOT%\src"
set "RAYLIB_LIB=%RAYLIB_ROOT%\build\raylib"

REM === Sanity checks ===
if not exist "%GPP%" (
  echo ERROR: %GPP% not found. Edit MSYS/MINGW paths at the top of this script.
  exit /b 1
)

REM Optional: verify Box2D import lib exists
if not exist "%MINGW%\lib\libbox2d.dll.a" (
  echo ERROR: Box2D import library not found: %MINGW%\lib\libbox2d.dll.a
  echo        Install it in the MSYS2 MinGW x64 shell with:
  echo        pacman -S mingw-w64-x86_64-box2d
  exit /b 1
)

REM Ensure runtime DLLs (box2d, libstdc++, etc.) can be found
set "PATH=%MINGW%\bin;%PATH%"

echo Building game...
"%GPP%" -std=c++17 -g ^
  main.cpp ^
  createB2bodies.cpp ^
  -o game.exe ^
  -I"%RAYLIB_INC%" ^
  -I"%MINGW%\include" ^
  -L"%RAYLIB_LIB%" ^
  -L"%MINGW%\lib" ^
  -lraylib ^
  -lbox2d ^
  -lopengl32 -lgdi32 -lwinmm

if errorlevel 1 (
    echo Build failed!
    exit /b %errorlevel%
)

echo Build successful! Running game...
game.exe
set "EXITCODE=%ERRORLEVEL%"
if not "%EXITCODE%"=="0" (
  echo Game exited with code %EXITCODE%.
)


REM --- Copy required runtime DLLs next to the EXE ---
REM Box2D DLL (name may vary slightly; copy whatever exists)
for %%F in ("%MINGW%\bin\libbox2d*.dll") do copy /Y "%%~fF" . >nul

REM MinGW C++ runtime DLLs
for %%F in ("%MINGW%\bin\libstdc++-6.dll" "%MINGW%\bin\libgcc_s_seh-1.dll" "%MINGW%\bin\libwinpthread-1.dll") do (
  if exist "%%~fF" copy /Y "%%~fF" . >nul
)

REM If your Raylib is a DLL, copy it too:
if exist "%RAYLIB_LIB%\raylib.dll" copy /Y "%RAYLIB_LIB%\raylib.dll" . >nul
if exist "%MINGW%\bin\raylib.dll"   copy /Y "%MINGW%\bin\raylib.dll"   . >nul


endlocal & exit /b %EXITCODE%
