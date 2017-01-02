@echo off

set LIBS="kernel32.lib user32.lib gdi32.lib"

if not exist build mkdir build
pushd build
msvc.bat ../tests/win32_test.c D %LIBS%
popd

