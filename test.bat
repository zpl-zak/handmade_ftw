@echo off
REM ctime -begin test.ctm

pushd build
start win32_test.exe
popd

REM ctime -end test.ctm