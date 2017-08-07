/// \Tests for the constructors of data_frame.
/// \To compile, use boost with the flag -lboost_unit_test_framework.
/// \To run use ./a.out --log_level=all to see the entire error report.

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE  TEST_COLV1

#include <boost/test/unit_test.hpp>
#include "../include/boost/numeric/ublas/df.hpp"
using namespace boost::numeric::ublas; 

BOOST_AUTO_TEST_CASE (Constructors) {
	///\ default constructor
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

BOOST_AUTO_TEST_CASE (Assignment) {
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

BOOST_AUTO_TEST_CASE (Equality_Check_Operators) {
	vector < double > a(3);
	a(0) = 1.1;
	a(1) = 1.2;
	a(2) = 1.32;
	vector < char > b(2);
	b(0) = 'a';
	b(1) = 'b';
	
	// should fail
	df_column A(a), B(b);
	BOOST_CHECK(A == B);
	// should pass
	BOOST_CHECK(A != B);

	df_column C(A);	
	// should pass 
	BOOST_CHECK(A == C);

	// should fail
	BOOST_CHECK(A != C);

	vector < char > d(2);
	d(0) = 'c';
	d(1) = 'd';
	df_column D(d);

	// should fail
	BOOST_CHECK(B == D);
	// should pass 
	BOOST_CHECK(B != D);
}

BOOST_AUTO_TEST_CASE (Unary_Operator) {
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

BOOST_AUTO_TEST_CASE (Binary_Operators) {
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

BOOST_AUTO_TEST_CASE (Statistical_Summaries) {

}

