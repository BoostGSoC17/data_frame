/**Test-1: 
  *check for basic elements of the class
*/

#include <stdio.h>
#include <boost/numeric/ublas/vector.hpp>
#include <string>
#include "../include/column_vector.hpp"
using namespace boost::numeric::ublas;

int main() {
	vector <int> I(3);
	for(size_t i = 0; i < 3; i++) I(i) = i;
	vector <double> D(3);
	for(size_t i = 0; i < 3; i++) D(i) = (double)i + 0.123456777777754345;
	vector <std::string> S(3);
	S(0) = "rishabh";
	S(1) = "mridul";
	S(2) = "maa";
	column_vector II("int", I), DD("double", D), SS("maa", S);
	II.print_info();
	DD.print_info();
	SS.print_info();
	return 0;
}

/* Works fine */ 