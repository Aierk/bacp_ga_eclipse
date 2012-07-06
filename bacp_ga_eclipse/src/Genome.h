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
	Genome(Config* c);
	virtual ~Genome();
	vector <int> Chromosome;
	float fitness;
	float r_fitness;
	float c_fitness;
	void set_fitness();
	void gen_chromosome();
	void gen_new();
	void print_me();
	int all_done;
	void Fitness();
protected:
	Config* config;
	bool is_ready();
	int calc_all_done();
	vector <int> credits_per_period;
	bool all_min_ok(int p);
	void calc_credits_per_period();
};

#endif /* GENOME_H_ */
