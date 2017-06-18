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
		// add conditions for template to be only string.
		std::cout << typeid(T).name() << ' ' << header_<< std::endl;
	}

	std::string header_;
	vector<T> col_;
	data_frame<TArgs...> data_;
};

template <>
class data_frame<> {
public: 
	data_frame() {
		std::cout << "yayyy! end of the recursive class template" << std::endl;
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
	
	data_frame<int, float, char, std::string> x("a", a, "b", b, "c", c, "d", d);
	return 0;
}