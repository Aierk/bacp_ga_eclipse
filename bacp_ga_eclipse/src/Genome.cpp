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
	all_done = 0;
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

void Genome::print_me(Config& config)
{
	for (int i = 1; i <= config.periods; i++)
	{
		cout << "Periodo " << i << " : ";
		for (int j = 0; j < config.courses.size(); j++)
		{
			if (Chromosome[j] == i) cout << config.courses[j] << " - ";
		}
		cout << endl;
	}
	cout << endl;;
}

bool Genome::is_ready()
{
	for (int i=0; i < Chromosome.size();i++)
	{
		if (Chromosome[i] == 0)
		{
			return false;
		}
	}
	return true;
}

int Genome::calc_all_done()
{
	all_done = 0;
	for (int i = 0; i < Chromosome.size(); i++)
	{
		if(Chromosome[i] == 0) all_done++;
	}
}

void Genome::gen_new(Config& config)
{
	calc_all_done();

	bool problem = false;
	int sum_restantes = 0;//Almacena la suma de weas sin instanciar
	sum_restantes = all_done;
	unsigned long int iseed;
	while(!is_ready())
	{
		for (int i = 1; i <= config.periods; i++) //Iterecion periodo
		{
			vector<int> ramos_instanciables;
			for (int j =0; j < config.courses.size(); j++) // iteración de ramos
			{
				if (Chromosome[j] != 0) continue; //ramo instanciado, nada que hacer

				bool allgood = true;
				if (config.prereq[j].size() != 0) //Tiene prerequisitos
				{
					for (int k=0; k < config.prereq[j].size(); k++) //  iteracion en prerquisitos
					{
						if (Chromosome[config.prereq[j][k]] >= i) //requisito instanciado en el mismo semestreo o superior
						{
							allgood = false;
						}
						if (Chromosome[config.prereq[j][k]] == 0)
						{
							allgood = false;
						}
					}
				}
				if (!allgood) continue; // No agregar como curso posible.
				if (config.periods - i < config.max_period[j]) continue;
				//En este punto, No esta instanciado y sus requisitos estan OK. Se agrega al arreglo de posibles
				ramos_instanciables.push_back(j);
			} // Fin de la creación de ramos instanciables.

			//Iniciamos la instanciación

			if (ramos_instanciables.size() == 0)
				{
					//problem = true;
					continue;
				}
			problem = false;
			//TODO: Es posible 1 ramo que no caiga en ningun lado. WHILE infinito solucionar
			int total_creditos = 0;
			int total_ramos = 0;
			for (int w=0; w < Chromosome.size(); w++)
			{
				if (Chromosome[w] == i)
				{
					total_ramos = total_ramos+1;
					total_creditos = total_creditos + config.credits[w];
				}
			}
			//iseed = (iseed + time(0));
			//srand(iseed); //Problema
			int pos_candidato = rand()%ramos_instanciables.size();
			int candidato = ramos_instanciables[pos_candidato];
			if (total_creditos + config.credits[candidato] > config.max_load) {}
			else if (total_ramos + 1 > config.max_courses) {}
			else
			{
				Chromosome[candidato] = i;
				total_ramos = total_ramos + 1;
				total_creditos = total_creditos + config.credits[candidato];
			}
			ramos_instanciables.erase(ramos_instanciables.begin()+pos_candidato);
		}
		calc_all_done();
		if(all_done == sum_restantes) break; //Si en un ciclo completo no se instancio nada.. no se encontro solucin
		sum_restantes = all_done; //Actualiza el valor del # de ramos sin instanciar despues deun ciclo completo
		if(problem) break;
	}

}


