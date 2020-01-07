#include<stdio.h>
#include<math.h>

void file_read(void);
void getpillow(void);	/* Pillow is the number of bytes to append to the scanline to create a 'long' end */

char name[10];
int d, pillow=0;

FILE *image;

struct tagBITMAPFILEHEADER {    /* bmfh */
    unsigned int	bfType;
    unsigned long	bfSize;		/* Long has 4 bytes */
    unsigned int	bfReserved1;
    unsigned int	bfReserved2;
    unsigned long	bfOffBits;
} BITMAPFILEHEADER;	

struct tagBITMAPINFOHEADER {    /* bmih */
    unsigned long	biSize;
    unsigned long	biWidth;
    unsigned long	biHeight;
    unsigned int	biPlanes;
    unsigned int	biBitCount;
    unsigned long	biCompression;
    unsigned long	biSizeImage;
    unsigned long	biXPelsPerMeter;
    unsigned long	biYPelsPerMeter;
    unsigned long	biClrUsed;
    unsigned long	biClrImportant;
} BITMAPINFOHEADER;

struct tagRGBQUAD {     /* rgbq */
	unsigned char	rgbBlue;
	unsigned char	rgbGreen;
	unsigned char	rgbRed;
	unsigned char	rgbReserved;
} RGBQUAD[256];

void main()
{
	int test;
	printf("Bitmap Info Reader\n\n");
	printf("Name of Bitmap?: ");
	scanf("%s", name);
	file_read();
	printf("Info\n----\n\n");
	printf("BITMAPFILEHEADER\n");
	printf("bfType: %x\n", BITMAPFILEHEADER.bfType);
	printf("bfSize: %.0f bytes\n", (float)BITMAPFILEHEADER.bfSize);
	printf("bfOffBits, offset 'till bitmap data: %i\n\n", (int)BITMAPFILEHEADER.bfOffBits);
	printf("BITMAPINFOHEADER\n");
	printf("biSize: %i\n", (int)BITMAPINFOHEADER.biSize);
	printf("biWidth: %i\n", (int)BITMAPINFOHEADER.biWidth);
	printf("biHeight: %i\n", (int)BITMAPINFOHEADER.biHeight);
	printf("biBitCount: %i\n", (int)BITMAPINFOHEADER.biBitCount);
	printf("biCompression: %i\n", (int)BITMAPINFOHEADER.biCompression);
	printf("biSizeImage: %i\n", (int)BITMAPINFOHEADER.biSizeImage);
	printf("Forget about the Pixels-Per-Meter stuff for now\n");
	printf("biClrUsed: %i\n", (int)BITMAPINFOHEADER.biClrUsed);
	printf("biClrImportant: %i\n", (int)BITMAPINFOHEADER.biClrImportant);
	getpillow();
	printf("Press any key for array table info\n\n");
	getch();
	if((int)BITMAPINFOHEADER.biBitCount!=24)
	{
		for(d=0;d<(int)pow(2, (double)BITMAPINFOHEADER.biBitCount);d++)
		{
			printf("RGBQUAD[%i]\n", d);
			printf("Blue: %i\n", RGBQUAD[d].rgbBlue);
			printf("Green: %i\n", RGBQUAD[d].rgbGreen);
			printf("Red: %i\n\n", RGBQUAD[d].rgbRed);
		}
	}
	if((int)BITMAPINFOHEADER.biBitCount==24)
		printf("24bit resolution does not have a ColorArray\n");
	getch();
}

void file_read()
{
	int x;
	image=fopen(name, "r");
	if(image==NULL)
	{
		printf("Error opening %s", name);
		getch();
		exit(1);
	}
	x=fread(&BITMAPFILEHEADER, sizeof(BITMAPFILEHEADER), 1, image);
	if(x==0)
	{
		printf("Problem with fread() funcion for BITMAPFILEHEADER");
		getch();
		exit(1);
	}
	x=fread(&BITMAPINFOHEADER, sizeof(BITMAPINFOHEADER), 1, image);
	if(x==0)
	{
		printf("Problem with fread() funcion for BITMAPINFOHEADER");
		getch();
		exit(1);
	}
	if((int)BITMAPINFOHEADER.biBitCount!=24)
	{
		for(d=0;d<(int)pow(2, (double)BITMAPINFOHEADER.biBitCount);d++)
			x=fread(&RGBQUAD[d], sizeof(RGBQUAD[d]), 1, image);
	}
	if(x==0)
	{
		printf("Problem with fread() function for RGBQUAD[]");
		getch();
		exit(1);
	}
	if((int)BITMAPINFOHEADER.biBitCount==24)
		printf("No need to import all this data\n");
}

void getpillow()
{
	unsigned float test, dd;
	if((int)BITMAPINFOHEADER.biBitCount==1)
		test=(float)BITMAPINFOHEADER.biWidth/8;
	if((int)BITMAPINFOHEADER.biBitCount==4)
		test=(float)BITMAPINFOHEADER.biWidth/2;
	if((int)BITMAPINFOHEADER.biBitCount==8)
		test=(float)BITMAPINFOHEADER.biWidth/1;
	if((int)BITMAPINFOHEADER.biBitCount==24)
		test=3*(float)BITMAPINFOHEADER.biWidth;

	if(test>(int)test)
		test=(int)test+1;
	dd=test/4;
	if(dd>(int)dd)
		dd=(int)dd+1;
	dd=(int)dd*4;
	pillow=dd-test;
	if(pillow==4)
		pillow=0;

	printf("Pillow: %i\n", pillow);
}
