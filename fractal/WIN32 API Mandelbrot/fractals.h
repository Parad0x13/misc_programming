#ifndef FRACTAL_H
#define FRACTAL_H

#include "main.h"

#include "jpeg.h"

int red, green, blue;

/*
	Strange Attractor Fractal Series
									*/

bool fractal_henon()
{
	int d, skip=0;                  // Skip allows the first few points not to be plotted, they wont be within the attractor
	float x=0, y=0, nx, ny;
	float a=1.4f, b=0.3f;
	glPushMatrix();
	glBegin(GL_POINTS);
	for(d=0;d<diterations;d++)
	{
		nx=1+y-a*(x*x);              // y+1-ax^2
		ny=b*x;                      // bx
		x=nx;
		y=ny;
		if(skip>50)
		{
			glColor3f(1.0f/x, 1.0f/y, 1.0f/(x*y));		// Simple Ronald McDonnald coloring algorithm, will improve later
			glVertex2f(x/2, y*2);
		}
		skip++;
	}
	glEnd();
	glPopMatrix();
	return(true);
}

bool fractal_tinkerbell()           // All sorts of screwed up
{
	int dd, skip=0;
	float x=0.1, y=0, nx, ny;
//	float a=0.3f, b=0.6f, c=2.0f, d=0.27f;
	float a=0.9f, b=0.6013f, c=2.0f, d=0.5f;            // Another version of the Tinkerbell attractor
	glPushMatrix();
	glBegin(GL_POINTS);
	for(dd=0;dd<diterations;dd++)
	{
		nx=(x*x)-(y*y)+(a*x)+(b*y);		// x^2-y^2+ax+by
		ny=(2*x*y)-(c*x)+(d*y);			// 2xy+cx+dy
		x=nx;
		y=ny;
		if(skip>50)
		{
			glColor3f(1.0f/x, 1.0f/y, 1.0f/(x*y));      // Ronald McDonnald Coloring Algorithm
			glVertex2f(x/2+0.2f, y/1.5f-0.3f);
		}
		skip++;
	}
	glEnd();
	glPopMatrix();
	return(true);
}

