/*
 * Config.h
 *
 *  Created on: 29-06-2012
 *      Author: sergio
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "Helper.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using std::ifstream;
using std::vector;
using std::string;

class Config {
public:
	Config(char const* path);
	virtual ~Config();
	int periods;
	int min_load;
	int max_load;
	int min_courses;
	int max_courses;
	vector<std::string> courses;
	vector<int> credits;
	vector<int> max_period;
	vector<vector<int> > prereq;
	void getData();
	void printParameters();
	bool file_ok;
	void calc_min_period();
	float max_balance;

protected:
	char* file_path;
	ifstream file;
	std::string line;
	std::stringstream ss;
	size_t found;
	std::string course;
	std::string credit;
	std::string master_course;
	std::string req_course;
	Helper helper;
	void post_req(int course, int actual);



};

#endif /* CONFIG_H_ */
