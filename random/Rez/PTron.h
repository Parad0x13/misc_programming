#pragma once

#include <vector>
#include <map>
#include <string>

class PTron {
	std::vector<PTron *>inputPTrons;
	std::vector<double>inputWeights;

public:
	double value;
	double bias;
	double learningConstant;
	std::string ID;

	// Value will default to 1.0f so that it is ignored for non input-type Perceptrons
	PTron(double _value = 1.0f, double _bias = 1.0f);

	void log();
	void addInput(PTron *input, double weight = INT_MAX);
	double makeGuess(std::vector<double>guessInputValues, std::vector<double>guessInputWeights);
	double getOutput();
	void train(std::vector<double>trainingInputValues, double desiredResult);
};
