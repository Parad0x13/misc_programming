#ifndef COLORING_H
#define COLORING_H

float shade(int bail, int finiteration, int power, float dist, int shadetype)
{
	if(shadetype==NORMALIZE_1)													// First normalizing algorithm
		return(finiteration+1-log(log(dist))/log(2));
	if(shadetype==NORMALIZE_2)													// Second normalizing algorithm, may still need to work with this one
		return((finiteration+log(2*log(bail))-log(log(dist)))/log(2));
	if(shadetype==ESCAPETIME)
		return(finiteration);

	if(shadetype==NORMALIZE_1_V1)
		return((finiteration+1-log(log(dist))/log(2))+z.real);
	if(shadetype==NORMALIZE_1_V2)
		return((finiteration+1-log(log(dist))/log(2))+z.immaginary);
	if(shadetype==NORMALIZE_1_V3)
		return((finiteration+1-log(log(dist))/log(2))+(z.real/z.immaginary));
	if(shadetype==NORMALIZE_1_V4)
		return((finiteration+1-log(log(dist))/log(2))+z.real+z.immaginary+(z.real/z.immaginary));
}

RGBquad colorArray(int escape)
{
	int d, t=0;
	int maxColor=10;
	RGBquad color;
	int cArray[10][3]={
		237, 28, 36,
		255, 126, 0,
		255, 194, 14,
		255, 242, 0,
		168, 230, 29,
		34, 177, 76,
		0, 183, 239,
		77, 109, 243,
		47, 54, 153,
		111, 49, 152};
	for(d=0;d<escape;d++)
	{
		t++;
		if(t>maxColor-1)
			t=0;
	}
	color.R=cArray[t][0];
	color.G=cArray[t][1];
	color.B=cArray[t][2];
	color.A=0;
	return(color);
}

void colorRamp(float value, float maximum)
{
	float pixelDistanceAlongPath;

	pixelDistanceAlongPath=100/(maximum/(value+1));         // Gives percent

	if(pixelDistanceAlongPath>0&&pixelDistanceAlongPath<10)
	{
		red=237;
		green=28;
		blue=36;
	}
	else if(pixelDistanceAlongPath>10&&pixelDistanceAlongPath<20)
	{
		red=255;
		green=126;
		blue=0;
	}
	else if(pixelDistanceAlongPath>20&&pixelDistanceAlongPath<30)
	{
		red=255;
		green=194;
		blue=14;
	}
	else if(pixelDistanceAlongPath>30&&pixelDistanceAlongPath<40)
	{
		red=255;
		green=242;
		blue=0;
	}
	else if(pixelDistanceAlongPath>40&&pixelDistanceAlongPath<50)
	{
		red=168;
		green=230;
		blue=29;
	}
	else if(pixelDistanceAlongPath>50&&pixelDistanceAlongPath<60)
	{
		red=34;
		green=177;
		blue=76;
	}
	else if(pixelDistanceAlongPath>60&&pixelDistanceAlongPath<70)
	{
		red=0;
		green=183;
		blue=239;
	}
	else if(pixelDistanceAlongPath>70&&pixelDistanceAlongPath<80)
	{
		red=77;
		green=109;
		blue=243;
	}
	else if(pixelDistanceAlongPath>80&&pixelDistanceAlongPath<90)
	{
		red=47;
		green=54;
		blue=153;
	}
	if(pixelDistanceAlongPath>90&&pixelDistanceAlongPath<100)
	{
		red=111;
		green=49;
		blue=152;
	}
}

#endif
