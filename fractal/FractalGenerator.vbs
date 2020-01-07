' VBS_FractalGenerator by Bryan Tremblay 22Sep2016@1415MST v1.0
' Lots of things I want to do with this are missing because VBScript
' doesn't fully support everything I'm used to in OOP. I very much so
' dislike how I pick between Mandelbrot and Julia. Passing julia_k is
' not the proper way of doing it but I think I'm going to leave the
' project here for now. It was a fun few hours of work I guess.

' [TODO] Find the correct aspect ratio of ASCII rather than guess

' Because VBScript doesn't support static class functions I have to hack around it with a pretend-singleton instance
set complex_global = (new complex)

fractal_instance = new Fractal
call fractal_instance.calculate(1, (new complex)(0, -1.01))
fractal_instance.render()

class Fractal
	public default function init()
		colors = "█▓▒░≈≡=-~:. "
		bounds = 3
		iterations = 32
		width = 100
		height = cint(width * (8 / 12))		' Keep aspect ratio 1:1 since ASCII isn't perfectly square
		set topLeft = (new complex)(-2, 2)
		set bottomRight = (new complex)(2, -2)
		set init = me
	end function

	function render()
		set objFile = CreateObject("Scripting.FileSystemObject").CreateTextFile("output.txt", true, true)
		for l = 0 to UBound(lines) step 1
			objFile.Write lines(l) & vbcrlf
		next
		objFile.Write calculationPeriod & " seconds to calculate " & width & "x" & height
		objFile.Close()
	end function

	' 0: Mandelbrot, 1: Julia
	' If not calculating julia set then julia_k is ignored
	function calculate(fractalType, julia_k)
		startTime = timer
		xDelta = (bottomRight.r - topLeft.r) / width
		yDelta = (bottomRight.i - topLeft.i) / height

		redim lines(height)
		for y = 0 to (height - 1) step 1
			currentLine = ""
			for x = 0 to (width - 1) step 1
				if fractalType = 0 then
					escapeVal = calculateEscapeVal_mandelbrot(x, y)
				end if
				if fractalType = 1 then
					escapeVal = calculateEscapeVal_julia(x, y, julia_k)
				end if
				index = escapeVal mod len(colors) + 1
				color = mid(colors, index, 1)
				currentLine = currentLine & color
			next
			lines(y) = currentLine
		next
		generateFractal = lines
		calculationPeriod = timer - startTime
	end function

	function calculateEscapeVal_mandelbrot(x, y)
		r = x * xDelta + topLeft.r
		i = y * yDelta + topLeft.i
		set z = (new complex)(0, 0)
		set c = (new complex)(r, i)

		iteration = 0
		for i = 0 to (iterations - 1) step 1
			iteration = i
			set z = complex_global.square(z)
			set z = complex_global.add(z, c)
			if (z.distance > bounds) then
				exit for
			end if

			if i = (iterations - 1) then
				iteration = 0
			end if
		next
		calculateEscapeVal_mandelbrot = iteration
	end function

	function calculateEscapeVal_julia(x, y, k)
		r = x * xDelta + topLeft.r
		i = y * yDelta + topLeft.i
		set z = (new complex)(r, i)
		set c = (new complex)(k.r, k.i)

		iteration = 0
		for i = 0 to (iterations - 1) step 1
			iteration = i
			set z = complex_global.square(z)
			set z = complex_global.add(z, c)
			if (z.distance > bounds) then
				exit for
			end if

			if i = (iterations - 1) then
				iteration = 0
			end if
		next
		calculateEscapeVal_julia = iteration
	end function

	dim bounds, iterations
	dim topLeft, bottomRight
	dim width, height
	dim xDelta, yDelta
	dim colors, lines
	dim calculationPeriod
end class

class complex
	public default function init(r, i)
		me.r = r
		me.i = i
		set init = me
	end function

	function copy()
		set retVal = (new complex)(r, i)
		set copy = retVal
	end function

	function add(a, b)
		set retVal = (new complex)(a.r + b.r, a.i + b.i)
		set add = retVal
	end function

	function subtract(a, b)
		set retVal = (new complex)(a.r - b.r, a.i - b.i)
		set subtract = retVal
	end function

	function square(a)
		new_r = (a.r * a.r) - (a.i * a.i)
		new_i = (a.r * a.i) * 2
		set retVal = (new complex)(new_r, new_i)
		set square = retVal
	end function

	function log()
		sign = conditional_if(i < 0, "", "+")
		wscript.echo r & sign & i & "i"
	end function

	function distance
		distance = sqr((r * r) + (i * i))
	end function

	dim r, i
end class

'''''VBScript Helpers'''''

function conditional_if(bClause, sTrue, sFalse)
	if CBool(bClause) then
		conditional_if = sTrue
	else
		conditional_if = sFalse
	end if
end function