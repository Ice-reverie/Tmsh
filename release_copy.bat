@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem Copies 3rd-party runtime files into the Release output folder.
rem Target: output\bin
rem Rules:
rem - Traverse Tools\Win64\<vendor>\...
rem - For each vendor, copy its "bin" folder (vendor\bin or vendor\<ver>\bin) into output\bin
rem - If no bin folder exists, skip
rem - Python37: copy vendor\Lib and vendor\*.dll into output\bin
rem - gmsh: copy vendor\bin into output\bin
rem - OpenGL: skip entirely

set "ROOT=%~dp0"
pushd "%ROOT%" >nul

set "TOOLS=%ROOT%Tools\Win64"
set "DEST=%ROOT%output\bin"

if not exist "%TOOLS%\" (
  echo [error] Tools path not found: "%TOOLS%"
  popd >nul
  exit /b 1
)

if not exist "%DEST%\" mkdir "%DEST%" >nul 2>&1

set "FAIL=0"
set "COPIED=0"
set "SKIPPED=0"

echo [info] Tools  : "%TOOLS%"
echo [info] Target : "%DEST%"

for /d %%D in ("%TOOLS%\*") do (
  set "VENDOR_DIR=%%~fD"
  set "VENDOR_NAME=%%~nxD"

  if /I "!VENDOR_NAME!"=="OpenGL" (
    echo [skip] OpenGL
    set /a SKIPPED+=1
  ) else if /I "!VENDOR_NAME!"=="Python37" (
    call :copy_python "!VENDOR_DIR!" "%DEST%"
  ) else if /I "!VENDOR_NAME!"=="gmsh" (
    call :copy_vendor_level "!VENDOR_DIR!" "!VENDOR_NAME!" "%DEST%" "bin"
  ) else (
    call :copy_vendor_level "!VENDOR_DIR!" "!VENDOR_NAME!" "%DEST%" "bin"
  )
)

echo [done] copied=!COPIED! skipped=!SKIPPED! fail=!FAIL!
popd >nul
exit /b !FAIL!

:copy_vendor_level
set "VENDOR_DIR=%~1"
set "VENDOR_NAME=%~2"
set "DEST=%~3"
set "SUB=%~4"

set "SRC=%VENDOR_DIR%\%SUB%"
if exist "%SRC%\" (
  call :robocopy "%SRC%" "%DEST%" "%VENDOR_NAME%\\%SUB%"
  exit /b 0
)

set "FOUND=0"
for /d %%S in ("%VENDOR_DIR%\*") do (
  if exist "%%~fS\%SUB%\" (
    set "FOUND=1"
    call :robocopy "%%~fS\%SUB%" "%DEST%" "%VENDOR_NAME%\\%%~nxS\\%SUB%"
  )
)

if "!FOUND!"=="0" (
  echo [skip] %VENDOR_NAME% - no %SUB%
  set /a SKIPPED+=1
)
exit /b 0

:copy_python
set "PYDIR=%~1"
set "DEST=%~2"

if exist "%PYDIR%\Lib\" (
  call :robocopy "%PYDIR%\Lib" "%DEST%\Lib" "Python37\\Lib"
) else (
  echo [skip] Python37\\Lib - not found
  set /a SKIPPED+=1
)

for %%F in ("%PYDIR%\*.dll") do (
  if exist "%%~fF" (
    copy /Y "%%~fF" "%DEST%\" >nul
    if errorlevel 1 (
      echo [fail] Python37\\%%~nxF
      set "FAIL=1"
    ) else (
      echo [file] Python37\\%%~nxF
      set /a COPIED+=1
    )
  )
)
exit /b 0

:robocopy
set "SRC=%~1"
set "DEST=%~2"
set "LABEL=%~3"

echo [copy] %LABEL%
robocopy "%SRC%" "%DEST%" /E /COPY:DAT /DCOPY:DAT /R:1 /W:1 /NFL /NDL /NJH /NJS /NP >nul
set "RC=%ERRORLEVEL%"
if %RC% GEQ 8 (
  echo [fail] robocopy %RC% : %LABEL%
  set "FAIL=1"
) else (
  set /a COPIED+=1
)
exit /b 0
