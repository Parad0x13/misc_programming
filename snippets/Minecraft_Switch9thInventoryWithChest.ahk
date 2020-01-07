; Pause
!Enter::Pause

; This is for switching the 9th item slot with your chest piece
Numpad9::
Send e
Sleep 250
Click 940, 780
Sleep 250
Click 355, 310
Sleep 250
Click 940, 780
Sleep 250
Send e
return

; This is for starting holding shift and forward continuously
!h::
Send {shift down}
Send {w down}
return

; Just holding shift
!s::
Send {shift down}
return

; For flying forward
!f::
Send {space down}
Send {w down}
return

; For auto left click
!l::
Click down
return

; For auto set liquid tele-pipe to lava (667 true)
Numpad6::
Loop 6 {	; 100
	Click 1000, 400
}
Loop 6 {	; 10
	Click 900, 400
}
Loop 7 {	; 1
	Click 700, 400
}
Click 350, 530
return

; For dropping clicked item from inventory
Numpad0::
SetDefaultMouseSpeed 0
MouseGetPos xPos, yPos
Click
Click 100, 500
MouseMove xPos, yPos
return

; For dropping clicked item from inventory for whole row
Numpad1::
Loop 9 {
	SetDefaultMouseSpeed 0
	MouseGetPos xPos, yPos
	Click
	Click 100, 500
	MouseMove xPos, yPos
	MouseMove, 54, 0, 0, R
}
return

; For dropping clicked item from inventory for whole 3 inventory
Numpad2::
SetDefaultMouseSpeed 0
MouseGetPos xPos, yPos
Loop 3 {
	MouseMove xPos, yPos+56*(a_index-1)
	Loop 9 {
		MouseGetPos xPosTwo, yPosTwo
		Click
		Click 100, 500
		MouseMove xPosTwo, yPosTwo
		MouseMove, 54, 0, 0, R
	}
}
return

; For adding mining array items into miners
; Mining Pipes x9
; OV-Scanner x9
; Diamond Drill x9
; Lapotron x9
; Order is:
; Make sure you hover your cursor over the Lapotron crystal before activating macro
Numpad3::
SetDefaultMouseSpeed 0
MouseGetPos xPos, yPos
Click
Click 650, 350	; Mining Pipe miner point
MouseMove xPos, yPos+56*1
Click
Click 750, 350	; OV-Scanner miner point
MouseMove xPos, yPos+56*2
Click
Click 550, 350	; Drill miner point
MouseMove xPos, yPos+56*3
Click
Click 650, 450	; Lapotron miner point
MouseMove xPos, yPos
Send {Escape}
return
