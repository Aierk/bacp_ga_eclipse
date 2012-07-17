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
	if (argc != 2)
	{
		cout << "BACP - GA" << endl;
		cout << "Autor: Sergio A. Morales - 2773021-3" << endl;
		cout << "Se debe pasar como parametro la ruta al archivo CSPLib con la data del problema" << endl;
		cout << "** Solo utilizar archivos oficiales (no se asegura nada en caso contrario) **" << endl << endl;
		cout << "Output por defecto: Genracion Calidad #Evaluaciones Castigos Max.Carga.Periodo" << endl << endl;
		return 1;
	}
	Config conf(argv[1]);
	conf.calc_min_period();
	conf.calc_max_balance();
	GA metodo (30,100, &conf, 0.6, 0.7);
	metodo.run();
	conf.printParameters();
}



