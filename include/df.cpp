

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
#include <boost/numeric/ublas/vector_proxy.hpp>


/// -----------------
/// ALLOWED TYPES YET
/// -----------------

/// \brief: shows the value stored in a particular vector
/// 		same as COLUMN_TYPES except the NOT_SUPPORTED value
enum allowed_inner_types {
	BOOL, 
	CHAR, UNSIGNED_CHAR,
	SHORT, UNSIGNED_SHORT,
	INT, UNSIGNED_INT,
	LONG, UNSIGNED_LONG,
	LONG_LONG, UNSIGNED_LONG_LONG,
	FLOAT, DOUBLE, LONG_DOUBLE, 
	STRING, STRING_STAR, 
	NOT_SUPPORTED = -1, 
};

/// \Basic allowed types for vectors
#define INNER_TYPE \
(bool) \
(char) (unsigned char) \
(short) (unsigned short) \
(int) (unsigned int) \
(long) (unsigned long) \
(long long) (unsigned long long) \
(float) (double) (long double) \
(std::string) (std::string*) \

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
#define VMACRO(r, product) (ublas::BOOST_PP_SEQ_ELEM(0, product)<BOOST_PP_SEQ_ELEM(1, product)>)
// Make a sequence of all types
#define ALL_VARIANT_TYPES BOOST_PP_SEQ_FOR_EACH_PRODUCT(VMACRO, (VECTOR_TYPE) (INNER_TYPE))
// Variant of column types
#define COLUMN_TYPES BOOST_PP_SEQ_ENUM(ALL_VARIANT_TYPES)
// Variant of the inner types
#define COLUMN_DATA_TYPES BOOST_PP_SEQ_ENUM(INNER_TYPE)

/// \brief: returns the enum value that corresponds to the given type
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

/// \To simplify the code...
namespace ublas = boost::numeric::ublas;

template < class T > 
BOOST_UBLAS_INLINE
void remove(ublas::vector<T>& v, size_t idx) {
    for (size_t i = idx; i < v.size() - 1; i++) {
        v(i) = v(i + 1);
    }
    v.resize(v.size() - 1);
    return;
}

namespace boost { namespace numeric { namespace ublas {

	/** \brief: Representation of column in a data_frame
	 * type: boost::variant < ublas::vector<int>, ublas::vector<char> ..... etc >    
	 */
	class df_column : public boost::variant<COLUMN_TYPES> {

	public:
		
		// Construction and destruction
		
		/// \brief: default constructor
		BOOST_UBLAS_INLINE 
		df_column () : 
			base_(), 
			size_(0) {}

		/// \brief: copies the ublas::vector<T> into a df_column.
		/// \param: const ublas::vector<T>
		template < class T > 
		BOOST_UBLAS_INLINE
		df_column (const ublas::vector<T>& data) :
			base_(data), 
			size_(data.size()), 
			type_(Type<T>()) {}	
		
		/// \brief: moves the ublas::vector<T> into a df_column.
		/// \param: const ublas::vector<T>
		template < class T >
		BOOST_UBLAS_INLINE 
		df_column (ublas::vector<T>&& data) {
			base_(std::forward<base_>(data));
			size_ = std::move(data.size());
			type_ = std::move(Type<T>());
		}

		// BOOST_UBLAS_INLINE 
		// ~df_column();

		//* Add constructors and assignment operators for vector_expression*//!!!

		/// \brief: copies the ublas::vector<T> into self.
		/// \param: const ublas::vector<T>
		template < class T >
		BOOST_UBLAS_INLINE
		df_column& operator = (const ublas::vector<T>& data) {
			base_::operator = (data);
			size_ = data.size();		
			type_ = Type<T>();
			return *this;
		}	
		
		/// \brief: moves the ublas::vector<T> into self.
		/// \param: const ublas::vector<T>
		template < class T >
		BOOST_UBLAS_INLINE
		df_column& operator = (ublas::vector<T>&& data) {
			base_::operator = (std::move(data));
			size_ = data.size();		
			type_ = Type<T>();
			return *this;
		}

		/// \brief: copies the col(df_column) into self.
		/// \param: df_column
		BOOST_UBLAS_INLINE
		df_column& operator = (const df_column& col) {
			base_::operator = (col);
			type_ = col.type();
			size_ = col.size();
			return *this;
		}


		/// \brief: moves the col(df_column) into self.
		/// \param: df_column
		// doesn't work :(. FIX IT!!!!!!!!!!!!
		// BOOST_UBLAS_INLINE
		// df_column& operator = (df_column&& col) {
		// 	base_::operator = (std::move(col));
		// 	type_ = std::move(col.type());
		// 	size_ = std::move(col.size());
		// 	return *this;
		// }

