#include <vector>
#include <unordered_map>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <variant>
using namespace boost::numeric::ublas;

// supports 3 data types for now
#define cumulative_type std::variant<std::monostate, vector<int>, vector<double>, vector<std::string> >

enum allowed_types {
	INT, 
	DOUBLE, 
	STRING, 
};

class data_frame {
public:

	/// \default constructor: no params
	data_frame() {
		row_ = col_ = 0;
	}
	/// \params : 
	///		number of columns
	data_frame(const size_t col) {
		col_ = col;
		header_.resize(col_);
		row_ = 0;
	}
	
	data_frame(std::vector<std::string> header, std::vector<cumulative_type> data) {
		// assuming equal sized vectors for now
		// set conditions for col and row
		for(size_t i = 0; i < header.size(); i++) {
			header_.push_back(header[i]);
			data_[header[i]] = data[i];
		}
	}

	template <class T>
	const vector<T>& f(std::string header) {
		return std::get< vector < T > >(data_[header]);
	}

	// // not working :( : Same thing worked for string version. Fix it fast!!!!
	// const cumulative_type& operator[] (const std::string header) {
	// 	return data_[header];
	// }

	cumulative_type& operator[] (const std::string header) {
		// if (data_.find(header) != data_.end()) {
		// 	return data_[header];
		// }
		// data_[header];
		data_[header] = cumulative_type();
		return data_[header];
	}
private:
	size_t col_, row_;
	std::unordered_map < std::string, cumulative_type > data_;
	std::vector< std::string > header_;
};


int main() {
	vector<int> x(2);
	vector<double> y(2);
	x(0) = 1;
	x(1) = 2;
	y(0) = 1.1;
	y(1) = 2.1;

	cumulative_type X = x, Y = y; 
	data_frame df({"x", "y"}, {X, Y});
	df["z"] = y;
	cumulative_type A = df["z"];
	vector<double> a = std::get<vector<double> >(A);
	for(int i = 0; i < (int)a.size(); i++) std::cout << a(i) << ' ';
	return 0;
}