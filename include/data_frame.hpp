
#ifndef _DATA_FRAME_HPP
#define _DATA_FRAME_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <boost/numeric/ublas/vector.hpp>
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
			std::terminate();
		}

		// add error conditions for all the column_type having same sizes
	}	

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