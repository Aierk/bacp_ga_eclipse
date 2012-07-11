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

Genome::Genome(vector <int> adn, Config* c)
{
	this->castigos = 0;
	this->c_fitness = 0.0;
	this->fitness = 0.0;
	this->r_fitness = 0.0;
	this->i_fitness = 0.0;
	this->all_done = 0;
	this->max_period_load = 0;
	this->config = c;
	Chromosome.resize(config->courses.size());
	for (size_t  i = 0; i< c->courses.size(); i++)
	{
		Chromosome[i] = adn[i];
	}
	this->credits_per_period.resize(this->config->periods + 1);
	for (size_t i=0; i < this->credits_per_period.size(); i++) this->credits_per_period[i] = 0;
	this->calc_credits_per_period();
	this->calc_all_done();
	this->Fitness();

}

Genome::Genome(Config* c ) {
	this->castigos = 0;
	all_done = 0;
	config = c;
	this->c_fitness = 0.0;
	this->fitness = 0.0;
	this->r_fitness = 0.0;
	this->max_period_load = 0;
	this->i_fitness = 0.0;
	Chromosome.resize(config->courses.size());
	for (size_t  i = 0; i< c->courses.size(); i++)
	{
		Chromosome[i] = 0;
	}
	this->credits_per_period.resize(this->config->periods + 1);
	for (size_t i=0; i < this->credits_per_period.size(); i++) this->credits_per_period[i] = 0;
}

Genome::~Genome() {
	// TODO Auto-generated destructor stub
}

void Genome::gen_chromosome()
{
	srand(time(0));
	for (int i = 1; i <= config->periods; i++) //Iterecion periodo
	{
		vector<int> ramos_instanciables;
		for (size_t j =0; j < config->courses.size(); j++) // iteración de ramos
		{
			if (Chromosome[j] != 0) continue; //ramo instanciado, nada que hacer

			if (config->prereq[j].size() != 0) //Tiene prerequisitos
			{
				for (size_t k=0; k < config->prereq[j].size(); k++) //  iteracion en prerquisitos
				{
					if (Chromosome[config->prereq[j][k]] == 0 || Chromosome[config->prereq[j][k]] == i) // Un prerequisito no instanciado salimos de for de ramos
					{
						continue;
					}
				}
			}
			if (config->periods - i < config->max_period[j]) continue;
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
			if (total_creditos + config->credits[candidato] > config->max_load) {}
			else if (total_ramos + 1 > config->max_courses) {}
			else
			{
				Chromosome[candidato] = i;
				total_ramos = total_ramos + 1;
				total_creditos = total_creditos + config->credits[candidato];
				cout << i << ": " << config->courses[candidato] << endl;
			}
			ramos_instanciables.erase(ramos_instanciables.begin()+pos_candidato);
		}
	}

}

void Genome::print_me()
{
	cout << "Fitness: " << this->fitness << endl;
	cout << "i.Fitness: " << this->i_fitness << endl;
	cout << "R.Fitness: " << this->r_fitness << endl;
	cout << "C.Fitness: " << this->c_fitness << endl;
	cout << "Castigos:" << this->castigos << endl;
	cout << "Max Load:" << this->max_period_load << endl;
	cout << "AllDone:" << this->all_done << endl;
	for (int i = 1; i <= config->periods; i++)
	{
		cout << "Periodo " << i << " : ";
		cout << "[" << this->credits_per_period[i] << "] ";
		for (size_t j = 0; j < config->courses.size(); j++)
		{
			if (Chromosome[j] == i) cout << config->courses[j] << " - ";
		}
		cout << endl;
	}
	cout << endl;;
}

bool Genome::is_ready()
{
	for (size_t i=0; i < Chromosome.size();i++)
	{
		if (Chromosome[i] == 0)
		{
			return false;
		}
	}
	return true;
}

void Genome::calc_all_done()
{
	all_done = 0;
	for (size_t i = 0; i < Chromosome.size(); i++)
	{
		if(Chromosome[i] == 0) all_done++;
	}

}

void Genome::calc_credits_per_period()
{
	for (size_t i=0; i < this->credits_per_period.size(); i++)
	{
		this->credits_per_period[i] = 0;
	}
	for (size_t i=0; i < this->Chromosome.size(); i++)
	{
		this->credits_per_period[this->Chromosome[i]] = this->credits_per_period[this->Chromosome[i]] + this->config->credits[i];
	}
	for (size_t i=1; i < this->credits_per_period.size(); i++) //el largo es +1;
	{
		if (this->credits_per_period[i] > this->max_period_load)
			this->max_period_load = this->credits_per_period[i];
	}
}
bool Genome::all_min_ok(int p)
{
	this->calc_credits_per_period();
	for (size_t i=1; i < this->credits_per_period.size(); i++)
	{
		if ( this->credits_per_period[i] < this->config->min_load && this->credits_per_period[p] >= this->config->min_load) return false;

	}
	return true;

}

