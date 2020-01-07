/*Bryan's 3D Program*/
/* V1.1 includes basic clipping for points only */
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<vga.h>

void regular_run(void);
void modeler(void);
void rot_trans(void);
int rot3d(float x, float y, float z, float tx, float ty, float tz);
void plot(void);	/* As of now, plot has no polygons, or individual objects, only plots all points in np[] */
void openreadfile(void);
void help(void);

float c[]={0, 0, 256, 0, 0, 0};	/*Camera (X, Y, Z, RotationX, RY, RZ) */
float p[3000]={0, 0, 0, 
		-1, 0, 0, 
		1, 0, 0, 
		0, 1, 0, 
		0, -1, 0, 
		0, 0, -1, 
		0, 0, 1}, np[3000];
int op[3000];
float d, l=256;	/* D for distance, L for lens */	/* I'll mess with the l=256 later */
int num=7;	/* Num is the amount of points to plot in total */
int ccc;

void main()
{
	int selection;	/* For yes no selection of open file */
	printf("Keikaku3D V1.0\n");
	printf("Options:\n\n1: Run Program with default mesh\n2: Load a mesh\n3: Modeler\n4: Help ");
	scanf("%i", &selection);
	if(selection==1)
		regular_run();
	if(selection==2)
	{
		openreadfile();		/* Don't do anything special if the selection was not yes */
		regular_run();
	}
	if(selection==3)
		modeler();
	if(selection==4)
		help();
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
	int cc;
	for(cc=0;cc<num;cc++)	/* This will copy 'num' amount of points from p[] to np[], or newpoints */
	{
		np[3*cc]=p[3*cc];	/* The 3*cc is because when cc=1 you will start at the third offset in p[] */
		np[3*cc+1]=p[3*cc+1];
		np[3*cc+2]=p[3*cc+2];
	}

	for(ccc=0;ccc<num;ccc++)	/* This is to rotate 'num' amount of points according to camera */
		rot3d(np[3*ccc], np[3*ccc+1], np[3*ccc+2], c[3], c[4], c[5]);	/*Rotate np[x][y][z]by tx ty and tz */

	for(cc=0;cc<num;cc++)	/* Now to translate 'num' points according to camera */
	{
		np[3*cc]=np[3*cc]-c[0];		/* NewpointX=OldPointX-CameraX */
		np[3*cc+1]=np[3*cc+1]-c[1];	/* And for Y */
		np[3*cc+2]=np[3*cc+2]-c[2];	/* Finally for Z */
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
	np[3*ccc]=x;
	np[3*ccc+1]=y;
	np[3*ccc+2]=z;
}

void plot()	/* Test with closeup clipping *//*Only Clips For points*/
{
	int cc;
	float d1, x1, y1, d2, x2, y2;
	for(cc=0;cc<num;cc++)	/* Draws 'num' amount of pixels */
	{
		if(np[cc*3+2]<c[2]-5)	/* 5 point z distance cutoff */
		{
			if(op[cc]==0)	/* Op0 is point */
			{
				d=(c[2]-256)-np[3*cc+2];
				/* c[2]-256 because it was the initial cz distance, now it is added on */
				if(((l*np[3*cc])/d)<160&&((l*np[3*cc])/d)>-160&&((l*np[3*cc+1])/d)<100&&((l*np[cc*3+1])/d)>-100) /* Edge Collition Detection */
					pixel(((l*np[3*cc])/d)+160, 100-((l*np[3*cc+1])/d), white);
				/* (Lens*X)/D, (Lens*Y)/D... And center */

/*printf("NPX value is %f\n", ((l*np[3*cc])/d));*/
			}

			if(op[cc]==1)	/* Op1 is line */
			{
				d1=(c[2]-256)-np[3*cc+5];	/* For z of next point */
				x1=((l*np[3*cc+3])/d1)+160;	/* For x of next point */
				y1=100-((l*np[3*cc+4])/d1);	/* For y of next point */
				d2=(c[2]-256)-np[3*cc+2];
				x2=((l*np[3*cc])/d2)+160;
				y2=100-((l*np[3*cc+1])/d2);
				line(x1, y1, x2, y2, white);
				line(x2, y2, x1, y1, white);
			}
		}
	}
	/* A HUDGE thing that I need to incorperate is clipping and not drawing points that are not seen by camera */
}

void openreadfile()
{
	FILE *myfile;
	char name[10];
	int y=0;	/* X is point num, Y is a dummy variable! And Z is just to temp store */
	int z=0;		/* Test to see if the file ends */
	printf("\nName of file?: ");
	scanf("%s", &name);
	myfile=fopen(name, "r");	/* Tries to open a file with typed name */
	if(myfile)
	{
		printf("Open Successful");
		getch();
	}
	if(myfile==NULL)		/* If it can't then it gives an error, and returns to openreadfile() */
	{
		printf("\nThere seems to be no '%s'... Try again", name);
		fclose(myfile);
		getch();
		exit(1);	/* I guess quit the program. No point in looping here, only an idiot would get here */
	}
	/*fscanf(myfile, "%i", z);*/	/* For some reason I have to drop a line, not quite sure... */
	while(z!=9)	/* I guess 'e' will be end of file */
	{
		fscanf(myfile, "%f", &p[3*y]);	/* Finds the value of p[] */
		fscanf(myfile, "%f", &p[3*y+1]);
		fscanf(myfile, "%f", &p[3*y+2]);
		fscanf(myfile, "%i", &op[y]);	/* Option for that point */
		z=op[y];
		y++;				/* Increments for every point */
		num=y;				/* Stores in 'num' the amount of points accumulated */
	}

	fclose(myfile);		/* Yeay the closing of the file! */
}

void modeler()
{
	FILE *model;
	char name[10];
/*	printf("Save as: ");
	scanf("%s", &name);	*/
	/*model=fopen(name, "w");*/	/* I need to write an error detection algorithm here... */
	printf("\nTo be coded!!!");
	getch();
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