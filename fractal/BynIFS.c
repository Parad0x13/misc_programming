/* Programmed by Bryan Anthony Tremblay. Yeay my first IFS fractal program!!! */
/* Unlike V1.0, V1.1 is designed specifically for speed, if anymore can be obtained that is, which apparantly it can lol */
/* Update, in this version V1.2 more fractals were added, but the custom options don't operate correctly yet... */
/* I also added an "Add your own IFS fractal" part to the program */
/* This is BynIFSV1.2 Designed for compiling with Keikaku's VGA1.3*/

#include<stdio.h>
#include<math.h>
#include<vga.h>

void setup(void);
int rnd(int range);
void seed(void);
int pickeqnum();
void math(int eqnum);
void draw(int colornum);
void custom_setup(void);
void random_setup(void);

float x=1, y=1, nx, ny;
float a[10], b[10], c[10], d[10], e[10], f[10], p[10];
int color[10], equation_num, type;

void main()
{
	int num;
	printf("BynIFS V1.2\n\n");
	printf("Choose your ISF Fractal:\n1: Maple Leaf\n2: Spleenwort Fern\n3: Small Tree\n4: Dragon\n5: Custom ISF\n6: Random ISF\n7: Exit\t");
	scanf("%i", &type);
	if(type<1||type>7)
	{
		printf("Option #%i does not exist", type);
		getch();
		exit(1);
	}
	seed();
	setup();	/* It's much easyer just to let this run */
	if(type==5)
		custom_setup();
	if(type==6)
		random_setup();
	if(type==7)
		exit(0);
	mode(graphics13h);
	while(!kbhit())
	{
		num=pickeqnum();
		math(num);
		draw(num);
	}
	exit(0);
}

void draw(int colornum)
{
	if(type==1)
		pixel((x*29)+160, (-y*29)+100, color[colornum]);		/* The -Y is so the fractal will flip rightside-up */
	if(type==2)
		pixel(y*30-60, x*30+320, color[colornum]);		/* The -Y is so the fractal will flip rightside-up */
	if(type==3)
		pixel(x*350+10, y*350-60, color[colornum]);
	if(type==4)
		pixel(x*25+220, -y*19-10, color[colornum]);
	else
		pixel(x+160, y+100, color[colornum]);
}

void math(int eqnum)
{
	nx=(a[eqnum]*x)+(b[eqnum]*y)+e[eqnum];
	ny=(c[eqnum]*x)+(d[eqnum]*y)+f[eqnum];
	x=nx;
	y=ny;
}

int pickeqnum()		/* It actually works!!! */
{
	float number, chanceadd=0;
	int dum;
	number=rnd(100);
	number=number/100;
	for(dum=0;dum<equation_num;dum++)
	{
		if(number>=chanceadd&&number<=(p[dum]+chanceadd))
			return(dum);
		chanceadd+=p[dum];
	}
}

