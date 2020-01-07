/* This is BestMandelbrot V1.6 made so that it uses NO VGA!!! ... Dang I hate vista */
#include<dos.h>
#include<math.h>
#include<stdio.h>
#include<string.h>

#define	esc	0x1b

int rnd(int range);
void seed(void);
void image_bitmap(void);
void initbitmap(void);

/* Stuff for bitmap image */
FILE *myfile;
int xval[2], yval[2];

int depth=32, screenx, screeny, run, color, colorchange=0, bailout=3, escapenum;
double d;
int dsmalljulia; /* DrawSmallJulia */
double zx=0, zi=0, nx, ni, distance=0;
double pointx[10], pointi[10];
double minx=-2.5, miny=-1.5, maxx=1.5, maxy=1.5;
double dminx=0, dminy=0, dmaxx=0, dmaxy=0, dbailout=0, ddepth=0;/* Differance MaxMinXY and iterations and stuff */
int xdimention=320, ydimention=200, draw, button, mx, my, protect=0, colorotate=0; /* mx: mousex, my: mousey */
double output_x, output_i;
union REGS i, o;
int set;
int VGA_increment=0;
char filename[10];
char filetype[10]={".bmp"};
char increment[10];
char prefix[10]={"BM"};

float sjulia[50][80]; /* Small Julia array, 80*50 */

/* Stuff for savepos() and loadpos() */
double sminx[10], smaxx[10], sminy[10], smaxy[10];

/* Stuff for undo and redo */
double urminx[2], urmaxx[2], urminy[2], urmaxy[2]; /* undo-redo */

int main()
{
	run=1;draw=1;
	minx=-2.5;miny=-1.5;maxx=1.5;maxy=1.5;
	dsmalljulia=1;
	set=1;

	seed();

	printf("1: Mandelbrot fractal\n2: Julia fractal\nESC: Exit\n\n");
	scan();
	image_bitmap();
}

int scan()
{
	int key;
	key=getch();
	if(key=='1')
		set=1;
	if(key=='2')
		set=2;
	if(key==esc)
		exit(0);
}

int coords()
{
	printf("\nMaxX: %.15f\nMaxY: %.15f\nMinX: %.15f\nMinY: %.15f", maxx, maxy, minx, miny);
	getch();
}

