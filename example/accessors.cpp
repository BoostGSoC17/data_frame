
#include "../include/boost/numeric/ublas/df.hpp"

int main() {
	
	using namespace boost::numeric::ublas;
	
	vector <int> a(2);
	vector <double> b(2); 
	a(0) = 1, a(1) = 2;
	b(0) = 'a', b(1) = 'b';
	df_column data(2);
	data(0) = a;
	data(1) = b;
	vector < std::string> h(2);
	h(0) = "a", h(1) = "b";
	data_frame df(h, data);

	// column accessors
	df_column A = df["a"];
	A.print();

	df_column B = df[1];
	B.print();

	// returns the column vector as ublas::vector<T>; T = TYPE
	vector<int> AA = df.column<int>("a");

	// returns the column vector as ublas::vector<T>; T = TYPE
	vector<int> BB = df.column<double>(1);
	
	// row accessors
	return 0;
}