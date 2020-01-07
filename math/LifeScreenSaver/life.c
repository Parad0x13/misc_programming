/* Programmed by Bryan Tremblay Keikaku Studios */
#include<stdio.h>
#include<vga.h>

void initialize(void);
void plot(void);
void test(void);
int rnd(int range);
void seed(void);

int grid[100][100];
int add=0, doit;

void main()
{
	seed();
	mode(graphics13h);
	initialize();
	while(!kbhit())
	{
		plot();
		test();
		add++;
		if(add==100)
		{
			for(doit=0;doit<25;doit++)
				grid[rnd(100)][rnd(100)]=1;
			add=0;
		}
	}
}

void initialize()
{
	int x, y;
	for(x=0;x<100;x++)
	{
		for(y=0;y<100;y++)
			grid[x][y]=rnd(2);
	}
}

void plot()
{
	int x, y;
	for(x=0;x<100;x++)
	{
		for(y=0;y<100;y++)
		{
			if(grid[x][y]==0)
				pixel(x, y, black);
			if(grid[x][y]==1)
				pixel(x, y, white);
		}
	}
	for(x=0;x<100;x++)
	{
		for(y=0;y<100;y++)
		{
			if(grid[y][x]==0)
				pixel(x+100, y, black);
			if(grid[y][x]==1)
				pixel(x+100, y, white);
		}
	}
	for(x=0;x<100;x++)
	{
		for(y=0;y<100;y++)
		{
			if(grid[x][y]==0)
				pixel(x+200, y, black);
			if(grid[y][x]==1)
				pixel(x+200, y, white);
		}
	}
	for(x=0;x<100;x++)
	{
		for(y=0;y<100;y++)
		{
			if(grid[y][x]==0)
				pixel(x, y+100, black);
			if(grid[x][y]==1)
				pixel(x, y+100, white);
		}
	}
	for(y=0;y<100;y++)
	{
		for(x=0;x<100;x++)
		{
			if(grid[x][y]==0)
				pixel(x+100, y+100, black);
			if(grid[x][y]==1)
				pixel(x+100, y+100, white);
		}
	}
	for(y=0;y<100;y++)
	{
		for(x=0;x<100;x++)
		{
			if(grid[y][x]==0)
				pixel(x+200, y+100, black);
			if(grid[y][x]==1)
				pixel(x+200, y+100, white);
		}
	}
}

void test()
{
	int ngrid[100][100], x, y, friends=0;
	for(y=0;y<100;y++)
	{
		for(x=0;x<100;x++)
		{
			if(grid[x-1][y]==1)
				friends++;
			if(grid[x+1][y]==1)
				friends++;
			if(grid[x][y-1]==1)
				friends++;
			if(grid[x][y+1]==1)
				friends++;
			if(grid[x-1][y-1]==1)
				friends++;
			if(grid[x+1][y-1]==1)
				friends++;
			if(grid[x-1][y+1]==1)
				friends++;
			if(grid[x+1][y+1]==1)
				friends++;
			if(grid[x][y]==0)
			{
				if(friends==3)
					ngrid[x][y]=1;
			}
			if(grid[x][y]==1)
			{
				if(friends<2)
					ngrid[x][y]=0;
				if(friends>3)
					ngrid[x][y]=0;
				if(friends==2)
					ngrid[x][y]=1;
			}

			friends=0;
		}
	}
	for(y=0;y<100;y++)
	{
		for(x=0;x<100;x++)
			grid[x][y]=ngrid[x][y];
	}
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
