#include <iostream>
#include <fstream>
#include <time.h>
#include <random>

#include "Network.hpp"

std::vector<std::pair<std::vector<double>, double>> dataset;
uint datasetSz;

void ReadDataFile() {
	std::ifstream strm("D:\\data.txt", std::ios::binary);
	double f;
	while (1) {
		strm >> f;
		if (strm.eof()) return;
		std::pair<std::vector<double>, double> data, data2;
		data.first.resize(10, f);
		data2.first.resize(10, f);
		for (uint a = 1; a < 10; a++) {
			strm >> data2.first[a];
			if (strm.eof()) return;
		}
		
		data.first[0] = (data2.first[0] / 2 + data2.first[1])/50;
		data.first[1] = data2.first[1] / 50;
		data.first[2] = (data2.first[2] + data2.first[3]) / 100;
		data.first[3] = data2.first[3] / 100;
		data.first[4] = data2.first[4] / 150;
		data.first[6] = data2.first[6] * 10;
		data.first[7] = data2.first[7] * 10;
		data.first[8] = data2.first[8] / 10000;
		data.first[9] = data2.first[9] / 2;

		for (uint a = 1; a < 10; a++) {
			data.first[a] = data.first[a] * 10 - 5;
		}

		strm >> f;
		data.second = (log10(f) / 2 - 2) / 2.5;
		if (strm.eof()) return;

		dataset.push_back(data);
		datasetSz++;
	}
}

Net* net;

int main() {
	srand(time(NULL));

	ReadDataFile();

	uint szs[3] = { 10, 20, 1 };
	net = new Net(3, szs);

	for (uint i = 0; i < 200; i++) {
		for (uint a = 0; a < 50; a++) {
			net->BP(datasetSz, dataset, 0.05f);
			//for (uint i = 0; i < 20; i++) {
			std::cout << i << ": total cost = " << net->layers[2]->nodes[0]->cost << std::endl;
			//}
			//std::cout << std::endl;
		}
		//std::cout << i << ": total cost = " << net->layers[2]->nodes[0]->cost << std::endl;
	}

	net->Save("D:\\comsolnet.txt");

	return 0;
}