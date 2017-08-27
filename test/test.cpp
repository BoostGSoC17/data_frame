//          Copyright(C) Rishabh Arora 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// To compile, use boost with the flag -lboost_unit_test_framework.
// To run use ./a.out --log_level=all to see the entire error report.


#define BOOST_DYN_LINK
#define BOOST_TEST_MODULE  DATA_FRAME

#include <boost/test/unit_test.hpp>
#include "../include/boost/numeric/ublas/FINAL_VERSIONS/df.hpp"
using namespace boost::numeric::ublas; 


BOOST_AUTO_TEST_CASE (df_column_constructors) {
	//  default constructor
	df_column c1;
	BOOST_CHECK(c1.size() == 0);

	// copy constructor with params: vector<T>
	vector <int> I(100);
	for(size_t i = 0; i < 100; i++) I(i) = i+2;
	df_column II(I);
	BOOST_CHECK(II.size() == 100);
	for(size_t i = 0; i < 100; i++) { 
		BOOST_CHECK(I(i) == II.get<int>()(i));
	}	

	// copy constructor with params: df_column
	df_column III(II);
	BOOST_CHECK(II.size() == III.size());
	for(size_t i = 0; i < 100; ++i) {
		BOOST_CHECK(II.get<int>()(i) == III.get<int>()(i));
	}

	df_column IV(II + III);
	BOOST_CHECK(IV.size() == II.size());
	for(size_t i = 0; i < 100; ++i) {
		BOOST_CHECK(II.get<int>()(i) + III.get<int>()(i) == IV.get<int>()(i));
	}
}

BOOST_AUTO_TEST_CASE (df_column_assignment) {
	vector < std::string > x(2);
	x(0) = "rishabh";
	x(1) = "arora";

	df_column X = x;
	BOOST_CHECK(X.size() == x.size());
	for(size_t i = 0; i < X.size(); ++i) {
		BOOST_CHECK(X.get<std::string>()(i) == x(i));
	}

	df_column Y = X;
	BOOST_CHECK(X.size() == Y.size());
	for(size_t i = 0; i < X.size(); ++i) {
		BOOST_CHECK(X.get<std::string>()(i) == Y.get<std::string>()(i));
	}
}

BOOST_AUTO_TEST_CASE (df_column_Equality_Check_Operators) {
	vector < double > a(3);
	a(0) = 1.1;
	a(1) = 1.2;
	a(2) = 1.32;
	vector < char > b(2);
	b(0) = 'a';
	b(1) = 'b';
	
	// should fail
	df_column A(a), B(b);
	//BOOST_CHECK(A == B);
	// should pass
	BOOST_CHECK(A != B);

	df_column C(A);	
	// should pass 
	BOOST_CHECK(A == C);

	// should fail
	//BOOST_CHECK(A != C);

	vector < char > d(2);
	d(0) = 'c';
	d(1) = 'd';
	df_column D(d);

	// should fail
	//BOOST_CHECK(B == D);
	// should pass 
	BOOST_CHECK(B != D);
}

BOOST_AUTO_TEST_CASE (df_column_Unary_Operator) {
	// unary - operator
	vector < int > z(3);
	z(0) = 15;
	z(1) = 12;
	z(2) = 13;
	df_column Z(z);
	// move assignment :)
	df_column T = -Z;

	BOOST_CHECK(T.size() == Z.size());
	for(size_t i = 0; i < T.size(); ++i) {
		BOOST_CHECK(T.get<int>()(i) == -Z.get<int>()(i));
	}
}

