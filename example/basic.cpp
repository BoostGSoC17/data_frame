//          Copyright(C) Rishabh Arora 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../include/boost/numeric/ublas/FINAL_VERSIONS/df.hpp"

// instantiation, assignment and printing.
int main() {

	using namespace boost::numeric::ublas;
	
	vector <int> a(2);
	vector <char> b(2); 
	a(0) = 1, a(1) = 2;
	b(0) = 'a', b(1) = 'b';
	vector < df_column > data(2);
	data(0) = a;
	data(1) = b;
	vector < std::string> h(2);
	h(0) = "a", h(1) = "b";
	data_frame df(h, data);

	// prints the data column-wise
	df.print();
	std::cout << std::endl;
	// assign entire columns of a data_frame
	df[0] = df[1];
	df.print();
	std::cout << std::endl;
	
	// assigns the value of data_frame df to data_frame df2;
	data_frame df2;
	df2 = df;
	df2.print();

	return 0;
}