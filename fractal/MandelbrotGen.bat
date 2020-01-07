REM Worked on this at work early October 2016 for a bit but didn't have the time or care to finish it. Maybe someday

@echo off
setlocal enabledelayedexpansion
REM Mandelbrot formula: Z = Z^2 + C
REM To combat the fact that batch doesn't handle floats I've multiplied everything
REM by a constant to instead do math on decimal values only... ugly

set line=
set escapeCount=
set /a multiplier=1000
set /a escape=50*%multiplier%
set /a iterations=10
set /a ULx=-2*%multiplier%
set /a ULy=1*%multiplier%
set /a BRx=1*%multiplier%
set /a BRy=-1*%multiplier%
set /a canvasWidth=80
set /a canvasHeight=24
set /a xDelta=(%BRx%-%ULx%)/%canvasWidth%
set /a yDelta=(%BRy%-%ULy%)/%canvasHeight%

REM for /l %%x in (0,1,%canvasWidth%-1) do (
for /l %%x in (0,1,%canvasWidth%) do (
        set /a Zr=0
        set /a Zi=0
        set /a Cr=!Zr!+%%x*!xDelta!
        set /a Ci=!Zi!-1
        set /a Cr=!Cr!*!multiplier!
        set /a Ci=!Ci!*!multiplier!
        for /l %%f in (1,1,%iterations%) do (
                set /a escapeCount=%%f
                REM Real becomes r^2-i^2 and imaginary becomes (ri)^2
                set /a Nr=!Zr!*!Zr!-!Zi!*!Zi!
                set /a Ni=!Zr!*!Zi!*2
                set /a Nr=!Nr!+!Cr!
                set /a Ni=!Ni!+!Ci!
                set /a Zr=!Nr!
                set /a Zi=!Ni!
                REM Distance calculations require square rooting and that is very slow to emulate in batch
                REM So instead I'm going to cheat by checking if z+i is outside the escape radius
                set /a pretendDistance=!Zr!+!Zi!
                if !pretendDistance! gtr !escape! (
                        goto:breakLoop
                )
        )
        :breakLoop
        echo escaped in %escapeCount%
)
pause

echo %line% > image.txt

REM Exit main
exit /b 0

:findSquare
setlocal
for /l %%f in (1,1,%1) do (
        set /a val=%%f*%%f
        if !val! gtr %1 (
                set /a tmp=%%f-1
                goto:break_findSquare
        )
)
:break_findSquare
endlocal & set retVal=%tmp%
exit /b 0
