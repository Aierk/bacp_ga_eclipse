/*
 * Helper.h
 *
 *  Created on: 29-06-2012
 *      Author: sergio
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <string>
#include <vector>
using std::vector;
using std::string;

class Helper {
public:
	Helper();
	virtual ~Helper();

	std::string trim(std::string const& source);
	void string_vector(std::vector<string> myvector);
	void print_int_vector(vector<int> myvector);
	void print_vector_vector_int(vector<vector<int> > matrix);
	int find_in_string_vector(string lost, vector<string> place);
protected:
	char const* delims;

};

#endif /* HELPER_H_ */
