
#ifndef _BOOST_UBLAS_DATA_FRAME_
#define _BOOST_UBLAS_DATA_FRAME_

#include <string>
#include <map>
#include <boost/variant.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <algorithm>
#include "./data_frame_exceptions.hpp"
#include "./vector_proxy.cpp"

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
/// \PreProcessing for the inner and outer types
// Expand macro
#define VMACRO(r, product) (ublas::BOOST_PP_SEQ_ELEM(0, product)<BOOST_PP_SEQ_ELEM(1, product)>)


// Make a sequence of all types
#define ALL_VARIANT_TYPES BOOST_PP_SEQ_FOR_EACH_PRODUCT(VMACRO, (VECTOR_TYPE) (INNER_TYPE))
// Variant of column types
#define COLUMN_TYPES BOOST_PP_SEQ_ENUM(ALL_VARIANT_TYPES)
// Variant of the inner types
#define COLUMN_DATA_TYPES BOOST_PP_SEQ_ENUM(INNER_TYPE)


#define VECTOR_RANGE_TYPE (vector_range)
#define ALL_RANGE_TYPES BOOST_PP_SEQ_FOR_EACH_PRODUCT(VMACRO, (VECTOR_RANGE_TYPE) (ALL_VARIANT_TYPES))
#define RANGE BOOST_PP_SEQ_ENUM(ALL_RANGE_TYPES)

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

	// possible expression templates for binary operations
	template <class subType>
	class Expression {
	public:
		// returns const reference of the actual type of this expression 
		inline const subType &self(void) const {
			return *static_cast<const subType*>(this);
		} 
	};

	template <class OP, class E1, class E2>
	class BinaryOP: public Expression < BinaryOP < OP, E1, E2 > > {
	public:
	  	const E1& l;
	  	const E2& r;

	 	 BinaryOP(const E1& l, const E2& r): l(l), r(r) {}
	 
	 	template < class T > 
	  	inline T eval(const std::size_t& i) const {
	    	return OP::op(l.template eval<T>(i), r.template eval<T>(i));
	  	}
	};

	// temporary vectors to be used by binary operators.
	vector < bool > A0; vector < char > A1; vector < unsigned char > A2;
	vector < short > A3; vector < unsigned short > A4; vector < int > A5; vector < unsigned int > A6;
	vector < long > A7; vector < unsigned long > A8; vector < long long > A9; vector < unsigned long long > A10;
	vector < float > A11; vector < double > A12; vector < long double > A13;
	vector < std::string > A14; vector < std::string* > A15;
	
	/**\brief: Representation of column in a data_frame
	 * type: boost::variant < ublas::vector<int>, ublas::vector<char> ..... etc >    
	 */
	class df_column : public boost::variant<COLUMN_TYPES> {

	public:
		
		friend df_column operator + (df_column a, df_column b);
		template < class T >
		friend df_column operator * (const T& x);
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

		BOOST_UBLAS_INLINE 
		~df_column() {}

		// /// \brief: copies the ublas::vector<T> into self.
		// /// \param: const ublas::vector<T>
		// template < class T >
		// BOOST_UBLAS_INLINE
		// df_column& operator = (const ublas::vector<T>& data) {
		// 	base_::operator = (data);
		// 	size_ = data.size();		
		// 	type_ = Type<T>();
		// 	return *this;
		// }	
		
		// /// \brief: moves the ublas::vector<T> into self.
		// /// \param: const ublas::vector<T>
		// template < class T >
		// BOOST_UBLAS_INLINE
		// df_column& operator = (ublas::vector<T>&& data) {
		// 	base_::operator = (std::move(data));
		// 	size_ = data.size();		
		// 	type_ = Type<T>();
		// 	return *this;
		// }

		// /// \brief: copies the col(df_column) into self.
		// /// \param: df_column
		// BOOST_UBLAS_INLINE
		// df_column& operator = (const df_column& col) {
		// 	base_::operator = (col);
		// 	type_ = col.type();
		// 	size_ = col.size();
		// 	return *this;
		// }

		BOOST_UBLAS_INLINE
		template < class T >
		df_column& operator = (const Expression<T>& _x) {
			const T& x = _x.self();
			for(int i = 0; i < size_; ++i) {
				switch (type_) {
					case BOOL: 
						A0(i) = x.template eval<bool>();
						break;
					// add error codes for non addable types.
					case SHORT: 
						A3(i) = x.template eval<short>();
						break;

					case UNSIGNED_SHORT: 
						A4(i) =  x.template eval<unsigned short>();
						break;
					case INT: 
						A5(i) =  x.template eval<int>();
						break;
					case UNSIGNED_INT: 
						A6(i) =  x.template eval<unsigned int>();
						break;
					case LONG: 
						A7(i) =  x.template eval<long>();						
						break;
					case UNSIGNED_LONG: 
						A8(i) =  x.template eval<unsigned long>();
						break;
					case LONG_LONG: 
						A9(i) =  x.template eval<long long>();
						break;
					case UNSIGNED_LONG_LONG: 
						A10(i) = x.template eval<unsigned long long>();
						break;
					case FLOAT: 
						A11(i) = x.template eval<float>();
						break;
					case DOUBLE: 
						A12(i) = x.template eval<double>();						
						break;
					case LONG_DOUBLE: 
						A13(i) = x.template eval<long double>();
						break;
					// case STRING: 
					// 	A14 = a.get<std::string>() + b.get<std::string*>();
					// 	X = A14;
					// 	break;
					// case STRING_STAR: 
					// 	A15 = a.get<std::string*>() + b.get<std::string*>();
					// 	X = A15;
					// 	break;
				}
			}
			return (*this);
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

		BOOST_UBLAS_INLINE
		df_column operator += (df_column col_) {
			return (*this) = (*this) + col_;
		}

		BOOST_UBLAS_INLINE
		template < class T >
		df_column operator *= (const T& x) {
			return (*this) = (*this) * x;
		}

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

		template < class T >
		BOOST_UBLAS_INLINE 
		ublas::vector<T>& get() {
			return boost::get<ublas::vector<T>>(*this); 
		}

		template < class T1, class T2 > 
		BOOST_UBLAS_INLINE 
		T2 Min () {
			T2 Minimum = get<T1> ()(0);
			for(size_t i = 1; i < size_; ++i) {
				if (get <T1> ()(i) < Minimum) {
					Minimum = (T2)get<T1>()(i);
				}
			}	
			return Minimum;
		}

		template < class T1, class T2> 
		BOOST_UBLAS_INLINE 
		T2 Max () {
			T2 Maximum = get<T1>()(0);
			for(size_t i = 1; i < size_; ++i) {
				if (get<T1>()(i) > Maximum) {
					Maximum = (T2) get<T1>()(i);
				}
			}	
			return Maximum;
		}

		template < class T1, class T2 >
		BOOST_UBLAS_INLINE
		T2 Mean() {
			T2 sum = 0;
			for(size_t i = 0; i < size_; ++i) {
				sum += (T2) get<T1>()(i);
			}
			return (T2) (sum / size_);
		}

		template < class T1, class T2 >
		BOOST_UBLAS_INLINE
		T2 Median() {
			ublas::vector<T1> v = get<T1>();
			std::sort(v.begin(), v.end());
			if (v.size() & 1) {
				return (T2)v[v.size()/2];
			}
			else {
				return ((T2)(v[v.size()/2-1] + v[v.size()/2]) / 2);
			}
		}

		template <class T1, class T2>
		void summary() {
			std::cout << "Min. : " << Min <T1, T2>()  << ", ";
			std::cout << "Max. : " << Max <T1, T2>()  << ", ";
			std::cout << "Mean : " << Mean <T1, T2>() << ", ";
			std::cout << "Median : " << Median <T1, T2>() << "  ";
			std::cout << std::endl;	 
		}

		template < class T > 
		const T eval(const size_t& i) const {
			return get<T>(i);
		}

	private:
		typedef boost::variant<COLUMN_TYPES> base_;
	
		size_t size_;
		allowed_inner_types type_;
	};

	class add{
	public: 
  		template < class T>
  		BOOST_UBLAS_INLINE
  		T op(const T& a, const T& b) {
    		return a + b;
   		}
	};

	/* operations on the matrix class */
	/* temporary class to generate the expression template for operation OP */
	template <class OP, class E1, class E2 >
	inline BinaryOP<OP, E1, E2> operate(const Expression<E1>& l, const Expression<E2>& r) {
	  	return BinaryOP<OP, E1, E2>(l.self(), r.self());
	}

	/* Addition operation overload */
	template < class E1, class E2 >
	inline BinaryOP<add, E1, E2> operator + (const Expression<E1>& l, const Expression<E2>& r) {
	 	return operate<add>(l, r);
	}

	// df_column operator + (df_column a, df_column b) {
	// 	try {
	// 		if (a.type() != b.type()) {
	// 			// throw something.
	// 		}

	// 		df_column X;
	// 		switch (a.type()) {
	// 			case BOOL: 
	// 				A0 = a.get<bool>() + b.get<bool>();
	// 				X = A0;
	// 				return X;
	// 			// case CHAR: 
	// 			// 	A1 = a.get<char>() + b.get<char>();
	// 			// 	X = A1;
	// 			// 	break;
	// 			// case UNSIGNED_CHAR: 
	// 			// 	A2 = a.get<unsigned char>() + b.get<unsigned char>();
	// 			// 	X = A2;
	// 			// 	break;
	// 			case SHORT: 
	// 				A3 = a.get<short>() + b.get<short>();
	// 				X = A3;
	// 				return X;
	// 			case UNSIGNED_SHORT: 
	// 				A4 = a.get<unsigned short>() + b.get<unsigned short>();
	// 				X = A4;
	// 				return X;
	// 			case INT: 
	// 				A5 = a.get<int>() + b.get<int>();
	// 				X = A5;
	// 				return X;
	// 			case UNSIGNED_INT: 
	// 				A6 = a.get<unsigned int>() + b.get<unsigned int>();
	// 				X = A6;
	// 				return X;
	// 			case LONG: 
	// 				A7 = a.get<long>() + b.get<long>();
	// 				X = A7;
	// 				return X;
	// 			case UNSIGNED_LONG: 
	// 				A8 = a.get<unsigned long>() + b.get<unsigned long>();
	// 				X = A8;
	// 				return X;
	// 			case LONG_LONG: 
	// 				A9 = a.get<long long>() + b.get<long long>();
	// 				X = A9;
	// 				return X;
	// 			case UNSIGNED_LONG_LONG: 
	// 				A10 = a.get<unsigned long long>() + b.get<unsigned long long>();
	// 				X = A10;
	// 				return X;
	// 			case FLOAT: 
	// 				A11 = a.get<float>() + b.get<float>();
	// 				X = A11;
	// 				return X;
	// 			case DOUBLE: 
	// 				A12 = a.get<double>() + b.get<double>();
	// 				X = A12;
	// 				return X;
	// 			case LONG_DOUBLE: 
	// 				A13 = a.get<long double>() + b.get<long double>();
	// 				X = A13;
	// 				return X;
	// 			// case STRING: 
	// 			// 	A14 = a.get<std::string>() + b.get<std::string*>();
	// 			// 	X = A14;
	// 			// 	break;
	// 			// case STRING_STAR: 
	// 			// 	A15 = a.get<std::string*>() + b.get<std::string*>();
	// 			// 	X = A15;
	// 			// 	break;
	// 		}
	// 		return X;
	// 	}
	// 	catch (std::exception& e) {
	// 		std::terminate();
	// 	}
	// }

	template < class T >
	df_column operator * (df_column a, const T& x) {
		try {
			df_column X;
			switch (a.type()) {
				case BOOL: 
					A0 = x * a.get<bool>();
					X = A0;
					return X;
				// case CHAR: 
				// 	A1 = a.get<char>() + b.get<char>();
				// 	X = A1;
				// 	break;
				// case UNSIGNED_CHAR: 
				// 	A2 = a.get<unsigned char>() + b.get<unsigned char>();
				// 	X = A2;
				// 	break;
				case SHORT: 
					A3 = x * a.get<short>();
					X = A3;
					return X;
				case UNSIGNED_SHORT: 
					A4 = x * a.get<unsigned short>();
					X = A4;
					return X;
				case INT: 
					A5 = x * a.get<int>();
					X = A5;
					return X;
				case UNSIGNED_INT: 
					A6 = x * a.get<unsigned int>();
					X = A6;
					return X;
				case LONG: 
					A7 = x * a.get<long>();
					X = A7;
					return X;
				case UNSIGNED_LONG: 
					A8 = x * a.get<unsigned long>();
					X = A8;
					return X;
				case LONG_LONG: 
					A9 = x * a.get<long long>();
					X = A9;
					return X;
				case UNSIGNED_LONG_LONG: 
					A10 = x * a.get<unsigned long long>();
					X = A10;
					return X;
				case FLOAT: 
					A11 = x * a.get<float>();
					X = A11;
					return X;
				case DOUBLE: 
					A12 = x * a.get<double>();
					X = A12;
					return X;
				case LONG_DOUBLE: 
					A13 = x * a.get<long double>();
					X = A13;
					return X;
				// case STRING: 
				// 	A14 = a.get<std::string>() + b.get<std::string*>();
				// 	X = A14;
				// 	break;
				// case STRING_STAR: 
				// 	A15 = a.get<std::string*>() + b.get<std::string*>();
				// 	X = A15;
				// 	break;
			}
			return X;
		}
		catch (std::exception& e) {
			std::terminate();
		}
	}

	template < class T >
	df_column operator * (const T& x, df_column a) {
		return a * x;
	}

	class data_frame : public std::map<std::string,df_column> {

	public:

		friend data_frame operator + (data_frame a, data_frame b);
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

		BOOST_UBLAS_INLINE
		~data_frame() {}

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
		BOOST_UBLAS_INLINE
		ublas::vector<T>& column(const std::string& header) {
			return (base_::operator[](header)).get<T>();
		}
		template < class T > 
		BOOST_UBLAS_INLINE
		ublas::vector<T>& column(const size_t& i) {
			return (base_::operator[](column_headers_(i))).get<T>();
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
		/// 	0-based indexing

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

		data_frame operator += (data_frame a){
			return (*this) = (*this) + a;
		} 
		BOOST_UBLAS_INLINE 
		void print() {
			for(size_t i = 0; i < ncol_; ++i) {
				std::cout << "[" << column_headers_(i) << "]" << ": ";
				 switch(base_::operator[](column_headers_(i)).type()) {
					case BOOL: 	
						print_column <BOOST_PP_SEQ_ELEM(0, INNER_TYPE) >(i); break;
					case CHAR: 	
						print_column <BOOST_PP_SEQ_ELEM(1, INNER_TYPE) >(i); break;
					case UNSIGNED_CHAR: 
						print_column <BOOST_PP_SEQ_ELEM(2, INNER_TYPE) >(i); break;
					case SHORT: 
						print_column <BOOST_PP_SEQ_ELEM(3, INNER_TYPE) >(i); break;
					case UNSIGNED_SHORT: 
						print_column <BOOST_PP_SEQ_ELEM(4, INNER_TYPE) >(i); break;
					case INT: 
						print_column <BOOST_PP_SEQ_ELEM(5, INNER_TYPE) >(i); break;
					case UNSIGNED_INT: 
						print_column <BOOST_PP_SEQ_ELEM(6, INNER_TYPE) >(i); break;
					case LONG: 
						print_column <BOOST_PP_SEQ_ELEM(7, INNER_TYPE) >(i); break;
					case UNSIGNED_LONG: 
						print_column <BOOST_PP_SEQ_ELEM(8, INNER_TYPE) >(i); break;
					case LONG_LONG: 
						print_column <BOOST_PP_SEQ_ELEM(9, INNER_TYPE) >(i); break;
					case UNSIGNED_LONG_LONG: 
						print_column <BOOST_PP_SEQ_ELEM(10, INNER_TYPE) >(i); break;
					case FLOAT: 
						print_column <BOOST_PP_SEQ_ELEM(11, INNER_TYPE) >(i); break;
					case DOUBLE: 
						print_column <BOOST_PP_SEQ_ELEM(12, INNER_TYPE) >(i); break;
					case LONG_DOUBLE: 
						print_column <BOOST_PP_SEQ_ELEM(13, INNER_TYPE) >(i); break;
					case STRING: 
						print_column <BOOST_PP_SEQ_ELEM(14, INNER_TYPE) >(i); break;
					case STRING_STAR: 
						print_column <BOOST_PP_SEQ_ELEM(15, INNER_TYPE) >(i); break;
				} 
			}
		}

		BOOST_UBLAS_INLINE
		void summary() {
			for(size_t i = 0; i < ncol_; ++i) {
				std::cout << "[" << column_headers_(i) << "]" << ": ";
				 switch(base_::operator[](column_headers_(i)).type()) {
					case BOOL: 	
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(0, INNER_TYPE), long double >(); break;
					// case CHAR: 	
					// 	(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(1, INNER_TYPE) >(); break;
					// case UNSIGNED_CHAR: 
					// 	(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(2, INNER_TYPE) >(); break;
					case SHORT: 
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(3, INNER_TYPE), long double >(); break;
					case UNSIGNED_SHORT: 
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(4, INNER_TYPE), long double >(); break;
					case INT: 
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(5, INNER_TYPE), long double >(); break;
					case UNSIGNED_INT: 
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(6, INNER_TYPE), long double >(); break;
					case LONG: 
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(7, INNER_TYPE), long double >(); break;
					case UNSIGNED_LONG: 
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(8, INNER_TYPE), long double >(); break;
					case LONG_LONG: 
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(9, INNER_TYPE), long double >(); break;
					case UNSIGNED_LONG_LONG: 
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(10, INNER_TYPE), long double >(); break;
					case FLOAT: 
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(11, INNER_TYPE), long double >(); break;
					case DOUBLE: 
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(12, INNER_TYPE), long double >(); break;
					case LONG_DOUBLE: 
						(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(13, INNER_TYPE), long double >(); break;
					// case STRING: 
					// 	(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(14, INNER_TYPE) >(); break;
					// case STRING_STAR: 
					// 	(base_::operator[](column_headers_(i))).summary <BOOST_PP_SEQ_ELEM(15, INNER_TYPE) >(); break;
				} 
			}
		}

		BOOST_UBLAS_INLINE
		ublas::vector<std::string>& headers() {
			return column_headers_;
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

		BOOST_UBLAS_INLINE
		const size_t ncol() const {
			return ncol_;
		}

		BOOST_UBLAS_INLINE
		const size_t nrow() const {
			return nrow_;
		}

		BOOST_UBLAS_INLINE
		const std::string colname(const size_t& i) {
			return column_headers_(i);
		}
	private:
		typedef std::map<std::string, df_column> base_;
		
		size_t ncol_, nrow_;
		ublas::vector<std::string> column_headers_;
		
		// fptr printers[] = { 
		// 	print_column<int>, 
		// 	print_column<float>
		// };

		/// \returns the default column name
		/// \used when the column name is not set by the user
		BOOST_UBLAS_INLINE
		std::string default_name(size_t i) {
			return ("Col-" + boost::lexical_cast<std::string>(i));
		}

		template < class T >
		BOOST_UBLAS_INLINE 
		T& elem (size_t i, size_t row) {
			return (boost::get< ublas::vector< T > >(base_::operator[](column_headers_(i)))) (row);
		}
	};

	data_frame operator + (data_frame a, data_frame b) {
		try {
			// if (a.ncol() != b.ncol()) {
			// 	// throw something
			// }
			// else if (a.nrow() != b.nrow()) {
			// 	// throw something
			// }
			// ublas::vector<std::string> header(a.ncol());
			// ublas::vector<df_column> col(a.ncol());
			// for(size_t i = 0; i < a.ncol(); ++i) {
			// 	if (a.colname(i) != b.colname(i)) {
			// 		// throw something
			// 	}
			// 	header(i) = a.colname(i);
			// 	col(i) = a[i] + b[i];
			// }
			//return data_frame(header, col);
			return data_frame();
		}
		catch (std::exception& e) {
			std::terminate();
		}
	}

	/// \ The current proxies are for columns of a data_frame.
	class data_frame_range {
	public:
		typedef ublas::vector<std::string>::size_type size_type;
		typedef ublas::vector<std::string>::difference_type difference_type;
		typedef ublas::basic_range<size_type, difference_type> range_type;

		BOOST_UBLAS_INLINE
		data_frame_range();

		BOOST_UBLAS_INLINE
		data_frame_range (data_frame* df, const range_type& range): 
			column_headers_(df->headers(), range) {
				df_ = df;
		}

		BOOST_UBLAS_INLINE
		data_frame DataFrame () {
			ublas::vector<std::string> v1(column_headers_.size());
			ublas::vector<ublas::df_column> v2(column_headers_.size());
			for(size_t i = 0; i < column_headers_.size(); ++i) {
				v1(i) = column_headers_(i);
				v2(i) = (*df_)[v1(i)];
			}
			return data_frame(v1, v2);
		} 

		BOOST_UBLAS_INLINE
		df_column& operator [] (const std::string& header) {
			/// \check if the string is included in the given range.
			return (*df_)[header];
		}

		/// here the index is the index in the vector_range
		BOOST_UBLAS_INLINE
		df_column& operator [] (const size_t& i) {
			/// \check if the index is valid.
			return (*df_)[column_headers_[i]];
		}

		BOOST_UBLAS_INLINE
		const size_t size() const {
			return column_headers_.size();
		} 

	private:
		data_frame *df_;
		ublas::vector_range < ublas::vector <std::string> > column_headers_;
	};

	class data_frame_slice {
	public:
		typedef ublas::vector<std::string>::size_type size_type;
		typedef ublas::vector<std::string>::difference_type difference_type;
		typedef ublas::basic_slice<size_type, difference_type> slice_type;

		BOOST_UBLAS_INLINE
		data_frame_slice ();

		BOOST_UBLAS_INLINE
		data_frame_slice (data_frame* df, const slice_type& slice): 
			column_headers_(df->headers(), slice) {
				df_ = df;
		}

		BOOST_UBLAS_INLINE
		data_frame DataFrame () {
			ublas::vector<std::string> v1(column_headers_.size());
			ublas::vector<ublas::df_column> v2(column_headers_.size());
			for(size_t i = 0; i < column_headers_.size(); ++i) {
				v1(i) = column_headers_(i);
				v2(i) = (*df_)[v1(i)];
			}
			return data_frame(v1, v2);
		} 

		BOOST_UBLAS_INLINE
		df_column& operator [] (const std::string& header) {
			/// \check if the string is included in the given range.
			return (*df_)[header];
		}

		/// here the index is the index in the vector_range
		BOOST_UBLAS_INLINE
		df_column& operator [] (const size_t& i) {
			/// \check if the index is valid.
			return (*df_)[column_headers_[i]];
		}

		const size_t size() const {
			return column_headers_.size();
		} 

	private:
		data_frame *df_;
		ublas::vector_slice < ublas::vector <std::string> > column_headers_;
	};

	// // T1 will be boost :: variant < (vector_range / vector_slice / vector_indirect) < vector < int, char.... > >
	// // T2 can be  ( vector_range / vector_slice / vector_indirect ) < std::string > 
	// template < class T1 = df_column, class T2 = vector<std::string> > 
	// class data_frame_proxy {
	// public: 
	// 	data_frame_proxy() {
	// 	}
	// private: 
	// 	T1 row_proxy_;
	// 	T2 column_proxy_;
	// };

	
	// class data_frame_range_range {
	// public:
	// 	#define vr ublas::vector_range 
	// 	data_frame_range_range (data_frame* df, size_t start, size_t end) {
	// 		std::cout << df->ncol() << std::endl;
	// 		rowrange_ .resize(2);
	// 		for(size_t i = 0; i < df->ncol(); ++i) {
	// 			std::cout << "hello" << std::endl;
	// 			allowed_inner_types type = (*df)[i].type();
	// 			switch (type) {
	// 				case 5:
	// 					typedef vr < ublas::vector < BOOST_PP_SEQ_ELEM(5, INNER_TYPE) > >::size_type size_type;
	// 					typedef vr < ublas::vector < BOOST_PP_SEQ_ELEM(5, INNER_TYPE) > >::difference_type difference_type;	
	// 					typedef ublas::basic_range < size_type, difference_type> range_type;
	// 					rowrange_ (i) = vr < vector < BOOST_PP_SEQ_ELEM(5, INNER_TYPE)> > ( (*df).column < BOOST_PP_SEQ_ELEM(5, INNER_TYPE) > (i), range_type(start, end) );
	// 				break;
	// 			}
	// 		}
	// 	}
	// private:
	// 	ublas::vector< boost::variant <RANGE> > rowrange_;
	// 	data_frame* df_;
	// };
}}}

#endif