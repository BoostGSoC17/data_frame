
#include "../include/boost/numeric/ublas/FINAL_VERSIONS/df.hpp"

int main() {
	using namespace boost::numeric::ublas;	
	vector < int > u(3);
	vector < double > v(3);
	u(0) = 0, u(1) = 1, u(2) = 2;
	v(0) = 0.15 * 2.012 , v(1) = 1.123 * 2.333 , v(2) = 2.1 * 2.987867;
	vector < df_column> U(2);
	U(0) = u, U(1) = v;
	vector < std::string > h(2);
	h(0) = "hello", h(1) = "world!";
	data_frame df(h, U);

	// column binary operations.
	df_column col1 = df["hello"] + 3;
	col1.print();
	std::cout << std::endl;

	// data_frame binary operations.
	df.print();
	std::cout << std::endl;
	
	data_frame df2 = 2 * df;
	df2.print();
	std::cout << std::endl;
	
	df += 5;
	df.print();
	std::cout << std::endl;
	
	df -= df2;
	df.print();
	return 0;
}