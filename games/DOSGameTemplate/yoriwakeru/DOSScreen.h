// DOS Screen manager for DOS Games by Bryan Tremblay of Keikaku Studios
/*	This allows quick non-flicker rendering for DOS games	*/
// I don't like how the cursor will flicker on screen wherever the cursor left off
// V1.1
#ifndef __DOSSCREEN__
#define __DOSSCREEN__

#include <windows.h>

#define DOS_SCREEN_WIDTH	80
#define DOS_SCREEN_HEIGHT	25

typedef enum {
	AXIS_ALIGNMENT_NORMAL,
	AXIS_ALIGNMENT_INVERTED
}AXIS_ALIGNMENTS;

typedef enum {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY,
	DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
}COLORS;

COORD COORDWith(int x, int y) {
	COORD retVal;
	retVal.X = x;
	retVal.Y = y;
	return retVal;
}

class DOSScreen {
public:
	DOSScreen(int width, int height) :
		width(width), height(height) {
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		buffer = new CHAR_INFO[resolution()];
		yAxisAlignment = AXIS_ALIGNMENT_INVERTED;
		foregroundColor = LIGHTGRAY;
		backgroundColor = BLACK;

		dwBufferSize.X = width;dwBufferSize.Y = height;
		dwBufferCoord.X = 0;dwBufferCoord.Y = 0;
		rcRegion.Left = 0;rcRegion.Top = 0;rcRegion.Right = width - 1;rcRegion.Bottom = height - 1;
		SetConsoleWindowInfo(hOut, true, &rcRegion);
	}
	bool existsOnScreen(int x, int y) {if(x >= 0 && x <= width && y >= 0 && y <= height)return true;return false;}
	void set(int x, int y, char icon) {set(x, y, icon, foregroundColor, backgroundColor);}
	void set(int x, int y, char icon, int color) {set(x, y, icon, color, backgroundColor);}
	void set(int x, int y, char icon, int frontColor, int backColor) {
		int actualY;
		if(yAxisAlignment == AXIS_ALIGNMENT_NORMAL)actualY = (height-1) - y;
		if(yAxisAlignment == AXIS_ALIGNMENT_INVERTED)actualY = y;
		if(existsOnScreen(x, y))set(actualY * width + x, icon, frontColor, backColor);
	}
	void clear() {clear(' ', foregroundColor);}
	void clear(char icon, int color) {for(int d = 0;d < resolution();d++)set(d, icon, color, backgroundColor);}
	void swapBuffers() {WriteConsoleOutput(hOut, buffer, dwBufferSize, dwBufferCoord, &rcRegion);}
	COORD dwBufferSize, dwBufferCoord;
	SMALL_RECT rcRegion;
	int resolution() {return width * height;}
	void gotoxy(int x, int y) {SetConsoleCursorPosition(hOut, (COORD){x, y});}
	HANDLE hOut;
	int width, height;
	int yAxisAlignment;
	CHAR_INFO* buffer;
	int backgroundColor, foregroundColor;
	WORD background, foreground;

private:
	void set(int index, char icon, int frontColor, int backColor) {
		buffer[index].Char.AsciiChar = icon;
		buffer[index].Attributes = frontColor | backColor<<4;
	}
};

#endif
