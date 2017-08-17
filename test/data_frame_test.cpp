/// \Tests for the constructors of data_frame.
/// \To compile, use boost with the flag -lboost_unit_test_framework.
/// \To run use ./a.out --log_level=all to see the entire error report.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE  TEST_COLV1

#include <boost/test/unit_test.hpp>
#include "../include/boost/numeric/ublas/FINAL_VERSIONS/df.hpp"
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

BOOST_AUTO_TEST_CASE (Column_Accessors_Manipulators) {
	// Column Retrieval
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

	/// Column Accessors + Updating
	// via string
	df["x"] = y;
	BOOST_CHECK(df["x"].size() == df.nrow());
	for(size_t i = 0; i < df.nrow(); ++i) {
		BOOST_CHECK(y(i) == df["x"].get<bool>()(i));
		BOOST_CHECK(y(i) == df[0].get<bool>()(i));	
		BOOST_CHECK(y(i) == df.column<bool>("x")(i));
		BOOST_CHECK(y(i) == df.column<bool>(0)(i));	
	}
	// via index
	df[0] = x;  // df is back to original
	BOOST_CHECK(df["x"].size() == df.nrow());
	for(size_t i = 0; i < df.nrow(); ++i) {
		BOOST_CHECK(x(i) == df["x"].get<char>()(i));
		BOOST_CHECK(x(i) == df[0].get<char>()(i));	
		BOOST_CHECK(x(i) == df.column<char>("x")(i));
		BOOST_CHECK(x(i) == df.column<char>(0)(i));	
	}

	/// erasing columns
	// via index
	size_t ncol_earlier = df.ncol();
	df.erase_column(0);
	BOOST_CHECK(df.ncol() == ncol_earlier-1);
	// test for correct restructuring of data_frame after the erase
	for(size_t i = 0; i < df.nrow(); ++i) {
		BOOST_CHECK(y(i) == df["y"].get<bool>()(i));
		BOOST_CHECK(y(i) == df[0].get<bool>()(i));	
		BOOST_CHECK(y(i) == df.column<bool>("y")(i));
		BOOST_CHECK(y(i) == df.column<bool>(0)(i));	
	}

	for(size_t i = 0; i < df.nrow(); ++i) {
		BOOST_CHECK(z(i) == df["z"].get<std::string>()(i));
		BOOST_CHECK(z(i) == df[1].get<std::string>()(i));	
		BOOST_CHECK(z(i) == df.column<std::string>("z")(i));
		BOOST_CHECK(z(i) == df.column<std::string>(1)(i));	
	}
	// via string
	ncol_earlier = df.ncol();
	df.erase_column("y");
	BOOST_CHECK(df.ncol() == ncol_earlier-1);
 	for(size_t i = 0; i < df.nrow(); ++i) {
		BOOST_CHECK(z(i) == df["z"].get<std::string>()(i));
		BOOST_CHECK(z(i) == df[0].get<std::string>()(i));	
		BOOST_CHECK(z(i) == df.column<std::string>("z")(i));
		BOOST_CHECK(z(i) == df.column<std::string>(0)(i));	
	}
		
	/// Add columns via accessor
	// via string
	df.print();
	ncol_earlier = df.ncol();
	df["y"] = y;
	BOOST_CHECK(df.ncol() == ncol_earlier + 1);
	df.print();
 }

BOOST_AUTO_TEST_CASE (Equality_Check_Operators) {
	// Column Retrieval
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
	data_frame df2(df);
	// should pass
	BOOST_CHECK(df == df2);

	data_frame empty;
	// should pass
	BOOST_CHECK(df != empty);

	// should fail
	BOOST_CHECK(df == empty);
}
 
BOOST_AUTO_TEST_CASE (Unary_Operators) {
	vector < std::string > names(4);
	names(0) = "a", names(1) = "b", names(2) = "c", names(3) = "d";
	vector < int > x(2), y(2);
	x(0) = 1;
	y(1) = 1;
	vector < df_column > cols(4);
	cols (0) = x;
	cols (1) = y;
	cols (2) = x;
	cols (3) = y;
	data_frame df(names, cols);
	// move constructor :) 
	data_frame negate_df(-df);
	BOOST_CHECK (negate_df.nrow() == df.nrow() && negate_df.ncol() == df.ncol());	
	for(size_t i = 0; i < negate_df.ncol(); ++i) negate_df.colname(i) == names(i);
	for(size_t i = 0; i < df.nrow(); ++i) {
		BOOST_CHECK(x(i) == -negate_df[0].get<int>()(i));
		BOOST_CHECK(x(i) == -negate_df[2].get<int>()(i));	
	}
	for(size_t i = 0; i < df.nrow(); ++i) {
		BOOST_CHECK(y(i) == -negate_df[1].get<int>()(i));
		BOOST_CHECK(y(i) == -negate_df[3].get<int>()(i));	
	}
}

BOOST_AUTO_TEST_CASE (Binary_Operators) {

}

BOOST_AUTO_TEST_CASE (Statistical_Summaries) {

}

