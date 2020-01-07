// TicTacToe 1.1
/* TicTacToe game programmed by Bryanandhallie */
#include<stdio.h>
#include<vga.h>

int grid[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int player = 1, game=1;
int win=0;
int posx = 1, posy = 1;

void main()
{
	mode(graphics13h);
	while(game==1)
	{
		clear(red);
		drawboard();
		play();
		checkwin();
		if(win==0)
			checktie();
	}
	if(win==1)
	{
		clear(white);
		printf("Player1 Wins!!!");
	}
	if(win==2)
	{
		clear(white);
		printf("Player2 Wins!!!");
	}
	if(win==3)
	{
		clear(white);
		printf("Nobody wins, it's a Tie!!!");
	}
	getch();
}

int drawboard()
{
	line(107, 0, 107, 200, blue);
	line(214, 0, 214, 200, blue);
	line(0, 67, 320, 67, blue);
	line(0, 134, 320, 134, blue);
}

int play()
{
	char key=0;
	int loop = 1;
beg:
	while(loop==1)
	{
		if(player == 1)
		{
			clear(red);
			drawboard();
			drawplaces();
			p1draw(posx, posy);
			if(grid[posx+posy*3] == 1)
				samep1draw(posx, posy);
			key=getch();
			if(key == 'a')
			{
				posx=posx-1;
				if(posx==-1)
					posx=2;
			}
			if(key == 'd')
			{
				posx=posx+1;
				if(posx==3)
					posx=0;
			}
			if(key == 'w')
			{
				posy=posy-1;
				if(posy==-1)
					posy=2;
			}
			if(key == 's')
			{
				posy=posy+1;
				if(posy==3)
					posy=0;
			}
			if(key == 'e')
			{
/*printf("X: %i, Y: %i, #: %i, Is: %i\n", posx, posy, posx+posy*3,  grid[posx+posy*3]);
				getch();*/
				if(grid[posx+posy*3]==0)
				{
					grid[posx+posy*3]=1;
					player=2;
					loop = 0;
					goto beg;
				}
				else
					goto beg;
			}
			if(key == 'q')
				exit(1);
		}
		if(player == 2)
		{
			clear(red);
			drawboard();
			drawplaces();
			p2draw(posx, posy);
			if(grid[posx+posy*3] == 2)
				samep2draw(posx, posy);
			key=getch();
			if(key == 'a')
			{
				posx=posx-1;
				if(posx==-1)
					posx=2;
			}
			if(key == 'd')
			{
				posx=posx+1;
				if(posx==3)
					posx=0;
			}
			if(key == 'w')
			{
				posy=posy-1;
				if(posy==-1)
					posy=2;
			}
			if(key == 's')
			{
				posy=posy+1;
				if(posy==3)
					posy=0;
			}
			if(key == 'e')
			{
/*printf("X: %i, Y: %i, #: %i, Is: %i\n", posx, posy, posx+posy*3,  grid[posx+posy*3]);
				getch();*/
				if(grid[posx+posy*3]==0)
				{
					grid[posx+posy*3]=2;
					player=1;
					loop = 0;
					goto beg;
				}
				else
					goto beg;
			}
			if(key == 'q')
				exit(1);
		}
	}
}

int p1draw(int x, int y)
{
	line(x*107, y*67, (x+1)*107, (y+1)*67, green);
	line(x*107, (y+1)*67, (x+1)*107, y*67, green);
}

int p2draw(int x, int y)
{
	line(x*107+54, y*67, x*107+54, (y+1)*67, blue);
	line(x*107, y*67+34, (x+1)*107, y*67+34, blue);
}

int samep1draw(int x, int y)
{
	line(x*107, y*67, (x+1)*107, (y+1)*67, lightgreen);
	line(x*107, (y+1)*67, (x+1)*107, y*67, lightgreen);
}

int samep2draw(int x, int y)
{
	line(x*107+54, y*67, x*107+54, (y+1)*67, lightblue);
	line(x*107, y*67+34, (x+1)*107, y*67+34, lightblue);
}

int drawplaces()
{
	int x, y;
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			if(grid[x+y*3]==1)
				p1draw(x, y);
			if(grid[x+y*3]==2)
				p2draw(x, y);
		}
	}
}

int checkwin()
{
	if(grid[0]==1&&grid[1]==1&&grid[2]==1)
	{
		win=1;
		game=0;
		line(0, posy*67+34, 320, posy*67+34, black);
		getch();
	}
	if(grid[0]==1&&grid[3]==1&&grid[6]==1)
	{
		win=1;
		game=0;
		line(54, 0, 54, 200, black);
		getch();
	}
	if(grid[0]==1&&grid[4]==1&&grid[8]==1)
	{
		win=1;
		game=0;
		line(0, 0, 320, 200, black);
		getch();
	}
	if(grid[1]==1&&grid[4]==1&&grid[7]==1)
	{
		win=1;
		game=0;
		line(320/2, 0, 320/2, 200, black);
		getch();
	}
	if(grid[2]==1&&grid[4]==1&&grid[6]==1)
	{
		win=1;
		game=0;
		line(0, 200, 320, 0, black);
		getch();
	}
	if(grid[2]==1&&grid[5]==1&&grid[8]==1)
	{
		win=1;
		game=0;
		line(320-54, 0, 320-54, 200, black);
		getch();
	}
	if(grid[3]==1&&grid[4]==1&&grid[5]==1)
	{
		win=1;
		game=0;
		line(0, 200/2, 320, 200/2, black);
		getch();
	}
	if(grid[6]==1&&grid[7]==1&&grid[8]==1)
	{
		win=1;
		game=0;
		line(0, 200-34, 320, 200-34, black);
		getch();
	}

	if(grid[0]==2&&grid[1]==2&&grid[2]==2)
	{
		win=2;
		game=0;
		line(0, posy*67+34, 320, posy*67+34, black);
		getch();
	}
	if(grid[0]==2&&grid[3]==2&&grid[6]==2)
	{
		win=2;
		game=0;
		line(54, 0, 54, 200, black);
		getch();
	}
	if(grid[0]==2&&grid[4]==2&&grid[8]==2)
	{
		win=2;
		game=0;
		line(0, 0, 320, 200, black);
		getch();
	}
	if(grid[1]==2&&grid[4]==2&&grid[7]==2)
	{
		win=2;
		game=0;
		line(320/2, 0, 320/2, 200, black);
		getch();
	}
	if(grid[2]==2&&grid[4]==2&&grid[6]==2)
	{
		win=2;
		game=0;
		line(0, 200, 320, 0, black);
		getch();
	}
	if(grid[2]==2&&grid[5]==2&&grid[8]==2)
	{
		win=2;
		game=0;
		line(320-54, 0, 320-54, 200, black);
		getch();
	}
	if(grid[3]==2&&grid[4]==2&&grid[5]==2)
	{
		win=2;
		game=0;
		line(0, 200/2, 320, 200/2, black);
		getch();
	}
	if(grid[6]==2&&grid[7]==2&&grid[8]==2)
	{
		win=2;
		game=0;
		line(0, 200-34, 320, 200-34, black);
		getch();
	}
}

int checktie()
{
	int d, i=0;
	for(d=0;d<9;d++)
	{
		if(grid[d]!=0)
			i=i++;
	}
	if(i==9)
	{
		win=3;
		game=0;
	}
}