		/// \brief: returns the size of the df_column
		BOOST_UBLAS_INLINE
		const size_t size() const {
			return size_;
		}
		/// \brief: return the type of the ublas::vector(of the df_column data)
		BOOST_UBLAS_INLINE
		const allowed_inner_types type() const {
			return type_;
		}

	private:
		typedef boost::variant<COLUMN_TYPES> base_;
			 
		size_t size_;
		allowed_inner_types type_;
	};

	class data_frame : public std::map<std::string,df_column> {

	public:

		// construction and destruction
		
		/// \default constructor
		BOOST_UBLAS_INLINE 
		data_frame () { 
			ncol_ = 0;
			nrow_ = 0;
			base_();
		}

		/// \params: 	
		/// 	column header
		/// 	data_ in the form of columns
		BOOST_UBLAS_INLINE 
		data_frame (const ublas::vector<std::string>& headers, const ublas::vector<df_column>& data) {
			try {
				/// \exception for unequal headers and columns
				if (headers.size() != data.size()) {
					throw inconsistent_arguments();
				}

				ncol_ = headers.size();
				nrow_ = (ncol_ > 0) ? data(0).size() : 0; 

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
					if ( (i != 0) && (base_::find(headers(i))) != base_::end())  {
						throw same_header();
					}	
					base_::insert({headers(i), data(i)});
				}
			}
			catch (std::exception &e) {
				std::terminate();
			}
		}

		// BOOST_UBLAS_INLINE
		// ~data_frame();

		/// ---------------- ///
		/// column accessors ///
		/// ---------------- ///

		BOOST_UBLAS_INLINE 
		df_column& operator[](const std::string& header) {
			/// \given column name already exists
			/// \return the particular column
			if(base_::find(header) != base_::end()) {
				return base_::operator[](header);
			}
			/// \create the column (will be furthur used by = operator in df_column)
			column_headers_.resize(ncol_ + 1);
			column_headers_(ncol_) = header;
			++ncol_;
			return base_::operator[](header);
		}
		
		BOOST_UBLAS_INLINE 
		df_column& operator[] (const size_t& i) {
			try {
				/// \given column name already exists
				/// \return the particular column
				if(i < ncol_) {
					return base_::operator[](column_headers_[i]);
				}
				else if(i > ncol_) {
					throw holes();
				} 
				/// \custom name if no name is set by default
				column_headers_.resize(ncol_ + 1);
				column_headers_(ncol_) = default_name(ncol_); 
				++ncol_;
				return base_::operator[](column_headers_[i]);
			}
			catch(std::exception &e) {
				std::terminate();
			}
		}

		template < class T > 
		vector<T>& column(const std::string& header) {
			return boost::get(vector<T>(base_::operator[](header)));
		}
		template < class T > 
		vector<T>& column(const size_t& i) {
			return boost::get(vector<T>(base_::operator[](column_headers_(i))));
		}
		
		/// ------------- ///
		/// row accessors ///
		/// ------------- ///
		
		BOOST_UBLAS_INLINE 
		ublas::vector < boost::variant < COLUMN_DATA_TYPES > > operator () (const size_t& row) {
			ublas::vector < boost::variant < COLUMN_DATA_TYPES > > ret(ncol_);
			for(size_t i = 0; i < ncol_; ++i) {
				switch(base_::operator[](column_headers_(i)).type()) {
					case BOOL: 	
						ret(i) = elem<bool>(i, row);
						break;
					case CHAR:
						ret(i) = elem<char>(i, row); 
						break;
					case UNSIGNED_CHAR:
						ret(i) = elem<unsigned char>(i, row); 
						break;
					case SHORT: 
						ret(i) = elem<short>(i, row);
						break;
					case UNSIGNED_SHORT: 
						ret(i) = elem<unsigned short>(i, row);
						break;
					case INT: 
						ret(i) = elem<int>(i, row);
						break;
					case UNSIGNED_INT: 
						ret(i) = elem<unsigned int>(i, row);
						break;
					case LONG: 
						ret(i) = elem<long>(i, row);
						break;
					case UNSIGNED_LONG: 
						ret(i) = elem<unsigned long>(i, row);
						break;
					case LONG_LONG: 
						ret(i) = elem<long long>(i, row);
						break;
					case UNSIGNED_LONG_LONG: 
						ret(i) = elem<unsigned long long>(i, row);
						break;
					case FLOAT: 
						ret(i) = elem<float>(i, row);
						break;
					case DOUBLE: 
						ret(i) = elem<double>(i, row);
						break;
					case LONG_DOUBLE: 
						ret(i) = elem<long double>(i, row);
						break;
					case STRING: 
						ret(i) = elem<std::string>(i, row);
						break;
					case STRING_STAR: 
						ret(i) = elem<std::string*>(i, row);
						break;
				} 
			}	
			return ret;
		}

