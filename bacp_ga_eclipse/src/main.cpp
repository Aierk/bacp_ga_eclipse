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

int main (int argc, char const *argv[])
{
	Config conf(argv[1]);
//	conf.printParameters();
	conf.calc_min_period();

	//g.gen_chromosome(conf);
	int n = 0;
	while(n != 50)
	{
		Genome* g = new Genome (conf.courses.size());
		g->gen_new(conf);
//		cout << g->all_done << std::endl;
		cout << time(NULL) << std::endl;
		if (g->all_done == 0){
			g->print_me(conf);
			n++;
		}
		delete g;
	}
}



