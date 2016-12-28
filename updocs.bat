@echo off
echo # Handmade FTW > README.md
type docs\overview.html >> README.md
type COPYRIGHT.md >> README.md

call docs docs.html docs\title.txt docs\overview.html
REM pageant
psftp -l zaklaus madaraszd.net -b updocs.scr