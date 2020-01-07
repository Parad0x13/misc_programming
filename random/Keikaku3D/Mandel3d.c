/* Programmed by Bryan Tremblay Keikaku Studios */
#include<dos.h>
#include<math.h>
#include<stdio.h>

#include<vga.h>

#define	esc	0x1b

int depth = 32;
int x, y, d;
float zx=0, zy=0, nx, ny, absolute=0;
float minX=-2, minY=-1, maxX=1, maxY=1;	
int maxx=38, maxy=25, choose;

void main()
{
	FILE *myfile;
	myfile = fopen("mandel.k3d", "w");

	printf("1: Generic Mandelbrot\n2: Own coordinates\n:");
	scanf("%i", &choose);
	printf("Iterations: ");
	scanf("%i", &depth);
	if(choose==1)
	{
		printf("\n\nOk then! Enjoy:)");
		getch();
	}
	if(choose==2)
	{
		printf("\n\nMinX: ");
		scanf("%f", &minX);
		printf("\nMaxX: ");
		scanf("%f", &maxX);
		printf("\nMinY: ");
		scanf("%f", &minY);
		printf("\nMaxY: ");
		scanf("%f", &maxY);
		printf("\n\nOk then! Enjoy:)");
		getch();
	}
	if(choose!=1&&choose!=2)
	{
		printf("\n\nNot an option sorry:(");
		getch();
		exit(1);
	}
	mode(graphics13h);
	for(y=0;y<maxy;y++)
	{
		for(x=0;x<maxx;x++)
		{
			for(d=0;d<depth;d++)
				math();

			if(absolute <= 2)
			{
				fprintf(myfile, "%i", x*2);
				fprintf(myfile, "%s", "\n");
				fprintf(myfile, "%i", y*2);
				fprintf(myfile, "%s", "\n");
				fprintf(myfile, "%i", 0);
				fprintf(myfile, "%s", "\n");
				fprintf(myfile, "%i", 0);
				fprintf(myfile, "%s", "\n");
				fprintf(myfile, "%s", "\n");
				pixel(x, y, white);
			}
			if(absolute > 2)
			{
				pixel(x, y, black);
			}

			zx = 0;
			zy = 0;
		}
	}
	fprintf(myfile, "%i", 9);
	fprintf(myfile, "%s", "\n");
	fprintf(myfile, "%i", 9);
	fprintf(myfile, "%s", "\n");
	fprintf(myfile, "%i", 9);
	fprintf(myfile, "%s", "\n");
	fprintf(myfile, "%i", 9);

	fclose(myfile);
	getch();
}

int math()
{
	nx = zx*zx-zy*zy+(x*((maxX-minX)/maxx)+minX);
	ny = zx*zy+zy*zx+(y*((maxY-minY)/maxy)+minY);
	zx = nx;
	zy = ny;
	absolute = sqrt(zx*zx+zy*zy);
	if(absolute > 2)
		d = depth;
}