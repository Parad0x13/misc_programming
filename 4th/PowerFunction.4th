(My first function in Forth, a power function)
(I'm not sure this is very elegant... But it uses the stack only)

: pow ( n1 n2 -- n1^n2 ) 1 swap 0 do over * loop swap drop ;
