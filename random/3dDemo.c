// Intended to be compiled with Borland's Turbo C Compiler
// Note that this will not be possible on 64 bit systems
/*Bryan's 3D Program*/
/* V1.0 */
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<vga.h>

void rot_trans(void);
int rot3d(float x, float y, float z, float tx, float ty, float tz);
void plot(void);	/* As of now, plot has no polygons, or individual objects, only plots all points in np[] */
void init();
int point(int ix, int iy, int iz, int pnum);
void mouse();

float c[]={0, 0, 2, 0, 0, 0};	/*Camera (X, Y, Z, RotationX, RY, RZ) */
float d, l=800;	/* D for distance, L for lens */	/* I'll mess with the l=256 later */
int ccc;
int xdir=0, ydir=0; /* 0=none, 1=left(or up), 2=right(or down) */
int button;

float px[1000]={-5, 5}, py[1000]={0, 0}, pz[1000]={0, 0}, npx[1000], npy[1000], npz[1000];
int num=2;

float z1=0, z2=0, z3=0, z4=0;
float tx, ty, tz=0;
int xx=0, yy=0;
int x=0, y=0, ox=0, oy=0;

union REGS i, o;

void main()
{
	setup_page();
	mode(graphics13h);		/* Enter VGA 13H Mode */
	init();
	while(!kbhit())
	{
		rot_trans();
		plot();
		flip();
		mouse();
		if(button==1)
		{
			if(xdir==0)
				c[4]=c[4];
			if(xdir==1)
				c[4]=c[4]-.025;
			if(xdir==2)
				c[4]=c[4]+.025;
		}
		if(button==2)
		{
			if(ydir==0)
				c[3]=c[3];
			if(ydir==1)
				c[3]=c[3]-.025;
			if(ydir==2)
				c[3]=c[3]+.025;
		}
		page_clear(black);
	}
}

void init()
{
	tx=-15;ty=0;
	point(-5+tx, -5+ty, -5+z1, 0);
	point(5+tx, -5+ty, -5+z1, 1);
	point(-5+tx, 5+ty, -5+z1, 2);
	point(5+tx, 5+ty, -5+z1, 3);
	point(-5+tx, -5+ty, 5+z1, 4);
	point(5+tx, -5+ty, 5+z1, 5);
	point(-5+tx, 5+ty, 5+z1, 6);
	point(5+tx, 5+ty, 5+z1, 7);

	tx=15;ty=0;
	point(-5+tx, -5+ty, -5+z2, 8);
	point(5+tx, -5+ty, -5+z2, 9);
	point(-5+tx, 5+ty, -5+z2, 10);
	point(5+tx, 5+ty, -5+z2, 11);
	point(-5+tx, -5+ty, 5+z2, 12);
	point(5+tx, -5+ty, 5+z2, 13);
	point(-5+tx, 5+ty, 5+z2, 14);
	point(5+tx, 5+ty, 5+z2, 15);

	tx=0;ty=-15;
	point(-5+tx, -5+ty, -5+z3, 16);
	point(5+tx, -5+ty, -5+z3, 17);
	point(-5+tx, 5+ty, -5+z3, 18);
	point(5+tx, 5+ty, -5+z3, 19);
	point(-5+tx, -5+ty, 5+z3, 20);
	point(5+tx, -5+ty, 5+z3, 21);
	point(-5+tx, 5+ty, 5+z3, 22);
	point(5+tx, 5+ty, 5+z3, 23);

	tx=0;ty=15;
	point(-5+tx, -5+ty, -5+z4, 24);
	point(5+tx, -5+ty, -5+z4, 25);
	point(-5+tx, 5+ty, -5+z4, 26);
	point(5+tx, 5+ty, -5+z4, 27);
	point(-5+tx, -5+ty, 5+z4, 28);
	point(5+tx, -5+ty, 5+z4, 29);
	point(-5+tx, 5+ty, 5+z4, 30);
	point(5+tx, 5+ty, 5+z4, 31);
	
	tx=0;ty=0;tz=15;
	point(-5+tx, -5+ty, -5+tz, 32);
	point(5+tx, -5+ty, -5+tz, 33);
	point(-5+tx, 5+ty, -5+tz, 34);
	point(5+tx, 5+ty, -5+tz, 35);
	point(-5+tx, -5+ty, 5+tz, 36);
	point(5+tx, -5+ty, 5+tz, 37);
	point(-5+tx, 5+ty, 5+tz, 38);
	point(5+tx, 5+ty, 5+tz, 39);

	tx=0;ty=0;tz=-15;
	point(-5+tx, -5+ty, -5+tz, 40);
	point(5+tx, -5+ty, -5+tz, 41);
	point(-5+tx, 5+ty, -5+tz, 42);
	point(5+tx, 5+ty, -5+tz, 43);
	point(-5+tx, -5+ty, 5+tz, 44);
	point(5+tx, -5+ty, 5+tz, 45);
	point(-5+tx, 5+ty, 5+tz, 46);
	point(5+tx, 5+ty, 5+tz, 47);

	num=48;
}

