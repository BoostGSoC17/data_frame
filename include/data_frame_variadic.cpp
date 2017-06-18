#include <iostream>
#include <string>
#include <boost/numeric/ublas/vector.hpp>
using namespace boost::numeric::ublas;


template <class T = void, class ... Args>
class data_frame {
public:
	data_frame() {
		std::cout << typeid(T).name() << std::endl;
	}
	vector<T> col_;
	data_frame<Args...> data_;
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
	data_frame<int, float, char, std::string> x;
	return 0;
}