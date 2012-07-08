/*
 * GA.cpp
 *
 *  Created on: 08/07/2012
 *      Author: smorales
 */

#include "GA.h"
#include "Genome.h"


GA::GA(int s, int g, Config* c) {
	this->size = s;
	this->generations = g;
	this->config = c;
	//this->new_population.resize(this->size);
	//this->population.reserve(this->size);
}

GA::~GA() {
	// TODO Auto-generated destructor stub
}

void GA::initial_population()
{
	int i = 0;
	while(i < this->size)
	{
		Genome g (this->config);
		g.gen_new();
		if(g.all_done == 0)
		{
			this->population.push_back(g);
			g.print_me();
			i++;
		}
	}

}
void GA::elite()
{

}