bool fractal_lorenz()
{
	int d;
	float x=3, dx, y=15, dy, z=1, dz, dt=.0001;
	float p=10.0f, r=28.0f, b=8/3;
//	float p=28, r=46.92f, b=4;                  // Another version that works of the Lorenz Attractor
	glPushMatrix();
	glRotatef(theta, 0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(d=0;d<diterations;d++)
	{
		dx=p*(y-x);
		dy=x*(r-z)-y;
		dz=(x*y)-(b*z);
		x+=dx*dt;
		y+=dy*dt;
		z+=dz*dt;
		colorRamp(d, diterations);
		glColor3ub(red, green, blue);
		if(theta==0)
		{
			glVertex2f(x/60-0.6f, z/60-0.2f);
			glVertex2f(y/60, z/60-0.95f);
			glVertex2f(x/60+0.6f, y/60+0.3f);
		}

		if(theta!=0)
			glVertex3f(x/60-0.1f, y/60, z/60);
	}
	glEnd();
	glPopMatrix();
	return(true);
}

bool fractal_rabinovichfabrikant()
{
	int d;
	float x=-1, dx, y=0, dy, z=0.5, dz, dt=.0001;
	float v=0.87f, a=1.1f;
	glPushMatrix();
	glRotatef(theta, 0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(d=0;d<diterations;d++)
	{
		dx=y*(z-1+x*x)+v*x;
		dy=x*(3*z+1-x*x)+v*y;
		dz=-2*z*(a+x*y);
		x+=dx*dt;
		y+=dy*dt;
		z+=dz*dt;
		colorRamp(d, diterations);
		glColor3ub(red, green, blue);
		if(theta==0)
		{
			glVertex2f(x/2, z/2);
			glVertex2f((y-2)/2.5, (z-1.8)/2);
			glVertex2f((x+2.2)/2, (y-0.8)/2);
		}

		if(theta!=0)
			glVertex3f((x+1.2)/2, (y-1)/2, z/2);
	}
	glEnd();
	glPopMatrix();
	return(true);
}

bool fractal_hadley()
{
	int d;
	float x=0, dx, y=0, dy, z=1.3f, dz, dt=0.0001f;
	float a=0.25f, b=4, f=8, g=1;

	glPushMatrix();
	glRotatef(theta, 0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);

	for(d=0;d<diterations;d++)
	{
		dx=-(y*y)-(z*z)-(a*x)+(a*f);
		dy=(x*y)-(b*x*z)-y+g;
		dz=(b*x*y)+(x*z)-z;
		x+=dx*dt;
		y+=dy*dt;
		z+=dz*dt;
		colorRamp(d, diterations);
		glColor3ub(red, green, blue);
		if(theta==0)
		{
			glVertex2f(x/4-0.8, z/4+0.3);
			glVertex2f(y/5, z/5-0.5);
			glVertex2f(x/4+0.3, y/4+0.3);
		}
		if(theta!=0)
		glVertex3f(x/4, y/4, z/4);
	}
	glEnd();
	glPopMatrix();
	return(true);
}

bool fractal_rossler()
{
	int d;
	float x=0, dx, y=-6.78, dy, z=0.02, dz, dt=0.0001f;
//	float a=0.2f, b=0.1f, c=14;
	float a=0.2, b=0.2, c=5.7;
	glPushMatrix();
	glRotatef(theta, 0.0f, 1.0f, 0.0f);
	glRotatef(45, 1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for(d=0;d<diterations;d++)
	{
		dx=-(y+z);
		dy=x+(a*y);
		dz=b+z*(x-c);
		x+=dx*dt;
		y+=dy*dt;
		z+=dz*dt;
		colorRamp(d, diterations);
		glColor3ub(red, green, blue);
		if(theta==0)
		{
			glVertex2f(x/20-0.5, z/20);
			glVertex2f(y/20-0.3, z/20-1.3);
			glVertex2f(x/20+0.4, y/20-0.5);
		}

		if(theta!=0)
			glVertex3f(x/18, z/18-0.6, y/18);
	}
	glEnd();
	glPopMatrix();
	return(true);
}

bool fractal_act()
{
	int d;
	float x=0.5, dx, y=0, dy, z=0, dz, dt=0.0001f;
	float a=1.8, b=-0.07, o=1.5, u=0.02;
	glPushMatrix();
	glRotatef(theta, 0.0f, 1.0f, 0.0f);
	glRotatef(45, 1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for(d=0;d<diterations;d++)
	{
		dx=a*(x-y);
		dy=(-4*a*y)+(x*z)+(u*(x*x*x));
		dz=-(o*a*z)+(x*y)+(b*(z*z));
		x+=dx*dt;
		y+=dy*dt;
		z+=dz*dt;
		colorRamp(d, diterations);
		glColor3ub(red, green, blue);
		if(theta==0)
		{
			glVertex2f(x/30-0.5, z/30);
			glVertex2f(y/30, z/30-1.3);
			glVertex2f(x/30+0.4, y/30+0.3);
		}

		if(theta!=0)
			glVertex3f(x/30, y/30, z/30-1);
	}
	glEnd();
	glPopMatrix();
	return(true);
}

bool fractal_duffing()				// We have serious problems with the duffing equation, no time to fix now will get to later
{
	int d;
	float x=0, dx, y=0, dy;
	float dt=.0001;
	float a=0.25, b=0.3, c=1;
	glPushMatrix();
	glBegin(GL_POINTS);
	for(d=0;d<diterations;d++)
	{
		dx=y;
//		dy=x-(x*x*x)-(a*y)+(b*cos(c*dt));
//		dy=(-b*x)+(a*y)-(y*y*y);
//		dy=x*(x*x*x)-(a*y);
		x+=dx;
		y+=dy;
		colorRamp(d, diterations);
		glColor3ub(red, green, blue);

		glVertex2f(x, y);
//		printf("X: %f, Y: %f\n", x, y);
//		getch();
	}
	glEnd();
	glPopMatrix();
	return(true);
}

bool fractal_pickover()							// Pickover fractal has a few problems too that I have to work out...
{
	int dd;
	float x=0, dx, y=0, dy, z=0, dz, dt=.0001;
	float a=2.24, b=0.43, c=-0.65, d=-2.43, e=1;
	glPushMatrix();
	glRotatef(theta, 0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(dd=0;dd<diterations;dd++)
	{
		dx=sin(a*y)-z*cos(b*x);
		dy=z*sin(c*x)-cos(d*y);
		dz=e*sin(x);
		x+=dx*dt;
		y+=dy*dt;
		z+=dz*dt;
		colorRamp(dd, diterations);
		glColor3ub(red, green, blue);
		if(theta==0)
		{
//			glColor3f(1, 1, 1);
			glVertex2f(x/20, y/20);
//			printf("dd: %i\, X: %f, Y: %f\n", dd, x, y);
		}

		if(theta!=0)
			glVertex3f(x/60-0.1f, y/60, z/60);
	}
	glEnd();
	glPopMatrix();
	return(true);
}

bool fractal_ikeda()			// Ikeda has its problems too, man I'm not having a good programming day!!! 3 unworking fractals inna row
{
	int dd;
	float x=0, dx, y=0, dy, z=0, dz, dt=.0001;
	float a=1, b=0.9, c=0.4, d=6;
	glPushMatrix();
	glRotatef(theta, 0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(dd=0;dd<diterations;dd++)
	{
		dx=a+b*(x*cos(z)-y*sin(z));
		dy=b*(x*sin(z)+y*cos(z));
		dz=c-d/(1+(x*x)+(y*y));
		x+=dx*dt;
		y+=dy*dt;
		z+=dz*dt;
		colorRamp(dd, diterations);
		glColor3ub(red, green, blue);
		if(theta==0)
		{
//			glVertex2f(x/60-0.6f, z/60-0.2f);
//			glVertex2f(y/60, z/60-0.95f);
//			glVertex2f(x/60+0.6f, y/60+0.3f);
			glVertex2f(x/200, y/200);
		}

		if(theta!=0)
			glVertex3f(x/200, y/200, z/200);
	}
	glEnd();
	glPopMatrix();
	return(true);
}

bool fractal_rucklidge()
{
	int d;
	float x=1, dx, y=0, dy, z=4.5, dz, dt=.0001;
	float k=2, s=6.7;
	glPushMatrix();
	glRotatef(theta, 0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(d=0;d<diterations;d++)
	{
		dx=(-k*x)+(s*y)-(y*z);
		dy=x;
		dz=-z+(y*y);
		x+=dx*dt;
		y+=dy*dt;
		z+=dz*dt;
		colorRamp(d, diterations);
		glColor3ub(red, green, blue);
		if(theta==0)
		{
			glVertex2f(x/20-0.5, z/20);
			glVertex2f(y/20, z/20-0.8);
			glVertex2f(x/20+0.4, y/20+0.4);
		}

		if(theta!=0)
			glVertex3f(x/20, y/20, z/20);
	}
	glEnd();
	glPopMatrix();
	return(true);
}

/*
	Iterated Function System Fractal Series(IFS)
												*/

/* 3D Support */
bool fractal_IFS_sierpinski()
{
	float pax=0, pay=-1, paz=0, pbx=-1.932, pby=1, pbz=-0.518, pcx=0.518, pcy=1, pcz=1.932, pdx=1.414, pdy=1, pdz=-1.414, px=0, py=0, pz=0;
	int pick, d, skip;
	if(theta==0)
	{
		pax=0;pay=-1;pbx=-1;pby=1;pcx=1;pcy=1;
	}
	glPushMatrix();
	glRotatef(theta, 0.0f, 1.0f, 0.0f);
	glRotatef(10, 1, 0, 0);
	glBegin(GL_POINTS);
	for(d=0;d<diterations;d++)
	{
		if(theta==0)
			pick=rnd(3);
		else
			pick=rnd(4);
		if(pick==0)
		{
			px=(px-pax)/2;
			py=(py-pay)/2;
			pz=(pz-paz)/2;
		}
		if(pick==1)
		{
			px=(px-pbx)/2;
			py=(py-pby)/2;
			pz=(pz-pbz)/2;
		}
		if(pick==2)
		{
			px=(px-pcx)/2;
			py=(py-pcy)/2;
			pz=(pz-pcz)/2;
		}
		if(pick==3)
		{
			px=(px-pdx)/2;
			py=(py-pdy)/2;
			pz=(pz-pdz)/2;
		}
		if(skip>50)
		{
			switch(pick)        // This coloring algorithm does something compleatly unexpected!
			{
				case 0:
				glColor3f(1.0f, 0.0f, 0.0f);
				break;
				case 1:
				glColor3f(0.0f, 1.0f, 0.0f);
				break;
				case 2:
				glColor3f(0.0f, 0.0f, 1.0f);
				break;
			}
			if(theta==0)
				glVertex2f(px/1.5, py/1.5);
			else
				glVertex3f(px/2, py/2, pz/2);
		}
		skip++;
	}
	glEnd();
	glPopMatrix();
	return(true);
}

/* 3D Support */
/* To be honest, I dont know why this equation down here works the way it does... */
bool fractal_IFS_triangles()
{
	float pax=0, pay=-1, paz=0, pbx=-1.932, pby=1, pbz=-0.518, pcx=0.518, pcy=1, pcz=1.932, pdx=1.414, pdy=1, pdz=-1.414, px=0, py=0, pz=0;
	int pick, d, skip;
	if(theta==0)
	{
		pax=0;pay=1;pbx=-1;pby=-1;pcx=1;pcy=-1;
	}
	glPushMatrix();
	glRotatef(theta, 0.0f, 1.0f, 0.0f);
	glRotatef(10, 1, 0, 0);
	glBegin(GL_POINTS);
	for(d=0;d<diterations;d++)
	{
		if(theta==0)
			pick=rnd(3);
		else
			pick=rnd(4);
		if(pick==0)
		{
			px=(pax-px)/2;
			py=(pay-py)/2;
			pz=(paz-pz)/2;
		}
		if(pick==1)
		{
			px=(pbx-px)/2;
			py=(pby-py)/2;
			pz=(pbz-pz)/2;
		}
		if(pick==2)
		{
			px=(pcx-px)/2;
			py=(pcy-py)/2;
			pz=(pcz-pz)/2;
		}
		if(pick==3)
		{
			px=(pdx-px)/2;
			py=(pdy-py)/2;
			pz=(pdz-pz)/2;
		}
		if(skip>50)
		{
			switch(pick)        // This coloring algorithm does something compleatly unexpected!
			{
				case 0:
				glColor3f(1.0f, 0.0f, 0.0f);
				break;
				case 1:
				glColor3f(0.0f, 1.0f, 0.0f);
				break;
				case 2:
				glColor3f(0.0f, 0.0f, 1.0f);
				break;
			}
			if(theta==0)
				glVertex2f(px/1.5, py/1.5);
			else
				glVertex3f(px/2, py/2, pz/2);
		}
		skip++;
	}
	glEnd();
	glPopMatrix();
	return(true);
}

bool fractal_IFS_pentagon()
{
	float pax=0.0f, pay=-1.0f, pbx=0.951f, pby=-0.309f, pcx=0.588f, pcy=0.809f, pdx=-0.588f, pdy=0.809f, pex=-0.951f, pey=-0.309, px=0, py=0;		// Bleh, had to use Blender to find the vertex coordinates
	int pick, d, skip;
	glPushMatrix();
	glBegin(GL_POINTS);
	for(d=0;d<diterations;d++)
	{
		pick=rnd(5);
		if(pick==0)
		{
			px=(px-pax)/2;
			py=(py-pay)/2;
		}
		if(pick==1)
		{
			px=(px-pbx)/2;
			py=(py-pby)/2;
		}
		if(pick==2)
		{
			px=(px-pcx)/2;
			py=(py-pcy)/2;
		}
		if(pick==3)
		{
			px=(px-pdx)/2;
			py=(py-pdy)/2;
		}
		if(pick==4)
		{
			px=(px-pex)/2;
			py=(py-pey)/2;
		}
		if(skip>50)
		{
			switch(pick)        // This coloring algorithm does something compleatly unexpected!
			{
				case 0:
				glColor3f(1.0f, 0.0f, 0.0f);
				break;
				case 1:
				glColor3f(0.0f, 1.0f, 0.0f);
				break;
				case 2:
				glColor3f(0.0f, 0.0f, 1.0f);
				break;
				case 3:
				glColor3f(1.0f, 1.0f, 1.0f);
				break;
				case 4:
				glColor3f(1.0f, 1.0f, 0.0f);
				break;
			}
			glVertex2f(px/1.0f, py/1.0f-0.1f);
		}
		skip++;
	}
	glEnd();
	glPopMatrix();
	return(true);
}

/*
	Complex Equation Fractal Series
									*/

bool fractal_mandelbrot(int shadetype)						// Using equation f(Z)=Z^2+C
{
	int pnum=2;			// Power number, must implement into fractal_mandelbrot(int shadetype, int pnum)
	float r, g, b;

	int y, x, d, exitnum;
	float distance;

	printf("Rendering Mandelbrot Fractal...\n");
	for(y=0;y<ScreenY;y++)
	{
		for(x=0;x<ScreenX;x++)
		{
			z=ikill(z);
			for(d=0;d<depth;d++)
			{
				n=imul(z, z);
				c=iscreen(maxX, minX, maxY, minY, ScreenX, ScreenY, x, y);
				n=iadd(n, c);
				z=iequal(z, n);
				distance=sqrt(z.real*z.real+z.immaginary*z.immaginary);
				if(distance>bailout)
				{
					exitnum=d;
					d=depth;
				}
			}

			glPushMatrix();
			glBegin(GL_POINTS);
			if(distance<=bailout)
			{
				glColor3f(0, 0, 0);
				glVertex2f((2.0f/ScreenX)*x-1.0f, (2.0f/ScreenY)*y-1.0f);	// Store the 2/ScreenX and ScreenY as variables to speed up program
//				raw_image[y*ScreenY+x]=0;
			}
			if(distance>bailout)
			{
				for(d=0;d<exitnum;d++)
				{
					r++;
					if(r==pnum)
						r=0;
				}
				r=shade(bailout, exitnum, 2, distance, shadetype);
//				glColor3f(1.0f/shade(bailout, exitnum, 2, distance, shadetype), 2.0f/shade(bailout, exitnum, 2, distance, shadetype), 0.5f/shade(bailout, exitnum, 2, distance, shadetype));
				glColor3ub(colorArray(exitnum).R, colorArray(exitnum).G, colorArray(exitnum).B);
				glVertex2f((2.0f/ScreenX)*x-1.0f, (2.0f/ScreenY)*y-1.0f);
//				glVertex2f((2.0f/ScreenX)*x-minX, (2.0f/ScreenY)*y-minY);
//				raw_image[y*ScreenY+x]=1;
			}

			glEnd();
			glPopMatrix();
		}
	}
	printf("Finished Rendering Mandelbrot Fractal\n");
	return(true);
}

bool test_mandelbrot(int shadetype, int rasterize)
{
	int pnum=2;			// Power number, must implement into fractal_mandelbrot(int shadetype, int pnum)
	float r, g, b;

	int y, x, d, exitnum;
	float distance;

	float xpixsize, ypixsize;

	xpixsize=2.0f/ScreenX;
	ypixsize=2.0f/ScreenY;

	int dd;

	printf("Rendering Test Mandelbrot Fractal...\n");
//	for(chunk_size(rasterize))
	for(dd=0;dd<rasterize;dd++)
	{
		for(y=0;y<ScreenY;y+=chunk_size[rasterize])
		{
			for(x=0;x<ScreenX;x+=chunk_size[rasterize])
			{
				z=ikill(z);
				for(d=0;d<depth;d++)
				{
					n=imul(z, z);
					c=iscreen(maxX, minX, maxY, minY, ScreenX, ScreenY, x, y);
					n=iadd(n, c);
					z=iequal(z, n);
					distance=sqrt(z.real*z.real+z.immaginary*z.immaginary);
					if(distance>bailout)
					{
						exitnum=d;
						d=depth;
					}
				}
	
				glPushMatrix();
				glBegin(GL_QUADS);
//				glBegin(GL_POINTS);
				if(distance<=bailout)
				{
					glColor3f(0, 0, 0);
//					glVertex2f((2.0f/ScreenX)*x-1.0f, (2.0f/ScreenY)*y-1.0f);	// Store the 2/ScreenX and ScreenY as variables to speed up program
					glVertex2f(xpixsize*x-1, ypixsize*y-1);
					glVertex2f(xpixsize*x-1, ypixsize*y+ypixsize*chunk_size[rasterize]-1);
					glVertex2f(xpixsize*x+xpixsize*chunk_size[rasterize]-1, ypixsize*y+ypixsize*chunk_size[rasterize]-1);
					glVertex2f(xpixsize*x+xpixsize*chunk_size[rasterize]-1, ypixsize*y-1);
				}
				if(distance>bailout)
				{
					for(d=0;d<exitnum;d++)
					{
						r++;
						if(r==pnum)
							r=0;
					}
					r=shade(bailout, exitnum, 2, distance, shadetype);
	//				glColor3f(1.0f/shade(bailout, exitnum, 2, distance, shadetype), 2.0f/shade(bailout, exitnum, 2, distance, shadetype), 0.5f/shade(bailout, exitnum, 2, distance, shadetype));
					glColor3ub(colorArray(exitnum).R, colorArray(exitnum).G, colorArray(exitnum).B);
					glVertex2f(xpixsize*x-1, ypixsize*y-1);
					glVertex2f(xpixsize*x-1, ypixsize*y+ypixsize*chunk_size[rasterize]-1);
					glVertex2f(xpixsize*x+xpixsize*chunk_size[rasterize]-1, ypixsize*y+ypixsize*chunk_size[rasterize]-1);
					glVertex2f(xpixsize*x+xpixsize*chunk_size[rasterize]-1, ypixsize*y-1);
//					glVertex2f((2.0f/ScreenX)*x-1.0f, (2.0f/ScreenY)*y-1.0f);
					
	//				raw_image[y*ScreenY+x]=1;
				}
	
				glEnd();
				glPopMatrix();
			}
		}
	}
	printf("Finished Rendering Mandelbrot Fractal\n");
	return(true);
}

bool fractal_mandelbar(int shadetype)						// Using equation f(Z)=Z^2+C
{
	int y, x, d, exitnum;
	float distance;

	ivar neg1;
	neg1.real=-1.0f;
	neg1.immaginary=0.0f;

	printf("Rendering Mandelbar Fractal...\n");
	for(y=0;y<ScreenY;y++)
	{
		for(x=0;x<ScreenX;x++)
		{
			z=ikill(z);
			for(d=0;d<depth;d++)
			{
				n=imul(z, z);
				n.immaginary=-n.immaginary;
				c=iscreen(maxX, minX, maxY, minY, ScreenX, ScreenY, x, y);
				n=iadd(n, c);
				z=iequal(z, n);
				distance=sqrt(z.real*z.real+z.immaginary*z.immaginary);
				if(distance>bailout)
				{
					exitnum=d;
					d=depth;
				}
			}

			glPushMatrix();
			glBegin(GL_POINTS);
			if(distance<=bailout)
			{
				glColor3f(0, 0, 0);
				glVertex2f((2.0f/ScreenX)*x-1.0f, (2.0f/ScreenY)*y-1.0f);	// Store the 2/ScreenX and ScreenY as variables to speed up program
			}
			if(distance>bailout)
			{
				glColor3f(1.0f/shade(bailout, exitnum, 2, distance, shadetype), 2.0f/shade(bailout, exitnum, 2, distance, shadetype), 0.5f/shade(bailout, exitnum, 2, distance, shadetype));
				glVertex2f((2.0f/ScreenX)*x-1.0f, (2.0f/ScreenY)*y-1.0f);
			}

			glEnd();
			glPopMatrix();
		}
	}
	return(true);
}

bool fractal_julia(int shadetype)						// Using equation f(Z)=Z^2+C
{
	int y, x, d, exitnum;
	float distance;
	c.real=0.5f;
	c.immaginary=0.05f;

	printf("Rendering Julia Fractal...\n");
	for(y=0;y<ScreenY;y++)
	{
		for(x=0;x<ScreenX;x++)
		{
			z=iscreen(maxX, minX, maxY, minY, ScreenX, ScreenY, x, y);
			for(d=0;d<depth;d++)
			{
				z=isquare(z, 3);

				z=iadd(z, c);
				distance=sqrt(z.real*z.real+z.immaginary*z.immaginary);
				if(distance>bailout)
				{
					exitnum=d;
					d=depth;
				}
			}

			glPushMatrix();
			glBegin(GL_POINTS);
			if(distance<=bailout)
			{
				glColor3f(0.0f, 1.0f/(exitnum*distance), 0.0f);
				glVertex2f((2.0f/ScreenX)*x-1.0f, (2.0f/ScreenY)*y-1.0f);	// Store the 2/ScreenX and ScreenY as variables to speed up program
			}
			if(distance>bailout)
			{
				glColor3f(1.0f/shade(bailout, exitnum, 2, distance, shadetype), 2.0f/shade(bailout, exitnum, 2, distance, shadetype), 0.5f/shade(bailout, exitnum, 2, distance, shadetype));
				glVertex2f((2.0f/ScreenX)*x-1.0f, (2.0f/ScreenY)*y-1.0f);
			}

			glEnd();
			glPopMatrix();
		}
	}
	return(true);
}

#endif
