/*

*/

#ifndef _DATA_FRAME_HPP
#define _DATA_FRAME_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/lexical_cast.hpp>
#include "./column_vector.hpp"

template <class column_type>
class data_frame {
public:
	data_frame() {}
	data_frame(std::vector <column_type> x) {
		colcount = x.size();
		for(auto column: x) {
			data[column.getname()] = column;
		}

		// add error conditions for all the column_type having same sizes
		rowcount = (x.size() > 0) ? x[0].getsize() : 0; 
	}

	void print_info() {
		std::cout << "data.frame data: " << std::endl;
		for(auto x: data) {
			x.second.print_info();
		}
	}

private:
	size_t colcount, rowcount;
	std::map < std::string, column_type > data;
	std::vector<std::string> column_names;
};

#endif