/* V1.6 programmed for VGA.H V1.3 */
/* BestMandelbrotV1.5 Includes double integers for extensively zoomed-in renders */
/* BM1.6Beta1 fixes some file handeling problems and allows for VGA output of bitmaps without problems */
/* BM1.6.2 Adds a 'Quick-Draw' button to the options roster, to incrementally and quickly save VGA images of the fractal onscreen. Genious Idea!!! And a quick Jul prob */
/* A very small addition for 1.6.3, only making single iteration increments available through '[' and']' */
#include<dos.h>
#include<vga.h>
#include<math.h>
#include<stdio.h>
#include<vgapal.h>
#include<string.h>

#define	esc	0x1b

void mandelbrot(void);
void julia(void);
void smalljulia(void);
void draw_mandelbrot(void);
void draw_julia(void);
void draw_smalljulia(void);
void mouse_info(void);
void draw_cursor(void);
void mouse_functions(void);
int rnd(int range);
void seed(void);int rnd(int range);
int savepos(int point);
int loadpos(int point);
void image_bitmap(void);
void initbitmap(void);

/* Stuff for bitmap image */
FILE *myfile;
int xval[2], yval[2];

int depth=32, screenx, screeny, run, d, color, colorchange=0, bailout=3, escapenum;
int dsmalljulia; /* DrawSmallJulia */
double zx=0, zi=0, nx, ni, distance=0;
double pointx[10], pointi[10];
double minx=-2.5, miny=-1.5, maxx=1.5, maxy=1.5;
double dminx=0, dminy=0, dmaxx=0, dmaxy=0, dbailout=0, ddepth=0;/* Differance MaxMinXY and iterations and stuff */
int xdimention=320, ydimention=200, draw, button, mx, my, protect=0, colorotate=0; /* mx: mousex, my: mousey */
double output_x, output_i;
union REGS i, o;
int set;
int VGA_increment=0;
char filename[10];
char filetype[10]={".bmp"};
char increment[10];
char prefix[10]={"BM"};

float sjulia[50][80]; /* Small Julia array, 80*50 */

byte far *mandel, *jul;

/* Stuff for savepos() and loadpos() */
double sminx[10], smaxx[10], sminy[10], smaxy[10];

/* Stuff for undo and redo */
double urminx[2], urmaxx[2], urminy[2], urmaxy[2]; /* undo-redo */

/* Stuff for mouse_functions() */
int drawrect=0, pan=0, bx, by;

int main()
{
	mandel=(byte far*)malloc(64000);
	jul=(byte far*)malloc(64000);
	run=1;draw=1;
	minx=-2.5;miny=-1.5;maxx=1.5;maxy=1.5;
	dsmalljulia=1;
	set=1;

	seed();
	setup_page();
	mode(graphics13h);

	while(run==1)
	{
		/* Checks to see if the set has changed at all, and if so, reports that it should be redrawn */
		if(maxx!=dmaxx||maxy!=dmaxy||minx!=dminx||miny!=dminy||depth!=ddepth||bailout!=dbailout)
		{
			urminx[1]=urminx[0]; /* Turns old change variable to the new undo variable */
			urmaxx[1]=urmaxx[0];
			urminy[1]=urminy[0];
			urmaxy[1]=urmaxy[0];
			draw=1;
		}
		dmaxx=maxx;dminx=minx;dmaxy=maxy;dminy=miny;ddepth=depth;dbailout=bailout;

		urminx[0]=minx;
		urmaxx[0]=maxx;
		urminy[0]=miny;
		urmaxy[0]=maxy;

		mouse_info();
		if(set==1)
			draw_mandelbrot();
		if(set==2)
			draw_julia();
		if(dsmalljulia==1)
		{
			smalljulia();
			draw_smalljulia();
		}
		draw_cursor();
		mouse_functions();
		flip();
		if(kbhit())
			scan();
		if(draw==1)
		{
			mode(text);
			gotoxy(0, 0);
			printf("Rendering...");
			if(set==1)
				mandelbrot();
			if(set==2)
				julia();
			mode(graphics13h);
		}
		draw=0;
	}
	if(run==0)
	{
		mode(text);
		gotoxy(0, 0);
		printf("GoodBye!"); /* May flicker briefly */
		exit(1);
	}
}

