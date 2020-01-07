#include <iostream>
#include <vector>

#include "EasyBMP.h"

using namespace std;

int main() {
	BMP bmp;
	bmp.SetSize(100, 100);
    bmp.SetBitDepth(24);

	bmp(0, 0)->Red = 255;
	bmp(0, 0)->Green = 127;
	bmp(0, 0)->Blue = 0;

	bmp.WriteToFile("output.bmp");

	return 0;
}
