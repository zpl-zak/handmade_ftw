@echo off
call docs docs.html docs\title.txt docs\overview.html
REM pageant
psftp -l zaklaus madaraszd.net -b updocs.scr