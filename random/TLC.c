#include<stdio.h>

float t, l, c=299792458;

int main()
{
	int option;
	printf("T=L/C\nTime=Length/Speed Of Light\n\n1: Enter a number\n2: Help\n3: Exit\n\t: ");
	scanf("%i", &option);
	if(option == 1)
	{
		enternum();
		exit(1);
	}
	if(option == 2)
	{
		help();
		exit(1);
	}
	if(option == 3)
		exit(1);
	else
	{
		printf("\n\n\tNot an option!");
		getch();
		exit(1);
	}
}

int enternum()
{
	printf("\n\nEnter the length of the object in meters: ");
	scanf("%f", &l);
	t=l/c;
	printf("\nT=%.2f/%.0f", l, c);
	printf("\n\tThe time lapse would be: %f seconds", t);
	getch();
}

int help()
{
	printf("\nDue to General Relativity, nothing can travel faster than the speed of light, \nand so \"data\" being transferred in physical objects can only be sent the \nspeed of light. This program tells you how long data would take to travel the \nlength of any object!");
	getch();
}
