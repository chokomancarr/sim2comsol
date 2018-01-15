#include "Network.hpp"
#include <fstream>
#include <string>

Node::Node(Layer* p, bool usesig) : usesig(usesig) {
	if (!p) {
		bias = 0;
		return;
	}
	parents = std::vector<Node*>(p->nodes);
	size = parents.size();
	weights.resize(size);
	dw.resize(size);
	for (uint a = 0; a < size; a++) weights[a] = Net::me->distri(Net::me->device);
	bias = 0;
}

void Node::Calc() {
	output = 0;
	doutput = 0;
	value = bias;
	for (uint a = 0; a < size; a++) {
		value += parents[a]->output*weights[a];
		dw[a] = parents[a]->output;// / 50 / size;
	}
	output = usesig? sigmoid(value) : value;
	double o2 = usesig ? sigmoid(value + 0.0001) : (value + 0.0001);
	doutput2 = (o2 - output) / 0.0001;
}

void Node::Cost(double tar, double _a) {
	double res = 0.5f * pow(output - tar, 2);
	//double out2 = usesig ? sigmoid(value + _a) : (value + _a);
	double res2 = 0.5f * pow(output + _a - tar, 2);

	cost = res;
	//double v2 = usesig ? isigmoid(output + _a) : output + _a;
	doutput = (res2 - res) / _a;
}

void Node::BP(double _a) {
	double err = doutput * doutput2 * _a;
	//double err = doutput * _a;
	bias -= err * _a;
	for (uint a = 0; a < size; a++) {
		weights[a] -= err * dw[a] * _a;
		parents[a]->doutput += err * weights[a];
	}
}


Layer::Layer(uint sz, Layer* p, bool usesig) {
	size = sz;
	nodes.resize(sz);
	for (uint a = 0; a < size; a++) {
		nodes[a] = new Node(p, usesig);
	}
}

void Layer::Set(const double* vals) {
	for (uint a = 0; a < size; a++) {
		nodes[a]->output = vals[a];
	}
}

void Layer::Calc() {
	for (uint a = 0; a < size; a++) {
		nodes[a]->Calc();
	}
}

void Layer::Clc() {
	for (uint a = 0; a < size; a++) {
		nodes[a]->doutput = 0;
		nodes[a]->doutput2 = 0;
		nodes[a]->cost = 0;
		for (uint b = 0; b < nodes[a]->dw.size(); b++) {
			nodes[a]->dw[b] = 0;
		}
	}
}

void Layer::Cost(const double* tars, double _a) {
	for (uint a = 0; a < size; a++) {
		nodes[a]->Cost(tars[a], _a);
	}
}

void Layer::MulCost(const double& val) {
	for (uint a = 0; a < size; a++) {
		nodes[a]->cost *= val;
		nodes[a]->doutput *= val;
		nodes[a]->doutput2 *= val;
	}
}

void Layer::BP(const double _a) {
	for (uint a = 0; a < size; a++) {
		nodes[a]->BP(_a);
	}
}


Net* Net::me = nullptr;

Net::Net(uint ls, uint* ns) : size(ls) {
	me = this;
	distri = std::normal_distribution<double>(0, 0.01);
	std::random_device rd{};
	device = std::mt19937(rd());

	layers.resize(ls);
	layers[0] = new Layer(ns[0]);
	for (uint a = 1; a < size; a++) {
		layers[a] = new Layer(ns[a], layers[a - 1], a != (size - 1));
	}
}

void Net::Eval(const double* vals) {
	layers[0]->Set(vals);
	for (uint a = 1; a < size; a++) {
		layers[a]->Calc();
	}
}

void Net::BP(uint cnt, const std::vector<std::pair<std::vector<double>, double>>& set, double _a) {
	cost = 0;
	for (uint a = 0; a < cnt; a++) {
		Eval(&set[a].first[0]);
		layers[size - 1]->Cost(&set[a].second, _a); //we have only 1 target anyway
		for (uint a = size - 1; a > 0; a--) {
			layers[a]->BP(_a);
		}
		cost += layers[size - 1]->nodes[0]->cost;
	}
}

#define _space  << " "
#define _nl  << std::endl

void Net::Save(const std::string& path) {
	std::ofstream strm(path, std::ios::binary);
	strm << size _nl;
	
	for (auto& l : layers) {
		strm _space << l->size _nl;
		for (auto& n : l->nodes) {
			strm _space _space << (n->usesig? 1 : 0);
			for (auto& w : n->weights) {
				strm _space << std::to_string(w);
			}
			strm _space << std::to_string(n->bias) _nl;
		}
	}
}