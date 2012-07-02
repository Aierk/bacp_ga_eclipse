/*
 * Genome.h
 *
 *  Created on: 01-07-2012
 *      Author: sergio
 */

#ifndef GENOME_H_
#define GENOME_H_
#include <vector>
using std::vector;
#include "Config.h"

class Genome {
public:
	Genome ();
	Genome(int size);
	virtual ~Genome();
	vector <int> Chromosome;
	float fitness;
	float r_fitness;
	float c_fitness;
	void set_fitness();
	void gen_chromosome(Config& config);
	void gen_new(Config& config);
	void print_me(Config& config);
protected:
	bool is_ready();
};

#endif /* GENOME_H_ */
