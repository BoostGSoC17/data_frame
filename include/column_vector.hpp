/* Boost GSoC '17
 * column_vector version-1.0
 * convert all data to std::string
 */

#ifndef _COLUMN_VECTOR_HPP
#define _COLUMN_VECTOR_HPP

#include <iostream>
#include <string>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <type_traits>
#include <exception>
using namespace boost::numeric::ublas;	

/// \------------------- ///
/// \supported datatypes ///
/// \------------------- ///

enum allowed_types {
	/// \values: 0, 1, 2, 3
	INT, SHORT_INT, LONG_INT, LONG_LONG_INT, 
	/// \values: 4, 5, 6, 7
	UNSIGNED_INT, UNSIGNED_SHORT_INT, UNSIGNED_LONG_INT, UNSIGNED_LONG_LONG_INT, 
	/// \values: 8, 9
	DOUBLE, LONG_DOUBLE,
	/// \values: 10
	FLOAT, 
	/// \values: 11
	CHAR, 
	/// \values: 12
	STRING,
	
	NOT_SUPPORTED = -1,
};

struct type_not_supported : public std::exception {
   const char * what () const throw () {
      return "type not supported";
   }
};

/// \return types of data stored
template <class T>
allowed_types Type() {
	if (std::is_same<T, int>::value)return INT;
	else if(std::is_same<T, short int>::value) return SHORT_INT;
	else if(std::is_same<T, long int>::value) return LONG_INT;
	else if(std::is_same<T, long long int>::value) return LONG_LONG_INT;
	else if(std::is_same<T, unsigned int>::value) return LONG_LONG_INT;
	else if(std::is_same<T, unsigned short int>::value) return SHORT_INT;
	else if(std::is_same<T, unsigned long int>::value) return LONG_INT;
	else if(std::is_same<T, unsigned long long int>::value) return LONG_LONG_INT;
	else if(std::is_same<T, double>::value) return DOUBLE;
	else if(std::is_same<T, long double>::value) return LONG_DOUBLE;
	else if(std::is_same<T, float>::value) return FLOAT;
	else if(std::is_same<T, char>::value) return CHAR;
	else if(std::is_same<T, std::string>::value) return STRING;
	else return NOT_SUPPORTED; /// figure out a way to return something invalid
}

class column_vector {
public:

	/// \constructors
	/// \default: no parameters
	column_vector() {}
	
	/// \parameters: boost::numeric::ublas::vector<T>
	/// \T should be in allowed types
	template <class T>
	column_vector(const vector<T> &data) {
		try {
			type_ = Type<T>();
			if (type_ == NOT_SUPPORTED) {
				throw type_not_supported();
			}
			data_.resize(data.size());
			for(size_t i = 0; i < data.size(); i++) {
				data_(i) = boost::lexical_cast<std::string>(data(i));
			}
		}
		catch (std::exception &e) {
			std::terminate();
		}
	}

	/// \-------------------- ///
	/// \-assignment operator ///
 	/// \-------------------- ///

	/* Make sure that no. of rows throughout the data_frame is preserved */
	/* haven't added check yet */
	template <class T>
	column_vector &operator = (const vector<T> &data) {
		try {
			type_ = Type<T>();
			if (type_ == NOT_SUPPORTED) {
				throw type_not_supported();
			}
			
			data_.resize(data.size());
			for(size_t i = 0; i < data.size(); i++) {
				data_(i) = boost::lexical_cast<std::string>(data(i));
			}
			return *this;
		}
		catch (std::exception &e) {
			std::terminate();
		}			
	}

	/// \returns the size of the ublas::vector
	size_t size() {
		return data_.size();
	}

	void print_info() {
		std::cout << "Type: " << type_ << std::endl;
		std::cout << "Vector Values:" << '\n';
		for(auto x: data_) std::cout << x << ' ';
		std::cout << '\n';
		return;
	}

private:
	allowed_types type_;
	vector <std::string> data_;	
};

#endif