int savepos(int point)
{
	smaxx[point]=maxx;
	sminx[point]=minx;
	smaxy[point]=maxy;
	sminy[point]=miny;
}

int loadpos(int point)
{
	maxx=smaxx[point];
	minx=sminx[point];
	maxy=smaxy[point];
	miny=sminy[point];
}

void mouse_functions()
{
	int x, y;
	double one, two, three, four;
	if(button==3)
	{
		drawrect=0;
		pan=0;
		protect=1;
	}
	if(button==1&&protect==0)
	{
		if(drawrect==0)
		{
			bx=mx;
			by=my;
			drawrect=1;
		}
		page_rectangle(bx, by, mx-bx, my-by, red);
		page_rectangle(mx, my, bx-mx, by-my, red);
		page_pixel(mx, by, red); /* This is 'cause i'm a perfectionist :|D */
	}
	if(button==2&&protect==0)
	{
		if(pan==0)
		{
			bx=mx;
			by=my;
			pan=1;
		}
		for(y=0;y<ydimention;y++)
		{
			for(x=0;x<xdimention;x++)
			{
				if(set==1)
					page_pixel(x, y, mandel[(y-(my-by))*320+x-(mx-bx)]);
				if(set==2)
					page_pixel(x, y, jul[(y-(my-by))*320+x-(mx-bx)]);
				if(x-(mx-bx)>319||x-(mx-bx)<0||y-(my-by)>199||y-(my-by)<0)
					page_pixel(x, y, black);
			}
		}
	}
	if(button==0&&protect==0)
	{
		if(drawrect==1)
		{
			if(bx<mx)
				one=(((maxx-minx)/xdimention)*bx+minx);
			if(bx>mx)
				one=(((maxx-minx)/xdimention)*mx+minx);
			if(mx>bx)
				two=(((maxx-minx)/xdimention)*mx+minx);
			if(mx<bx)
				two=(((maxx-minx)/xdimention)*bx+minx);
			if(by<my)
				three=(((maxy-miny)/ydimention)*by+miny);
			if(by>my)
				three=(((maxy-miny)/ydimention)*my+miny);
			if(my<by)
				four=(((maxy-miny)/ydimention)*by+miny);
			if(my>by)
				four=(((maxy-miny)/ydimention)*my+miny);
			minx=one;
			maxx=two;
			miny=three;
			maxy=four;
		}
		if(pan==1)
		{
			one=(((maxx-minx)/xdimention)*(bx-mx)+minx);
			two=maxx+(((maxx-minx)/xdimention)*(bx-mx));
			three=(((maxy-miny)/ydimention)*(by-my)+miny);
			four=maxy+(((maxy-miny)/ydimention)*(by-my));

			minx=one;
			maxx=two;
			miny=three;
			maxy=four;
		}
		drawrect=0;
		pan=0;
	}
	if(button==0)
		protect=0;
}

void draw_mandelbrot()
{
	long dd;
	for(dd=0;dd<64000;dd++)
		page[dd]=mandel[dd]+colorotate;
}

void draw_julia()
{
	long dd;
	for(dd=0;dd<64000;dd++)
		page[dd]=jul[dd]+colorotate;
}

void draw_smalljulia()
{
	int x, y;
	for(y=0;y<50;y++)
	{
		for(x=0;x<80;x++)
			page_pixel(x+240, y+150, sjulia[y][x]+colorotate);
	}
}