int point(int ix, int iy, int iz, int pnum)
{
	px[pnum]=ix;
	py[pnum]=iy;
	pz[pnum]=iz;
}

void rot_trans()
{
	int cc;
	for(cc=0;cc<num;cc++)	/* This will copy 'num' amount of points from p[] to np[], or newpoints */
	{
		npx[cc]=px[cc];	/* The 3*cc is because when cc=1 you will start at the third offset in p[] */
		npy[cc]=py[cc];
		npz[cc]=pz[cc];
	}

	for(ccc=0;ccc<num;ccc++)	/* This is to rotate 'num' amount of points according to camera */
		rot3d(npx[ccc], npy[ccc], npz[ccc], c[3], c[4], c[5]);	/*Rotate np[x][y][z]by tx ty and tz */

	for(cc=0;cc<num;cc++)	/* Now to translate 'num' points according to camera */
	{
		npx[cc]=npx[cc]-c[0];		/* NewpointX=OldPointX-CameraX */
		npy[cc]=npy[cc]-c[1];	/* And for Y */
		npy[cc]=npy[cc]-c[2];	/* Finally for Z */
	}
}

int rot3d(float x, float y, float z, float tx, float ty, float tz)
{
	float keep;	/* This is a buffer for rotation */

	keep=y*cos(tx)-z*sin(tx);
	z=z*cos(tx)+y*sin(tx);
	y=keep;
	keep=z*cos(ty)-x*sin(ty);
	x=x*cos(ty)+z*sin(ty);
	z=keep;				/* Final Z */
	keep=x*cos(tz)-y*sin(tz);
	y=y*cos(tz)+x*sin(tz);		/* Final Y */
	x=keep;				/* Final X */
	npx[ccc]=x;
	npy[ccc]=y;
	npz[ccc]=z;
}

void plot()
{
	int cc;
	float d1, x1, y1, d2, x2, y2;
	for(cc=0;cc<num;cc++)	/* Draws 'num' amount of pixels */
	{
			d=(c[2]-256)-npz[cc];
			/* c[2]-256 because it was the initial cz distance, now it is added on */
			page_pixel(((l*npx[cc])/d)+160, 100-((l*npy[cc])/d), white);
	}
}

void mouse()
{
	i.x.ax=3;				/* Read mouse function */
	int86(0x33, &i, &o);
	x=o.x.cx;
	y=o.x.dx;
	if(x-ox==0)
		xdir=0; /* No move */
	if(x-ox<0)
		xdir=1; /* Move left */
	if(x-ox>0)
		xdir=2; /* Move right */
	if(y-oy==0)
		ydir=0;
	if(y-oy<0)
		ydir=1;
	if(y-oy>0)
		ydir=2;
	if(o.x.cx==639)
	{
		i.x.ax=4;
	    i.x.cx=1;
	    i.x.dx=y;
	    int86(0x33,&i,&o);
	}
	if(o.x.cx==0)
	{
		i.x.ax=4;
	    i.x.cx=638;
	    i.x.dx=y;
	    int86(0x33,&i,&o);
	}
	if(o.x.dx==199)
	{
		i.x.ax=4;
		i.x.cx=x;
		i.x.dx=1;
		int86(0x33,&i,&o);
	}
	if(o.x.dx==0)
	{
		i.x.ax=4;
		i.x.cx=x;
		i.x.dx=198;
		int86(0x33,&i,&o);
	}
	button=o.x.bx;			/* 1=Left, 2=Right, 3=Both */
	ox=x;
	oy=y;
}
