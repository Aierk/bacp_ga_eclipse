/*
 * GA.cpp
 *
 *  Created on: 08/07/2012
 *      Author: Sergio A. Morales
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
	this->elite_restart = 0.0;
}

GA::~GA() {
}

void GA::initial_population()
{
	/* Generar una poblacion donde todos los ramos esten instanciados */
	int i = 0;
	while(i < this->size)
	{
		Genome g (this->config);
		g.gen_new();
		if(g.faltan_por_instanciar == 0)
		{
			this->population.push_back(g);
			i++;
		}
	}

}
void GA::elite()
{
	/* Busca el mejor y lo agrega la siguiente generación */
	float min = 0.0;
	min = this->population[0].fitness;
	for(int i=1; i < this->size; i++ )
	{
		if (this->population[i].fitness < min) min = this->population[i].fitness;
	}
	for(int i=0; i < this->size; i++ )
	{
		if (this->population[i].fitness == min)
		{
			this->to_new_pop(this->population[i].Chromosome);
			return;
		}
	}
}
void GA::print_elite()
{
	/* Busca el mejor e imprime la data importante */
	float min = 0.0;
	min = this->population[0].fitness;
	for(int i=1; i < this->size; i++ )
	{
		if (this->population[i].fitness < min) min = this->population[i].fitness;
	}
	for(int i=0; i < this->size; i++ )
	{
		if (this->population[i].fitness == min)
		{
			std::cout << this->population[i].fitness << " ";
			std::cout << this->population[i].my_count << " ";
			//std::cout << "- Castigos: " << this->population[i].castigos;
			//std::cout << "- Max load: " << this->population[i].max_period_load << std::endl;
			std::cout << this->population[i].castigos << " ";
			std::cout << this->population[i].max_period_load << std::endl;
			this->elite_restart = this->population[i].fitness;
			return;
		}
	}
}

void GA::set_rfitness()
{
	/* Genera la torta para la selección */
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
	/* Imprime soluciones de la generación actual */
	for(int i=0; i < this->size; i++ )
	{
		this->population[i].print_me();
	}
}
void GA::print_new_population()
{
	/* Imprime soluciones de la siguiente generación */
	for(size_t i=0; i < this->new_population.size(); i++ )
	{
		this->new_population[i].print_me();
	}
}
vector <int> GA::select_subject()
{
	/* Utilizando la torta, selecciona un individuo */

	float p = rand()%1000 / 1000.00;
	if ( p < this->population[0].c_fitness) return this->population[0].Chromosome;
	for(int i=1; i < this->size-1 ; i++ )
	{
		if (p >= this->population[i].c_fitness && p < this->population[i+1].c_fitness)
			{
				return this->population[i].Chromosome;
			}
	}
	return this->population[this->size -1].Chromosome;

}
void GA::to_new_pop(vector <int> c)
{
	/* Toma un cromosoma, lo convierte en Genome y lo agrega a la siguiente generacion */
	if ((int) this->new_population.size() >= size) return;
	Genome g (c,this->config);
	this->new_population.push_back(g);
}

void GA::mutate()
{
	/*Mutación simple.
	 * Contiene posibles mejoras utilizadas en los experimentos
	 */
	float p = rand()%1000 / 1000.0;
	if (p <= this->mutate_ok)
	{
		Genome g (this->select_subject(),this->config);
		int periodo = 0;
		size_t g_size = g.Chromosome.size();
		for (size_t i=0; i < g_size; i++)
		{
			p = rand()%1000 / 1000.0;
			if (p > this->mutate_ok) continue; //probabilidad para cada cromosoma
			periodo = (rand()%this->config->periods) + 1;
			while (periodo == g.Chromosome[i])
			{
				periodo = (rand()%this->config->periods) + 1;
			}
			//if (g.credits_per_period[periodo] < this->config->min_load) continue; // si ya tiene muy pocos.
			//if (g.credits_per_period[periodo] > this->config->max_load) continue; // si ya tiene muchos
			g.Chromosome[i] = periodo;
			g.re_calc_stats();
		}

		this->to_new_pop(g.Chromosome);
	}
}

