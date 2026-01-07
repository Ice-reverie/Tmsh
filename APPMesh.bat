
call "D:\VS2017\build\VC\Auxiliary\Build\vcvarsall.bat" x64 10.0.19041.0

SET "PATH=D:\QT\build\5.14.2\5.14.2\msvc2017_64\bin\;D:\VS2017\build\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64;%PATH%"

qmake -r -tp vc APPMesh.pro

pause
