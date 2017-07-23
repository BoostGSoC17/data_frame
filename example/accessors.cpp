
int main() {
	using namespace boost::numeric::ublas;
	
	vector <int> a(2), b(2);
	a(0) = 1, a(1) = 2;
	b(0) = 2, b(1) = 10;
	df_column data(2);
	data(0) = a;
	data(1) = b;
	vector < std::string> h(2);
	h(0) = "a", h(1) = "b";
	data_frame df(h, data);

	// column accessors
	df_column A = df["a"];
	// print A
	df_column B = df[1];
	// print B

	// row accessors
	return 0;
}