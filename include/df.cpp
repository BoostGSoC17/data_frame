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


enum allowed_inner_types {
	BOOL, 
	CHAR, UNSIGNED_CHAR,
	SHORT, UNSIGNED_SHORT,
	INT, UNSIGNED_INT,
	LONG, UNSIGNED_LONG,
	LONG_LONG, UNSIGNED_LONG_LONG,
	FLOAT, DOUBLE, LONG_DOUBLE, 
	STRING, STRING_STAR, 
	NOT_SET = -1, 
	NOT_SUPPORTED = -2, 
};


/// \return types of data stored
template <class T>
allowed_inner_types Type() {
	if (std::is_same<T, bool>::value)return INT;
	else if(std::is_same<T, char>::value) return CHAR;
	else if(std::is_same<T, unsigned char>::value) return UNSIGNED_CHAR;
	else if(std::is_same<T, short>::value) return SHORT;
	else if(std::is_same<T, unsigned short>::value) return UNSIGNED_SHORT;
	else if(std::is_same<T, int>::value) return INT;
	else if(std::is_same<T, unsigned int>::value) return UNSIGNED_INT;
	else if(std::is_same<T, long>::value) return LONG;
	else if(std::is_same<T, unsigned long>::value) return UNSIGNED_LONG;
	else if(std::is_same<T, long long>::value) return LONG_LONG;
	else if(std::is_same<T, unsigned long long >::value) return UNSIGNED_LONG_LONG;
	else if(std::is_same<T, float>::value) return FLOAT;
	else if(std::is_same<T, double>::value) return DOUBLE;
	else if(std::is_same<T, long double>::value) return LONG_DOUBLE;
	else if(std::is_same<T, std::string>::value) return STRING;
	else if(std::is_same<T, std::string*>::value) return STRING_STAR;
	else return NOT_SUPPORTED; /// figure out a way to return something invalid
}

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
#define the_list BOOST_PP_SEQ_ENUM(INNER_TYPE)

namespace boost {
	namespace numeric { 	
		namespace ublas {

			class df_column : public boost::variant<THE_LIST> {
			
			public:

				df_column () : boost::variant<THE_LIST>(), type_(NOT_SET), size_(0) {}

				template < class T > 
				df_column (const boost::numeric::ublas::vector<T>& data) :
					base(data), size_(data.size()), type_(Type<T>()) {}

				df_column operator = (const df_column& col) {
					base::operator=(col);
					type_ = col.type();
					size_ = col.size();
					return *this;
				}

				template < class T >
				df_column operator = (const boost::numeric::ublas::vector<T>& data) {
					base::operator=(data);
					type_ = Type<T>();
					size_ = data.size();		
					return *this;
				}

				const size_t size() const {
					return size_;
				}
				const allowed_inner_types type() const {
					return type_;
				}

			private:
				typedef boost::variant<THE_LIST> base;
					 
				allowed_inner_types type_;
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

				/// ------------- ///
				/// row accessors ///
				/// ------------- ///

				std::vector < boost::variant < the_list > > operator () (const size_t row) {
					std::vector < boost::variant < the_list > > ret;
					for(size_t i = 0; i < ncol_; ++i) {
						switch(base::operator[](column_headers_[i]).type()) {
							case BOOL: 	
								ret.push_back(elem<bool>(i, row));
								break;
							case CHAR: 
								ret.push_back(elem<char>(i, row));
								break;
							case UNSIGNED_CHAR: 
								ret.push_back(elem<unsigned char>(i, row));
								break;
							case SHORT: 
								ret.push_back(elem<short>(i, row));
								break;
							case UNSIGNED_SHORT: 
								ret.push_back(elem<unsigned short>(i, row));
								break;
							case INT: 
								ret.push_back(elem<int>(i, row));
								break;
							case UNSIGNED_INT: 
								ret.push_back(elem<unsigned int>(i, row));
								break;
							case LONG: 
								ret.push_back(elem<long>(i, row));
								break;
							case UNSIGNED_LONG: 
								ret.push_back(elem<unsigned long>(i, row));
								break;
							case LONG_LONG: 
								ret.push_back(elem<long long>(i, row));
								break;
							case UNSIGNED_LONG_LONG: 
								ret.push_back(elem<unsigned long long>(i, row));
								break;
							case FLOAT: 
								ret.push_back(elem<float>(i, row));
								break;
							case DOUBLE: 
								ret.push_back(elem<double>(i, row));
								break;
							case LONG_DOUBLE: 
								ret.push_back(elem<long double>(i, row));
								break;
							case STRING: 
								ret.push_back(elem<std::string>(i, row));
								break;
							case STRING_STAR: 
								ret.push_back(elem<std::string*>(i, row));
								break;
						} 
					}
					return ret;
				}

				void print() {
					for(size_t i = 0; i < ncol_; ++i) {
						std::cout << "[" << column_headers_[i] << "]" << ": ";
						switch(base::operator[](column_headers_[i]).type()) {
							case BOOL: 	
								print_column<bool>(i);
								break;
							case CHAR:
								print_column<char>(i);
								break;
							case UNSIGNED_CHAR: 
								print_column<unsigned char>(i);
								break;
							case SHORT: 
								print_column<short>(i);
								break;
							case UNSIGNED_SHORT: 
								print_column<unsigned short>(i);
								break;
							case INT: 
								print_column<int>(i);
								break;
							case UNSIGNED_INT: 
								print_column<unsigned int>(i);
								break;
							case LONG: 
								print_column<long>(i);
								break;
							case UNSIGNED_LONG: 
								print_column<unsigned long>(i);
								break;
							case LONG_LONG: 
								print_column<long long>(i);
								break;
							case UNSIGNED_LONG_LONG: 
								print_column<unsigned long long>(i);
								break;
							case FLOAT: 
								print_column<float>(i);
								break;
							case DOUBLE: 
								print_column<double>(i);
								break;
							case LONG_DOUBLE: 
								print_column<long double>(i);
								break;
							case STRING: 
								print_column<std::string>(i);
								break;
							case STRING_STAR: 
								print_column<std::string*>(i);
								break;
						} 
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


				template < class T >
				T elem (size_t i, size_t row) {
					return (boost::get<boost::numeric::ublas::vector<T>>( base::operator[](column_headers_[i])) )(row);
				}

				template < class T >
				void print_column(size_t col) {
					for(size_t i = 0; i < nrow_; i++) {
						std::cout << elem<T>(col, i) << ' '; 
					}
					std::cout << std::endl;
					return;
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
	df(0);
	df.print();
	return 0;
}