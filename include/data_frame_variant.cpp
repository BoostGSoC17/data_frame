#include <vector>
#include <unordered_map>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <variant>
using namespace boost::numeric::ublas;

// supports 3 data types for now
#define cumulative_type std::variant<vector<int>, vector<double>, vector<std::string> >

enum allowed_types {
	INT, 
	DOUBLE, 
	STRING, 
};

/// \return types of data stored
template <class T>
allowed_types Type() {
	if (std::is_same<T, int>::value)return INT;
	else if(std::is_same<T, double>::value) return DOUBLE;
	else if(std::is_same<T, std::string>::value) return STRING;
}

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
		types_.resize(col_);
		row_ = 0;
	}
	
	data_frame(std::vector<std::string> header, std::vector<cumulative_type> data) {
		// assuming equal sized vectors for now
		for(size_t i = 0; i < header.size(); i++) {
			types_.push_back(data.index());
			header_.push_back(header[i]);
			data_[header[i]] = data[i];
		}
	}

private:
	size_t col_, row_;
	std::unordered_map < std::string, cumulative_type > data_;
	std::vector< std::string > header_;
	std::vector<allowed_types> types_;
};


int main() {
	return 0;
}