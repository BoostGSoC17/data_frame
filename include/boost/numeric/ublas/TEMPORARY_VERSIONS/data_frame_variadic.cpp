#include <iostream>
#include <string>
#include <boost/numeric/ublas/vector.hpp>
#include <type_traits> 
#include "./data_frame_exceptions.hpp"

using namespace boost::numeric::ublas;

// T - the current ublas::vector<T> under consideration.
// TArgs... - rest of the data_frame arguments
template <class T = void, class ... TArgs>
class data_frame {
public:

	/// \default constructor: 
	/// \params: {}
	data_frame() {}

	/// \params: {column_name, ublas::vector<T>}*
	/// \recursive function: calls on inner arguments.
	template <class F1, class F2, class ... FArgs>
	data_frame(F1 header, vector<F2> col, FArgs ... fargs) : 
		header_(header), col_(col), data_(fargs...) {}
	
	/// \params: data_frame with exact same template arguments. 
	data_frame(data_frame<T, TArgs...>& df) :
		col_(df.col_), header_(df.header_), data_(df.data_) {}

	/// \brief: returns the ublas::vector<T> on the basis of header.
	template < class T1 > 
	const vector<T1>& column(const std::string header) {
		try {
			/// \header match return the current column.
			if (header == header_) {	
				if (std::is_same<T, T1>::value) {
					return (vector<T1>&)col_;
				}
				throw column_type_mismatch();
			}

			/// \search in the inner data_frame
			return data_.template column <T1> (header);
		}

		catch (std::exception& e) {
			std::terminate();
		}
	}
	
	/// \brief: * '=' operator overload for the data_frame. 
	/// 		* sets the current set of header_, col_ and calls the inner data_frame
	data_frame<T, TArgs...>& operator = (data_frame<T, TArgs...>& df) {
		col_ = df.col();
		header_ = df.header();
		data_ = df.data();
		return *this;
	}

	template < class T1 >
	void add_column(data_frame<T, TArgs..., T1>& df, 
					const std::string& header, 
					const vector<T1>& col) {
		df.col() = col_;
		df.header() = header_;
		data_.add_column(df.data(), header, col);
		return;
	}

	/// \should work!? FIX IT !!!!
	// No idea why it doesn't work! 
	template < class ... TArgs1 > 
	void erase_column(data_frame<TArgs1...>& df, const std::string& header) {
		if (header == header_) {
			data_.erase_column(df, header);
			return;
		}
		df.col() = col_;
		df.header() = header_;
		data_.erase_column(df.data(), header);
		return;
	}

	void print() {
		std::cout << "[" << header_ << "]" << ": ";
		for(size_t i = 0; i < col_.size(); ++i) std::cout << col_(i) << ' ';
		std::cout << std::endl;
		data_.print();
	}

	const bool end() const {
		return false;
	}
	vector<T>& col() {
		return col_;
	}
	const vector<T>& col() const {
		return col_;
	}
	data_frame<TArgs...>& data() {
		return data_;
	}
	const data_frame<TArgs...>& data() const {
		return data_;
	}
	std::string& header() {
		return header_;
	}
	const std::string& header() const {
		return header_;
	}

//private:
	std::string header_;
	vector<T> col_;
	data_frame<TArgs...> data_;
};

/// \the inner most data_frame class
template <>
class data_frame<> {

public: 
	data_frame() {}
	
	template < class T > 
	void add_column(data_frame<T>& df, const std::string header, const vector<T>& col) {
		df.header() = header;
		df.col() = col;
		return;
	}

	template <class ... TArgs>
	void erase_column(data_frame<TArgs...>& df, const std::string& header) {
		/* end the erase column operation */
		return;
	}

	data_frame<>& operator = (data_frame<>& df) {
		return *this;
	}

	template <class T1> 
	const vector<T1>& column(const std::string header) {
		try {
			throw undefined_column_header();
		}
		catch (std::exception& e) {
			std::terminate();
		}
	}

	void print() {}

	const bool end() const {
		return true;
	}
};

int main() {
	vector<int> a(2);
	a(0) = 1;
	a(1) = 2;
	vector<float> b(2);
	b(0) = 1.1;
	b(1) = 2.1;
	vector<char> c(2);
	c(0) = 'a';
	c(1) = 'b';
	vector<std::string> d(2);
	d(0)= "rishabh";
	d(1) = "mridul";
	
	data_frame<int> df1("a", a);
	//data_frame<int, float, char> df2;
	data_frame<int, float> df2;
	df1.add_column(df2, "b", b);
	// std::cout << std::endl;
	// df1.print();
	// std::cout << std::endl;
	// df2.print();
	// data_frame<int, float, char> df3;
	// df2.add_column(df3, "c", c);
	// df3.print();
	df2.erase_column(df1, "b");
	return 0;	
}