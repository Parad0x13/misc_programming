#include<stdio.h>
#include<dos.h>
#include<vga.h>

#define enter 0x0D

int rnd(int range);
void seed(void);

int x=0, y=50, xx=-50, yy=-50, xxx=50, yyy=-50, a, b, p, amount=1000, px=0, py=0, z=0, e=9, lin;

void main()
{
	FILE *myfile;

	myfile = fopen("serp.k3d", "w");

	printf("How many points?: ");
	scanf("%i", &amount);
	printf("With lines? [0, 1]: ");
	scanf("%i", &lin);
	if(lin>1)
	{
		printf("Bad response");
		getch();
		exit(1);
	}
	if(lin<0)
	{
		printf("Bad response");
		getch();
		exit(1);
	}
	mode(graphics13h);
	for(a=0;a<amount;a++)
	{
		p=rnd(3);
		if(p==0)
		{
			px=(px-x)/2;
			py=(py-y)/2;
		}
		if(p==1)
		{
			px=(px-xx)/2;
			py=(py-yy)/2;
		}
		if(p==2)
		{
			px=(px-xxx)/2;
			py=(py-yyy)/2;
		}
		pixel(px+160, py+100, white);
		fprintf(myfile, "%i", -px);
		fprintf(myfile, "%s", "\n");
		fprintf(myfile, "%i", -py);
		fprintf(myfile, "%s", "\n");
		fprintf(myfile, "%i", z);
		fprintf(myfile, "%s", "\n");
		if(lin==0)
			fprintf(myfile, "%i", z);
		if(lin==1)
			fprintf(myfile, "%i", 1);
		fprintf(myfile, "%s", "\n");
		fprintf(myfile, "%s", "\n");
	}
	fprintf(myfile, "%i", e);
	fprintf(myfile, "%s", "\n");
	fprintf(myfile, "%i", e);
	fprintf(myfile, "%s", "\n");
	fprintf(myfile, "%i", e);
	fprintf(myfile, "%s", "\n");
	fprintf(myfile, "%i", e);

	fclose(myfile);
	getch();
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