BOOST_AUTO_TEST_CASE (df_column_Binary_Operators) {
	vector < int > z(3);
	z(0) = 15;
	z(1) = 12;
	z(2) = 13;
	df_column Z(z);
	df_column Y(2*Z);
	BOOST_CHECK(Z.size() == Y.size());
	for(size_t i = 0; i < Y.size(); ++i) {
		BOOST_CHECK(2 * Z.get<int>()(i) == Y.get<int>()(i));
	}	

	Z *= 2;
	for(size_t i = 0; i < Y.size(); ++i) {
		BOOST_CHECK(Z.get<int>()(i) == Y.get<int>()(i));
	}

	df_column X = Z + 3;
	BOOST_CHECK(X.size() == Z.size());
	for(size_t i = 0; i < Z.size(); ++i) {
		BOOST_CHECK(X.get <int>()(i) == Z.get<int>()(i) + 3);
	}
	X += 3;
	for(size_t i = 0; i < Z.size(); ++i) {
		BOOST_CHECK(X.get <int>()(i) == Z.get<int>()(i) + 6);
	}
	X -= 3;
	for(size_t i = 0; i < Z.size(); ++i) {
		BOOST_CHECK(X.get <int>()(i) == Z.get<int>()(i) + 3);
	}

	df_column A = X + Z;
	BOOST_CHECK(A.size() == X.size() && A.size() == Z.size());
	for(size_t i = 0; i < Z.size(); ++i) {
		BOOST_CHECK(A.get<int>()(i) == X.get <int>()(i) + Z.get<int>()(i));
	}
	A -= X;
	for(size_t i = 0; i < Z.size(); ++i) {
		BOOST_CHECK(A.get<int>()(i) == Z.get<int>()(i));
	}
}

// No static_assert checking. Manual looking shows that results are perfect. :-)
BOOST_AUTO_TEST_CASE (df_column_Printing) {
	vector < int > z(3);
	z(0) = 15;
	z(1) = 12;
	z(2) = 13;
	df_column Z(z);
	df_column Y(2*Z);
	Z.print();
	Y.summary < int, double> ();
}

BOOST_AUTO_TEST_CASE (df_column_Statistical_Summaries) {

	// Mean
	vector<double> v(6);
    v(0) = 10, v(1) = 20, v(2) = 30, v(3) = 40, v(4) = 50, v(5) = 60.123777;
    df_column V(v);
    double mean = V.Mean<double, double>();
    double var = (10.0 + 20.0 + 30.0 + 40.0 + 50.0 + 60.123777) / 6.0;
    //std::cout << mean << ' ' << var << '\n';
    BOOST_CHECK(fabs(mean - var) < 1e-9);

	// Median
	double Median = V.Median<double, double>();
	BOOST_CHECK( fabs(Median - 35) < 1e-9);
	v.resize(7);
	v(6) = 70;
	V = v;
	Median = V.Median<double, double>();
	BOOST_CHECK( fabs(Median - 40) < 1e-9);
	
	// Max, Min
    vector < long long > r(10);
    for(int i = 0; i < 10; ++i) r(i) = 1e16 + rand() % 1000;
    long long mx = -1, mn = 1e18;
	for(int i = 0; i < 10; ++i) { 
		if (r(i) > mx) mx = r(i);
		if (r(i) < mn) mn = r(i);
	}
	df_column R(r);
	long long MAX = R.Max<long long, long long>();
	long long MIN = R.Min<long long, long long>();
	BOOST_CHECK(mx == MAX);
	BOOST_CHECK(mn == MIN);	
}

