/*
 * GA.h
 *
 *  Created on: 08/07/2012
 *      Author: smorales
 */
#include "Genome.h"
#include <vector>
#include "Config.h"
#ifndef GA_H_
#define GA_H_

class GA {
public:
	vector <Genome> population;
	vector <Genome> new_population;
	GA(int s, int g, Config* c);
	void initial_population();
	virtual ~GA();
	void elite();
protected:
	Config* config;
	int size;
	int generations;
};

#endif /* GA_H_ */
