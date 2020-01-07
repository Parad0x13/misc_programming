@echo off

REM Given enough iterations the size on disk will be smaller than the size of the folder itself
REM Easily cleaned up with git gc

mkdir test
cd test

git init
git config user.name = "AI"
git config user.email = "na"

set loopcount=100

:loop
echo %RANDOM% >randomNumber
git add --all > NULL
git commit -m "I" > NULL

set /a loopcount=loopcount-1
if %loopcount%==0 goto exitloop
goto loop

:exitloop
