/* Keikaku3d V2.0
   Bryan Tremblay */

#include<stdio.h>
#include<math.h>
#include<vga.h>

void init(void);
void scan(void);

/* 'num' is how many meshes there are in total */
int num=1;

/* Stores X, Y, Z and ScreenX, ScreenY */
struct vector
{
	int x, y, z;
	int sx, sy;
};

struct vector triangle[3];

void main()
{
	init();
	scan();
}

void init()
{
	/* Sets up first mesh */
	triangle = {
	{0, -1, 0, 0, 0},
	{-1, 0, 0, 0, 0},
	{1, 0, 0, 0, 0}
	};
}

void scan()
{
	int key;
	key=getch();
	switch(key)
	{
		case 0x1b:
			exit(1);
			break;
		default:
			printf("\nNot an option");
			getch();
			break;
	}
}