#include<graphics.h>

int bx=100,by=100,fx=100,fy=100;

void box()
{
	setcolor(WHITE);
	rectangle(bx,by,bx+fx,by+fy);
}

void erasebox()
{
	setcolor(BLACK);
	rectangle(bx,by,bx+fx,by+fy);
}