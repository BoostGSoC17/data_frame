#include <boost/numeric/ublas/vector.hpp>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
using namespace boost::numeric::ublas;

class category_vector {
public:
	category_vector() {}
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

	size_t getsize() {
		return column_data.size();
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

class data_frame {
public:
	data_frame() {}
	data_frame(std::vector <category_vector> x) {
		colcount = x.size();
		for(auto column: x) {
			data.push_back(column);
		}

		// add error conditions for all the category_vector having same sizes
		rowcount = (x.size() > 0) ? x[0].getsize() : 0; 
	}

	void print_info() {
		std::cout << "data.frame data: " << std::endl;
		for(size_t i = 0; i < data.size(); i++) {
			data[i].print_info();
		}
	}

private:
	size_t colcount, rowcount;
	std::vector < category_vector > data;
	std::vector<std::string> column_names;
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
	// II.print_info();
	// DD.print_info();
	// SS.print_info();
	data_frame df({II, DD, SS});
	df.print_info();
	return 0;	
}