#include<stdio.h>
#include<time.h>
#include<vga.h>

int rnd(int range);
void seed(void);int rnd(int range);
void initialize(void);
void algorithm(void);
void draw(void);
void mouse_info(void);
void draw_cursor(void);
void press_mouse(void);
void scan(void);

byte far *b, *bb;
union REGS i, o;
int button, mx, my, ctype=2;
int freeze=-1, color=-1;

void main()
{
	long num;
	b=(byte far*)malloc(64000);
	bb=(byte far*)malloc(64000);
	setup_page();
	seed();
	initialize();
	mode(graphics13h);
	for(;;)
	{
		if(kbhit())
			scan();
		draw();
		mouse_info();
		draw_cursor();
		press_mouse();
		if(freeze==-1)
		{
			algorithm();
			for(num=0;num<64000;num++)
				b[num]=bb[num];
		}
		flip();
	}
}

void scan()
{
	char key;
	long num;
	key=getch();
	if(key=='r')
		initialize();
	if(key=='c')
		color=-color;
	if(key==0x0d)
	{
		for(num=0;num<64000;num++)
			b[num]=0;
	}
	if(key=='1')
		ctype=1;
	if(key=='2')
		ctype=2;
	if(key=='3')
		ctype=3;
	if(key=='4')
		ctype=4;
	if(key==0x20)
		freeze=-freeze;
	if(key==0x1b)
		exit(1);
}

void press_mouse()
{
	if(button==1)
	{
		if(ctype==1)
			b[my*320+mx]=1;
		if(ctype==2)
		{
			b[(my-1)*320+mx]=1;
			b[(my+1)*320+mx]=1;
			b[my*320+mx-1]=1;
			b[my*320+mx+1]=1;
			b[my*320+mx]=1;
		}
		if(ctype==3)
		{
			b[(my-1)*320+mx]=1;
			b[my*320+mx-1]=1;
			b[(my+1)*320+mx-1]=1;
			b[(my+1)*320+mx]=1;
			b[(my+1)*320+mx+1]=1;
		}
		if(ctype==4)
		{
			b[my*320+mx]=1;
			b[my*320+mx+1]=1;
			b[(my+1)*320+mx]=1;
			b[(my+1)*320+mx+1]=1;
		}
	}
	if(button==2)
	{
		if(ctype==1)
			b[my*320+mx]=0;
		if(ctype==2)
		{
			b[(my-1)*320+mx]=0;
			b[(my+1)*320+mx]=0;
			b[my*320+mx-1]=0;
			b[my*320+mx+1]=0;
			b[my*320+mx]=0;
		}
		if(ctype==3)
		{
			b[(my-1)*320+mx]=0;
			b[my*320+mx-1]=0;
			b[(my+1)*320+mx-1]=0;
			b[(my+1)*320+mx]=0;
			b[(my+1)*320+mx+1]=0;
		}
		if(ctype==4)
		{
			b[my*320+mx]=0;
			b[my*320+mx+1]=0;
			b[(my+1)*320+mx]=0;
			b[(my+1)*320+mx+1]=0;
		}
	}
	
}

void mouse_info()
{
	i.x.ax=3;				/* Read mouse function */
	int86(0x33, &i, &o);
	button=o.x.bx;			/* 1=Left, 2=Right, 3=Both */
	if(o.x.cx>319)
	{
		i.x.ax=4;			/* Set mouse function */
		i.x.cx=319;
		i.x.dx=my;
		int86(0x33,&i,&o);
	}
	if(o.x.dx>199)
	{
		i.x.ax=4;
		i.x.cx=mx;
		i.x.dx=199;
		int86(0x33,&i,&o);
	}
	mx=o.x.cx;
	my=o.x.dx;
}

void draw()
{
	long num;
	for(num=0;num<64000;num++)
	{
		if(color==-1)
		{
			if(b[num]==0)
				page[num]=black;
			if(b[num]==1)
				page[num]=white;
		}
		if(color==1)
		{
			if(b[num]==0)
				page[num]=black;
			if(b[num]==1)
			{
				page[num]=rnd(16);
				if(page[num]==black)
					page[num]=white;
			}
		}
	}
}

void draw_cursor()
{
	int overdraw=0;
	if(color==-1)
	{
		if(page[my*320+mx]==white)
			overdraw=1;
	}
	if(ctype==1)
		page_pixel(mx, my, white);
	if(ctype==2)
	{
		page_pixel(mx, my-1, white);
		page_pixel(mx, my+1, white);
		page_pixel(mx-1, my, white);
		page_pixel(mx+1, my, white);
		page_pixel(mx, my, white);
	}
	if(ctype==3)
	{
		page_pixel(mx, my-1, white);
		page_pixel(mx-1, my, white);
		page_pixel(mx-1, my+1, white);
		page_pixel(mx, my+1, white);
		page_pixel(mx+1, my+1, white);
	}
	if(ctype==4)
	{
		page_pixel(mx, my, white);
		page_pixel(mx+1, my, white);
		page_pixel(mx, my+1, white);
		page_pixel(mx+1, my+1, white);
	}
	if(overdraw==1)
		page_pixel(mx, my, rnd(16));
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

void initialize()
{
	long num;
	for(num=0;num<64000;num++)
		b[num]=rnd(2);
}

void algorithm()
{
	int x, y, friends=0;
	for(y=0;y<200;y++)
	{
		for(x=0;x<320;x++)
		{
			if(b[(y-1)*320+x-1]==1)
				friends++;
			if(b[(y-1)*320+x]==1)
				friends++;
			if(b[(y-1)*320+x+1]==1)
				friends++;
			if(b[y*320+x-1]==1)
				friends++;
			if(b[y*320+x+1]==1)
				friends++;
			if(b[(y+1)*320+x-1]==1)
				friends++;
			if(b[(y+1)*320+x]==1)
				friends++;
			if(b[(y+1)*320+x+1]==1)
				friends++;
			if(b[y*320+x]==0)
			{
				if(friends==3)
					bb[y*320+x]=1;
				if(friends!=3)
					bb[y*320+x]=0;
			}
			if(b[y*320+x]==1)
			{
				if(friends<2)
					bb[y*320+x]=0;
				if(friends>3)
					bb[y*320+x]=0;
				if(friends==2||friends==3)
					bb[y*320+x]=1;
			}
			friends=0;
		}
	}
}
