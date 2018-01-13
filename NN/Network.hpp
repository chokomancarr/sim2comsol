#pragma once

#include <vector>
#include <math.h>

inline double sigmoid(const double& value) {
	//return 1.0f / (1 + exp(-value));
	return value;

}

inline double isigmoid(const double& value) {
	//return -log((1.0f / value) - 1);
	return value;
}

typedef unsigned int uint;

class Node;
class Layer;
class Net;

class Node {
public:
	Node(Layer* p = nullptr, bool usesig = true);
	const bool usesig;

	std::vector<Node*> parents;
	std::vector<double> weights;
	uint size;
	double bias, value, output, target, cost, doutput, doutput2;
	void Calc();
	void Cost(double tar, double _a);
	void BP(double a);
};

class Layer {
public:
	Layer(uint sz, Layer* p = nullptr, bool usesig = true);

	std::vector<Node*> nodes;
	uint size;

	void Set(const double* vals);
	void Calc();
	void Clc();
	void Cost(const double* tars, double _a);
	void MulCost(const double& val);
	void BP(const double a);
};

class Net {
public:
	const Net* me;
	std::vector<Layer*> layers;
	uint size;

	Net(uint ls, uint* ns);

	void Eval(const double* vals);
	void BP(uint cnt, const std::vector<std::pair<std::vector<double>, double>>& set, double a);
	void Save(const std::string& path);
};