@echo off

REM IF NOT DEFINED clset (call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64)
REM SET clset=64

REM SET SRC=%1
REM if "%SRC%" == "" SET SRC=test.c

REM SET OPTS=/W4 /wd4189 /wd4310 /wd4100 /wd4201 /wd4505 /wd4996 /wd4127 /wd4510 /wd4512 /wd4610 /wd4457 /WX
REM SET OPTS=%OPTS% /GR- /nologo /FC /O2
REM SET DEBUG=/Zi /DEBUG /Zo

REM SET CODE_HOME=%~dp0

REM ctime -begin test_case.ctm
REM cl /I%CODE_HOME% %OPTS% %DEBUG% %SRC%
REM ctime -end test_case.ctm

REM file spammation preventation
REM del *.obj

msvc.bat test.c D

