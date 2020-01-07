#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

FILE *fp;

int bitsize, pillow, sch=1;
long int d, x, y, dx, dy;

/* bmfh */
typedef struct						/**** BMP file header structure ****/
{
	unsigned short	bfType;			/* Magic number for file */
	unsigned int	bfSize;			/* Size of file */
	unsigned short	bfReserved1;	/* Reserved */
	unsigned short	bfReserved2;	/* ... */
	unsigned int	bfOffBits;		/* Offset to bitmap data */
}BITMAPFILEHEADER;

/* bmih */
typedef struct						/**** BMP file info structure ****/
{
	unsigned int	biSize;			/* Size of info header */
	int				biWidth;		/* Width of image */
	int				biHeight;		/* Height of image */
	unsigned short	biPlanes;		/* Number of color planes */
	unsigned short	biBitCount;		/* Number of bits per pixel */
	unsigned int	biCompression;	/* Type of compression to use */
	unsigned int	biSizeImage;	/* Size of image data */
	int				biXPelsPerMeter;/* X pixels per meter */
	int				biYPelsPerMeter;/* Y pixels per meter */
	unsigned int	biClrUsed;		/* Number of colors used */
	unsigned int	biClrImportant;	/* Number of important colors */
}BITMAPINFOHEADER;

/* color array */
typedef struct						/**** Colormap entry structure ****/
{
	unsigned char	rgbBlue;		/* Blue value */
	unsigned char	rgbGreen;		/* Green value */
	unsigned char	rgbRed;			/* Red value */
	unsigned char	rgbReserved;	/* Reserved */
}RGBQUAD;

/* bitmap colors */
typedef struct							/**** Bitmap information structure ****/
{
	BITMAPINFOHEADER	bmiHeader;		/* Image header */
	RGBQUAD				bmiColors[256];	/* Image colormap */
}BITMAPINFO;

BITMAPFILEHEADER bmfh;
BITMAPINFO bmi;

RGBQUAD *array;

void initbitmap()
{
	// Need to find pillow amount
	d=0;
    while(d<x)
	    d+=4;
	d-=x;
	if(d==4)
	    d=0;
	pillow=d;

	/* Initialize BitmapFileHeader */
	bmfh.bfType='MB';     // BM backwards
	bmfh.bfSize=14+40+(3*x*y)+(pillow*x*y); // BITMAPFILEHEADERsize+BITMAPINFOHEADERsize+BITsize+PillowForBitsize
	bmfh.bfReserved1=0;
	bmfh.bfReserved2=0;
	bmfh.bfOffBits=0x30;    // Test to see if 1078 works

	/* Intialize BitmapInfoHeader */
	bmi.bmiHeader.biSize=40;         // BITMAPINFOHEADER is 40 bytes is size
	bmi.bmiHeader.biWidth=x;  	     // X Dimention
	bmi.bmiHeader.biHeight=y;      // Y Dimention
	bmi.bmiHeader.biPlanes=1;        // Specifies the number of planes of the target device, must be set to zero. I don't know why they set it to 1 though */
	bmi.bmiHeader.biBitCount=0x18;      // Bits per pixel, 3*8=24 bits or 0x18
	bmi.bmiHeader.biCompression=0;   // 0 means no compression
	bmi.bmiHeader.biSizeImage=(3*x*y)+(pillow*x*y);
	bmi.bmiHeader.biXPelsPerMeter=0; // Horizontal pixels per meter, usually 0
	bmi.bmiHeader.biYPelsPerMeter=0; // Vertical pixels per meter, usually 0
	bmi.bmiHeader.biClrUsed=0;       // Colors used in the bitmap, if zero then the number of colors is calculated using the biBitCount member
	bmi.bmiHeader.biClrImportant=0;  // Colors important in the bitmap, if zero then all colors are important

	array=(RGBQUAD*)malloc(bmi.bmiHeader.biSizeImage*sizeof(RGBQUAD));
	if(array!=NULL)
	    printf("Allocated Memory Needed...\n\n");
	else
	    printf("Was not able to Allocate Memory Needed...\n\n");

	printf("Now writing the Bitmap, please be patient...\n\n");
	/* Initialize BitmapInfoColorMap */
	for(dy=0;dy<y;dy++)
	{
		for(dx=0;dx<x;dx++)		// Set all colors to 0 value
		{
			if(sch==1)
			{
				array[dy*y+dx].rgbBlue=100;
                array[dy*y+dx].rgbGreen=100;
                array[dy*y+dx].rgbRed=100;
			}
			if(sch==0)
			{
				array[dy*y+dx].rgbBlue=255;
                array[dy*y+dx].rgbGreen=255;
                array[dy*y+dx].rgbRed=255;
			}
			sch++;
			if(sch>1)
			    sch=0;
		}
	}
}

bool writebitmap()
{
	fp=fopen("Test.bmp", "wb");

	fwrite(&bmfh.bfType,1, 2, fp);
	fwrite(&bmfh.bfSize, 1, 4, fp);
	fwrite(&bmfh.bfReserved1,1, 2, fp);
	fwrite(&bmfh.bfReserved2,1, 2, fp);
	fwrite(&bmfh.bfOffBits, 1, 4, fp);
    fwrite(&bmi.bmiHeader, 1, sizeof(BITMAPINFOHEADER), fp);

	for(dy=0;dy<y;dy++)
	{
		for(dx=0;dx<x;dx++)
		{
			fwrite(&array[dy*y+dx].rgbBlue, 1, 1, fp);
			fwrite(&array[dy*y+dx].rgbGreen, 1, 1, fp);
			fwrite(&array[dy*y+dx].rgbRed, 1, 1, fp);
		}
		for(d=0;d<pillow;d++)
		    fwrite(&array[0].rgbReserved, 1, 1, fp);
	}
	fclose(fp);
	printf("Freeing Allocated Memory...\n\n");
	free(array);
	return true;
}

bool oddtest(int number)
{
	int test=0, test2;
	while(test<number)
	    test+=2;
	test2=test-number;
//	printf("test: %i, test2: %i\n\n", test, test2);
	if(test2==1)
	    return(true);
	else
	    return(false);
}

int main(int argc, char *argv[])
{
	printf("Resolution MUST be in multiples of 2 int both dimentions\nAnd for now try not to exceed 2000 in either dimention\n\n");
	printf("X: ");
	scanf("%i", &x);
	printf("Y: ");
	scanf("%i", &y);
	if(oddtest(x)==true)
	{
		printf("X is odd, fixing...\n\n");
	    x++;
	}
	if(oddtest(y)==true)
	{
		printf("Y is odd, fixing...\n\n");
	    y++;
	}
	initbitmap();
	printf("Wrote the very most simple of Bitmaps\n\n");
	if(writebitmap()!=true)
	    printf("Bitmap file did not write :(\n\n");
	else
	    printf("Bitmap file wrote succesfully :)");
	getch();
    return EXIT_SUCCESS;
}
