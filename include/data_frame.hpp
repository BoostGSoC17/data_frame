#ifndef _data__FRAME_HPP
#define _data__FRAME_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include "./column_vector.hpp"
#include <exception>

struct insufficient_names : public std::exception {
   const char * what () const throw () {
      return "Insufficient names compared to the data__frame";
   }
};
struct unequal_rows : public std::exception {
   const char * what () const throw () {
      return "Unequal number of rows";
   }
};
/* iski condition dalio abhi tak nahi dali */
struct same_names : public std::exception {
   const char * what () const throw () {
      return "data__frame columns not allowed to have same name";
   }
};
struct invalid_index : public std::exception {
   const char * what () const throw () {
      return "Invalid index";
   }
};
struct invalid_name : public std::exception {
   const char * what () const throw () {
      return "Invalid name";
   }
};


/** \Allowed column_type should be:
 * column_vector
 * column_matrix
 * Still to add conditions on allowed data_types.
*/
template <class column_type>
class data_frame {
public:
	
	/// \no parameters
	data_frame() {}

	/// \parameters: 
	/// 	column names
	/// 	data_ in the form of columns
	data_frame(std::vector <std::string> names, std::vector <column_type> data) {
		try {
			/// \exception for lesser column labels than data__frame columns
			if (names.size() < data.size()) {
				throw insufficient_names();
			}
			rowcount = (colcount > 0) ? data[0].size() : 0; 
			/// \exception for unequal no. of rows in data__frame 
			if (colcount) {
				for(size_t i = 1; i < data.size(); i++) {
					if (data[i].size() != rowcount) {
						throw unequal_rows();
					}
				}	
			}
			/// \initialise the data_ members
			for(size_t i = 0; i < names.size(); i++) {
				if (i < data_.size()) {
					column_names.push_back(names[i]);
					data_[names[i]] = data[i];
				}	
				else {
					data_[names[i]];
				}
			}
		}
		catch(std::exception e) {
			/// \incorrect instantiation leads to termination of the program
			std::terminate();
		}

	}	

	// ------------ // 
	// erase column //
	// ------------ //

	/// \erase column[$index]
	/// 	0-based indexing
	void erase_column(size_t index) {
		try {
			if ( (index >= column_names.size()) || (index < 0)) {
				throw invalid_index();
			}
			/// \index is valid so delete the column[$index]
			data_.erase(column_names[index]);
			column_names.erase(column_names.begin() + index);
		}
		catch(std::exception e){
			std::terminate();
		}
	}
	/// \erase column[$name]
	void erase_column(std::string name) {
		try {
			if (data_.find(name) == data_.end()) {
				throw invalid_name();
			}
			/// \name is valid so delete the column[$name]
			data_.erase(name);
			column_names.erase(std::find(column_names.begin(), column_names.end(), name));
		}
		catch(std::exception e) {
			std::terminate();
		}
	}


	/// \access operators
	const column_type operator[] (const std::string name) const {
		return data_[name];
	}	
	column_type& operator[](const std::string name) {
		/// \given column name already exists
		/// \return the particular column_type
		if(data_.find(name) != data_.end()) {
			return data_[name];
		}
		return data_[name] = column_type();
	}

	const column_type operator[] (const size_t index) const {
		return data_[column_names[index]];
	}	
	
	void print_info() {
		std::cout << "data.frame data_: " << std::endl;
		for(size_t i = 0; i < column_names.size(); ++i) {
			std::cout << column_names[i] << std::endl;
			data_[column_names[i]].print_info();
		}
	}

private:
	size_t colcount, rowcount;
	std::unordered_map < std::string, column_type > data_;
	std::vector< std::string > column_names;
};

#endif