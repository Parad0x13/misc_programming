/* 13h graphics mode headerfile by Bryan Tremblay */
/*No polygon support as of yet, nor circles*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

#define graphics13h		0x13
#define text		0x03
#define black		0
#define blue		1
#define green		2
#define cyan		3
#define red		4
#define magenta		5
#define brown		6
#define lightgray		7
#define darkgray		8
#define lightblue		9
#define lightgreen		10
#define lightcyan		11
#define lightred		12
#define lightmagenta	13
#define yellow		14
#define white		15

typedef unsigned char  byte;
byte far *VGA=(byte far*)0xA0000000L;

void mode(byte mode)
{
	union REGS regs;
	regs.h.ah = 0x00;
	regs.h.al = mode;
	int86(0x10, &regs, &regs);
}

void pixel(int x,int y,byte color)
{
	VGA[(y<<6)+(y<<8)+x]=color;
}

void mediumpixel(int x,int y,byte color)
{
	VGA[y*320+x]=color;
}


void slowpixel(int x, int y, byte color)
{
	union REGS regs;
	regs.h.ah = 0x0C;
	regs.h.al = color;
	regs.x.cx = x;
	regs.x.dx = y;
	int86(0x10,&regs,&regs);
}

void line(int x1, int y1, int x2, int y2, byte color)	/*Not Bresenham's algorithm, so it will be slow*/
{
	int x, y, counter, dx, dy;
	float slope;
	dx=x2-x1;
	dy=y2-y1;

	/*This is just a test*/
	if(dx==0)
		dx=1;

	if(abs(dx)>abs(dy))			/*If Horizontal > Vertical*/
	{
		slope=(float)dy/(float)dx;
		for(counter=0;counter<dx;counter++)
		{
			x=x1+counter;
			y=slope*counter+y1;
			pixel(x, y, color);
		}
	}
	else				/*If Vertical > Horizontal*/
	{
		slope=(float)dx/(float)dy;
		for(counter=0;counter<dy;counter++)
		{
			x=slope*counter+x1;
			y=y1+counter;
			pixel(x, y, color);
		}
	}	
}

void rectangle(int x, int y, int xs, int ys, byte color)
{
	line(x, y, x+xs, y, color);
	line(x, y, x, y+ys, color);
	line(x, y+ys, x+xs, y+ys, color);
	line(x+xs, y, x+xs, y+ys, color);
	pixel(x+xs, y+ys, color);
}

void clear(byte color)
{
	long count;
	for(count=0;count<64000;count++)
		VGA[count]=color;
}
