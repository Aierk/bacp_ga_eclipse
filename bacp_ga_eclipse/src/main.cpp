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
#include "GA.h"
#include <ctime>
#include <cstdlib>

int main (int argc, char const *argv[])
{
	srand(time(0));
	Config conf(argv[1]);
//	conf.printParameters();
	conf.calc_min_period();
	conf.calc_max_balance();

	GA metodo (8,10, &conf);
	metodo.initial_population();
}



