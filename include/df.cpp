#include <string>
#include <map>
#include <vector>
#include <boost/variant.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/lexical_cast.hpp>
#include "./data_frame_exceptions.hpp"
// Basic allowed types for vectors
#define INNER_TYPE \
(bool) \
(char) (unsigned char) \
(short) (unsigned short) \
(int) (unsigned int) \
(long) (unsigned long) \
(long long) (unsigned long long) \
(float) (double) (long double) \
(std::string) (std::string*)

// Vector allowed types for columns
/* WARNING: we can't use all the vector types for the time being because boost::variant has a limit
 * of 20 different types because of another limitation in boost MPL
#define VECTOR_TYPE (scalar_vector)(unit_vector)(vector)(zero_vector)
*/

#define VECTOR_TYPE (vector)

// The following types will be integrated later
/*bounded_vector
compressed_vector
coordinate_vector
c_vector
mapped_vector
mapped_vector_of_mapped_vector
matrix_column_vector
matrix_row_vector
matrix_vector_indirect
matrix_vector_range
matrix_vector_slice
vector_indirect
vector_of_vector
vector_range
vector_slice
*/

// Expand macro
#define VMACRO(r, product) (boost::numeric::ublas::BOOST_PP_SEQ_ELEM(0, product)<BOOST_PP_SEQ_ELEM(1, product)>)

// Make a sequence of all types
#define ALL_VARIANT_TYPES BOOST_PP_SEQ_FOR_EACH_PRODUCT(VMACRO, (VECTOR_TYPE) (INNER_TYPE))
#define THE_LIST BOOST_PP_SEQ_ENUM(ALL_VARIANT_TYPES)

// Variant of the inner types
//#define the_list BOOST_PP_SEQ_ENUM(INNER_TYPE)

// template <class T>
// short Type() {

// }

namespace boost {

	namespace numeric { 
		
		namespace ublas {

			class df_column : public boost::variant<THE_LIST> {
			
			public:

				df_column () : boost::variant<THE_LIST>() {}

				template < class T > 
				df_column (const boost::numeric::ublas::vector<T>& data) :
					base(data), size_(data.size()) {}

				df_column operator = (const df_column& col) {
					base::operator=(col);
					return *this;
				}

				template < class T >
				df_column operator = (const boost::numeric::ublas::vector<T>& data) {
					base::operator=(data);
					return *this;
				}

				const size_t size() const {
					return size_;
				}

			private:
				typedef boost::variant<THE_LIST> base;
					 
				short type_;
				size_t size_;
			};

			class data_frame : public std::map<std::string,df_column> {

			public:

				/// \default constructor: no params
				data_frame () { 
					ncol_ = 0;
					nrow_ = 0;
					std::map<std::string, df_column>();
				}

				/// \params: 	
				/// 	column header
				/// 	data_ in the form of columns
				data_frame (const std::vector<std::string>& headers, const std::vector<df_column>& data) {
					try {
						/// \exception for lesser column headers than columns
						if (headers.size() != data.size()) {
							throw inconsistent_arguments();
						}

						ncol_ = headers.size();
						nrow_ = (ncol_ > 0) ? data[0].size() : 0; 

						/// \exception for unequal no. of rows in data_frame columns
						for(size_t i = 1; i < ncol_; ++i) {
							if (data[i].size() != nrow_) {
								throw differing_rows();
							}
						}	

						/// \initialise the data_ members
	 					column_headers_ = headers;	
						for(size_t i = 0; i < ncol_; ++i) {
							// if a column_name already exists throw an error
							if ( (i != 0) && (base::find(headers[i])) != base::end())  {
								throw same_header();
							}	
							base::insert({headers[i], data[i]});
						}
					}
					catch (std::exception &e) {
						std::terminate();
					}
				}

				/// ---------------- ///
				/// column accessors ///
				/// ---------------- ///

				// const df_column& operator[] (const std::string& header) const {
				// 	try {
				// 		std::cout << header << ": dekh" << std::endl;
				// 		if (base::find(header) == base::end()) {
				// 			//throw undefined_name();
				// 		}
				// 		const df_column t = (*(base::find(header))).second;
				// 		return t;
				// 	} 
				// 	catch (std::exception &e) {
				// 		std::terminate();
				// 	}
				// }	

				df_column& operator[](const std::string& header) {
					/// \given column name already exists
					/// \return the particular column

					if(base::find(header) != base::end()) {
						return base::operator[](header);
					}

					/// \create the column (will be furthur used by = operator in df_column)
					column_headers_.push_back(header);
					++ncol_;
					return base::operator[](header);
				}
				
				// const df_column& operator[] (const size_t i) const {
				// 	try {
				// 		if ( (i >= col_) || (i < 0) ) {
				// 			throw undefined_index();
				// 		}
				// 		return base::operator[](column_headers_[i]);
				// 	}
				// 	catch (std::exception &e) {
				// 		std::terminate();
				// 	}
				// }

				df_column& operator[] (const size_t i) {
					/// \given column name already exists
					/// \return the particular column
					if(i < ncol_) {
						return base::operator[](column_headers_[i]);
					}
					try {
						if(i > ncol_) {
							throw holes();
						} 
						/// \custom name if no name is set by default
						column_headers_.push_back(default_name(i));
						++ncol_;
						return base::operator[](column_headers_[i]);
					}
					catch(std::exception &e) {
						std::terminate();
					}
				}
				
			private:
				typedef std::map<std::string, df_column> base;
				
				size_t ncol_, nrow_;
				std::vector<std::string> column_headers_;

				/// \returns the default column name
				/// \used when the column name is not set by the user
				std::string default_name(size_t i) {
					return ("Col-" + boost::lexical_cast<std::string>(i));
				}
			};
}}}

int main() {

	using namespace boost::numeric::ublas;

	vector < int > X(1);
	X(0) = 1;
	vector < char > Y(1);
	Y(0) = 'a';

	df_column x(X), y(Y);
	data_frame df({"x", "y"}, {x, y});
	df_column t = df["x"];

	return 0;
}