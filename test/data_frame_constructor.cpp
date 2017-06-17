
/// \Tests for the constructors of data_frame.
/// \To compile, use boost with the flag -lboost_unit_test_framework.
/// \To run use ./a.out --log_level=all to see the entire error report.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE  TEST_DF1

#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../include/data_frame.hpp"
#include "../include/column_vector.hpp"
#include <string>
#include <boost/numeric/ublas/vector.hpp>
using namespace boost::numeric::ublas; 


BOOST_AUTO_TEST_CASE(Constructors) {
	///\ default constructor

	data_frame <column_vector> df1;
	BOOST_CHECK(df1.col_size() == 0 && df1.row_size() == 0);

	data_frame <column_vector> df2(10);
	BOOST_CHECK(df2.col_size() == 10 && df2.row_size() == 0);
	
	vector <int> I(3);
	for(size_t i = 0; i < 3; i++) I(i) = i;
	vector <double> D(3);
	for(size_t i = 0; i < 3; i++) D(i) = (double)i + 0.123456777777754345;
	column_vector II(I), DD(D);
	data_frame<column_vector> df3({"int", "double", "string"}, {II, DD});
	BOOST_CHECK(df3.col_size() == 3 && df3.row_size() == 3);	

	
	data_frame <column_vector> df4(10);
	BOOST_CHECK(df4.col_size() == 10 && df4.row_size() == 0);
	
}