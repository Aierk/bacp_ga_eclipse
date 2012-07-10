/*
 * GA.h
 *
 *  Created on: 08/07/2012
 *      Author: smorales
 */
#include "Genome.h"
#include <vector>
#include "Config.h"
#include "Helper.h"
#ifndef GA_H_
#define GA_H_

class GA {
public:
	vector <Genome> population;
	vector <Genome> new_population;
	GA(int s, int g, Config* c, float mut, float cross);
	void initial_population();
	virtual ~GA();
	void elite();
	void set_rfitness();
	void print_population();
	void print_new_population();
	vector <int> select_subject();
	void to_new_pop(vector <int> c);
	void mutate();
	void cross_over();
	void run();
protected:
	Config* config;
	int size;
	int generations;
	float mutate_ok;
	float cross_over_ok;
	Helper helper;
};

#endif /* GA_H_ */
