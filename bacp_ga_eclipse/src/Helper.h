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

class Helper {
public:
	Helper();
	virtual ~Helper();

	std::string trim(std::string const& source, char const* delims = " \t\r\n;}<>");
	void print_string_vector(std::vector<std::string> myvector);
	void print_int_vector(std::vector<int> myvector);
	void print_vector_vector_int(std::vector< std::vector<int> > matrix);
	int find_in_string_vector(std::string lost, std::vector<std::string> place);

};

#endif /* HELPER_H_ */
