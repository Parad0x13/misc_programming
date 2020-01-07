#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

#include ImageManagementStardewValley

fishing() {
	WinGetPos,,, width, height, A

	Click down
	Sleep 1040
	Click up

	deterministicallyFind([[-1, -1]], ["IsFishing.png"])

	oldFishY := 0
	Loop {
		fishLoc := deterministicallyFind([[-1, -1]], ["Fish.png"])
		fishY := fishLoc[2]
		delta := oldFishY - fishY
		msgbox % delta
		Sleep 1000
	}
}

strength() {
	Click down
	Sleep 100
	Click up

	Sleep 200

	Click down
	Sleep 100
	Click up
}

!S::
strength()
Return

!F::
fishing()
Return

^!R::
MsgBox, Stuff Reloaded
Reload
