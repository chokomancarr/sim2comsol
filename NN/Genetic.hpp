#pragma once
#include "Network.hpp"
#include <array>

class Gene {
public:
	Gene(bool gen = true);

	std::array<double, 10> values;
	double score;

	void Eval(Net* net);
	void Mutate(uint cnt);
};

class GN {
public:
	GN(uint cnt, uint kill, uint rand, uint randc);

	void Eval(Net* net), Mutate();

	std::vector<Gene*> genes;
	uint geneSz, killSz, randSz, randC;
};
