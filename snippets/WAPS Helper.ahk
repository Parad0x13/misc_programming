#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.

textX = 690
textY = 160
xOffset = 10
yOffset = 80

open() {
	global textX
	global textY
	global xOffset
	global yOffset
	Click %textX%, %textY%
	MouseMove %xOffset%, %yOffset%,, R
}

black() {
	MouseGetPos x, y
	open()
	MouseMove 0, 0,, R
	Click
	MouseMove %x%, %y%
}

grey() {
	MouseGetPos x, y
	open()
	MouseMove 60, 0,, R
	Click
	MouseMove %x%, %y%
}

lightgrey() {
	MouseGetPos x, y
	open()
	MouseMove 120, 0,, R
	Click
	MouseMove %x%, %y%
}

red() {
	MouseGetPos x, y
	open()
	MouseMove 20, 30,, R
	Click
	MouseMove %x%, %y%
}

!1::
black()
Return

!2::
grey()
Return

!3::
lightgrey()
Return

!4::
red()
Return

^!R::
MsgBox, Script Reloaded
Reload
