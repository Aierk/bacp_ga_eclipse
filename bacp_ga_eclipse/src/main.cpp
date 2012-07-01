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
	conf.getData();
	conf.printParameters();
	Genome g (conf.courses.size());
	g.gen_chromosome(conf);
	g.print_me();
}



