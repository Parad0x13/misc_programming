// I'm almost certain this doesn't work...
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

typedef struct {
	int count;
	char *value;
}longNumber;

longNumber longNumberWith(char *value) {
	longNumber retVal;
	retVal.count = strlen(value);
	retVal.value = (char *)malloc(retVal.count * sizeof(char));
	strcpy(retVal.value, value);
	return retVal;
}

longNumber longNumberWithCount(int count) {
	longNumber retVal;
	int d;
	retVal.count = count;
	retVal.value = (char *)malloc(retVal.count * sizeof(char));
	for(d=0;d<retVal.count;d++)
		retVal.value[d] = '0';
	retVal.value[retVal.count] = 0x00;	// End Of Line: 0x00
	return retVal;
}

longNumber longNumberAdd(longNumber number1, longNumber number2) {
	longNumber retVal;
	int maxDigits, d;
	int carry = 0;

	if(number1.count > number2.count)
		maxDigits = number1.count;
	else
		maxDigits = number2.count;

	retVal = longNumberWithCount(maxDigits+1);	// +1 is to account for max value rollover

	for(d=0;d<retVal.count;d++) {
		int sum = 0;
		int value1 = 0, value2 = 0;
		sum+=carry;
		carry = 0;
		if(number1.count >= d+1) {
			char temp[1];
			temp[0] = number1.value[number1.count-d-1];
			value1 = atoi(temp);
		}
		if(number2.count >= d+1) {
			char temp[1];
			temp[0] = number2.value[number2.count-d-1];
			value2 = atoi(temp);
		}
		sum += value1+value2;
		if(sum > 9) {
			carry = 1;	// Addition can't go over a carry of 1. E.G. 9+9+1(carry) = 19
			sum-=10;
		}
		if(sum == 0)
			retVal.value[maxDigits-d] = '0';
		if(sum == 1)
			retVal.value[maxDigits-d] = '1';
		if(sum == 2)
			retVal.value[maxDigits-d] = '2';
		if(sum == 3)
			retVal.value[maxDigits-d] = '3';
		if(sum == 4)
			retVal.value[maxDigits-d] = '4';
		if(sum == 5)
			retVal.value[maxDigits-d] = '5';
		if(sum == 6)
			retVal.value[maxDigits-d] = '6';
		if(sum == 7)
			retVal.value[maxDigits-d] = '7';
		if(sum == 8)
			retVal.value[maxDigits-d] = '8';
		if(sum == 9)
			retVal.value[maxDigits-d] = '9';
	}

	return retVal;
}

void main() {
	longNumber value1 = longNumberWith("87345676543456774345678434565445676543345678765433456789095432345674238957283957230985723758");
	longNumber value2 = longNumberWith("23421113456654356532876345678909876543234567898765432345678987654323456789090987876545434323");
	longNumber value3 = longNumberAdd(value1, value2);
	printf("Sum: %s\n", value3.value);
}