void draw_cursor()
{
	page_pixel(mx, my, rnd(256));
	page_pixel(mx-1, my, rnd(256));
	page_pixel(mx+1, my, rnd(256));
	page_pixel(mx, my-1, rnd(256));
	page_pixel(mx, my+1, rnd(256));
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

void mandelbrot()
{
	pointx[0]=0;	/* ZX */
	pointi[0]=0;	/* Zi */
	for(screeny=0;screeny<ydimention;screeny++)
	{
		for(screenx=0;screenx<xdimention;screenx++)
		{
			pointx[0]=0;
			pointi[0]=0;
			for(d=0;d<depth;d++)
			{
				/* Multiplies the two imaginary numbers */
				output_x=(pointx[0]*pointx[0])-(pointi[0]*pointi[0]);/* By combining these two functions, I speed up the program */
				output_i=(pointx[0]*pointi[0])+(pointi[0]*pointx[0]);

				/* This gets the grid equivilant of the screenx and screeny coordinates */
				pointx[0]=output_x+(((maxx-minx)/xdimention)*screenx+minx);
				pointi[0]=output_i+(((maxy-miny)/ydimention)*screeny+miny);

				distance=sqrt(pointx[0]*pointx[0]+pointi[0]*pointi[0]);

				if(distance>bailout)
				{
					escapenum=d;
					d=depth;
				}
			}
			if(distance<=bailout)
				mandel[(screeny<<6)+(screeny<<8)+screenx]=white;
			if(distance>bailout)
				mandel[(screeny<<6)+(screeny<<8)+screenx]=escapenum;
		}
	}
}

void smalljulia()
{
	pointx[1]=(((maxx-minx)/xdimention)*mx+minx);
	pointi[1]=(((maxy-miny)/ydimention)*my+miny);
	for(screeny=0;screeny<50;screeny++)
	{
		for(screenx=0;screenx<80;screenx++)
		{
			pointx[0]=.05*screenx-2;
			pointi[0]=.04*screeny-1;
			for(d=0;d<depth;d++)
			{
				output_x=(pointx[0]*pointx[0])-(pointi[0]*pointi[0]);
				output_i=(pointx[0]*pointi[0])+(pointi[0]*pointx[0]);
				pointx[0]=output_x+pointx[1];
				pointi[0]=output_i+pointi[1];
				distance=sqrt(pointx[0]*pointx[0]+pointi[0]*pointi[0]);
				if(distance>bailout)
				{
						escapenum=d;
						d=depth;
				}
			}
			if(distance<=bailout)
				sjulia[screeny][screenx]=white;
			if(distance>bailout)
				sjulia[screeny][screenx]=escapenum;
		}
	}
}

void julia()
{
	for(screeny=0;screeny<ydimention;screeny++)
	{
		for(screenx=0;screenx<xdimention;screenx++)
		{
			pointx[0]=((maxx-minx)/xdimention)*screenx+minx;
			pointi[0]=((maxy-miny)/ydimention)*screeny+miny;
			for(d=0;d<depth;d++)
			{
				output_x=(pointx[0]*pointx[0])-(pointi[0]*pointi[0]);
				output_i=(pointx[0]*pointi[0])+(pointi[0]*pointx[0]);
				pointx[0]=output_x+pointx[1];
				pointi[0]=output_i+pointi[1];
				distance=sqrt(pointx[0]*pointx[0]+pointi[0]*pointi[0]);
				if(distance>bailout)
				{
						escapenum=d;
						d=depth;
				}
			}
			if(distance<=bailout)
				jul[screeny*320+screenx]=white;
			if(distance>bailout)
				jul[screeny*320+screenx]=escapenum;
		}
	}
}

int scan()
{
	int key, numkey;
	double nmaxy, nmaxx, nminy, nminx;
	key=getch();
	if(key=='+') /* Right now you can only change it by +/-10 */
		depth=depth+10;
	if(key=='_')
		depth=depth-10;
	if(key=='[')
		depth--;
	if(key==']')
		depth++;
	if(key=='=')
		colorotate++;
	if(key=='-')
		colorotate--;
	if(key=='s')
	{
		mode(text);
		gotoxy(0, 0);
		printf("Save in which slot? 1-9: ");
		scanf("%i", &numkey);
		savepos(numkey);
		mode(graphics13h);
	}
	if(key=='l')
	{
		mode(text);
		gotoxy(0, 0);
		printf("Load from which slot? 1-9: ");
		scanf("%i", &numkey);
		loadpos(numkey);
		mode(graphics13h);
	}
	if(key=='r')
	{
		minx=-2.5;
		miny=-1.5;
		maxx=1.5;
		maxy=1.5;
	}
	if(key=='p')
		colorotate=0;
	if(key=='z')
	{
		minx=urminx[1];
		maxx=urmaxx[1];
		miny=urminy[1];
		maxy=urmaxy[1];
	}
	if(key==0x20) /* Space-key */
		dsmalljulia=-dsmalljulia;
	if(key=='j')
	{
		if(set==1)
		{
			dsmalljulia=0;
			pointx[1]=(((maxx-minx)/xdimention)*mx+minx);
			pointi[1]=(((maxy-miny)/ydimention)*my+miny);
			set=2;
			draw=1;
			maxx=2;
			minx=-2;
			maxy=1;
			miny=-1;
			goto end;
		}
		if(set==2)
		{
			dsmalljulia=1;
			set=1;
			draw=1;
			goto end;
		}
end:;
	}
	if(key=='i')
		image_bitmap();
	if(key==0x0048)
	{
		nmaxy=maxy-.1*(maxy-miny);
		nminy=miny-.1*(maxy-miny);
		maxy=nmaxy;
		miny=nminy;
		
	}
	if(key==0x0050)
	{
		nmaxy=maxy+.1*(maxy-miny);
		nminy=miny+.1*(maxy-miny);
		maxy=nmaxy;
		miny=nminy;
		
	}
	if(key==0x004b)
	{
		nmaxx=maxx-.1*(maxx-minx);
		nminx=minx-.1*(maxx-minx);
		maxx=nmaxx;
		minx=nminx;
	}
	if(key==0x004d)
	{
		nmaxx=maxx+.1*(maxx-minx);
		nminx=minx+.1*(maxx-minx);
		maxx=nmaxx;
		minx=nminx;
	}
	if(key==0x0049)
	{
		nmaxx=maxx-((maxx-minx)*.1);
		nminx=minx+((maxx-minx)*.1);
		nmaxy=maxy-((maxy-miny)*.1);
		nminy=miny+((maxy-miny)*.1);
		maxx=nmaxx; /* This here is great! Now there is no more messing up with zooming!!! */
		minx=nminx;
		maxy=nmaxy;
		miny=nminy;
	}
	if(key==0x0051)
	{
		nmaxx=maxx+((maxx-minx)*.1);
		nminx=minx-((maxx-minx)*.1);
		nmaxy=maxy+((maxy-miny)*.1);
		nminy=miny-((maxy-miny)*.1);
		maxx=nmaxx;
		minx=nminx;
		maxy=nmaxy;
		miny=nminy;
	}
	if(key=='v')
	{
		int x, y;
		/* I need to rewrite this whole nameing thingy here, it's really uncoordinated... */
		sprintf(filename, "%d", VGA_increment);
		strncat(filename, prefix, 2);
		strncat(filename, increment, 2);
		strncat(filename, filetype, 4);
		VGA_increment++;
		xval[0]=320;
		xval[1]=320/256;
		yval[0]=200;
		yval[1]=200/256;
		initbitmap();
		for(y=200;y>0;y--)			/* This way it will be generated the way a bitmap likes it, upside-down */
		{
			for(x=0;x<320;x++)
			{
				/*printf("page[%i(y)*320+%i(x)]: %i\n", y, x, page[y*320+x]);*/
				/*printf("Blue VgaPal[0][%i]: %i\n", (page[y*320+x]), VgaPal[0][page[y*320+x]]);*/
				if(set==1)
				{
					fputc(VgaPal[(mandel[y*320+x])+colorotate][0], myfile);
					fputc(VgaPal[(mandel[y*320+x])+colorotate][1], myfile);
					fputc(VgaPal[(mandel[y*320+x])+colorotate][2], myfile);
				}
				if(set==2)
				{
					fputc(VgaPal[(jul[y*320+x])+colorotate][0], myfile);
					fputc(VgaPal[(jul[y*320+x])+colorotate][1], myfile);
					fputc(VgaPal[(jul[y*320+x])+colorotate][2], myfile);
				}
			}
		}
		fclose(myfile);
	}
	if(key=='q')
	{
		bailout--;
		if(bailout==0)
			bailout=1;
	}
	if(key=='w')
		bailout++;
	if(key=='r') colorchange=colorchange+1;
	if(key=='c') coords();
	if(key==esc) run=0;
}

int coords()
{
	printf("\nMaxX: %.15f\nMaxY: %.15f\nMinX: %.15f\nMinY: %.15f", maxx, maxy, minx, miny);
	getch();
}

void image_bitmap()
{
	int xsize, ysize, dd=0, pillow, x, y, iterations;
	int r=0, g=0, b=0;
	int cr, cg, cb;
	float percent=0;
	int graphicstype;
	mode(text);
	gotoxy(0, 0);
	
	if(set==1)
		printf("Generating Mandelbrot Set\n\n");
	if(set==2)
		printf("Generating Julia Set\n\n");
	printf("1: VGA Graphics\n2: Specific Graphics\n3: Nevermind: ");
	scanf("%i", &graphicstype);
	printf("\n");

	if(graphicstype==1)				/* Still in development */
	{
		printf("File name (Don't forget '.bmp'!): ");
		scanf("%s", &filename);
		xval[0]=320;
		xval[1]=320/256;
		yval[0]=200;
		yval[1]=200/256;
		initbitmap();
		for(y=200;y>0;y--)			/* This way it will be generated the way a bitmap likes it, upside-down */
		{
			for(x=0;x<320;x++)
			{
				/*printf("page[%i(y)*320+%i(x)]: %i\n", y, x, page[y*320+x]);*/
				/*printf("Blue VgaPal[0][%i]: %i\n", (page[y*320+x]), VgaPal[0][page[y*320+x]]);*/
				if(set==1)
				{
					fputc(VgaPal[(mandel[y*320+x])+colorotate][0], myfile);
					fputc(VgaPal[(mandel[y*320+x])+colorotate][1], myfile);
					fputc(VgaPal[(mandel[y*320+x])+colorotate][2], myfile);
				}
				if(set==2)
				{
					fputc(VgaPal[(jul[y*320+x])+colorotate][0], myfile);
					fputc(VgaPal[(jul[y*320+x])+colorotate][1], myfile);
					fputc(VgaPal[(jul[y*320+x])+colorotate][2], myfile);
				}
			}
		}
	}
	if(graphicstype==2)
	{
		printf("\t\t\t--- Bitmap information ---\n");
		printf("xsize: ");
		scanf("%i", &xsize);
		printf("ysize: ");
		scanf("%i", &ysize);
		while(dd<xsize) /* This whole process finds how many times that you need to add 0x00 so that the line ends in a dword */
			dd+=4;
		dd-=4;
		pillow=xsize-dd;
		if(pillow==4)
			pillow=0;
		printf("Iterations[0 for depth from VGA]: ");
		scanf("%i", &iterations);
		if(iterations==0)
			iterations=depth;
		printf("Red Color Multiplier: ");
		scanf("%i", &cr);
		printf("Green Color Multiplier: ");
		scanf("%i", &cg);
		printf("Blue Color Multiplier: ");
		scanf("%i", &cb);
		printf("File name (Don't forget '.bmp'!): ");
		scanf("%s", &filename);
		xval[0]=xsize;
		xval[1]=xsize/256;
		yval[0]=ysize;
		yval[1]=ysize/256;
		initbitmap();
		clrscr();
		printf("ESC-Quit\tENT-Percent Complete\nRendering image, please wait...\n");
		if(set==1)
		{
			for(y=ysize;y>0;y--)			/* This way it will be generated the way a bitmap likes it, upside-down */
			{
				for(x=0;x<xsize;x++)
				{
					pointx[0]=0;
					pointi[0]=0;
					for(d=0;d<iterations;d++)
					{
						output_x=(pointx[0]*pointx[0])-(pointi[0]*pointi[0]);
						output_i=(pointx[0]*pointi[0])+(pointi[0]*pointx[0]);
						pointx[0]=output_x+(((maxx-minx)/xsize)*x+minx);
						pointi[0]=output_i+(((maxy-miny)/ysize)*y+miny);

						distance=sqrt(pointx[0]*pointx[0]+pointi[0]*pointi[0]);
						if(distance>bailout)
						{
							/*r=cr*d;
							g=cg*d;
							b=cb*d;*/
							r=d*100;
							g=d*150;
							b=d*200;
							fputc((int)b, myfile);
							fputc((int)g, myfile);
							fputc((int)r, myfile);
							d=iterations;
						}
					}
					if(distance<=bailout)
					{
						r=255;g=255;b=255;
						fputc((int)b, myfile);
						fputc((int)g, myfile);
						fputc((int)r, myfile);
					}
				}
				percent=percent+(100.1/ysize); /* I'm not sure what the problem with this is... but 100.1 is required */
				for(d=0;d<pillow;d++)
					fprintf(myfile, "%c", 0x00); /* Padding, line must end on dword */
				if(kbhit())
				{
					button=getch();
					if(button==0x1b)
					{
						fclose(myfile);
						exit(1);
					}
					if(button==0x0d)
						printf("%.1f\%\n", percent);
				}
				gotoxy(10, 4);
				printf("[");
				for(d=0;d<percent/2;d++)
					printf("%c", 0xb1);
				for(d=0;d<50-percent/2;d++)
					printf(" ");
				printf("] ");
			}
		}
	}
	if(set==2&&graphicstype==2)
	{
		for(y=ysize;y>0;y--)			/* This way it will be generated the way a bitmap likes it, upside-down */
		{
				for(x=0;x<xsize;x++)
				{
					pointx[0]=((maxx-minx)/xsize)*x+minx;
					pointi[0]=((maxy-miny)/ysize)*y+miny;
					for(d=0;d<iterations;d++)
					{
						output_x=(pointx[0]*pointx[0])-(pointi[0]*pointi[0]);
						output_i=(pointx[0]*pointi[0])+(pointi[0]*pointx[0]);
						pointx[0]=output_x+pointx[1];
						pointi[0]=output_i+pointi[1];
						
						distance=sqrt(pointx[0]*pointx[0]+pointi[0]*pointi[0]);
						if(distance>bailout)
						{
							r=d*cr;
							g=d*cg;
							b=d*cb;
							fputc((int)b, myfile);
							fputc((int)g, myfile);
							fputc((int)r, myfile);
							d=iterations;
						}
					}
					if(distance<=bailout)
					{
						r=255;g=255;b=255;
						fputc((int)b, myfile);
						fputc((int)g, myfile);
						fputc((int)r, myfile);
					}
				}
				percent=percent+(100.1/ysize); /* I'm not sure what the problem with this is... but 100.1 is required */
				for(d=0;d<pillow;d++)
					fprintf(myfile, "%c", 0x00); /* Padding, line must end on dword */
				if(kbhit())
				{
					button=getch();
					if(button==0x1b)
					{
						fclose(myfile);
						exit(1);
					}
					if(button==0x0d)
						printf("%.1f\%\n", percent);
				}
				gotoxy(10, 4);
				printf("[");
				for(d=0;d<percent/2;d++)
					printf("%c", 0xb1);
				for(d=0;d<50-percent/2;d++)
					printf(" ");
				printf("] ");
		}
	}
	if(graphicstype==3)
		goto end;

	fclose(myfile);
	printf("\nFinished, Thank-You!");
	getch();
end:;
	mode(graphics13h);
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
	
	/*fprintf(myfile, "%c", xval[0]);
	fprintf(myfile, "%c", xval[1]);*/
	fputc((int)xval[0], myfile);
	fputc((int)xval[1], myfile);
	fprintf(myfile, "%c", 0x00);
	fprintf(myfile, "%c", 0x00);

/*	fprintf(myfile, "%c", 0xff);
	fprintf(myfile, "%c", 0xff);
	fprintf(myfile, "%c", xsize);
	fprintf(myfile, "%c", 0xff);
	fprintf(myfile, "%c", 0xff);	*/

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
