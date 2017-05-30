#include <boost/numeric/ublas/vector.hpp>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
using namespace boost::numeric::ublas;

class category_vector {
public:
	category_vector(vector <int> x) {
		column_data.resize(x.size());
		for(size_t i = 0; i < x.size(); i++)
			column_data(i) = boost::lexical_cast<std::string>(x(i));
		type = "int";
	}

	category_vector(vector <double> x) {
		column_data.resize(x.size());
		for(size_t i = 0; i < x.size(); i++)
			column_data(i) = boost::lexical_cast<std::string>(x(i));
		type = "double";	
	}

	category_vector(vector <std::string> x) {
		column_data.resize(x.size());
		for(size_t i = 0; i < x.size(); i++)
			column_data(i) = x(i);	
		type = "string";
	}

	void print_info() {
		std::cout << "Type: " << type << '\n';
		std::cout << "Vector Values:" << '\n';
		for(auto x: column_data) std::cout << x << ' ';
		std::cout << '\n';
		return;
	}
private:
	std::string type;
	vector <std::string> column_data;	
};

int main() {
	vector <int> I(3);
	for(size_t i = 0; i < 3; i++) I(i) = i;
	vector <double> D(3);
	for(size_t i = 0; i < 3; i++) D(i) = (double)i + 0.1;
	vector <std::string> S(3);
	S(0) = "rishabh";
	S(1) = "mridul";
	S(2) = "maa";

	category_vector II(I), DD(D), SS(S);
	II.print_info();
	DD.print_info();
	SS.print_info();
	
	return 0;	
}