BOOST_AUTO_TEST_CASE (data_frame_Constructors) {
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

BOOST_AUTO_TEST_CASE (data_frame_Column_Accessors_Manipulators) {
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

BOOST_AUTO_TEST_CASE (data_frame_Equality_Check_Operators) {
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
	//BOOST_CHECK(df == empty);
}
 
BOOST_AUTO_TEST_CASE (data_frame_Unary_Operators) {
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

BOOST_AUTO_TEST_CASE (data_frame_Statistical_Summaries) {
	vector < int > x(2);
	vector < std::string > y(2);
	vector < double > z(2);
	x(0) = 10, x(1) = 11;
	y(0) = "rishabh", y(1) = "arora";
	z(0) = 1.2, z(1) = 2.0;
	vector < std::string > headers(3);
	headers(0) = "x", headers(1) = "y", headers(2) = "z";
	vector < df_column > A(3);
	A(0) = x, A(1) = y, A(2) = z;
	data_frame df(headers, A);
	// Already checked above that df_column_summaries work fine. and data_Frame summaries are extension of df_column_summaries for each column of the data_Frame.
	df.summary();
}

BOOST_AUTO_TEST_CASE (data_frame_Binary_Operators) {
	vector < std::string > headers(2);
	headers(0) = "a";
	headers(1) = "b";
	vector < double > a(2), b(2);
	a(0) = 1.1, a(1) = 2.1;
	b(0) = 1.2, b(1) = 2.3;
	vector < df_column > col(2);
	col(0) = a;
	col(1) = b;
	data_frame df1(headers, col);
	vector < df_column > col2(2);
	a = 2 * a;
	b = 2 * b;
	col2(0) = a;
	col2(1) = b;
	data_frame df2(headers, col2);
	
	data_frame df3 = df1 + df2;
	BOOST_CHECK(df3.nrow() == df1.nrow() && df3.nrow() == df2.nrow());
	BOOST_CHECK(df3.ncol() == df1.ncol() && df3.ncol() == df2.ncol());

	for(size_t i = 0; i < df3.nrow(); ++i) {
		BOOST_CHECK(df3[0].get<double>()(i) == df1[0].get<double>()(i) + df2[0].get<double>()(i)); 
	}
	for(size_t i = 0; i < df3.nrow(); ++i) {
		BOOST_CHECK(df3[1].get<double>()(i) == df1[1].get<double>()(i) + df2[1].get<double>()(i)); 
	}	

	data_frame df4 = df3 - df2;
	BOOST_CHECK(df4.nrow() == df3.nrow() && df4.nrow() == df2.nrow());
	BOOST_CHECK(df4.ncol() == df3.ncol() && df4.ncol() == df2.ncol());

	for(size_t i = 0; i < df4.nrow(); ++i) {
		BOOST_CHECK(fabs(df4[0].get<double>()(i) - df1[0].get<double>()(i)) < 1e-9); 
	}
	for(size_t i = 0; i < df3.nrow(); ++i) {
		BOOST_CHECK(fabs(df4[1].get<double>()(i) - df1[1].get<double>()(i)) < 1e-9); 
	}

	data_frame df5 = df1 + 5;
	BOOST_CHECK(df5.nrow() == df1.nrow());
	BOOST_CHECK(df5.ncol() == df1.ncol());

	for(size_t i = 0; i < df5.nrow(); ++i) {
		BOOST_CHECK(fabs(df5[0].get<double>()(i) - df1[0].get<double>()(i) - 5) < 1e-9); 
	}
	for(size_t i = 0; i < df5.nrow(); ++i) {
		BOOST_CHECK(fabs(df5[1].get<double>()(i) - df1[1].get<double>()(i)- 5) < 1e-9); 
	}

	data_frame df6 = df1 - 10;
	BOOST_CHECK(df6.nrow() == df1.nrow());
	BOOST_CHECK(df6.ncol() == df1.ncol());

	for(size_t i = 0; i < df6.nrow(); ++i) {
		BOOST_CHECK(fabs(df6[0].get<double>()(i) - df1[0].get<double>()(i) + 10) < 1e-9); 
	}
	for(size_t i = 0; i < df6.nrow(); ++i) {
		BOOST_CHECK(fabs(df6[1].get<double>()(i) - df1[1].get<double>()(i) + 10) < 1e-9); 
	}

	data_frame df7 = 2 * df1;
	BOOST_CHECK(df7.nrow() == df1.nrow());
	BOOST_CHECK(df7.ncol() == df1.ncol());

	for(size_t i = 0; i < df7.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[0].get<double>()(i) - 2 * df1[0].get<double>()(i)) < 1e-9); 
	}
	for(size_t i = 0; i < df7.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[1].get<double>()(i) - 2 * df1[1].get<double>()(i)) < 1e-9); 
	}

	df7 += df1;
	for(size_t i = 0; i < df7.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[0].get<double>()(i) - 3 * df1[0].get<double>()(i)) < 1e-9); 
	}
	for(size_t i = 0; i < df7.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[1].get<double>()(i) - 3 * df1[1].get<double>()(i)) < 1e-9); 
	}

	df7 -= df1;
	for(size_t i = 0; i < df7.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[0].get<double>()(i) - 2 * df1[0].get<double>()(i)) < 1e-9); 
	}
	for(size_t i = 0; i < df7.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[1].get<double>()(i) - 2 * df1[1].get<double>()(i)) < 1e-9); 
	}

	df7 += 5;
	for(size_t i = 0; i < df7.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[0].get<double>()(i) - 2 * df1[0].get<double>()(i) - 5) < 1e-9); 
	}
	for(size_t i = 0; i < df6.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[1].get<double>()(i) - 2 * df1[1].get<double>()(i) - 5) < 1e-9); 
	}	

	df7 -= 10;
	for(size_t i = 0; i < df7.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[0].get<double>()(i) - 2 * df1[0].get<double>()(i) + 5) < 1e-9); 
	}
	for(size_t i = 0; i < df6.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[1].get<double>()(i) - 2 * df1[1].get<double>()(i) + 5) < 1e-9); 
	}	

	df7 += 5;
	df7 *= 3;

	for(size_t i = 0; i < df7.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[0].get<double>()(i) - 6 * df1[0].get<double>()(i)) < 1e-9); 
	}
	for(size_t i = 0; i < df7.nrow(); ++i) {
		BOOST_CHECK(fabs(df7[1].get<double>()(i) - 6 * df1[1].get<double>()(i)) < 1e-9); 
	}
}

