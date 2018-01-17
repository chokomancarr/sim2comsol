#include "Genetic.hpp"
#include <algorithm>

#define _rand (double)(0.01*(rand()%100))

Gene::Gene(bool gen) {
	if (!gen) return;
	for (uint a = 0; a < 10; a++) {
		values[a] = _rand;
	}
}

void Gene::Eval(Net* net) {
	net->Eval(&values[0]);
	score = net->layers[2]->nodes[0]->output;
}

void Gene::Mutate(uint cnt) {
	std::vector<uint> op;
	for (uint a = 0; a < cnt; a++) {
		uint pos;
		do {
			pos = rand() % 10;
		} while (std::find(op.begin(), op.end(), pos) != op.end());
		values[pos] = _rand;
		op.push_back(pos);
	}
}


Gene* Breed(Gene* ga, Gene* gb) {
	Gene* g = new Gene(false);
	uint pos = rand() % 8 + 1;
	memcpy(&g->values[0], &ga->values[0], pos * sizeof(double));
	memcpy(&g->values[pos], &ga->values[pos], (10 - pos) * sizeof(double));
	return g;
}


GN::GN(uint cnt, uint kill, uint rand, uint randc) {
	geneSz = cnt;
	killSz = kill;
	randSz = rand;
	randC = randc;
	genes.resize(cnt);
	for (uint a = 0; a < cnt; a++) genes[a] = new Gene();
}

void GN::Eval(Net* net) {
	for (auto& a : genes) {
		a->Eval(net);
	}
}

void GN::Mutate() {
	//sort best to worst
	std::sort(genes.begin(), genes.end(), [](Gene* ga, Gene* gb) {
		return ga->score > gb->score;
	});

	//replace the weak
	for (uint a = killSz; a < geneSz; a++) {
		delete(genes[a]);
		uint g1 = rand() % killSz;
		uint g2 = g1;
		while (g2 == g1) g2 = rand() % killSz;
		genes[a] = Breed(genes[g1], genes[g2]);
	}

	//mutate the strong
	for (uint a = 0; a < randSz; a++) {
		genes[rand() % killSz]->Mutate(randC);
	}
}