void GA::mutate_smart()
{
	/*
	 * Mutación inteligente con respecto a los prerequisitos
	 * Se aplica a todos una vez activada la tasa de mutacion
	 * Intenta mejoras 4 veces, si no muta igual
	 */
	int counter = 0;
	float p = rand()%1000 / 1000.0;
	if (p <= this->mutate_ok)
	{
		Genome g (this->select_subject(),this->config);
		int periodo = 0;
		size_t g_size = g.Chromosome.size();
		p = rand()%1000 / 1000.0;
		for (size_t i=0; i < g_size; i++)
		{
			if(this->config->skeleton[i] == 1) continue;
			periodo = (rand()%this->config->periods) + 1;
			while (periodo == g.Chromosome[i])
			{
				periodo = (rand()%this->config->periods) + 1;
			}
			Genome test (g.Chromosome,this->config);
			test.Chromosome[i] = periodo;
			test.re_calc_stats();
			if(test.fitness > g.fitness && counter != 4)
			{
				i--;
				counter++;
			}else
			{
				counter = 0;
				g.Chromosome[i] = periodo;
				g.re_calc_stats();
				p = rand()%1000 / 1000.0;
			}
		}

		this->to_new_pop(g.Chromosome);
	}
}

void GA::cross_over()
{
/*
 * Cross_over simple en un punto del cromosoma
 */
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
		while(pos == 0 || pos == (int)a.size() -1 ) pos = rand()% (int)a.size();

		for (int i=0; i < (int) a.size(); i++)
		{
			if (pos >= i)
			{
				c[i] = a[i];
				d[i] = b[i];
			}
			else
			{	/* Tener en cuenta el equeleto a la hora de cruzar */
				if(this->config->skeleton[i] == 0){
					d[i] = a[i];
					c[i] = b[i];
				}else
				{
					c[i] = a[i];
					d[i] = b[i];
				}

			}
		}
		this->to_new_pop(c);
		this->to_new_pop(d);
	}
}


void GA::cross_over_period()
{
	/*
	 * Cruza por periodo
	 * Inteligente con respecto al esqueleto
	 */
	float p = rand()%1000 / 1000.0;
	if (p > this->cross_over_ok) return;
	// C herada fuertemente de a
	// d hereda fuertemente de b
	vector <int> a;
	vector <int> b;
	a = this->select_subject();
	b = this->select_subject();
	vector <int> c (a.size());
	vector <int> d (b.size()) ;

	for (size_t i = 0; i < a.size(); i++)
	{
		c[i] = a[i];
		d[i] = b[i];
	}

	int pos = (rand() % this->config->periods) + 1;
	while(pos == 1 || pos == this->config->periods) pos = (rand() % this->config->periods) + 1; //Semestre random elegido

	for (int i = pos; i <= this->config->periods; i++)
	{
		for (size_t i = 0; i < a.size(); i++)
		{
				if( this->config->skeleton[i] == 1) continue;
				if(a[i] == pos)
				{
					d[i] = a[i];
					c[i] = b[i];
				}
				if(b[i] == pos)
				{
					c[i] = b[i];
					d[i] = a[i];
				}
		}

	}
	this->to_new_pop(c);
	this->to_new_pop(d);
}

void GA::run()
{
	this->initial_population();
	this->set_rfitness();
	int generation = 1;

	float last_elite;
	last_elite = -1.0;

	/*
	int n = 0;
	int m = 0;
	float orignal_mutate = this->mutate_ok;*/
	while(generation <= this->generations)
	{
		std::cout << generation << " ";
		this->print_elite();
		this->elite();
		/*(last_elite == this->elite_restart) ? n++ : m++;

		if (n>15)
		{
			n=0;
			this->mutate_ok = this->mutate_ok * 2;
		}else {
			this->elite();
			if (m > 3)
			{
				m = 0;
				this->mutate_ok = orignal_mutate;
			}

		}*/ // Experimentos de reset de elitismo y mutacion


		while ((int)this->new_population.size() != this->size)
		{
			//this->mutate();
			this->cross_over_period();
			//this->cross_over();
			this->mutate_smart();
		}
		this->population.clear();
		this->population = this->new_population;
		this->set_rfitness();
		this->new_population.clear();
		generation++;
		last_elite = this->elite_restart;
	}
}