void setup()
{
	if(type==1)		/* Maple Leaf */
	{
		equation_num=4;
		a[0]=.14;b[0]=.01;c[0]=0;d[0]=.51;e[0]=-.08;f[0]=-1.31;p[0]=.1;color[0]=14;
		a[1]=.43;b[1]=.52;c[1]=-.45;d[1]=.5;e[1]=1.49;f[1]=-.75;p[1]=.35;color[1]=4;
		a[2]=.45;b[2]=-.49;c[2]=.47;d[2]=.47;e[2]=-1.62;f[2]=-.74;p[2]=.35;color[2]=15;
		a[3]=.49;b[3]=0;c[3]=0;d[3]=.51;e[3]=.02;f[3]=1.62;p[3]=.2;color[3]=2;
	}
	if(type==2)		/* Spleenwort Fern */
	{
		equation_num=4;
		a[0]=0;b[0]=0;c[0]=0;d[0]=.16;e[0]=0;f[0]=0;p[0]=.01;color[0]=14;
		a[1]=.85;b[1]=.04;c[1]=-.04;d[1]=.85;e[1]=0;f[1]=1.6;p[1]=.85;color[1]=4;
		a[2]=.2;b[2]=-.26;c[2]=.23;d[2]=.22;e[2]=0;f[2]=1.6;p[2]=.07;color[2]=15;
		a[3]=-.15;b[3]=.28;c[3]=.26;d[3]=.24;e[3]=0;f[3]=.44;p[3]=.07;color[3]=2;
	}
	if(type==3)
	{
		equation_num=3;
		a[0]=-.467;b[0]=.02;c[0]=-.113;d[0]=.015;e[0]=.4;f[0]=.4;p[0]=.333;color[0]=14;
		a[1]=.387;b[1]=.43;c[1]=.43;d[1]=-.387;e[1]=.256;f[1]=.522;p[1]=.333;color[1]=4;
		a[2]=.441;b[2]=-.091;c[2]=-.009;d[2]=-.322;e[2]=.421;f[2]=.505;p[2]=.333;color[2]=15;
	}
	if(type==4)
	{
		equation_num=2;
		a[0]=0.824074;b[0]=0.281428;c[0]=-0.212346;d[0]=0.864198;e[0]=-1.882290;f[0]=-0.110607;p[0]=.8;color[0]=14;
		a[1]=0.088272;b[1]=0.520988;c[1]=-0.463889;d[1]=-0.377778;e[1]=0.785360;f[1]=8.095795;p[1]=.2;color[1]=4;
	}
}

void random_setup()
{
	equation_num=4;
a[0]=rnd(100);b[0]=rnd(100);c[0]=rnd(100);d[0]=rnd(100);e[0]=rnd(100);f[0]=rnd(100);p[0]=.1;color[0]=14;
a[1]=rnd(100);b[1]=rnd(100);c[1]=rnd(100);d[1]=rnd(100);e[1]=rnd(100);f[1]=rnd(100);p[1]=.35;color[1]=4;	
a[2]=rnd(100);b[2]=rnd(100);c[2]=rnd(100);d[2]=rnd(100);e[2]=rnd(100);f[2]=rnd(100);p[2]=.35;color[2]=15;
a[3]=rnd(100);b[3]=rnd(100);c[3]=rnd(100);d[3]=rnd(100);e[3]=rnd(100);f[3]=rnd(100);p[3]=.2;color[3]=2;
a[0]=a[0]/100;b[0]=b[0]/100;c[0]=c[0]/100;d[0]=d[0]/100;e[0]=e[0]/100;f[0]=f[0]/100;
a[1]=a[1]/100;b[1]=b[1]/100;c[1]=c[1]/100;d[1]=d[1]/100;e[1]=e[1]/100;f[1]=f[1]/100;
a[2]=a[2]/100;b[2]=b[2]/100;c[2]=c[2]/100;d[2]=d[2]/100;e[2]=e[2]/100;f[2]=f[2]/100;
a[3]=a[3]/100;b[3]=b[3]/100;c[3]=c[3]/100;d[3]=d[3]/100;e[3]=e[3]/100;f[3]=f[3]/100;
}

void custom_setup()
{
	int dum;
	printf("How many equations to be in the IFS fractal?: ");
	scanf("%i", &equation_num);
	for(dum=0;dum<equation_num;dum++)
	{
		printf("A[%i]: ", dum);scanf("%f", &a[dum]);
		printf("B[%i]: ", dum);scanf("%f", &b[dum]);
		printf("C[%i]: ", dum);scanf("%f", &c[dum]);
		printf("D[%i]: ", dum);scanf("%f", &d[dum]);
		printf("E[%i]: ", dum);scanf("%f", &e[dum]);
		printf("F[%i]: ", dum);scanf("%f", &f[dum]);
		printf("Chance percent for EQ#%i remember, all have to add to 1: ", dum);
		scanf("%f", &p[dum]);
		printf("\nColor Chart:\n0: Black\n1: Blue\n2: Green\n3: Cyan\n4: Red\n5: Magenta\n6: Brown\n7: LightGray\n8: DarkGray\n9: LightBlue\n10: LightGreen\n11: LightCyan\n12: LightRed\n13: LightMagenta\n14: Yellow\n15: White\n");
		printf("Color[%i]: ", dum);scanf("%i", &color[dum]);
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
