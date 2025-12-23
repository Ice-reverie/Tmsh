@echo off
setlocal EnableExtensions

rem Deploys Qt runtime dependencies into the Debug output folder via windeployqt.
rem Default target: output\bin_d\PythonQt_QtAll-Qt5-Python3.7_d.dll
rem Usage:
rem   deploy_qt_debug.bat [path-to-target-dll-or-exe] [--no-pause|--pause] [--trace]

set "ROOT=%~dp0"
pushd "%ROOT%" >nul

set "OPT_NO_PAUSE="
set "OPT_FORCE_PAUSE="
set "OPT_TRACE="
set "DEFAULT_TARGET=output\bin_d\PythonQt_QtAll-Qt5-Python3.7_d.dll"
set "TARGET="

:parse_args
if "%~1"=="" goto :args_done
if /I "%~1"=="--no-pause" ( set "OPT_NO_PAUSE=1" & shift & goto :parse_args )
if /I "%~1"=="--pause" ( set "OPT_FORCE_PAUSE=1" & shift & goto :parse_args )
if /I "%~1"=="--trace" ( set "OPT_TRACE=1" & shift & goto :parse_args )
if not defined TARGET set "TARGET=%~1"
shift
goto :parse_args

:args_done
if defined OPT_TRACE echo on
if not defined TARGET set "TARGET=%DEFAULT_TARGET%"

call :resolve_target "%TARGET%"
if errorlevel 1 (
  set "RC=1"
  goto :exit_fail
)

call :find_qt_env
if errorlevel 1 (
  set "RC=1"
  goto :exit_fail
)

echo [info] Target: "%TARGET_ABS%"
echo [info] QtEnv : "%QTENV2%"

call "%QTENV2%"
if errorlevel 1 (
  echo [error] Failed to call qtenv2.bat
  set "RC=1"
  goto :exit_fail
)

set "LOCAL_TEMP=%ROOT%output\\_qt_temp"
if not exist "%LOCAL_TEMP%\\" mkdir "%LOCAL_TEMP%" >nul 2>&1
set "TEMP=%LOCAL_TEMP%"
set "TMP=%LOCAL_TEMP%"

set "WINDEPLOYQT=%QT_BIN%windeployqt.exe"
if not exist "%WINDEPLOYQT%" set "WINDEPLOYQT=windeployqt.exe"

echo [run ] %WINDEPLOYQT% "%TARGET_ABS%"
"%WINDEPLOYQT%" "%TARGET_ABS%"
set "RC=%ERRORLEVEL%"
if not "%RC%"=="0" (
  echo [error] windeployqt failed with code %RC%
  goto :exit_fail
)

echo [done] Qt deployment finished.
goto :exit_ok

:resolve_target
set "T=%~1"
set "TARGET_ABS="

if exist "%T%" (
  for %%I in ("%T%") do set "TARGET_ABS=%%~fI"
  exit /b 0
)

if exist "%ROOT%%T%" (
  for %%I in ("%ROOT%%T%") do set "TARGET_ABS=%%~fI"
  exit /b 0
)

echo [error] Target not found: "%T%"
exit /b 1

:find_qt_env
set "CACHE_DIR=%LOCALAPPDATA%\APPMesh"
set "CACHE_FILE=%CACHE_DIR%\qt_qtenv2_path.txt"
set "QTENV2="
set "QTENV2_ANY="
set "QTENV2_MSVC2017_64="
set "QT_BIN="

echo [info] Locating qtenv2.bat...

rem 1) Cache
if exist "%CACHE_FILE%" (
  for /f "usebackq delims=" %%L in ("%CACHE_FILE%") do (
    call :consider_qtenv "%%L"
  )
)
rem Only accept the MSVC2017_64 kit.
call :finish_if_found && exit /b 0

rem 2) Env vars
if defined QTDIR (
  if exist "%QTDIR%\bin\qtenv2.bat" call :consider_qtenv "%QTDIR%\bin\qtenv2.bat"
  if exist "%QTDIR%\qtenv2.bat" call :consider_qtenv "%QTDIR%\qtenv2.bat"
)
if defined QT_DIR (
  if exist "%QT_DIR%\bin\qtenv2.bat" call :consider_qtenv "%QT_DIR%\bin\qtenv2.bat"
  if exist "%QT_DIR%\qtenv2.bat" call :consider_qtenv "%QT_DIR%\qtenv2.bat"
)
call :finish_if_found && exit /b 0

rem 3) PATH
for /f "delims=" %%P in ('where.exe qtenv2.bat 2^>nul') do (
  call :consider_qtenv "%%P"
)
call :finish_if_found && exit /b 0

