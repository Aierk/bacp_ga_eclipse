/*
 * Config.cpp
 *
 *  Created on: 29-06-2012
 *      Author: sergio
 */

#include "Config.h"
#include "Helper.h"
#include <iostream>
#include <fstream>
using std::ifstream;
using std::getline;
using std::cout;
using std::endl;

Config::Config(char const* path) {
	file.open(path);
	if (!file.good())
		file_ok = false;
}

Config::~Config() {
}

void Config::getData()
{
	bool more_const = true;
	int equal;
	int colon;
	std::string pre_value;
	while(more_const)
	{
        getline(file,line);
        if (! line.length()) continue;
        if (line[0] == '/') continue;
        if (line[0] == ' ') continue;
        if (line[0] == '%') continue;

        if (line[0] == 'p')
        {
        	equal = line.find('=');
        	colon = line.find(';');
        	ss << helper.trim(line.substr(equal+1,colon));
        	ss >> periods;
        	ss.clear();
        }
        if (line[0] == 'a')
        {
        	equal = line.find('=');
        	colon = line.find(';');
        	ss << helper.trim(line.substr(equal+1,colon));
        	ss >> min_load;
        	ss.clear();
        }
        if (line[0] == 'b')
        {
        	equal = line.find('=');
        	colon = line.find(';');
        	ss << helper.trim(line.substr(equal+1,colon));
        	ss >> max_load;
        	ss.clear();
        }
        if (line[0] == 'c')
        {
        	equal = line.find('=');
        	colon = line.find(';');
        	ss << helper.trim(line.substr(equal+1,colon));
        	ss >> min_courses;
        	ss.clear();
        }
        if (line[0] == 'd')
        {
        	equal = line.find('=');
        	colon = line.find(';');
        	ss << helper.trim(line.substr(equal+1,colon));
        	ss >> max_courses;
        	ss.clear();
        	more_const = false;
        }
	}
}

void Config::printParameters()
{
	cout << "Parametros leidos: " << endl;
	cout << "Periodos: " << periods << endl;
	cout << "Min load: " << min_load << endl;
	cout << "Max load: " << max_load << endl;
	cout << "Min Courses: " << min_courses << endl;
	cout << "Max Courses: " << max_courses << endl;
}

