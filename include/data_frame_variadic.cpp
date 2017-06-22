#include <iostream>
#include <string>
#include <boost/numeric/ublas/vector.hpp>
#include <type_traits> 
using namespace boost::numeric::ublas;

template <class T = void, class ... TArgs>
class data_frame {
public:
	data_frame() {}

	template <class F1, class F2, class ... FArgs>
	data_frame(F1 header, vector<F2> col, FArgs ... fargs) : 
		header_(header), col_(col), data_(fargs...) {}

	data_frame(data_frame<T, TArgs...>& df) :
		col_(df.col_), header_(df.header_), data_(df.data_) {}

	template < class T1 > 
	const vector<T1>& column(const std::string header) {
		if (header == header_) {	
			if (std::is_same<T, T1>::value) {
				return (vector<T1>&)col_;
			}
			std::terminate();
		}
		return data_.template column <T1> (header);
	}
	
	data_frame<T, TArgs...>& operator = (data_frame<T, TArgs...>& df) {
		col_ = df.col();
		header_ = df.header();
		data_ = df.data();
		return *this;
	}

	template < class T1 >
	void add_column(data_frame<T, TArgs..., T1>& df, const std::string& header, const vector<T1>& col) {
		df.col() = col_;
		df.header() = header_;
		data_.add_column(df.data(), header, col);
		return;
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
	const vector<T>& data() const {
		return data_;
	}
	std::string& header() {
		return header_;
	}
	const std::string header() const {
		return header_;
	}

private:
	std::string header_;
	vector<T> col_;
	data_frame<TArgs...> data_;
};

template <>
class data_frame<> {

public: 
	data_frame() {}
	
	template < class T1 > 
	void add_column(data_frame<T1>& df, const std::string header, const vector<T1>& col) {
		df.header() = header;
		df.col() = col;
		return;
	}

	data_frame<>& operator = (data_frame<>& df) {
		return *this;
	}

	template <class T1> 
	const vector<T1>& column(const std::string header) {
		std::terminate();
	}

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

	vector<float> x = df2.column<float>("b");
	for(int i = 0; i < x.size(); i++) std::cout << x(i) << ' ';
	std::cout << std::endl;
	vector<int> y = df2.column<int>("a");
	for(int i = 0; i < y.size(); i++) std::cout << y(i) << ' ';
	std::cout << std::endl;
	return 0;	
}