BOOST_AUTO_TEST_CASE (data_frame_row_accessor) {
	vector < int > a(3);
	vector < std::string > b(3); 
	vector < bool > c(3);
	vector < long long > d(3); 
	vector < char > e(3);
	vector < std::string > headers(5);
	headers(0) = "a", headers(1) = "b", headers(2) = "c", headers(3) = "d", headers(4) = "e";
	a(0) = 1, a(1) = 2, a(2) = 3;
	b(0) = "R", b(1) = "RArora", b(2) = "Arora";
	c(0) = 0, c(1) = 1, c(2) = 0;
	d(0) = (long long)1e10, d(1) = (long long)1e10 + 100, d(2) = (long long)1e10 - 100;
	e(0) = 'a', e(1) = 'b', e(2) = 'c';
	vector < df_column > col(5);
	col(0) = a, col(1) = b, col(2) = c, col(3) = d, col(4) = e;
	data_frame df(headers, col);
	vector < boost::variant < COLUMN_DATA_TYPES > > row = df(0);
	BOOST_CHECK(row.size() == df.ncol());
	BOOST_CHECK(boost::get<int>(row(0)) == a(0));
	BOOST_CHECK(boost::get<std::string>(row(1)) == b(0));
	BOOST_CHECK(boost::get<bool>(row(2)) == c(0));
	BOOST_CHECK(boost::get<long long>(row(3)) == d(0));
	BOOST_CHECK(boost::get<char>(row(4)) == e(0));
}