rem 4) Common roots (non-recursive list, but uses where /r within those roots)
call :scan_root "C:\Qt"
call :finish_if_found && exit /b 0
call :scan_root "D:\Qt"
call :finish_if_found && exit /b 0
call :scan_root "%ProgramFiles%\Qt"
call :finish_if_found && exit /b 0
call :scan_root "%ProgramFiles(x86)%\Qt"
call :finish_if_found && exit /b 0
call :scan_root "%USERPROFILE%\Qt"
call :finish_if_found && exit /b 0

rem 5) Prompt user
echo [error] Cannot find qtenv2.bat automatically.
echo [hint ] Must be MSVC2017_64: ...\msvc2017_64\bin\qtenv2.bat (not mingw / other kits).
set "INPUT="
set /p INPUT=Enter path to qtenv2.bat (or Qt bin folder): 
if not defined INPUT exit /b 1

set "CAND=%INPUT%"
if /I "%CAND:~-10%"=="qtenv2.bat" (
  set "QTENV2=%CAND%"
) else (
  if exist "%CAND%\qtenv2.bat" (
    set "QTENV2=%CAND%\qtenv2.bat"
  ) else if exist "%CAND%\bin\qtenv2.bat" (
    set "QTENV2=%CAND%\bin\qtenv2.bat"
  )
)

if not defined QTENV2 (
  echo [error] Invalid path: "%INPUT%"
  exit /b 1
)
if not exist "%QTENV2%" (
  echo [error] Not found: "%QTENV2%"
  exit /b 1
)

call :consider_qtenv "%QTENV2%"
call :finish_if_found
if errorlevel 1 (
  echo [error] The selected Qt kit is not MSVC2017_64: "%QTENV2%"
  echo [hint ] Please point to ...\msvc2017_64\bin\qtenv2.bat
  exit /b 1
)
exit /b 0

:finish_if_found
if not defined QTENV2_MSVC2017_64 exit /b 1
set "QTENV2=%QTENV2_MSVC2017_64%"
echo [info] Selected Qt kit: msvc2017_64
for %%I in ("%QTENV2%") do set "QT_BIN=%%~dpI"

if not exist "%QTENV2%" (
  echo [error] Not found: "%QTENV2%"
  exit /b 1
)

if not exist "%CACHE_DIR%\" mkdir "%CACHE_DIR%" >nul 2>&1
> "%CACHE_FILE%" echo %QTENV2%

exit /b 0

:consider_qtenv
set "CAND=%~1"
if not defined CAND exit /b 0
if not exist "%CAND%" exit /b 0

if not defined QTENV2_ANY set "QTENV2_ANY=%CAND%"

set "CANDPATH=%CAND%"
set "CANDPATH2=%CANDPATH:\msvc2017_64\bin\qtenv2.bat=%"
if not "%CANDPATH2%"=="%CANDPATH%" if not defined QTENV2_MSVC2017_64 set "QTENV2_MSVC2017_64=%CAND%"
set "CANDPATH2=%CANDPATH:\MSVC2017_64\bin\qtenv2.bat=%"
if not "%CANDPATH2%"=="%CANDPATH%" if not defined QTENV2_MSVC2017_64 set "QTENV2_MSVC2017_64=%CAND%"
set "CANDPATH2=%CANDPATH:/msvc2017_64/bin/qtenv2.bat=%"
if not "%CANDPATH2%"=="%CANDPATH%" if not defined QTENV2_MSVC2017_64 set "QTENV2_MSVC2017_64=%CAND%"
set "CANDPATH2=%CANDPATH:/MSVC2017_64/bin/qtenv2.bat=%"
if not "%CANDPATH2%"=="%CANDPATH%" if not defined QTENV2_MSVC2017_64 set "QTENV2_MSVC2017_64=%CAND%"
exit /b 0

:scan_root
set "ROOTDIR=%~1"
if not defined ROOTDIR exit /b 0
if not exist "%ROOTDIR%\" exit /b 0
set "ROOTDIR_S="
for %%I in ("%ROOTDIR%") do set "ROOTDIR_S=%%~sI"
if not defined ROOTDIR_S set "ROOTDIR_S=%ROOTDIR%"
for /f "delims=" %%P in ('where.exe /r %ROOTDIR_S% qtenv2.bat 2^>nul') do call :consider_qtenv "%%P"
exit /b 0

:exit_ok
call :maybe_pause 0
popd >nul
exit /b 0

:exit_fail
if not defined RC set "RC=1"
call :maybe_pause %RC%
popd >nul
exit /b %RC%

:maybe_pause
set "MAYBE_RC=%~1"

if defined OPT_NO_PAUSE exit /b 0
if defined OPT_FORCE_PAUSE goto :do_pause

rem If launched by double-click (cmd.exe /c), pause so output is visible.
echo.%CMDCMDLINE% | find /I "/c" >nul && goto :do_pause
exit /b 0

:do_pause
echo.
echo [exit] code=%MAYBE_RC%
pause
exit /b 0
