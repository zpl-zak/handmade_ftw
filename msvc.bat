@echo off

IF NOT DEFINED clset (call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64)
SET clset=64

SET SRC=%1
if "%SRC%" == "" SET SRC=main.cpp

SET ADD=%2

SET OPTS=/W4 /wd4189 /wd4310 /wd4100 /wd4201 /wd4505 /wd4996 /wd4127 /wd4510 /wd4512 /wd4610 /wd4457 /WX
SET OPTS=%OPTS% /GR- /nologo -MTd -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127

SET DEBUG=/Zi /Zo /DEBUG
SET OPTI=/FC /GS- -O2
SET O=%OPTI%
if "%ADD%" == "D" SET O=%DEBUG%

SET CODE_HOME=%~dp0
SET HFTW_PATH=%HFTW_PATH%

ctime -begin %SRC%.ctm
cl /I%CODE_HOME% /I%HFTW_PATH% %OPTS% %O% %SRC% /link -incremental:no -opt:ref
ctime -end %SRC%.ctm

rem del *.obj
rem del *.exp
rem del *.ilk
