/*
 * GA.cpp
 *
 *  Created on: 08/07/2012
 *      Author: smorales
 */

#include "GA.h"
#include "Genome.h"
#include <cstdlib>
#include <iostream>


GA::GA(int s, int g, Config* c, float mut, float cross) {
	this->size = s;
	this->generations = g;
	this->config = c;
	this->mutate_ok = mut;
	this->cross_over_ok = cross;
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
			i++;
		}
	}

}
void GA::elite()
{
	float max = 0.0;
	for(int i=0; i < this->size; i++ )
	{
		if (this->population[i].fitness > max) max = this->population[i].fitness;
	}
	for(int i=0; i < this->size; i++ )
	{
		if (this->population[i].fitness == max)
		{
			this->to_new_pop(this->population[i].Chromosome);
			return;
		}
	}
}

void GA::set_rfitness()
{
	float total = 0.0;
	for(int i=0; i < this->size; i++ ) //suma inicial de todos los fitness
	{
		total = total + this->population[i].i_fitness;
	}
	for(int i=0; i < this->size; i++ ) //calculo efectivo del rfitness;
	{
		this->population[i].r_fitness = this->population[i].i_fitness / total;
	}

	this->population[0].c_fitness = this->population[0].r_fitness; // Con el primero no se hace nada
	for(int i=1; i < this->size; i++ )
	{
		this->population[i].c_fitness = this->population[i-1].c_fitness + this->population[i].r_fitness;
	}
}
void GA::print_population()
{
	for(int i=0; i < this->size; i++ )
	{
		this->population[i].print_me();
	}
}
void GA::print_new_population()
{
	for(size_t i=0; i < this->new_population.size(); i++ )
	{
		this->new_population[i].print_me();
	}
}
vector <int> GA::select_subject()
{

	float p = rand()%1000 / 1000.0;
	if ( p < this->population[0].c_fitness) return this->population[0].Chromosome;
	for(int i=0; i < this->size -1 ; i++ )
	{
		if (p >= this->population[i].c_fitness && p < this->population[i+1].c_fitness) return this->population[i].Chromosome;
	}
	return this->population[this->size -1].Chromosome;
}
void GA::to_new_pop(vector <int> c)
{
	if (this->new_population.size() >= size) return;
	Genome g (c,this->config);
	this->new_population.push_back(g);
}

void GA::mutate()
{
	float p = rand()%1000 / 1000.0;
	if (p <= this->mutate_ok)
	{
		Genome g (this->select_subject(),this->config);
		int periodo = 0;
		int g_size = (int) g.Chromosome.size();
		for (size_t i=0; i < g_size; i++)
		{
			p = rand()%1000 / 1000.0;
			if (p > this->mutate_ok) continue; //probabilidad para cada cromosoma

			periodo = ( rand()%(this->config->periods + 1) ) + 1;

			if (g.credits_per_period[periodo] < this->config->max_load) continue; // si ya tiene muy pocos.

			while (periodo != g.Chromosome[i])
			{
				if (g.credits_per_period[periodo] < this->config->max_load) // para que no tenga quede con muchos
				{
					g.Chromosome[i] = periodo;
					break;
				}
				periodo = ( rand()%(this->config->periods + 1) ) + 1; // En caso que sea el mismo
			}
			g.re_calc_stats();
		}

		this->to_new_pop(g.Chromosome);
	}
}

void GA::cross_over()
{
	float p = rand()%1000 / 1000.0;
	if (p <= this->cross_over_ok)
	{
		vector <int> a;
		vector <int> b;
		vector <int> c;
		vector <int> d;
		a = this->select_subject();
		b = this->select_subject();
		c.resize(a.size());
		d.resize(b.size());


		int pos = rand()% (int)a.size();

		for (size_t i=0; i < a.size(); i++)
		{
			if (pos >= i)
			{
				c[i] = a[i];
				d[i] = b[i];
			}
			else
			{
				d[i] = a[i];
				c[i] = b[i];
			}
		}
		this->to_new_pop(c);
		this->to_new_pop(d);

	}
}
void GA::run()
{
	this->initial_population();
	this->set_rfitness();

	int generation = 1;
	while(generation <= this->generations)
	{
		std::cout << "Mejor de la generacion " << generation << std::endl;
		this->elite();
		this->new_population[0].print_me();
		while (this->new_population.size() != this->size)
		{
			this->mutate();
			this->cross_over();
		}
		this->population.clear();
		this->population = this->new_population;
		this->set_rfitness();
		this->new_population.clear();
		generation++;
	}
}
