/**Boost GsoC'17
  *column_vector version 1.0
  *typecast all data as std::string
*/

#ifndef _COLUMN_VECTOR_HPP
#define _COLUMN_VECTOR_HPP

#include <iostream>
#include <string>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/lexical_cast.hpp>
using namespace boost::numeric::ublas;	

class column_vector {
public:
	column_vector() {}
	column_vector(std::string name, vector <int> x) {
		colname = name;
		column_data.resize(x.size());
		for(size_t i = 0; i < x.size(); i++)
			column_data(i) = boost::lexical_cast<std::string>(x(i));
		type = "int";
	}
	column_vector(std::string name, vector <double> x) {
		colname = name;
		column_data.resize(x.size());
		for(size_t i = 0; i < x.size(); i++)
			column_data(i) = boost::lexical_cast<std::string>(x(i));
		type = "double";	
	}
	column_vector(std::string name, vector <std::string> x) {
		colname = name;
		column_data.resize(x.size());
		for(size_t i = 0; i < x.size(); i++)
			column_data(i) = x(i);	
		type = "string";
	}

	size_t getsize() {
		return column_data.size();
	}
	std::string getname() {
		return colname; 
	}

	void print_info() {
		std::cout << "Type: " << type << ' ' << "Name:" << colname << '\n';
		std::cout << "Vector Values:" << '\n';
		for(auto x: column_data) std::cout << x << ' ';
		std::cout << '\n';
		return;
	}

private:
	std::string type, colname;
	vector <std::string> column_data;	

};

#endif