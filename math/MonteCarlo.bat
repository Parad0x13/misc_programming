:: Bryan Tremblay 26Jan2016. Monte Carlo v1.0 simulator for command prompt

echo off
setlocal enabledelayedexpansion

call :playGame 0 100
echo %winCount%

pause

:: Creates global randVal
:rand
	if not defined randSeeded (
		set randSeeded=1
		call :rand
	)
	set limit=100
	if not [%1]==[] set limit=%1
	set /a randVal=%random% * %limit% / 32768 + 1
exit /b

:: GameModes, 0 for do not switch, 1 for always switch
:: Creates global winCount
:playGame
	if [%1]==[] (
		echo [ERROR] Game Mode needs to be set
			exit /b
		)
		if [%2]==[] (
			echo [ERROR] Number of games to play needs to be set
			exit /b
		)

	set winCount=0
	set gameMode=%1
	set gameCount=%2

	for /L %%i in (1, 1, !gameCount!) do (
		set gameWon=0
		call :rand 3
		set door=!randVal!
		call :rand 3
		set selectedDoor=!randVal!

		if !gameMode!==0 (
			if !selectedDoor!==!door! set gameWon=1
		)
		if !gameMode!==1 (
			call :rand 2
			set newDoorIsWin=!randVal!
			if !newDoorIsWin!==1 set gameWon=1
		)
		if !gameWon!==1 set /a winCount=!winCount! + 1
	)
exit /b
