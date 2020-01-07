/* Create Fractal program */
#include<stdio.h>
#include<vga.h>

int i_mul(float input_x1, float input_i1, float input_x2, float input_i2);
float gridx(float screenx);
float gridy(float screeny);
void mandelbrot();
void julia();
void scan_input();

int xdimention=320, ydimention=200;
int screenx, screeny;
int escapenum;
int run, d; /* d is just a dummy variable */
float output_x, output_i;
float distance;

float maxx=2, minx=-2, maxy=2, miny=-2;
float pointx[10], pointi[10];
int iterations=32, bailout=3;
int set;

void main()
{
	setup_page();
	printf("Choose Fractal Set\n\n1: Mandelbrot\n2: Julia\t");
	scanf("%i", &set);
	run=1;
	if(set==1)
	{
		mode(graphics13h);
		while(run==1)
		{
			mandelbrot();
			flip();
			scan_input();
		}
	}
	if(set==2)
	{
		printf("Cx: ");
		scanf("%f", &pointx[1]);
		printf("Ci: ");
		scanf("%f", &pointi[1]);
		mode(graphics13h);
		while(run==1)
		{
			julia();
			flip();
			scan_input();
		}
	}
}

int i_mul(float input_x1, float input_i1, float input_x2, float input_i2)
{
	output_x=(input_x1*input_x2)-(input_i1*input_i2);
	output_i=(input_x1*input_i2)+(input_i1*input_x2);
}

float gridx(float screenx)
{
	float gx;
	gx=((maxx-minx)/xdimention)*screenx+minx;
	return(gx);
}

float gridy(float screeny)
{
	float gy;
	gy=((maxy-miny)/ydimention)*screeny+miny;
	return(gy);
}

void mandelbrot()
{
	pointx[0]=0;	/* ZX */
	pointi[0]=0;	/* Zi */
	for(screeny=0;screeny<ydimention;screeny++)
	{
		for(screenx=0;screenx<xdimention;screenx++)
		{
			pointx[0]=0;
			pointi[0]=0;
			for(d=0;d<iterations;d++)
			{
				i_mul(pointx[0], pointi[0], pointx[0], pointi[0]);
				pointx[0]=output_x+gridx(screenx);
				pointi[0]=output_i+gridy(screeny);
				distance=sqrt(pointx[0]*pointx[0]+pointi[0]*pointi[0]);
				if(distance>bailout)
				{
					escapenum=d;
					d=iterations;
				}
			}
			if(distance<=bailout)
				page_pixel(screenx, screeny, white);
			if(distance>bailout)
				page_pixel(screenx, screeny, escapenum);
		}
	}
}

void julia()
{
	for(screeny=0;screeny<ydimention;screeny++)
	{
		for(screenx=0;screenx<xdimention;screenx++)
		{
			pointx[0]=gridx(screenx);
			pointi[0]=gridy(screeny);
			for(d=0;d<iterations;d++)
			{
				i_mul(pointx[0], pointi[0], pointx[0], pointi[0]);
				pointx[0]=output_x+pointx[1];
				pointi[0]=output_i+pointi[1];
				distance=sqrt(pointx[0]*pointx[0]+pointi[0]*pointi[0]);
				if(distance>bailout)
				{
						escapenum=d;
						d=iterations;
				}
			}
			if(distance<=bailout)
				page_pixel(screenx, screeny, white);
			if(distance>bailout)
				page_pixel(screenx, screeny, escapenum);
		}
	}
}

void scan_input()
{
	char key;
	float nmaxx, nmaxy, nminx, nminy;
	key=getch();
	if(key=='w')
	{
		nmaxy=maxy-.1*(maxy-miny);
		nminy=miny-.1*(maxy-miny);
		maxy=nmaxy;
		miny=nminy;
	}
	if(key=='s')
	{
		nmaxy=maxy+.1*(maxy-miny);
		nminy=miny+.1*(maxy-miny);
		maxy=nmaxy;
		miny=nminy;
	}
	if(key=='a')
	{
		nmaxx=maxx-.1*(maxx-minx);
		nminx=minx-.1*(maxx-minx);
		maxx=nmaxx;
		minx=nminx;
	}
	if(key=='d')
	{
		nmaxx=maxx+.1*(maxx-minx);
		nminx=minx+.1*(maxx-minx);
		maxx=nmaxx;
		minx=nminx;
	}
	if(key=='e')
	{
		maxx=maxx-((maxx-minx)*.1);
		minx=minx+((maxx-minx)*.1);
		maxy=maxy-((maxy-miny)*.1);
		miny=miny+((maxy-miny)*.1);
	}
	if(key=='q')
	{
		maxx=maxx+((maxx-minx)*.1);
		minx=minx-((maxx-minx)*.1);
		maxy=maxy+((maxy-miny)*.1);
		miny=miny-((maxy-miny)*.1);
	}
	if(key=='b')
	{
		printf("New Bailout value: ");
		scanf("%i", &bailout);
	}
	if(key=='+')
		iterations+=10;
	if(key=='-')
		iterations-=10;
	if(set==2)
	{
		if(key==0x0048)
			pointi[1]=pointi[1]+pointi[1]*.1; /* Up */
		if(key==0x0050)
			pointi[1]=pointi[1]-pointi[1]*.1; /* Down */
		if(key==0x004b)
			pointx[1]=pointx[1]-pointx[1]*.1; /* Left */
		if(key==0x004d)
			pointx[1]=pointx[1]+pointx[1]*.1; /* Right */
	}
	if(key==0x1b)
		run=0;
}