		// ------------ //
		// erase column //
		// ------------ //

		/// \erase column[$i]
		/// 	0-base_d indexing

		BOOST_UBLAS_INLINE 
		void erase_column(const size_t i) {
			try {
				if ( (i >= column_headers_.size()) || (i < 0)) {
					throw undefined_index();
				}
				
				/// \i is valid so delete the column[$i]
				base_::erase(column_headers_[i]);
				remove(column_headers_, i);
				//column_headers_.erase(column_headers_.begin() + i);
				--ncol_;
			}
			catch(std::exception &e){
				std::terminate();
			}
		}
		/// \erase column[$name]
		BOOST_UBLAS_INLINE 
		void erase_column(const std::string& header) {
			try {
				if (base_::find(header) == base_::end()) {
					throw undefined_column_name();
				}
				/// \name is valid so delete the column[$name]
				base_::erase(header);
				for(size_t i = 0; i < column_headers_.size(); i++) {
					if (header == column_headers_(i)) {
						remove(column_headers_, i);
						break;
					}
				}
				--ncol_;
			}
			catch(std::exception &e) {
				std::terminate();
			}
		}

		BOOST_UBLAS_INLINE 
		void print() {
			for(size_t i = 0; i < ncol_; ++i) {
				std::cout << "[" << column_headers_(i) << "]" << ": ";
				//base_::operator[](column_headers_[i]).type();
				//print_column <BOOST_PP_SEQ_ELEM(type, INNER_TYPE) >(i);
				//print_column <ublas::BOOST_PP_SEQ_ELEM(base_::operator[](column_headers_[i]).type(), 1, INNER_TYPE)>(i)
				
				 switch(base_::operator[](column_headers_(i)).type()) {
					case BOOL: 	
						print_column <BOOST_PP_SEQ_ELEM(0, INNER_TYPE) >(i);
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

		BOOST_UBLAS_INLINE 
		template < class T >
		void print_column(size_t col) {
			for(size_t i = 0; i < nrow_; i++) {
				std::cout << elem<T> (col, i) << ' '; 
			}
			std::cout << std::endl;
			return;
		}	



	private:
		typedef std::map<std::string, df_column> base_;
		
		size_t ncol_, nrow_;
		ublas::vector<std::string> column_headers_;

		/// \returns the default column name
		/// \used when the column name is not set by the user
		std::string default_name(size_t i) {
			return ("Col-" + boost::lexical_cast<std::string>(i));
		}

		template < class T >
		BOOST_UBLAS_INLINE 
		T elem (size_t i, size_t row) {
			return (boost::get< ublas::vector< T > >(base_::operator[](column_headers_(i)))) (row);
		}
	};

	// class data_frame_range {

	// public:

	// 	/// T = basic_range <> ;
	// 	template < class T >
	// 	data_frame_range (data_frame& df, const T& range) :
	// 		df_ (&df), 
	// 		column_headers_ (df.headers(), range) {}

	// 	data_frame DataFrame() {
	// 		// make a vector 
	// 		return data_frame(df.headers(), );
	// 	}

	// private:
	// 	ublas::data_frame &df_;
	// 	ublas::vector_range < ublas::vector <std::string> > column_headers_;
	// };
}}}

int main() {

	using namespace ublas;

	vector < int > X(2);
	X(0) = 0;
	X(1) = 1;

	vector < int > Y(2);
	Y(0) = 2;
	Y(1) = 3;
	df_column x(X), y(Y);

	vector < std::string> v(2);
	v(0) = "x", v(1) = "y";
	vector < df_column > V(2);
	V(0) = x, V(1) = y;
	data_frame df(v, V);
	df.print();
	df_column t = df["x"];
	df["y"] = t;
	df.print();
	df["z"] = t;
	df.print();
	df["a"] = y;
	df.print();
	df.print();
	ublas::vector < boost::variant < COLUMN_DATA_TYPES > > r = df(0);
	std::cout << boost::get<int>(df(0)(0)) << std::endl;
	std::cout << boost::get<int>(df(0)(1)) << std::endl;
	std::cout << df(0)(2) << std::endl;
	return 0;
}