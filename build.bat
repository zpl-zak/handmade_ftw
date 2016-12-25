@echo off

set LIBS=kernel32.lib user32.lib

if not exist build mkdir build
pushd build
call msvc.bat ../win32_test.c D "/link /NODEFAULTLIB /SUBSYSTEM:WINDOWS %LIBS%"
popd

