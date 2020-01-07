#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include "PTron.h"
#include "common.h"

std::vector<std::string>registeredIDs;

std::string generateAvailableID(int length) {
	static std::string a = "0123456789ABCDEF";
	std::string retVal;
	for(int i = 0;i < length;i++)
		retVal.push_back(a.at(size_t(getRand(a.size()))));

	for(std::vector<std::string>::const_iterator i = registeredIDs.begin();i != registeredIDs.end();i++) {
		if((*i).compare(retVal) == 0)
			return generateAvailableID(length);
	}

	return "PTron_" + retVal;
}

double generateAppropriateWeight() {
	return (getRand(1000) / 1000.0f) * (getRand(2)?1.0f:-1.0f);	// Range of -1 to +1
}

PTron::PTron(double _value, double _bias) {
	value = _value;
	bias = _bias;
	learningConstant = 0.01f;

	ID = generateAvailableID(4);
	registeredIDs.push_back(ID);
}

void PTron::log() {
	std::cout << "[" << ID << "] Value: " << value << "\tBias: " << bias << std::endl;

	if(inputPTrons.size() == 0) {
		std::cout << "\tThis perceptron does not have any inputs yet" << std::endl;
	}

	for(int d=0;d<inputPTrons.size();d++) {
		std::cout << "\t[Input " << inputPTrons[d]->ID << "] Value: " << inputPTrons[d]->value <<  "\tWeight: " << inputWeights[d] << std::endl;
	}

	std::cout << "\tCurrent Output: " << getOutput() << std::endl;
}

// [WARNING] Do I need to ensure PTrons being added are not also connected as outputs? Infinite loops might arise, but I'm not sure
void PTron::addInput(PTron *input, double weight) {
	if(weight == INT_MAX)
		weight = generateAppropriateWeight();
	inputPTrons.push_back(input);
	inputWeights.push_back(weight);
}

double PTron::makeGuess(std::vector<double>guessInputValues, std::vector<double>guessInputWeights) {
	// If the Perceptron doesn't have any inputs then it should pass it's intrinsic value since it is an input-type
	if(guessInputValues.size() == 0) {
		return value;
	}

	double sum = 0;

	for(int d=0;d<guessInputValues.size();d++) {
		sum += guessInputValues[d] * guessInputWeights[d];
	}
	sum += bias;

	if(sum <= 0)
		return 0;
	return 1;
}

double PTron::getOutput() {
	std::vector<double>values;
	for(int d=0;d<inputPTrons.size();d++) {
		values.push_back(inputPTrons[d]->getOutput());
	}

	return makeGuess(values, inputWeights);
}

void PTron::train(std::vector<double>trainingInputValues, double desiredResult) {
	double guess = makeGuess(trainingInputValues, inputWeights);
	double error = desiredResult - guess;

	std::cout << "PTron guessed " << guess << " with correct value of " << desiredResult << " giving an error of " << error << std::endl;

	for(int d=0;d<inputWeights.size();d++) {
		double newWeight = inputWeights[d] + error * trainingInputValues[d] * learningConstant;
		inputWeights[d] = newWeight;
	}
}
