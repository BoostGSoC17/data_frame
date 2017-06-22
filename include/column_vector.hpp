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
#include "./data_frame_exceptions.hpp"

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
	
	NOT_SET = -1, 

	NOT_SUPPORTED = -2, 
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
	/// \default: no params
	column_vector() {
		size_ = 0;
		type_ = NOT_SET;
		is_data_frame_column_ = false;
	}
	/// \params: size of column_vector
	column_vector(const size_t i) {
		size_ = i;
		data_.resize(i);
		type_ = NOT_SET;
		is_data_frame_column_ = false;
	}
	/// \params: boost::numeric::ublas::vector<T>
	/// \T should be in allowed types
	template <class T>
	column_vector(const vector<T> &data) {
		try {
			type_ = Type<T>();
			if (type_ == NOT_SUPPORTED) {
				throw type_not_supported();
			}
			is_data_frame_column_ = false;
			size_ = data.size();
			data_.resize(size_);
			for(size_t i = 0; i < size_; ++i) {
				data_(i) = boost::lexical_cast<std::string>(data(i));
			}
		}
		catch (std::exception &e) {
			std::cout << e.what() << std::endl;
			//std::terminate();
		}
	}

	/// \-------------------- ///
	/// \-assignment operator ///
 	/// \-------------------- ///

	template <class T>
	column_vector& operator = (const vector<T> &data) {
		try {
			type_ = Type<T>();
			if (type_ == NOT_SUPPORTED) {
				throw type_not_supported();
			}
			else if (is_data_frame_column_) {
				if (data.size() != size_) {
					throw differing_rows();
				}
			}
			else {
				size_ = data.size();
				data_.resize(size_);
			}

			for(size_t i = 0; i < size_; ++i) {
				data_(i) = boost::lexical_cast<std::string>(data(i));
			}
			return *this;
		}
		catch (std::exception &e) {
			std::cout << e.what() << std::endl;
			//std::terminate();
		}			
	}

	/// \access operator
	/// \returns the string version of the actual data for now.
	/// \add to it later.
	const std::string operator[] (const size_t i) const {
		try {
			if ( (i >= size_) || (i < 0) ) {
				throw undefined_index();
			}
			return data_(i);
		}
		catch (std::exception &e) {
			std::cout << e.what() << std::endl;
			////std::terminate();
		}
	}	
	

	/// \returns the size of the ublas::vector
	const size_t size() {
		return size_;
	}
	/// \ checks if the given column is a part of the data_frame
	void set_data_frame_column() {
		is_data_frame_column_ = true;
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
	size_t size_;
	bool is_data_frame_column_;
};

#endif