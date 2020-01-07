#include<time.h>
#include<stdio.h>
#include<vga.h>

void kb_scan(void);
void grid_init(void);
void grid_draw(void);
void grid_bikes(void);
void player_draw(void);
void bikes_move(void);
int detect_collition(void);	/* 1: redwithwall, 2: bluewithwall, 3: redwithblue, 4: bluewithred, 5: bluewithblue, 6: redwithred */
void delay_milli(int msecs);	/* Need to figure out how to do milliseconds... */
void reset(void);

byte far *grid;
int p1x, p1y, p2x, p2y, p1dir, p2dir;
int collition;
/*int old_p1x, old_p1y, old_p2x, old_p2y;*/
int old_p1, old_p2, score_p1=0, score_p2=0;

void main()
{
	setup_page();
	grid=(byte far*)malloc(64000);
	grid_init();
	p1dir=4;	/* Right */
	p2dir=3;	/* Left */
	p1x=50;
	p1y=100;
	p2x=269;
	p2y=100;
	mode(graphics13h);
	for(;;)
	{
		if(kbhit())
			kb_scan();
		page_clear(darkgrey);
		bikes_move();
		grid_bikes();
		grid_draw();
		player_draw();
		flip();
		collition=detect_collition();
		if(collition!=0)
		{
			getch();
			if(collition==1)
			{
				mode(text);
				score_p2++;
				printf("Red has collided with the wall");
				printf("\nPlayer One's Score: %i\nPlayer Two's Score: %i", score_p1, score_p2);
				getch();
				getch();
				reset();
			}
			if(collition==2)
			{
				mode(text);
				score_p1++;
				printf("Blue has collided with the wall");
				printf("\nPlayer One's Score: %i\nPlayer Two's Score: %i", score_p1, score_p2);
				getch();
				getch();
				reset();
			}
			if(collition==3)
			{
				mode(text);
				score_p2++;
				printf("Red has collided with Blue");
				printf("\nPlayer One's Score: %i\nPlayer Two's Score: %i", score_p1, score_p2);
				getch();
				getch();
				reset();
			}
			if(collition==4)
			{
				mode(text);
				score_p1++;
				printf("Blue has collided with Red");
				printf("\nPlayer One's Score: %i\nPlayer Two's Score: %i", score_p1, score_p2);
				getch();
				getch();
				reset();
			}
			if(collition==5)
			{
				mode(text);
				score_p2++;
				printf("Red has collided with itself");
				printf("\nPlayer One's Score: %i\nPlayer Two's Score: %i", score_p1, score_p2);
				getch();
				getch();
				reset();
			}
			if(collition==6)
			{
				mode(text);
				score_p1++;
				printf("Blue has collided with itself");
				printf("\nPlayer One's Score: %i\nPlayer Two's Score: %i", score_p1, score_p2);
				getch();
				getch();
				reset();
			}
		}
		collition=0;
		/*delay_milli(1);*/ 
	}
}

void kb_scan()
{
	char key;
	key=getch();
	if(key=='w')
		p1dir=1;
	if(key=='s')
		p1dir=2;
	if(key=='a')
		p1dir=3;
	if(key=='d')
		p1dir=4;
	if(key==0x0048)	/* Up*/
		p2dir=1;
	if(key==0x0050)	/* Down */
		p2dir=2;
	if(key==0x004b)	/* Left */
		p2dir=3;
	if(key==0x004d)	/* Right */
		p2dir=4;
	if(key==0x1b)
		exit(0);	/* Exit without error */
}

void grid_draw()
{
	long d;
	for(d=0;d<64000;d++)
	{
		if(grid[d]!=0)
			page[d]=grid[d];
	}
}

void player_draw(void)
{
	page_pixel(p1x, p1y, yellow);
	page_pixel(p2x, p2y, yellow);
}

void grid_init()
{
	long d;
	for(d=0;d<64000;d++)
		grid[d]=0;
}

void grid_bikes()
{
	grid[p1y*320+p1x]=red;
	grid[p2y*320+p2x]=blue;
}

void bikes_move()
{
	if(p1dir==1)
	{
		old_p1=grid[(p1y-1)*320+p1x];
		p1y--;
	}
	if(p1dir==2)
	{
		old_p1=grid[(p1y+1)*320+p1x];
		p1y++;
	}
	if(p1dir==3)
	{
		old_p1=grid[p1y*320+p1x-1];
		p1x--;
	}
	if(p1dir==4)
	{
		old_p1=grid[p1y*320+p1x+1];
		p1x++;
	}

	if(p2dir==1)
	{
		old_p2=grid[(p2y-1)*320+p2x];
		p2y--;
	}
	if(p2dir==2)
	{
		old_p2=grid[(p2y+1)*320+p2x];
		p2y++;
	}
	if(p2dir==3)
	{
		old_p2=grid[p2y*320+p2x-1];
		p2x--;
	}
	if(p2dir==4)
	{
		old_p2=grid[p2y*320+p2x+1];
		p2x++;
	}
}

void delay_milli(int msecs)	/* Not a millisecond timer */
{
	time_t t1, t2;
	unsigned int y;
	int d;
	for(d=0;d<msecs;d++)
	{
		(void)time(&t1);
		for(;;)
		{
			(void)time(&t2);
			y=difftime(t2, t1);
			if(y!=0)
				goto mid;
		}
		mid:;
	}
}

int detect_collition(void)	/* 1: redwithwall, 2: bluewithwall, 3: redwithblue, 4: bluewithred, 5: bluewithblue, 6: redwithred */
{
	if(p1x<0)
		return(1);
	if(p1x>319)
		return(1);
	if(p1y<0)
		return(1);
	if(p1y>199)
		return(1);

	if(p2x<0)
		return(2);
	if(p2x>319)
		return(2);
	if(p2y<0)
		return(2);
	if(p2y>199)
		return(2);

	if(old_p1==blue)
		return(3);
	if(old_p2==red)
		return(4);

	if(old_p1==red)
		return(5);
	if(old_p2==blue)
		return(6);
	return(0);
}

void reset()
{
	grid_init();
	p1dir=4;	/* Right */
	p2dir=3;	/* Left */
	p1x=50;
	p1y=100;
	p2x=269;
	p2y=100;
	mode(graphics13h);
}
