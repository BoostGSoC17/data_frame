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

BOOST_AUTO_TEST_CASE (Column_Accessors) {
	vector < std::string > names(3);
	names(0) = "x";
	names(1) = "y";
	names(2) = "z";

	vector < char > x(2);
	vector < bool > y(2);
	vector < std::string > z(2);
	x(0) = 'p';
	x(1) = 'q';
	y(0) = true;
	y(1) = false;
	z(0) = "Oh!";
	z(1) = "Yeah..";

	vector < df_column > cols(3);
	cols (0) = x;
	cols (1) = y;
	cols (2) = z;

	data_frame df(names, cols);
	BOOST_CHECK(df.ncol() == 3 && df.nrow() == 2);

	for(size_t i = 0; i < df.nrow(); ++i) {
		BOOST_CHECK(x(i) == df["x"].get<char>()(i));
		BOOST_CHECK(x(i) == df[0].get<char>()(i));
		BOOST_CHECK(x(i) == df.column<char>("x")(i));
		BOOST_CHECK(x(i) == df.column<char>(0)(i));		
	}

	for(size_t i = 0; i < df.nrow(); ++i) {
		BOOST_CHECK(y(i) == df["y"].get<bool>()(i));
		BOOST_CHECK(y(i) == df[1].get<bool>()(i));	
		BOOST_CHECK(y(i) == df.column<bool>("y")(i));
		BOOST_CHECK(y(i) == df.column<bool>(1)(i));	
	}

	for(size_t i = 0; i < df.nrow(); ++i) {
		BOOST_CHECK(z(i) == df["z"].get<std::string>()(i));
		BOOST_CHECK(z(i) == df[2].get<std::string>()(i));	
		BOOST_CHECK(z(i) == df.column<std::string>("z")(i));
		BOOST_CHECK(z(i) == df.column<std::string>(2)(i));	
	}
}