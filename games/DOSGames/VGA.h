/* 13h graphics mode headerfile by Bryan Tremblay */
/* No polygon support as of yet, nor circles */
/* VGA1.3 Supports 2 pages, VGA and PAGE, you have to compile as a hudge model if you initialize 'setup_page()' */
/* VGA1.3 Has been totally revamped! I'm lovin' it! */

/* I still need to fix the line backwardsdraw problem */

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

void setup_page(void);
byte mode(byte mode);
int pixel(int x, int y, byte color);
int page_pixel(int x,int y,byte color);
int line(int x1, int y1, int x2, int y2, byte color);
page_line(int x1, int y1, int x2, int y2, byte color);
int rectangle(int x, int y, int xs, int ys, byte color);
int page_rectangle(int x, int y, int xs, int ys, byte color);
byte clear(byte color);
byte page_clear(byte color);
void flip(void);

byte far *page;

void setup_page()
{
	page=(byte far*)malloc(65000);
}

byte mode(byte mode)
{
	union REGS regs;
	regs.h.ah = 0x00;
	regs.h.al = mode;
	int86(0x10, &regs, &regs);
}

int pixel(int x,int y,byte color)
{
	VGA[(y<<6)+(y<<8)+x]=color;
}

int page_pixel(int x,int y,byte color)
{
	page[(y<<6)+(y<<8)+x]=color;
}

int line(int x1, int y1, int x2, int y2, byte color)	/*Not Bresenham's algorithm, so it will be slow*/
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

int page_line(int x1, int y1, int x2, int y2, byte color)	/*Not Bresenham's algorithm, so it will be slow*/
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
			page_pixel(x, y, color);
		}
	}
	else				/*If Vertical > Horizontal*/
	{
		slope=(float)dx/(float)dy;
		for(counter=0;counter<dy;counter++)
		{
			x=slope*counter+x1;
			y=y1+counter;
			page_pixel(x, y, color);
		}
	}	
}

int rectangle(int x, int y, int xs, int ys, byte color)
{
	line(x, y, x+xs, y, color);
	line(x, y, x, y+ys, color);
	line(x, y+ys, x+xs, y+ys, color);
	line(x+xs, y, x+xs, y+ys, color);
	pixel(x+xs, y+ys, color);
}

int page_rectangle(int x, int y, int xs, int ys, byte color)
{
	page_line(x, y, x+xs, y, color);
	page_line(x, y, x, y+ys, color);
	page_line(x, y+ys, x+xs, y+ys, color);
	page_line(x+xs, y, x+xs, y+ys, color);
	page_pixel(x+xs, y+ys, color);
}

byte clear(byte color)
{
	int x, y;
	for(y=0;y<200;y++)
	{
		for(x=0;x<320;x++)
			VGA[y*320+x]=color;
	}
}

byte page_clear(byte color)
{
	int x, y;
	for(y=0;y<200;y++)
	{
		for(x=0;x<320;x++)
			page[y*320+x]=color;
	}
}

void flip()
{
	int x, y;
	for(y=0;y<200;y++)
	{
		for(x=0;x<320;x++)
			VGA[y*320+x]=page[y*320+x];
	}
}
