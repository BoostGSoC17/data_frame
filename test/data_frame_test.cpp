/// \Tests for the constructors of data_frame.
/// \To compile, use boost with the flag -lboost_unit_test_framework.
/// \To run use ./a.out --log_level=all to see the entire error report.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE  TEST_COLV1

#include <boost/test/unit_test.hpp>
#include "../include/boost/numeric/ublas/df.hpp"
using namespace boost::numeric::ublas; 

BOOST_AUTO_TEST_CASE (Constructors) {
	// default constructor
	data_frame df1;
	BOOST_CHECK(df1.nrow() == 0 && df1.ncol() == 0);

	vector < std::string > names(2);
	names(0) = "col1";
	names(1) = "col2";

	vector < int > x(2), y(2);
	x(0) = 1;
	y(1) = 1;
	vector < df_column > cols(2);
	cols (0) = x;
	cols (1) = y;
	data_frame df2(names, cols);
	BOOST_CHECK(df2.ncol() == 2 && df2.nrow() == 2);

	for(size_t i = 0; i < df2.ncol(); ++i) {
		BOOST_CHECK(names(i) == df2.headers()(i));
	}
	
	for(size_t j = 0; j < df2.nrow(); ++j) {
		BOOST_CHECK(x(j) == df2["col1"].get<int>()(j));
	}
	for(size_t j = 0; j < df2.nrow(); ++j) {
		BOOST_CHECK(y(j) == df2["col2"].get<int>()(j));
	}
}
