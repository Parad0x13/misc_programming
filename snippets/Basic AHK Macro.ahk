#NoEnv			; Recommended for performance and compatibility with future AutoHotkey releases.
SendMode Input	; Recommended for new scripts due to its superior speed and reliability.

; Some useful reminders:
; ^ shift
; ! alt
; + shift
; # LWin/RWin
; Send Commands: https://autohotkey.com/docs/commands/Send.htm

click() {
	Loop {
		Click
	}
}

^!C::
click()
Return

location() {
	MouseGetPos x, y
	MsgBox %x%, %y%
}

^!L::
location()
Return

^!R::
MsgBox, Script Reloaded
Reload