void image_bitmap()
{
	int xsize, ysize, dd=0, pillow, x, y, iterations;
	int r=0, g=0, b=0;
	int cr, cg, cb;
	float percent=0;
	int graphicstype;
	gotoxy(0, 0);

	printf("Set=%i\n\n", set);
	
	if(set==1)
		printf("Generating Mandelbrot Set\n\n");
	if(set==2)
		printf("Generating Julia Set\n\n");
	graphicstype=2;

	if(graphicstype==2)
	{
		printf("\t\t\t--- Bitmap information ---\n");
		printf("xsize: ");
		scanf("%i", &xsize);
		printf("ysize: ");
		scanf("%i", &ysize);
		while(dd<xsize) /* This whole process finds how many times that you need to add 0x00 so that the line ends in a dword */
			dd+=4;
		dd-=4;
		pillow=xsize-dd;
		if(pillow==4)
			pillow=0;
		printf("Iterations[0 for default depth(32)]: ");
		scanf("%i", &iterations);
		if(iterations==0)
			iterations=depth;
		printf("Red Color Multiplier: ");
		scanf("%i", &cr);
		printf("Green Color Multiplier: ");
		scanf("%i", &cg);
		printf("Blue Color Multiplier: ");
		scanf("%i", &cb);
		printf("File name (Don't forget '.bmp'!): ");
		scanf("%s", &filename);

		xval[0]=xsize;
		xval[1]=xsize/256;
		yval[0]=ysize;
		yval[1]=ysize/256;
		initbitmap();
		clrscr();
		printf("ESC-Quit\tENT-Percent Complete\nRendering image, please wait...\n");
		if(set==1)
		{
			for(y=ysize;y>0;y--)			/* This way it will be generated the way a bitmap likes it, upside-down */
			{
				for(x=0;x<xsize;x++)
				{
					pointx[0]=0;
					pointi[0]=0;
					for(d=0;d<iterations;d++)
					{
						output_x=(pointx[0]*pointx[0])-(pointi[0]*pointi[0]);
						output_i=(pointx[0]*pointi[0])+(pointi[0]*pointx[0]);
						pointx[0]=output_x+(((maxx-minx)/xsize)*x+minx);
						pointi[0]=output_i+(((maxy-miny)/ysize)*y+miny);

						distance=sqrt(pointx[0]*pointx[0]+pointi[0]*pointi[0]);
						if(distance>bailout)
						{
							/*r=cr*d;
							g=cg*d;
							b=cb*d;*/

							/* where i put the smoothing algorithm */

							r=d*100;
							g=d*150;
							b=d*200;
							fputc((int)b, myfile);
							fputc((int)g, myfile);
							fputc((int)r, myfile);
							d=iterations;
						}
					}
					if(distance<=bailout)
					{
						r=255;g=255;b=255;
						fputc((int)b, myfile);
						fputc((int)g, myfile);
						fputc((int)r, myfile);
					}
				}
				percent=percent+(100.1/ysize); /* I'm not sure what the problem with this is... but 100.1 is required */
				for(d=0;d<pillow;d++)
					fprintf(myfile, "%c", 0x00); /* Padding, line must end on dword */
				if(kbhit())
				{
					button=getch();
					if(button==0x1b)
					{
						fclose(myfile);
						exit(1);
					}
					if(button==0x0d)
						printf("%.1f\%\n", percent);
				}
				gotoxy(10, 4);
				printf("[");
				for(d=0;d<percent/2;d++)
					printf("%c", 0xb1);
				for(d=0;d<50-percent/2;d++)
					printf(" ");
				printf("] ");
			}
		}
	}
	if(set==2)
	{
		for(y=ysize;y>0;y--)			/* This way it will be generated the way a bitmap likes it, upside-down */
		{
				for(x=0;x<xsize;x++)
				{
					pointx[0]=((maxx-minx)/xsize)*x+minx;
					pointi[0]=((maxy-miny)/ysize)*y+miny;
					for(d=0;d<iterations;d++)
					{
						output_x=(pointx[0]*pointx[0])-(pointi[0]*pointi[0]);
						output_i=(pointx[0]*pointi[0])+(pointi[0]*pointx[0]);
						pointx[0]=output_x+pointx[1];
						pointi[0]=output_i+pointi[1];
						
						distance=sqrt(pointx[0]*pointx[0]+pointi[0]*pointi[0]);
						if(distance>bailout)
						{
							r=d*cr;
							g=d*cg;
							b=d*cb;
							fputc((int)b, myfile);
							fputc((int)g, myfile);
							fputc((int)r, myfile);
							d=iterations;
						}
					}
					if(distance<=bailout)
					{
						r=255;g=255;b=255;
						fputc((int)b, myfile);
						fputc((int)g, myfile);
						fputc((int)r, myfile);
					}
				}
				percent=percent+(100.1/ysize); /* I'm not sure what the problem with this is... but 100.1 is required */
				for(d=0;d<pillow;d++)
					fprintf(myfile, "%c", 0x00); /* Padding, line must end on dword */
				if(kbhit())
				{
					button=getch();
					if(button==0x1b)
					{
						fclose(myfile);
						exit(1);
					}
					if(button==0x0d)
						printf("%.1f\%\n", percent);
				}
				gotoxy(10, 4);
				printf("[");
				for(d=0;d<percent/2;d++)
					printf("%c", 0xb1);
				for(d=0;d<50-percent/2;d++)
					printf(" ");
				printf("] ");
		}
	}

	fclose(myfile);
	printf("\nFinished, Thank-You!");
	getch();
}

void initbitmap()
{
	myfile=fopen(filename, "w");
	fprintf(myfile, "%c", 'B'); /* Header "BM" */
	fprintf(myfile, "%c", 'M');
	
	fprintf(myfile, "%c", 0x00); /* Total filesize, test to see if all 0s will work */
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x00); /* Reserved#1 */
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x00); /* Reserved#2*/
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x36); /* Offset untill pixeldata, I'm testing with a 54d offset, or 36h */
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x28); /* Header offset, it is 40d here, 28h */
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	
	/*fprintf(myfile, "%c", xval[0]);
	fprintf(myfile, "%c", xval[1]);*/
	fputc((int)xval[0], myfile);
	fputc((int)xval[1], myfile);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);

/*	fprintf(myfile, "%c", 0xff);
	fprintf(myfile, "%c", 0xff);
	fprintf(myfile, "%c", xsize);
	fprintf(myfile, "%c", 0xff);
	fprintf(myfile, "%c", 0xff);	*/

	fprintf(myfile, "%c", yval[0]); /* Image Height - 4bytes long */
	fprintf(myfile, "%c", yval[1]);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x01); /* planes stuff, must be 1 */
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x18); /* bits per pixel, 24d but 18h */
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x00); /* Compression */
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x00); /* Image size, all 0's for uncompressed image */
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x00); /* Stupid preferred stuff */
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x00); /* Stupid preferred stuff */
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x00); /* Number Color Map entries that are actually used, testing as all 0's */
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	
	fprintf(myfile, "%c", 0x00); /* Number of significant colors, 0 means all are significant */
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);
}

int rnd(int range)
{
	int r;
	r=rand()%range;
	return(r);
}

void seed()
{
	srand((unsigned)time(NULL));
}
