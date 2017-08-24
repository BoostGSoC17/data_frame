//          Copyright(C) Rishabh Arora 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../include/boost/numeric/ublas/FINAL_VERSIONS/df.hpp"

int main() {

	using namespace boost::numeric::ublas;
	// data_frame summary
	vector < int > u(3);
	vector < double > v(3);
	vector < std::string > x(3);
	u(0) = 0, u(1) = 1, u(2) = 2;
	v(0) = 0.15 * 2.012 , v(1) = 1.123 * 2.333 , v(2) = 2.1 * 2.987867;
	x(0) = "test1", x(1) = "test2", x(2) = "test3";
	vector < df_column >  U(3);
	U(0) = u, U(1) = v, U(2) = x;
	vector<std::string> h(3);
	h(0) = "U", h(1) = "V", h(2) = "X"; 
	data_frame df(h, U);
	// prints column-wise summary;
	df.summary();

	// prints summary of column df["U"]
	df["U"].summary<int, double>();
	// first parameter(int): column type
	// second parameter(double): type of the statistics variable
}