void Genome::gen_new()
{
	calc_all_done();

	bool problem = false;
	int sum_restantes = 0;//Almacena la suma de weas sin instanciar
	sum_restantes = all_done;
	while(!is_ready())
	{
		for (int i = 1; i <= config->periods; i++) //Iterecion periodo
		{
			vector<int> ramos_instanciables;
			for (size_t j =0; j < config->courses.size(); j++) // iteración de ramos
			{
				if (Chromosome[j] != 0) continue; //ramo instanciado, nada que hacer

				bool allgood = true;
				if (config->prereq[j].size() != 0) //Tiene prerequisitos
				{
					for (size_t k=0; k < config->prereq[j].size(); k++) //  iteracion en prerquisitos
					{
						if (Chromosome[config->prereq[j][k]] >= i) //requisito instanciado en el mismo semestreo o superior
						{
							allgood = false;
						}
						if (Chromosome[config->prereq[j][k]] == 0)
						{
							allgood = false;
						}
					}
				}
				if (!allgood) continue; // No agregar como curso posible.
				if (config->periods - i < config->max_period[j]) continue; //arbol de dependencia
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
			for (size_t w=0; w < Chromosome.size(); w++)
			{
				if (Chromosome[w] == i)
				{
					total_ramos = total_ramos+1;
					total_creditos = total_creditos + config->credits[w];
				}
			}
			while(ramos_instanciables.size() != 0 && this->all_min_ok(i) )
			{
				int pos_candidato = rand()%ramos_instanciables.size();

				int candidato = ramos_instanciables[pos_candidato];
				if (total_creditos + config->credits[candidato] > config->max_load) {}
				else if (total_ramos + 1 > config->max_courses) {}
				else
				{
					Chromosome[candidato] = i;
					total_ramos = total_ramos + 1;
					total_creditos = total_creditos + config->credits[candidato];
				}
				ramos_instanciables.erase(ramos_instanciables.begin()+pos_candidato);
			}
		}
		calc_all_done();
		if(all_done == sum_restantes) break; //Si en un ciclo completo no se instancio nada.. no se encontro solucin
		sum_restantes = all_done; //Actualiza el valor del # de ramos sin instanciar despues deun ciclo completo
		if(problem) break;
	}

	//Set credits per periods
	this->Fitness();
}

void Genome::Fitness()
{
	float current_fitness = 0.0;
	this->castigos = 0;
	this->calc_credits_per_period();
	for (size_t i=1; i < this->credits_per_period.size(); i++) //parte de 1 y tiene largo +1
	{
		float temp;
		temp = this->config->max_balance - this->credits_per_period[i];
		if (temp < 0) current_fitness = current_fitness - temp;
		else current_fitness = current_fitness + temp;
	} //Calculo de sum{|p_i - w|} finalizado.

	this->fitness = current_fitness;
	this->calc_all_done(); 	// Al done es el numero de ramos no instanciados.

	this->fitness = this->fitness + this->fitness*(this->all_done/10); //castigo por ramos no instanciados

	int prereq_broken = 0;
	for (size_t i = 0; i < this->Chromosome.size(); i++)
	{
		for (size_t j=0; j < this->config->prereq[i].size(); j++)
		{
			if (this->Chromosome[this->config->prereq[i][j]] == 0) prereq_broken = prereq_broken +1;
			if (this->Chromosome[this->config->prereq[i][j]] >= this->Chromosome[i]) prereq_broken = prereq_broken +1;
		}
	}
	this->fitness = this->fitness + this->config->max_balance*(prereq_broken);
	this->castigos = prereq_broken;
	if(prereq_broken != 0) this->problem.push_back("rereq"); //sacar

	for (size_t i =1; i < this->credits_per_period.size(); i++)
	{
		if (this->credits_per_period[i] < this->config->min_load)
		{
			this->fitness = this->fitness + this->fitness*(this->config->min_load - this->credits_per_period[i])/10;
			this->castigos++;
		}

		if (this->credits_per_period[i] > this->config->max_load)
		{
			this->fitness = this->fitness + this->fitness*(this->credits_per_period[i] - this->config->max_load)/10;
			this->castigos++;
		}
	}

	//Inversor de fitness para invertir min a max.
	this->fitness = this->fitness + this->castigos;
	this->i_fitness = 1 / this->fitness;
}

void Genome::re_calc_stats()
{
	this->c_fitness = 0.0;
	this->fitness = 0.0;
	this->r_fitness = 0.0;
	this->i_fitness = 0.0;
	this->all_done = 0;
	this->calc_credits_per_period();
	this->calc_all_done();
	this->Fitness();

}



