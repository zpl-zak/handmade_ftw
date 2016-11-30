@echo off

IF NOT DEFINED clset (call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64)
SET clset=64

SET SRC=%1
if "%SRC%" == "" SET SRC=test.c

SET OPTS=/W4 /wd4189 /wd4310 /wd4100 /wd4201 /wd4505 /wd4996 /wd4127 /wd4510 /wd4512 /wd4610 /wd4457 /WX
SET OPTS=%OPTS% /GR- /nologo /FC /O2
SET DEBUG=/Zi /DEBUG /Zo

SET CODE_HOME=%~dp0

cl /I%CODE_HOME% %OPTS% %DEBUG% %SRC%

REM file spammation preventation
del *.obj

