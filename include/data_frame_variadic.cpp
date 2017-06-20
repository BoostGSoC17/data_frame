# include <iostream>
# include <string>
# include <vector>
# include <type_traits> 
using namespace std;

template <class T = void, class ... TArgs>
class data_frame {
public:
	
	data_frame() {}

	template <class F1, class F2, class ... FArgs>
	data_frame(F1 header, vector<F2> col, FArgs ... headers) : 
		header_(header), col_(col), data_(headers...) {}	

	// works now
	template <class T1> 
	vector<T1>& column(std::string header) {
		if (header == header_) {	
			if (is_same<T, T1>::value) {
				return (vector<T1>&)col_;
			}
			std::terminate();
		}
		return data_.template column <T1> (header);
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
		
	template <class T1> 
	vector<T1>& column(std::string x) {
		std::terminate();
	}
};

int main() {
	vector<int> a(2);
	a[0] = 1;
	a[1] = 2;
	vector<float> b(2);
	b[0] = 1.1;
	b[1] = 2.1;
	vector<char> c(2);
	c[0] = 'a';
	c[1] = 'b';
	vector<std::string> d(2);
	d[0] = "rishabh";
	d[1] = "mridul";
	
	data_frame<int, float, char, string> df("a", a, "b", b, "c", c, "d", d);
	return 0;	
}