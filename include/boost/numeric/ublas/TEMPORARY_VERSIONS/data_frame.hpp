//          Copyright(C) Rishabh Arora 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/*
	Boost GSoC '17 
	Author: Rishabh Arora
	Mentor: David Bellot
*/

#ifndef _DATA_FRAME_HPP
#define _DATA_FRAME_HPP

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "./column_vector.hpp"

/** \Allowed T should be:
 * column_vector
 * column_matrix
 * Still to add conditions on allowed data_types.
*/
template <class T>
class data_frame {

public:

	typedef const T const_reference;
	typedef T &reference;
	typedef T column_type;

	/// \default constructor: no params
	data_frame() {
		row_ = col_ = 0;
	}
	/// \params : 
	///		number of columns
	data_frame(const size_t col) {
		col_ = col;
		header_.resize(col_);
		row_ = 0;
	}
	/// \params: 	
	/// 	column header
	/// 	data_ in the form of columns
	data_frame(const std::vector <std::string> header, std::vector <T> data) {
		try {
			/// \exception for lesser column headers than columns
			if (header.size() != data.size()) {
				throw inconsistent_arguments();
			}
			
			col_ = header.size();
			row_ = (col_ > 0) ? data[0].size() : 0; 

			/// \exception for unequal no. of rows in data_frame 
			if (col_) {
				for(size_t i = 1; i < col_; ++i) {
					if (data[i].size() != row_) {
						throw differing_rows();
					}
				}	
			}

			/// \initialise the data_ members
			for(size_t i = 0; i < col_; ++i) {
				header_.push_back(header[i]);
				// if a column_name already exists throw an error
				if ( (i != 0) && (data_.find(header[i])) != data_.end())  {
					throw same_header();
				}	
				data_[header[i]] = data[i];
				data_[header[i]].set_data_frame_column();	
			}
		}

		catch(std::exception &e) {
			/// \incorrect instantiation leads to termination of the program
			//std::terminate();
			std::cout << e.what() << std::endl;
			std::terminate();
		}
	}	

	// ------------ //
	// erase column //
	// ------------ //

	/// \erase column[$i]
	/// 	0-based indexing
	void erase_column(const size_t i) {
		try {
			if ( (i >= header_.size()) || (i < 0)) {
				throw undefined_index();
			}
			/// \i is valid so delete the column[$i]
			data_.erase(header_[i]);
			header_.erase(header_.begin() + i);
			--col_;
		}
		catch(std::exception &e){
			std::cout << e.what() << std::endl;
			//std::terminate();
		}
	}
	/// \erase column[$name]
	void erase_column(const std::string name) {
		try {
			if (data_.find(name) == data_.end()) {
				throw undefined_name();
			}
			/// \name is valid so delete the column[$name]
			data_.erase(name);
			header_.erase(std::find(header_.begin(), header_.end(), name));
			--col_;
		}
		catch(std::exception &e) {
			std::cout << e.what() << std::endl;
			//std::terminate();
		}
	}

	/// ---------------- ///
	/// access operators ///
	/// ---------------- ///

	const_reference operator[] (const std::string name) const {
		try {
			if (data_.find(name) == data_.end()) {
				throw undefined_name();
			}
			return data_[name];
		} 
		catch (std::exception &e) {
			std::cout << e.what() << std::endl;
			//std::terminate();
		}
	}	
	reference operator[](const std::string name) {
		/// \given column name already exists
		/// \return the particular column
		if(data_.find(name) != data_.end()) {
			return data_[name];
		}
		/// \create the column (will be furthur used by = operator in column_type)
		header_.push_back(name);
		++col_;
		data_[name] = column_type(row_);
		data_[name].set_data_frame_column();
		return data_[name];
	}	
	const_reference operator[] (const size_t i) const {
		try {
			if ( (i >= col_) || (i < 0) ) {
				throw undefined_index();
			}
			return data_[header_[i]];
		}
		catch (std::exception &e) {
			std::cout << e.what() << std::endl;
			//std::terminate();
		}
	}	
	reference operator[] (const size_t i) {
		/// \given column name already exists
		/// \return the particular column
		if(i < col_) {
			return data_[header_[i]];
		}
		try {
			if(i > col_) {
				throw holes();
			} 
			/// \custom name if no name is set by default
			header_.push_back(default_name(i));
			++col_;
			data_[header_[i]] = column_type(row_);
			data_[header_[i]].set_data_frame_column();
			return data_[header_[i]];
		}
		catch(std::exception &e) {
			std::cout << e.what() << std::endl;
			//std::terminate();
		}
	}

	/// \return data_frame dimensions
	const size_t row_size() {
		return row_;
	}
	const size_t col_size() {
		return col_;
	}

	void print_info() {
		std::cout << "data.frame data_: " << std::endl;
		for(size_t i = 0; i < header_.size(); ++i) {
			std::cout << header_[i] << std::endl;
			data_[header_[i]].print_info();
		}
	}

private:
	size_t col_, row_;
	std::unordered_map < std::string, column_type > data_;
	std::vector< std::string > header_;

	/// \returns the default column name
	/// \used when the column name is not set by the user
	std::string default_name(size_t i) {
		return ("Col-" + boost::lexical_cast<std::string>(i));
	}
};

#endif

