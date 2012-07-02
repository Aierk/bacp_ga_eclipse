/*
 * Genome.cpp
 *
 *  Created on: 01-07-2012
 *      Author: sergio
 */

#include "Genome.h"
#include "Config.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

Genome::Genome(){
};

Genome::Genome(int size ) {
	Chromosome.resize(size);
	for (int  i = 0; i< size; i++)
	{
		Chromosome[i] = 0;
	}
}

Genome::~Genome() {
	// TODO Auto-generated destructor stub
}

void Genome::gen_chromosome(Config& config)
{
	srand(time(0));
	for (int i = 1; i <= config.periods; i++) //Iterecion periodo
	{
		vector<int> ramos_instanciables;
		for (int j =0; j < config.courses.size(); j++) // iteración de ramos
		{
			if (Chromosome[j] != 0) continue; //ramo instanciado, nada que hacer

			if (config.prereq[j].size() != 0) //Tiene prerequisitos
			{
				for (int k=0; k < config.prereq[j].size(); k++) //  iteracion en prerquisitos
				{
					if (Chromosome[config.prereq[j][k]] == 0 || Chromosome[config.prereq[j][k]] == i) // Un prerequisito no instanciado salimos de for de ramos
					{
						continue;
					}
				}
			}
			if (config.periods - i < config.max_period[j]) continue;
			//En este punto, No esta instanciado y sus requisitos estan OK. Se agrega al arreglo de posibles
			ramos_instanciables.push_back(j);
		} // Fin de la creación de ramos instanciables.

		//Iniciamos la instanciación

		int total_creditos = 0;
		int total_ramos = 0;

		while(ramos_instanciables.size() != 0)
		{
			int pos_candidato = rand()%ramos_instanciables.size();
			int candidato = ramos_instanciables[pos_candidato];
			if (total_creditos + config.credits[candidato] > config.max_load) {}
			else if (total_ramos + 1 > config.max_courses) {}
			else
			{
				Chromosome[candidato] = i;
				total_ramos = total_ramos + 1;
				total_creditos = total_creditos + config.credits[candidato];
				cout << i << ": " << config.courses[candidato] << endl;
			}
			ramos_instanciables.erase(ramos_instanciables.begin()+pos_candidato);
		}
	}

}

void Genome::print_me()
{
	cout << "[";
	for(int i=0; i < Chromosome.size(); i++)
	{
		cout << Chromosome[i] << ",";
	}
	cout << "]" << endl;
}
