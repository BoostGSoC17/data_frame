
#ifndef _DATA_FRAME_HPP
#define _DATA_FRAME_HPP

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
      return "Insufficient names compared to the data_frame";
   }
};
struct unequal_rows : public std::exception {
   const char * what () const throw () {
      return "Unequal number of rows";
   }
};
/// \iski condition dalio abhi tak nahi dali
struct same_names : public std::exception {
   const char * what () const throw () {
      return "data_frame columns not allowed to have same name";
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




template <class column_type>
/** \Allowed column_type should be:
 * column_vector
 * column_matrix
 * Still to add conditions on allowed datatypes.
*/
class data_frame {
public:
	
	/// \no parameters
	data_frame() {}

	/// \parameters: 
	/// 	column names
	/// 	data in the form of columns
	data_frame(std::vector <std::string> names, std::vector <column_type> Data) {
		try {
			/// \exception for lesser column labels than data_frame columns
			if (names.size() < Data.size()) {
				throw insufficient_names();
			}
			rowcount = (colcount > 0) ? Data[0].size() : 0; 
			/// \exception for unequal no. of rows in data_frame 
			if (colcount) {
				for(size_t i = 1; i < Data.size(); i++) {
					if (Data[i].size() != rowcount) {
						throw unequal_rows();
					}
				}	
			}
			/// \initialise the data members
			for(size_t i = 0; i < names.size(); i++) {
				if (i < Data.size()) {
					column_names.push_back(names[i]);
					data[names[i]] = Data[i];
				}	
				else {
					data[names[i]];
				}
			}
		}
		catch(std::exception e) {
			/// \incorrect instantiation leads to termination of the program
			std::terminate();
		}

	}	

	/// \erase column[$index]
	/// 	0-based indexing
	void erase_column(size_t index) {
		try {
			if ( (index >= column_names.size()) || (index < 0)) {
				throw invalid_index();
			}
			/// \index is valid so delete the column[$index]
			data.erase(column_names[index]);
			column_names.erase(column_names.begin() + index);
		}
		catch(std::exception e){
			std::terminate();
		}
	}

	/// \erase column[$name]
	void erase_column(std::string name) {
		std::cout << "erase call hua" << std::endl;
		try {
			//std::cout << 11 << name << std::endl;
			if (data.find(name) == data.end()) {
				throw invalid_name();
			}
			/// \name is valid so delete the column[$name]
			data.erase(name);
			column_names.erase(std::find(column_names.begin(), column_names.end(), name));
		}
		catch(std::exception e) {
			std::terminate();
		}
	}

	// /// \access operators
	// column_type& operator[](std::string name) {
	// 	/// \given column name already exists
	// 	/// \return the particular column_type
	// 	if(data.find(name) != data.end()) {
	// 		return data[name];
	// 	}
	// }	

	// column_type operator[](size_t index) {
	// 	return data[column_names[index]];
	// }

	void print_info() {
		std::cout << "data.frame data: " << std::endl;
		for(size_t i = 0; i < column_names.size(); ++i) {
			std::cout << column_names[i] << std::endl;
			data[column_names[i]].print_info();
		}
	}

private:
	size_t colcount, rowcount;
	std::unordered_map < std::string, column_type > data;
	std::vector<std::string> column_names;
};

#endif