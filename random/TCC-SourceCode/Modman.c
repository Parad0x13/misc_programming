/* Modulus Mandelbrot, renormalized equation for mandelbrot shading algorithm */
#include<stdio.h>
#include<math.h>

#define ESC 0x1B

void iterate_mandelbrot(void);
void initbitmap(void);
int rnd(int range);
void seed(void);

int depth=32, bailout=3, escapenum=0;
float minX=-2, maxX=1, minI=-1, maxI=1;
float distance=0;
float x, i;
float mu;		/* Renormalized Count */

/* Image variables */
int xdim=100, idim=100, xval[2], yval[2];
char filename[10]={"Image.bmp"};
FILE *myfile;

void main()
{
	seed();
	printf("Welcome to Modulus Mandelbrot V1.0, Press any key to continue...");
	getch();

	xval[0]=xdim;
	xval[1]=xdim/256;
	yval[0]=idim;
	yval[1]=idim/256;
	initbitmap();
	iterate_mandelbrot();
	printf("\n\nFinished, thank you! Press any key to exit...");
	getch();
	fclose(myfile);
	exit(0);
}

void iterate_mandelbrot()
{
	float zx=0, zi=0;		/* They allways start off as 0, initial iteration */
	float nzx=0, nzi=0;		/* Every thing to 0, for good measure */
	float gridx, gridi;
	int escape=0, prisoner=0;	/* Has not escaped yet */
	int r, g, b;
	int pillow, dd=0;
int num;
	printf("\n\nIterating Mandelbrot equations...");

	while(dd<xdim) /* This whole process finds how many times that you need to add 0x00 so that the line ends in a dword */
		dd+=4;
	dd-=4;
	pillow=xdim-dd;
	if(pillow==4)
		pillow=0;

	for(i=idim;i>0;i--)
	{
		gridi=((maxI-minI)/idim)*i+minI;
		for(x=0;x<xdim;x++)
		{
			gridx=((maxX-minX)/xdim)*x+minX;
			zx=0;zi=0;nzx=0;nzi=0;escape=0;prisoner=0;	/* Need to reset all these values */
			while(escape==0&&prisoner==0)
			{
				nzx=zx*zx-zi*zi+gridx;
				nzi=zx*zi+zi*zx+gridi;
				zx=nzx;
				zi=nzi;
				distance=sqrt(zx*zx+zi*zi);
				if(distance>bailout)
				{
					mu=bailout+1-log(log(distance))/log(2);
					r=234;
					g=230;
					b=156;
					printf("B: %i ", b);
					fputc(256/mu, myfile);
					fputc(1024/mu, myfile);
					fputc(2048/mu, myfile);
					for(dd=0;dd<pillow;dd++)
						fprintf(myfile, "%c", 0x00); /* Padding, line must end on dword */
					escape=1;
				}
				if(escapenum==depth)
				{
					r=0;
					g=0;
					b=0;
					fputc(b, myfile);
					fputc(g, myfile);
					fputc(r, myfile);
					for(dd=0;dd<pillow;dd++)
						fprintf(myfile, "%c", 0x00); /* Padding, line must end on dword */
					prisoner=1;
				}
				escapenum++;		/* Increment the iteration counter */
			}
		}
	}
	escapenum=0;					/* Need to reset it */
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
	
	fputc((int)xval[0], myfile);
	fputc((int)xval[1], myfile);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);

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
