#include<stdio.h>
#include<math.h>
#include<vga.h>

void initpicture(void);
int rnd(int range);
void seed(void);
void vgamandel(void);
int gridx(float gx);
int gridy(float gy);
void input(void);

float x, y, maxx=1, minx=-2, maxy=1, miny=-1, zx, zy, q, distance, percent=0, tgx, tgy, cx, cy, nx, ny;
int r, g, b, depth, depth2=32, xsize, ysize, d, pillow, dd, button, cr, cg, cb, color, ask, xval[2], yval[2];
int draw=0;
char filename[20];
int bar=0;
FILE *myfile;

void main()
{
	seed();
	printf("1: Generic Mandelbrot set\n2: Specific Coordinates\n");
	scanf("%i", &ask);
	if(ask==2)
	{
		printf("MaxX: ");
		scanf("%f", &maxx);
		printf("MinX: ");
		scanf("%f", &minx);
		printf("MaxY: ");
		scanf("%f", &maxy);
		printf("MinY: ");
		scanf("%f", &miny);
	}
	vgamandel();
	printf("\t\t\t--- Bitmap information ---\n");
	printf("xsize: ");
	scanf("%i", &xsize);
	printf("ysize: ");
	scanf("%i", &ysize);
	if(xsize==10)		/* Stupid 0D 0A problem I can't fix... */
		xsize=11;
	if(ysize==10)
		ysize=11;
	while(dd<xsize) /* This whole process finds how many times that you need to add 0x00 so that the line ends in a dword */
		dd+=4;
	dd-=4;
	pillow=xsize-dd;
	if(pillow==4)
		pillow=0;
	printf("Iterations: ");
	scanf("%i", &depth);
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
	initpicture();
	clrscr();
	printf("ESC-Quit\tENT-Percent Complete\nRendering image, please wait...\n");
	for(y=ysize;y>0;y--)			/* This way it will be generated the way a bitmap likes it, upside-down */
	{
		for(x=0;x<xsize;x++)
		{
			zx=0;zy=0;
			for(d=0;d<depth;d++)
			{
				q=zx*zx-zy*zy+((((maxx-minx)/xsize)*x)+minx);
				zy=zy*zx+zx*zy+((((maxy-miny)/ysize)*y)+miny);
				zx=q;
				distance=abs(sqrt(zx*zx+zy*zy));
				if(distance>3)
				{
					if(d==10)
						d=11;
					r=d*cr;
					g=d*cg;
					b=d*cb;
					fprintf(myfile, "%c", b); /* BGR values and 0 */
					fprintf(myfile, "%c", g);
					fprintf(myfile, "%c", r);
					d=depth;
				}
			}
			if(distance<=3)
			{
				r=255;g=255;b=255;
				fprintf(myfile, "%c", b); /* BGR values and 0 */
				fprintf(myfile, "%c", g);
				fprintf(myfile, "%c", r);
			}
		}
		percent+=100.1/ysize; /* I'm not sure what the problem with this is... but 100.1 is required */
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
			printf("%c", 0xB1);
		for(d=0;d<50-percent/2;d++)
			printf(" ");
		printf("]");
	}
	printf("\nFinished, Thank-You!");
	getch();
}

void initpicture()
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
	
/* random 0D is added here if next byte=0x0A, I don't know why..., umm maybe because dec10 = 0Dx0A in character! */
	fprintf(myfile, "%c", xval[0]); /* Image Width - 4bytes long */ /* I'm sooo dumb, if the value in a character is like 10, then its enterline duh! */
	fprintf(myfile, "%c", xval[1]); /* 65535 mark, or so I think... */ /* I just need to find a way to get around that! How stupid am I!!! */
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

void vgamandel()
{
	mode(graphics13h);
	while(draw==0)
	{
		for(y=0;y<200;y++)
		{	
			for(x=0;x<320;x++)
			{
				zx=0;
				zy=0;
				gridx(x);
				gridy(y);
				cx=tgx;
				cy=tgy;
				for(d=0;d<depth2;d++)
				{
					nx=zx*zx-zy*zy+cx;
					ny=zy*zx+zx*zy+cy;
					zx=nx;
					zy=ny;
					distance=sqrt(zx*zx+zy*zy);
					if(distance>3)
						d=depth2;
					color++;
				}
				if(color==depth2)
					color=15;
				pixel(x, y, color);
				color=0;
			}
		}
		input();
	}
}

int gridx(float gx)
{
	gx=((maxx-minx)/320)*gx+minx;
	tgx=gx;
}

int gridy(float gy)
{
	gy=((maxy-miny)/200)*gy+miny;
	tgy=gy;
}

void input()
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
	if(key==0x0048)
	{
		nmaxy=maxy-.02*(maxy-miny);
		nminy=miny-.02*(maxy-miny);
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
	if(key==0x0050)
	{
		nmaxy=maxy+.02*(maxy-miny);
		nminy=miny+.02*(maxy-miny);
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
	if(key==0x004b)
	{
		nmaxx=maxx-.02*(maxx-minx);
		nminx=minx-.02*(maxx-minx);
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
	if(key==0x004d)
	{
		nmaxx=maxx+.02*(maxx-minx);
		nminx=minx+.02*(maxx-minx);
		maxx=nmaxx;
		minx=nminx;
	}
	if(key==0x0048)
		cy=cy+cy*.1;
	if(key==0x0050)
		cy=cy-cy*.1;
	if(key==0x004b)
		cx=cx-cx*.1;
	if(key==0x004d)
		cx=cx+cx*.1;
	if(key==0x0d)
	{
		mode(text);
		draw=1;
	}
	if(key=='+')
		depth2=depth2+10;
	if(key=='-')
		depth2=depth2-10;
	if(key=='=')
		depth2=depth2+1;
	if(key=='_')
		depth2=depth2-1;
	if(key=='e')
	{
		maxx=maxx-((maxx-minx)*.1);
		minx=minx+((maxx-minx)*.1);
		maxy=maxy-((maxy-miny)*.1);
		miny=miny+((maxy-miny)*.1);
	}
	if(key==0x0049)
	{
		maxx=maxx-((maxx-minx)*.02);
		minx=minx+((maxx-minx)*.02);
		maxy=maxy-((maxy-miny)*.02);
		miny=miny+((maxy-miny)*.02);
	}
	if(key=='q')
	{
		maxx=maxx+((maxx-minx)*.1);
		minx=minx-((maxx-minx)*.1);
		maxy=maxy+((maxy-miny)*.1);
		miny=miny-((maxy-miny)*.1);
	}
	if(key==0x0051)
	{
		maxx=maxx+((maxx-minx)*.02);
		minx=minx-((maxx-minx)*.02);
		maxy=maxy+((maxy-miny)*.02);
		miny=miny-((maxy-miny)*.02);
	}
	if(key==0x1b)
		exit(1);
}
