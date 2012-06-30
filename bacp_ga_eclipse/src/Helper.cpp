/*
 * Helper.cpp
 *
 *  Created on: 29-06-2012
 *      Author: sergio
 */

#include "Helper.h"
#include <iostream>
using std::cout;
using std::endl;

Helper::Helper() {
	// TODO Auto-generated constructor stub

}

Helper::~Helper() {
	// TODO Auto-generated destructor stub
}

Helper::trim(std::string const& source, char const* delims = " \t\r\n;}<>")
{
	  std::string result(source);
	  std::string::size_type index = result.find_last_not_of(delims);
	  if(index != std::string::npos)
	    result.erase(++index);

	  index = result.find_first_not_of(delims);
	  if(index != std::string::npos)
	    result.erase(0, index);
	  else
	    result.erase();
	  return result;
}

void print_string_vector(std::vector<std::string> myvector)
{
        for (int i = 0; i < myvector.size(); i++)
        {
                cout << "Vector[" << i << "] = " << myvector[i] << endl;
        }
}

Helper::print_int_vector(std::vector<int> myvector)
{
        for (int i = 0; i < myvector.size(); i++)
        {
                cout << "Vector[" << i << "] = " << myvector[i] << endl;
        }
}
Helper::print_vector_vector_int(std::vector< std::vector<int> > matrix)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        if (matrix[i].size() != 0)
        {
            cout << "Ramo[" << i << "]: ";
            for (int j=0; j < matrix[i].size(); j++)
            {
                cout << matrix[i][j] << ",";
            }
            cout << endl;
        }
    }
}
Helper::find_in_string_vector(std::string lost, std::vector<std::string> place)
{
        for (int i=0; i< place.size(); i++)
        {
                if ( place[i].compare(lost) == 0) return i;
        }
        return -1;
}
