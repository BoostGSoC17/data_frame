#include <iostream>
#include <string>
#include <boost/numeric/ublas/vector.hpp>
using namespace boost::numeric::ublas;

template <class T = void, class ... TArgs>
class data_frame {
public:
	data_frame() {
		std::cout << typeid(T).name() << std::endl;
	}

	template <class F1, class F2, class ... FArgs>
	data_frame(F1 header, vector<F2> col, FArgs ... headers) : 
		header_(header), col_(col), data_(headers...) {
		// add conditions for F1 to be only string.
		end_ = data_.end();
		std::cout << typeid(T).name() << ' ' << header_ << ' ' << end_ << std::endl;
	}	
	
	// not working: no idea why?! :( 
	template <class T1> 
	const vector<T1>& column(const std::string header) const {
		if (header == header_) {
			return col_;
		}
		else if (end_) {
			return vector<T1>();
		}
		return data_.column<T1>(header);
	}

	bool end() {
		return false;
	}

	std::string header_;
	vector<T> col_;
	data_frame<TArgs...> data_;
	bool end_;
};

template <>
class data_frame<> {
public: 
	data_frame() {
		std::cout << "yayyy! end of the recursive class template" << std::endl;
	}
	template <class T1> 
	const vector<T1>& column(const std::string header = "") {
		vector<T1> x;
		return x;
	}
	bool end() {
		return true;
	}
	// kuch nahi hai abhi yahan pe 
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
	d(0) = "rishabh";
	d(1) = "mridul";
	
	data_frame<int, float, char, std::string> df("a", a, "b", b, "c", c, "d", d);

	//vector<int> u = df.column<int>("a") ;

	return 0;
}