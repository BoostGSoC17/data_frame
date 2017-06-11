/*
	Include License :-)
*/

#ifndef _DATA_FRAME_HPP
#define _DATA_FRAME_HPP

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "./column_vector.hpp"

struct insufficient_headers : public std::exception {
   const char * what () const throw () {
      return "insufficient column names";
   }
};
struct differing_rows : public std::exception {
   const char * what () const throw () {
      return "differing number of rows";
   }
};
/* Add required conditions */
struct same_header : public std::exception {
   const char * what () const throw () {
      return "data_frame columns not allowed to have same header";
   }
};
struct undefined_column_index : public std::exception {
   const char * what () const throw () {
      return "undefined column index";
   }
};
struct undefined_column_name : public std::exception {
   const char * what () const throw () {
      return "undefined column name";
   }
};
struct holes : public std::exception {
   const char * what () const throw () {
      return "new columns will leave holes after existing columns";
   }
};

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

	/// \no parameters
	data_frame() {}

	/// \paramters : 
	///		number of columns
	data_frame(size_t col) {
		col_ = col;
		header_.resize(col_);
	}

	/// \parameters: 	
	/// 	column header
	/// 	data_ in the form of columns
	/* Fix the header.size() vs data.size() issue */
	data_frame(std::vector <std::string> header, std::vector <T> data) {
		try {
			/// \exception for lesser column headers than columns
			if (header.size() < data.size()) {
				throw insufficient_headers();
			}
			
			col_ = header.size();
			row_ = (col_ > 0) ? data[0].size() : 0; 

			/// \exception for unequal no. of rows in data_frame 
			if (col_) {
				for(size_t i = 1; i < data.size(); i++) {
					if (data[i].size() != row_) {
						throw differing_rows();
					}
				}	
			}

			/// \initialise the data_ members
			for(size_t i = 0; i < header.size(); i++) {
				header_.push_back(header[i]);
				// initialise with some data
				if (i < data_.size()) {
					data_[header[i]] = data[i];
				}	
				// empty column
				else {
					data_[header[i]];
				}
			}
		}

		catch(std::exception &e) {
			/// \incorrect instantiation leads to termination of the program
			std::terminate();
		}
	}	

	// ------------ //
	// erase column //
	// ------------ //

	/// \erase column[$i]
	/// 	0-based iing
	void erase_column(size_t i) {
		try {
			if ( (i >= header_.size()) || (i < 0)) {
				throw undefined_column_index();
			}
			/// \i is valid so delete the column[$i]
			data_.erase(header_[i]);
			header_.erase(header_.begin() + i);
			--col_;
		}
		catch(std::exception &e){
			std::terminate();
		}
	}
	/// \erase column[$name]
	void erase_column(std::string name) {
		try {
			if (data_.find(name) == data_.end()) {
				throw undefined_column_name();
			}
			/// \name is valid so delete the column[$name]
			data_.erase(name);
			header_.erase(std::find(header_.begin(), header_.end(), name));
			--col_;
		}
		catch(std::exception &e) {
			std::terminate();
		}
	}

	/// --------------- ///
	/// access operator ///
	/// --------------- ///

	const_reference operator[] (const std::string name) const {
		try {
			if (data_.find(name) == data_.end()) {
				throw undefined_column_name();
			}
			return data_[name];
		} 
		catch (std::exception &e) {
			std::terminate();
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
		return data_[name] = column_type();
	}
	const_reference operator[] (const size_t i) const {
		try {
			if ( (i >= col_) || (i < 0) ) {
				throw undefined_column_index();;
			}
			return data_[header_[i]];
		}
		catch (std::exception &e) {
			std::terminate();
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
			return data_[header_[i]] = column_type();
		}
		catch(std::exception &e) {
			std::terminate();
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
	std::unordered_map < std::string, T > data_;
	std::vector< std::string > header_;

	/// \returns the default column name
	/// \used when the column name is not set by the user
	std::string default_name(size_t i) {
		return ("Col-" + boost::lexical_cast<std::string>(i));
	}
};

#endif

