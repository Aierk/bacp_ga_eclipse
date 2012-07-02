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
#include <string>
using std::ifstream;
using std::getline;
using std::cout;
using std::endl;
using std::string;
using std::vector;

Config::Config(char const* path) {
	file.open(path);
	if (!file.good())
		file_ok = false;
	getData();
	max_period.resize(courses.size());
	for (int i = 0; i < courses.size(); i++)
	{
		max_period[i] = 0;
	}
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

	ss.clear();
	while (std::getline(file,line))
	{
		if (! line.length()) continue;
		if (line[0] == '/') continue;
		if (line[0] == ' ' && line.find("/") != std::string::npos) continue; //algunas pueden empezar vacias
		if (line[0] == '%') continue;
		if (line[0] == '}') break;
		if (line[0] == ']') break;

		found = line.find("courses");
		if (found!=std::string::npos) continue; //ignorar la primera linea de courses;


		ss << line; //transforma la linea en un stream

		while(getline(ss,course,',')) //parsea ","
		{
			course = helper.trim(course);
			if (!course.length()) continue; // por si queda en blanco
			courses.push_back(course);
	    }
		ss.clear();
		found = line.find("}");
		if (found!=std::string::npos) break; //encontro un }, termina este bloeuque
	}
	while (std::getline(file,line))
	{
		if (! line.length()) continue;
		if (line[0] == '/') continue;
		if (line[0] == ' ' && line.find("/") != std::string::npos) continue; //algunas pueden empezar vacias
		if (line[0] == '%') continue;
		if (line[0] == '}') break;
		if (line[0] == ']') break;

		found = line.find("credit");
		if (found!=std::string::npos) continue; //ignorar la primera linea de courses;
		ss << line; //transforma la linea en un stream

		while(getline(ss,credit,',')) //parsea ","
		{
			credit = helper.trim(credit);
			if (! credit.length()) continue; //por si queda en blanco
			std::stringstream ss_helper;
			ss_helper << credit;
			int to_vector;
			ss_helper >> to_vector;
			credits.push_back(to_vector);
			ss_helper.clear();
		}
	   	ss.clear();
	    found = line.find("]");
	   	if (found!=std::string::npos) break; //encontro un }, termina este bloeuque
	}

	prereq.resize(courses.size()); //tamaño de la matrix de pre-requisitos
	while (std::getline(file,line))
	{
		if (! line.length()) continue;
		if (line[0] == '/') continue;
		if (line[0] == ' ' && line.find("/") != std::string::npos) continue; //algunas pueden empezar vacias
		if (line[0] == '%') continue;
		if (line[0] == '}') break;
		if (line[0] == ']') break;
		found = line.find("prereq");
		if (found!=std::string::npos) continue; //ignorar la primera linea de prereq;
	    ss << line;
	    while(getline(ss,course,','))
	    {
	    	master_course = helper.trim(course);
	    	if (!master_course.length()) continue;
	    	getline(ss,course,',');
	    	req_course = helper.trim(course);
	    	if (!req_course.length()) continue;
	    	prereq[helper.find_in_string_vector(master_course,courses)].push_back(helper.find_in_string_vector(req_course,courses));
	    }
	    ss.clear();

	    found = line.find("}");
	    if (found != std::string::npos) break;
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
	cout << "Ramos Leidos:" << endl;
	for (vector<string>::size_type i=0; i < courses.size(); i++)
		{
			cout << "Ramo[" << i << "]: ";
			cout << courses[i] << endl;
		}
	cout << "Creditos leidos: " << endl;
	helper.print_int_vector(credits);
	cout << "Pre-requisitos: " << endl;
	helper.print_vector_vector_int(prereq);
}

void Config::calc_min_period()
{
	for (int i = 0; i < courses.size(); i++)
	{
		post_req(i,0);
	}
}

void Config::post_req(int course, int actual)
{
	if (prereq[course].size() == 0)
	{
		if (max_period[course] < actual){  max_period[course] = actual;}
		return;
	}
	for (int i=0; i < prereq[course].size(); i++)
	{
		post_req(prereq[course][i],actual +1);
	}
}
