#include <iostream>
#include "PTron.h"

// This is an interesting example. Adds two binary digits using only PTrons
// Diagram for this: http://neuralnetworksanddeeplearning.com/chap1.html
// x1 and x2 act as bits in the adder. Set them to either true or false
void PTronNANDAdder(bool x1, bool x2) {
	PTron *ptron_1_1 = new PTron(x1);
	PTron *ptron_1_2 = new PTron(x2);

	PTron *ptron_2_1 = new PTron(1, 3);
	ptron_2_1->addInput(ptron_1_1, -2);
	ptron_2_1->addInput(ptron_1_2, -2);

	PTron *ptron_3_1 = new PTron(1, 3);
	ptron_3_1->addInput(ptron_1_1, -2);
	ptron_3_1->addInput(ptron_2_1, -2);

	PTron *ptron_3_2 = new PTron(1, 3);
	ptron_3_2->addInput(ptron_2_1, -2);
	ptron_3_2->addInput(ptron_1_2, -2);

	// This PTron is the left most bit (the carry bit)
	PTron *ptron_3_3 = new PTron(1, 3);
	//ptron_3_3->addInput(ptron_2_1, -4);	Can also just combine the weights from two references of -2 to become one reference of -4
	ptron_3_3->addInput(ptron_2_1, -2);
	ptron_3_3->addInput(ptron_2_1, -2);

	// This PTron is the right most bit
	PTron *ptron_4_1 = new PTron(1, 3);
	ptron_4_1->addInput(ptron_3_1, -2);
	ptron_4_1->addInput(ptron_3_2, -2);

	bool leftBit = ptron_3_3->getOutput();
	bool rightBit = ptron_4_1->getOutput();

	std::cout << "Adder input: ";
	if(x1)std::cout << "1";
	else std::cout << "0";
	std::cout << " + ";
	if(x2)std::cout << "1";
	else std::cout << "0";

	std::cout << std::endl;

	std::cout << "The adder reports: ";
	if(leftBit)std::cout << "1";
	else std::cout << "0";
	if(rightBit)std::cout << "1";
	else std::cout << "0";

	std::cout << std::endl;
}

void PTronXOR(bool x1, bool x2) {
	PTron *ptron_1_1 = new PTron(x1);
	PTron *ptron_1_2 = new PTron(x2);
}
