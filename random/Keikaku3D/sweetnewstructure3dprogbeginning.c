/*Bryan's 3D Program*/
/* V1.0 */
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<vga.h>

void regular_run(void);
void modeler(void);
void rot_trans(void);
int rot3d(float x, float y, float z, float tx, float ty, float tz);
void plot(void);	/* As of now, plot has no polygons, or individual objects, only plots all points in np[] */
void help(void);
void init_mesh(void);

float c[]={0, 0, 256, 0, 0, 0};	/*Camera (X, Y, Z, RotationX, RY, RZ) */
float d, l=256;	/* D for distance, L for lens */	/* I'll mess with the l=256 later */
int cc, ccc;
int numobj=2;	/* Number of polygons, or objects */

struct object
{
	int num;
	float p[1000];
	float np[1000];
	int op[1000];
};

struct object mesh[1];		/* Only one mesh for now */

void main()
{
	int selection;	/* For yes no selection of open file */
	printf("Keikaku3D V1.0\n");
/*	printf("Options:\n\n1: Run Program with default mesh\n2: Load a mesh\n3: Modeler\n4: Help ");
	scanf("%i", &selection);
	if(selection==1)
		regular_run();
	if(selection==2)
	{
		openreadfile();
		regular_run();
	}
	if(selection==3)
		modeler();
	if(selection==4)
		help();		*/
	init_mesh();
	regular_run();
}

void init_mesh()
{
	mesh[0].num=2;		/* Two points */
	mesh[0].p[0]=1;		/* p1x */
	mesh[0].p[1]=0;		/* p1y etc... */
	mesh[0].p[2]=0;
	mesh[0].op[0]=0;

	mesh[0].p[3]=-1;		/* p1x */
	mesh[0].p[4]=0;		/* p1y etc... */
	mesh[0].p[5]=0;
	mesh[0].op[0]=0;

	mesh[1].num=2;		/* Two points */
	mesh[1].p[0]=0;		/* p1x */
	mesh[1].p[1]=1;		/* p1y etc... */
	mesh[1].p[2]=0;
	mesh[1].op[0]=0;

	mesh[1].p[3]=0;		/* p1x */
	mesh[1].p[4]=-1;		/* p1y etc... */
	mesh[1].p[5]=0;
	mesh[1].op[0]=0;
}

void regular_run()
{
	int d;
	mode(graphics13h);		/* Enter VGA 13H Mode */
	for(;;)
	{
		rot_trans();
		plot();
		d=getch();
		if(d=='w')
			c[1]++;
		if(d=='s')
			c[1]--;
		if(d=='a')
			c[0]--;
		if(d=='d')
			c[0]++;
		if(d=='q')
			c[2]++;
		if(d=='e')
			c[2]--;
		if(d=='x')		/* FYI all the rotations are messed up, they don't rotate with the camera */
			c[3]+=.1;
		if(d=='y')
			c[4]+=.1;
		if(d=='z')
			c[5]+=.1;
		if(d=='r')		/* Resets Rotation */
		{
			c[3]=0;
			c[4]=0;
			c[5]=0;
		}
		if(d==0x1B)
			exit(1);
		clear(black);
	}
}

void rot_trans()
{
	for(cc=0;cc<numobj;cc++)	/* This will copy 'num' amount of points from p[] to np[], or newpoints */
	{
		for(ccc=0;ccc<mesh[cc].num;ccc++)
		{
			mesh[cc].np[3*ccc]=mesh[cc].p[3*ccc];
			mesh[cc].np[3*ccc+1]=mesh[cc].p[3*ccc+1];
			mesh[cc].np[3*ccc+2]=mesh[cc].p[3*ccc+2];
		}
	}

	for(cc=0;cc<numobj;cc++)
	{
		for(ccc=0;ccc<mesh[cc].num;ccc++)
			rot3d(mesh[cc].np[3*ccc], mesh[cc].np[3*ccc+1], mesh[cc].np[3*ccc+2], c[3], c[4], c[5]);
	}

	for(cc=0;cc<numobj;cc++)	/* Now to translate 'num' points according to camera */
	{
		for(ccc=0;ccc<mesh[cc].num;ccc++)
		{
			mesh[cc].np[3*ccc]=mesh[cc].np[3*ccc]-c[0];		/* NewpointX=OldPointX-CameraX */
			mesh[cc].np[3*ccc+1]=mesh[cc].np[3*ccc+1]-c[1];	/* And for Y */
			mesh[cc].np[3*ccc+2]=mesh[cc].np[3*ccc+2]-c[2];	/* Finally for Z */
		}
	}
}

int rot3d(float x, float y, float z, float tx, float ty, float tz)
{
	float keep;	/* This is a buffer for rotation */

	keep=y*cos(tx)-z*sin(tx);
	z=z*cos(tx)+y*sin(tx);
	y=keep;
	keep=z*cos(ty)-x*sin(ty);
	x=x*cos(ty)+z*sin(ty);
	z=keep;				/* Final Z */
	keep=x*cos(tz)-y*sin(tz);
	y=y*cos(tz)+x*sin(tz);		/* Final Y */
	x=keep;				/* Final X */
	mesh[cc].np[3*ccc]=x;
	mesh[cc].np[3*ccc+1]=y;
	mesh[cc].np[3*ccc+2]=z;
}

void plot()
{
	float d1, x1, y1, d2, x2, y2;
	for(cc=0;cc<numobj;cc++)
	{
		for(ccc=0;ccc<mesh[cc].num;ccc++)
		{
			if(mesh[cc].op[ccc]==0)	/* Op0 is point */
			{
				d=(c[2]-256)-mesh[cc].np[3*ccc+2];
				pixel(((l*mesh[cc].np[3*ccc])/d)+160, 100-((l*mesh[cc].np[3*ccc+1])/d), white);
			}

		}
	}
	/* A HUDGE thing that I need to incorperate is clipping and not drawing points that are not seen by camera */
}

/* I compleatly got rid of the openreadfile() algorithm */

void modeler()
{
	FILE *model;
	char name[10];
	printf("\nSave as: ");
	scanf("%s", &name);
/*	model=fopen(name, "w");*/	/* I need to write an error detection algorithm here... */
}

void help()
{
	printf("\n\nHey Everyone!! This is my first 3d program ever coded, so please be nice _^*^_");
	printf("\nI still have much work to do, but sofar this is what i've compleated\nand I'm pretty happy :)");
	printf("\nButton configuration for the 3d viewer is as follows:\n'w': up, 's': down, 'a': left, 'd': right, 'q': move out, 'e': move in");
	printf("\nThese control the camera, by the way I had to figure all this out on my own XD");
	printf("\nYou can rotate about the three axis for the world,\njust press 'x', 'y', or 'z' respectively.\n'r' will reset all rotation. It's really helpful:-).");
	printf("\nYou can import 'models' also, just remember that the file you want to open\nhas to be in the .exe's directory");
	printf("\nTo make your own models, the hard way, this is the format for the code:");
	printf("\nThe first three numbers are x, y, and z, and the fourth is an option.\nSofar I've only 3 total");
	printf("\n0: Just a point, 1: Line connecting preceeding point to next, 9: end of file.\nTo end file, put four 9's");
	printf("\nThe intermediate spaces are not needed, but they help when trying to organize\ndifferent points...");
	printf("\tIn the 3d viewer, Esc will quit the program");
	printf("\n\nEnjoy!. I'd love to see any models anyone makes, if anyone is interested in\nsending them to me my email is:\nbryanandhallie@yahoo.com feel free to send me comments too!");
	getch();
}