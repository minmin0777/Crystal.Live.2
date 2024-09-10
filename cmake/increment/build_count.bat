@echo off
set count_file=build_count.txt
if not exist %count_file% echo 0 > %count_file%
set /p count=<%count_file%
set /a count=%count%+1
echo %count% > %count_file%
echo Build number: %count%