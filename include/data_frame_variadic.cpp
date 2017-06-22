#include <iostream>
#include <string>
#include <boost/numeric/ublas/vector.hpp>
#include <type_traits> 
using namespace boost::numeric::ublas;

template <class T = void, class ... TArgs>
class data_frame {

public:
	data_frame() {}

	// add the required conditions.
	template <class F1, class F2, class ... FArgs>
	data_frame(F1 header, vector<F2> col, FArgs ... fargs) : 
		header_(header), col_(col), data_(fargs...) {}

	// works now!!!! :))))
	template <class T1> 
	vector<T1>& column(const std::string header = "") {
		if (header == header_) {	
			if (std::is_same<T, T1>::value) {
				return (vector<T1>&)col_;
			}
			std::terminate();
		}
		return data_.template column <T1> (header);
	}
	
	template < class T2, class T1, class ... TArgs1 > 
	void copy_data_frame (	data_frame <T1, TArgs1 ... >& df1, 
						 	data_frame <T1, TArgs1 ... , T2>& df2,
						 	const std::string& header, 
						 	const vector<T2>& col) {
		std::cout << df1.header_ << std::endl;
		df2.header_ = df1.header_;
		df2.col_ = df1.col_;
		copy_data_frame(df1.data_, df2.data_, header, col);
		return; 
	}

	template < class T2 >
	void copy_data_frame (	data_frame <>& df1, 
						 	data_frame <T2>& df2,
						 	const std::string& header, 
						 	const vector<T2>& col) {
		std::cout << header << std::endl;
		df2.header_ = header;	
		df2.col_ = col;
		return; 
	}
	
	// '=' operator works when used without add_column and add_column presumably works perfect. FIX IT!
	data_frame<T, TArgs...>& operator = (data_frame<T, TArgs...>& df) {
		//std::cout << df.header_ << std::endl;
		col_ ;//= df.col_;
		header_;// = df.header_;
		(*this).data_ = df.data_;
		return *this;
	}

	template <class T1>
	data_frame <T, TArgs..., T1>& add_column(const std::string new_header, const vector<T1>& new_col) {
		data_frame <T, TArgs..., T1> df;
		// is_end is called with false for now but later it will be set on the basis of no. of columns.
		copy_data_frame(*this, df, new_header, new_col);
		return df;
	}

	// it should work, compiler error: incorrect deduction of return type but return type 
	// is only deduced when header == header_ and then remains the same throughout back-tracking 
	// of the recursion! FIX IT :( 
	// ok, it doesn't work because all the return statements should have the same type 
	// so till now, no way to overload [] operator to return ublas::vector<T>
	/*
	auto operator[] (std::string header) {
		if (header == header_) {
			return (vector<T>&)col_;
		}
		return data_[header];
	}
	*/

	const bool end() const {
		return false;
	}

	std::string header_;
	vector<T> col_;
	data_frame<TArgs...> data_;
};

template <>
class data_frame<> {
public: 
	data_frame() {}
	
	data_frame<>& operator = (data_frame<>& df) {
		std::cout << "end" << std::endl;
		return *this;
	}
	template <class T1> 
	vector<T1>& column(std::string header = "") {
		std::terminate();
	}

	const bool end() {
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
	data_frame<int, float>df2;
	df2 = (data_frame<int, float>&)df1.add_column("b", b);

	//vector<float> x = df.column<float>("b");
	return 0;	
}