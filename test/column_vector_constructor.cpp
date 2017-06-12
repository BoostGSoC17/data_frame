/// \Tests for the constructors of data_frame.
/// \To compile, use boost with the flag -lboost_unit_test_framework.
/// \To run use ./a.out --log_level=all to see the entire error report.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE  TEST_COLV1

#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../include/column_vector.hpp"
#include <string>
#include <boost/numeric/ublas/vector.hpp>
using namespace boost::numeric::ublas; 


BOOST_AUTO_TEST_CASE(Constructors) {
	///\ default constructor
	column_vector c1;
	BOOST_CHECK(c1.size() == 0);

	column_vector c2(10);
	BOOST_CHECK(c2.size() == 10);
	
	vector <int> I(100);
	for(size_t i = 0; i < 100; i++) I(i) = i+2;
	column_vector II(I);
	BOOST_CHECK(II.size() == 100);

	for(size_t i = 0; i < 100; i++) { 
		BOOST_CHECK(II[i] == boost::lexical_cast<std::string>(I(i)));
	}	
}

BOOST_AUTO_TEST_CASE(assignment) {
	column_vector c1;
	vector < double> d(120);
	for(size_t i = 0; i < 120; i++) d(i) = 1.0*i + 10.1231;
	c1 = d;
	BOOST_CHECK(c1.size() == 120);
	for(size_t i = 0; i < 120; i++) { 
		BOOST_CHECK(c1[i] == boost::lexical_cast<std::string>(d(i)));
	}	
}

// also covered in the above test cases
BOOST_AUTO_TEST_CASE(access_operator) {
	vector < std::string > x(11);
	for(size_t i = 0; i < 11; i++) {
		x(i) = "test" + boost::lexical_cast<std::string>(i);
	}
	column_vector c(x);

	for(size_t i = 0; i < 11; i++) {
		std::cout << x(i) << ' ' << c[i] << std::endl;
		BOOST_CHECK(c[i] == boost::lexical_cast<std::string>(x(i)));
	}
}