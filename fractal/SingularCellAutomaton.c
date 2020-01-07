/* Singular Cell Automaton intended for Borland's Turbo C Compiler */
#include<stdio.h>
#include<vga.h>

int grid1[320], grid2[320], y=0;

void main() {
	int d, dd;
	mode(graphics13h);
	for(d=0;d<320;d++)
			grid1[d]=0;
	for(d=0;d<320;d++)
			grid2[d]=0;
	grid1[0]=1;
	grid1[200]=1;
	for(dd=0;dd<200;dd++)
	{
		for(d=0;d<320;d++)
		{
			if(grid1[d]==1)
			{
				if(grid1[d+1]==0)
					grid2[d+1]=1;
				if(grid1[d+1]==1)
					grid2[d+1]=0;
			}
		}
		for(d=0;d<320;d++)
			grid1[d]=grid2[d];
		for(d=0;d<320;d++)
		{
			if(grid1[d]==0)
				pixel(d, y, black);
			if(grid1[d]==1)
				pixel(d, y, white);
		}
		y++;
	}
	getch();
}