BOOST_AUTO_TEST_CASE (data_frame_proxies) {
	vector < int > a(3);
	vector < std::string > b(3); 
	vector < bool > c(3);
	vector < long long > d(3); 
	vector < char > e(3);

	vector < std::string > headers(5);
	
	headers(0) = "a", headers(1) = "b", headers(2) = "c", headers(3) = "d", headers(4) = "e";
	a(0) = 1, a(1) = 2, a(2) = 3;
	b(0) = "R", b(1) = "RArora", b(2) = "Arora";
	c(0) = 0, c(1) = 1, c(2) = 0;
	d(0) = (long long)1e10, d(1) = (long long)1e10 + 100, d(2) = (long long)1e10 - 100;
	e(0) = 'a', e(1) = 'b', e(2) = 'c';
	
	vector < df_column > col(5);
	col(0) = a, col(1) = b, col(2) = c, col(3) = d, col(4) = e;
	data_frame df(headers, col);
	data_frame_range dfr1(&df, range(0, 3));
	BOOST_CHECK(dfr1.size() == 3);
	//dfr1.print();
	for (int i = 0; i < 3; ++i) {
		BOOST_CHECK(dfr1[0].get<int>()(i) == a(i));	
	}
	for (int i = 0; i < 3; ++i) {
		BOOST_CHECK(dfr1[1].get<std::string>()(i) == b(i));	
	}
	for (int i = 0; i < 3; ++i) {
		BOOST_CHECK(dfr1[2].get<bool>()(i) == c(i));	
	}

	data_frame_slice dfs1(&df, slice(0, 2, 3));
	BOOST_CHECK(dfs1.size() == 3);
	//dfs1.print();
	for (int i = 0; i < 3; ++i) {
		BOOST_CHECK(dfs1[0].get<int>()(i) == a(i));	
	}
	for (int i = 0; i < 3; ++i) {
		BOOST_CHECK(dfs1[1].get<bool>()(i) == c(i));	
	}
	for (int i = 0; i < 3; ++i) {
		BOOST_CHECK(dfs1[2].get<char>()(i) == e(i));	
	}

	indirect_array ia(3);
	ia[0] = 0, ia[1] = 1, ia[2] = 4;
	data_frame_indirect dfi1(&df, ia);
	// dfi1.print();
	BOOST_CHECK(dfs1.size() == 3);

	for (int i = 0; i < 3; ++i) {
		BOOST_CHECK(dfi1[0].get<int>()(i) == a(i));	
	}
	for (int i = 0; i < 3; ++i) {
		BOOST_CHECK(dfi1[1].get<std::string>()(i) == b(i));	
	}
	for (int i = 0; i < 3; ++i) {
		BOOST_CHECK(dfi1[2].get<char>()(i) == e(i));	
	}
	
	data_frame_range dfr(&df, range(3, 4));
	dfr += 3;	
	dfr.print();
	// now only column 3 is updated, rest of the data_frame remains the same.
	for(size_t i = 0; i < 3; ++i) {
		BOOST_CHECK(dfr[0].get<long long>()(i) == d(i) + 3);	
	}

	dfr -= 3;
	for(size_t i = 0; i < 3; ++i) {
		BOOST_CHECK(dfr[0].get<long long>()(i) == d(i));	
	}

	dfr *= 5;
	for(size_t i = 0; i < 3; ++i) {
		BOOST_CHECK(dfr[0].get<long long>()(i) == 5 * d(i));	
	}

	data_frame_slice dfs(&df, slice(0, 3, 2));
	dfs += 3;
	
	for(size_t i = 0; i < 3; ++i) {
		BOOST_CHECK(dfs[0].get<int>()(i) == a(i) + 3);			
		BOOST_CHECK(dfs[1].get<long long>()(i) == 5 * d(i) + 3);	
	}

	dfs -= 3;
	for(size_t i = 0; i < 3; ++i) {
		BOOST_CHECK(dfs[0].get<int>()(i) == a(i));			
		BOOST_CHECK(dfs[1].get<long long>()(i) == 5 * d(i));	
	}

	dfs *= 2;
	for(size_t i = 0; i < 3; ++i) {
		BOOST_CHECK(dfs[0].get<int>()(i) == 2 * a(i));			
		BOOST_CHECK(dfs[1].get<long long>()(i) == 10 * d(i) );	
	}

	indirect_array <> ia1(2);
	ia1[0] = 0, ia1[1] = 3;
	data_frame_indirect dfi(&df, ia1);
	dfi += 1;
	for(size_t i = 0; i < 3; ++i) {
		BOOST_CHECK(dfi[0].get<int>()(i) == 2 * a(i) + 1);			
		BOOST_CHECK(dfi[1].get<long long>()(i) == 10 * d(i) + 1);	
	}	
	dfi -= 1;
	for(size_t i = 0; i < 3; ++i) {
		BOOST_CHECK(dfi[0].get<int>()(i) == 2 * a(i));			
		BOOST_CHECK(dfi[1].get<long long>()(i) == 10 * d(i));	
	}
	dfi *= 100;
	for(size_t i = 0; i < 3; ++i) {
		BOOST_CHECK(dfi[0].get<int>()(i) == 200 * a(i));			
		BOOST_CHECK(dfi[1].get<long long>()(i) == 1000 * d(i));	
	}
}	

BOOST_AUTO_TEST_CASE(a) {
	vector < int > a(3);
	vector < std::string > b(3); 
	vector < bool > c(3);
	vector < long long > d(3); 
	vector < char > e(3);

	vector < std::string > headers(5);
	
	headers(0) = "a", headers(1) = "b", headers(2) = "c", headers(3) = "d", headers(4) = "e";
	a(0) = 1, a(1) = 2, a(2) = 3;
	b(0) = "R", b(1) = "RArora", b(2) = "Arora";
	c(0) = 0, c(1) = 1, c(2) = 0;
	d(0) = (long long)1e10, d(1) = (long long)1e10 + 100, d(2) = (long long)1e10 - 100;
	e(0) = 'a', e(1) = 'b', e(2) = 'c';
	
	vector < df_column > col(5);
	col(0) = a, col(1) = b, col(2) = c, col(3) = d, col(4) = e;
	data_frame df(headers, col);
	df_column x = df["a"];
}
