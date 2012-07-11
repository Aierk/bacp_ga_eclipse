/*
 * main.cpp
 *
 *  Created on: 29-06-2012
 *      Author: sergio
 */

#include <iostream>
using std::cout;
using std::endl;
#include "Config.h"
#include "Genome.h"
#include "Helper.h"
#include "GA.h"
#include <ctime>
#include <cstdlib>

int main (int argc, char const *argv[])
{
	srand(time(0));

	Config conf(argv[1]);
	conf.calc_min_period();
	conf.calc_max_balance();

	//50,60,0.4,07 se ve bien
	GA metodo (40,200, &conf, 0.2, 0.8);
	metodo.run();
	conf.printParameters();
}



