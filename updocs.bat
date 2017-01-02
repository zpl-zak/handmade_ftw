@echo off

echo # Handmade FTW > README.md
type docs\overview.html >> README.md
type COPYRIGHT.md >> README.md

pushd code\codecs
call updocs
popd

pushd code\formats
call updocs
popd

call docs docs.html docs\title.txt docs\overview.html
REM pageant
psftp -l zaklaus madaraszd.net -b docs\updocs.scr

move docs.html docs\