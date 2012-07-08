/*
 * main.cpp
 *
 *  Created on: 29-06-2012
 *      Author: sergio
 */

#include <iostream>
using std::cout;
#include "Config.h"
#include "Genome.h"
#include <ctime>
#include <cstdlib>

int main (int argc, char const *argv[])
{
	srand(time(0));
	Config conf(argv[1]);
//	conf.printParameters();
	conf.calc_min_period();
	//g.gen_chromosome(conf);
	int n = 0;

	while(n != 50)
	{
		Genome* g = new Genome (&conf);
		g->gen_new();
		if (g->all_done == 0){
			g->print_me();
			n++;
		}
		delete g;
	}
}



