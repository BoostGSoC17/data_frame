
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


namespace boost { namespace numeric { namespace ublas {
	
	// temporary vectors to be used by binary operators.
	vector < bool > A0; vector < char > A1; vector < unsigned char > A2;
	vector < short > A3; vector < unsigned short > A4; vector < int > A5; vector < unsigned int > A6;
	vector < long > A7; vector < unsigned long > A8; vector < long long > A9; vector < unsigned long long > A10;
	vector < float > A11; vector < double > A12; vector < long double > A13;
	vector < std::string > A14; vector < std::string* > A15;
	
	class print_data_frame_column: public boost::static_visitor<void> {
	public:
		template < class T >
		void operator () (const T& x_) const {
			for(size_t i = 0; i < x_.size(); ++i) {
				std::cout << x_ (i) << ' ';
			}
			std::cout << std::endl;
			return;
		}
	};

	class df_column {
	public:
		friend df_column operator + (df_column& a, df_column& b);
		friend df_column operator - (df_column& a, df_column& b);
		template < class T > friend df_column operator + (df_column& a, const T& val);
		template < class T > friend df_column operator - (df_column& a, const T& val);
		template < class T > friend df_column operator * (df_column& a, const T& val);
		
		df_column (): 
			size_ (0) {}

		/// Copy Constructor
		template < class T > 
		df_column (const vector<T>& data) :
			data_ (data), 
			size_ (data.size()) {}

		df_column (const df_column& col) :
			data_ (col.data()), 
			size_ (col.size()) {}

		/// Move Constructor
		template < class T > 
		df_column (const vector <T>&& data) {
			data_ = std::move(data);
			size_ = std::move(size_);
		} 

		df_column (const df_column&& col) {
			data_ = std::move(col.data());
			size_ = std::move(col.size());
		} 
		BOOST_UBLAS_INLINE
		~df_column() {}
		
		/// Copy Assignment Operator
		template < class T > 
		df_column& operator = (const vector<T>& data) {
			data_ = data;
			size_ = data.size(); 
			return *this;
		}

		df_column& operator = (const df_column& col) {
			data_  = col.data();
			size_  = col.size();
			return *this;
		}

		/// Move Assignment Operator
		template < class T > 
		df_column& operator = (const vector <T>&& data) {
			data_ = std::move(data);
			size_ = std::move(size_);
			return *this;
		}

		df_column& operator = (const df_column&& col) {
			data_ = std::move(col.data());
			size_ = std::move(col.size());
			return *this;
		}	

		df_column operator += (df_column& x) {
			(*this) = (*this) + x;
			return *this;
		}
		df_column operator -= (df_column& x) {
			return (*this) = (*this) - x;
		}
		template < class T > 
		df_column operator += (const T& val) {
			return (*this) = (*this) + val;
		}
		template < class T > 
		df_column operator -= (const T& val) {
			return (*this) = (*this) - val;
		}
		template < class T > 
		df_column operator *= (const T& val) {
			return (*this) = (*this) * val;
		}

		/// Storage Accessors
		const size_t size() const {
			return size_;
		}	
		const boost::variant < COLUMN_TYPES > data() const {
			return data_;
		} 
		const short type() const {
			return data_.which();
		}

		template < class T >
		BOOST_UBLAS_INLINE 
		vector<T>& get() {
			return boost::get<vector<T>>(data_); 
		}

		template < class T > 
		const T eval(const size_t& i) const {
			return get<T> (i);
		}

		void print() {
			boost::apply_visitor (print_data_frame_column{}, data_);
		}
		
	private:
		boost::variant < COLUMN_TYPES > data_;
		size_t size_;
	};

	df_column operator - (df_column& a) {
		try {	
			df_column X;
			switch (a.type()) {
				case 0: 
					A0 = -a.get<bool>();
					return X = A0;
				case 3: 
					A3 = -a.get<short>();
					X = A3;
					return X;
				case 4: 
					A4 = -a.get<unsigned short>();
					X = A4;
					return X;
				case 5: 
					A5 = -a.get<int>();
					X = A5;
					return X;
				case 6: 
					A6 = -a.get<unsigned int>();
					X = A6;
					return X;
				case 7: 
					A7 = -a.get<long>();
					X = A7;
					return X;
				case 8: 
					A8 = -a.get<unsigned long>();
					X = A8;
					return X;
				case 9: 
					A9 = -a.get<long long>();
					X = A9;
					return X;
				case 10: 
					A10 = -a.get<unsigned long long>();
					X = A10;
					return X;
				case 11: 
					A11 = -a.get<float>();
					X = A11;
					return X;
				case 12: 
					A12 = -a.get<double>();
					X = A12;
					return X;
				case 13: 
					A13 = -a.get<long double>();
					X = A13;
					return X;
			}
		}
		catch(std::exception& e) {
			std::terminate();
		}
	}
	df_column operator + (df_column& a, df_column& b) {
		try {
			if (a.type() != b.type()) {
				// throw something
			}	
			df_column X;
			switch (a.type()) {
				case 0: 
					A0 = a.get<bool>() + b.get<bool>();
					X = A0;
					return X;
				case 3: 
					A3 = a.get<short>() + b.get<short>();
					X = A3;
					return X;
				case 4: 
					A4 = a.get<unsigned short>() + b.get<unsigned short>();
					X = A4;
					return X;
				case 5: 
					A5 = a.get<int>() + b.get<int>();
					X = A5;
					return X;
				case 6: 
					A6 = a.get<unsigned int>() + b.get<unsigned int>();
					X = A6;
					return X;
				case 7: 
					A7 = a.get<long>() + b.get<long>();
					X = A7;
					return X;
				case 8: 
					A8 = a.get<unsigned long>() + b.get<unsigned long>();
					X = A8;
					return X;
				case 9: 
					A9 = a.get<long long>() + b.get<long long>();
					X = A9;
					return X;
				case 10: 
					A10 = a.get<unsigned long long>() + b.get<unsigned long long>();
					X = A10;
					return X;
				case 11: 
					A11 = a.get<float>() + b.get<float>();
					X = A11;
					return X;
				case 12: 
					A12 = a.get<double>() + b.get<double>();
					X = A12;
					return X;
				case 13: 
					A13 = a.get<long double>() + b.get<long double>();
					X = A13;
					return X;
			}
		}
		catch(std::exception& e) {
			std::terminate();
		}
	}
	df_column operator - (df_column& a, df_column& b) {
		try {
			if (a.type() != b.type()) {
				// throw something
			}	
			df_column X;
			switch (a.type()) {
				case 0: 
					A0 = a.get<bool>() - b.get<bool>();
					X = A0;
					return X;
				case 3: 
					A3 = a.get<short>() - b.get<short>();
					X = A3;
					return X;
				case 4: 
					A4 = a.get<unsigned short>() - b.get<unsigned short>();
					X = A4;
					return X;
				case 5: 
					A5 = a.get<int>() - b.get<int>();
					X = A5;
					return X;
				case 6: 
					A6 = a.get<unsigned int>() - b.get<unsigned int>();
					X = A6;
					return X;
				case 7: 
					A7 = a.get<long>() - b.get<long>();
					X = A7;
					return X;
				case 8: 
					A8 = a.get<unsigned long>() - b.get<unsigned long>();
					X = A8;
					return X;
				case 9: 
					A9 = a.get<long long>() - b.get<long long>();
					X = A9;
					return X;
				case 10: 
					A10 = a.get<unsigned long long>() - b.get<unsigned long long>();
					X = A10;
					return X;
				case 11: 
					A11 = a.get<float>() - b.get<float>();
					X = A11;
					return X;
				case 12: 
					A12 = a.get<double>() - b.get<double>();
					X = A12;
					return X;
				case 13: 
					A13 = a.get<long double>() - b.get<long double>();
					X = A13;
					return X;
			}
		}
		catch(std::exception& e) {
			std::terminate();
		}
	}
	template < class T > 
	df_column operator + (df_column& a, const T& val) {
		try {
			df_column X;
			for(size_t i = 0; i < a.size(); ++i) {
				switch (a.type()) {
					case 0: 
						if (i == 0) {
							A0.resize(a.size());
						}
						A0(i) = a.get<bool>()(i) + val;
						if (i == a.size()-1) {
							X = A0;
							A0.resize(0);
							return X;
						}
					break;
					case 3:
						if (i == 0) {
							A3.resize(a.size());
						} 
						A3(i) = a.get<short>()(i) + val;
						if (i == a.size()-1) {
							X = A3;
							A3.resize(0);
							return X;
						}
					break;
					case 4: 
						if (i == 0) {
							A4.resize(a.size());
						}
						A4(i) = a.get<unsigned short>()(i) + val;
						if (i == a.size()-1) {
							X = A4;
							A4.resize(0);
							return X;
						}
					break;
					case 5: 
						if (i == 0) {
							A5.resize(a.size());
						}
						A5(i) = a.get<int>()(i) + val;
						if (i == a.size()-1) {
							X = A5;
							A5.resize(0);
							return X;
						}
					break;
					case 6:
						if (i == 0) {
							A6.resize(a.size());
						} 
						A6(i) = a.get<unsigned int>()(i) + val;
						if (i == a.size()-1) {
							X = A6;
							A6.resize(0);
							return X;
						}
					break;
					case 7:
						if (i == 0) {
							A7.resize(a.size());
						} 
						A7(i) = a.get<long>()(i) + val;
						if (i == a.size()-1) {
							X = A7;
							A7.resize(0);
							return X;
						}
					break;
					case 8: 
						if (i == 0) {
							A8.resize(a.size());
						}
						A8(i) = a.get<unsigned long>()(i) + val;
						if (i == a.size()-1) {
							X = A8;
							A8.resize(0);
							return X;
						}
					break;
					case 9:
						if (i == 0) {
							A9.resize(a.size());
						} 
						A9(i) = a.get<long long>()(i) + val;
						if (i == a.size()-1) {
							X = A9;
							A9.resize(0);
							return X;
						}
					break;
					case 10:
						if (i == 0) {
							A10.resize(a.size());
						} 
						A10(i) = a.get<unsigned long long>()(i) + val;
						if (i == a.size()-1) {
							X = A10;
							A10.resize(0);
							return X;
						}
					break;
					case 11:
						if (i == 0) {
							A11.resize(a.size());
						} 
						A11(i) = a.get<float>()(i) + val;
						if (i == a.size()-1) {
							X = A11;
							A11.resize(0);
							return X;
						}
					break;
					case 12:
						if (i == 0) {
							A12.resize(a.size());
						} 
						A12(i) = a.get<double>()(i) + val;
						if (i == a.size()-1) {
							X = A12;
							A12.resize(0);
							return X;
						}
					break;
					case 13:
						if (i == 0) {
							A13.resize(a.size());
						} 
						A13(i) = a.get<long double>()(i) + val;
						if (i == a.size()-1) {
							X = A13;
							A13.resize(0);
							return X;
						}
					break;
				}
			}
		}
		catch(std::exception& e) {
			std::terminate();
		}
	}
	template < class T > 
	df_column operator + (const T& val, df_column& a) {
		return a + val;
	}
	template < class T > 
	df_column operator - (df_column& a, const T& val) {
		return a + (-val);
	}
	template < class T > 
	df_column operator * (df_column& a, const T& val) {
		try {
			df_column X;
			switch (a.type()) {
				case 0: 
					A0 = a.get<bool>() * val;
					X = A0;
					return X;
				case 3: 
					A3 = a.get<short>() * val;
					X = A3;
					return X;
				case 4: 
					A4 = a.get<unsigned short>() * val;
					X = A4;
					return X;
				case 5: 
					A5 = a.get<int>() * val;
					X = A5;
					return X;
				case 6: 
					A6 = a.get<unsigned int>() * val;
					X = A6;
					return X;
				case 7: 
					A7 = a.get<long>() * val;
					X = A7;
					return X;
				case 8: 
					A8 = a.get<unsigned long>() * val;
					X = A8;
					return X;
				case 9: 
					A9 = a.get<long long>() * val;
					X = A9;
					return X;
				case 10: 
					A10 = a.get<unsigned long long>() * val;
					X = A10;
					return X;
				case 11: 
					A11 = a.get<float>() * val;
					X = A11;
					return X;
				case 12: 
					A12 = a.get<double>() * val;
					X = A12;
					return X;
				case 13: 
					A13 = a.get<long double>() * val;
					X = A13;
					return X;
			}
		}
		catch(std::exception& e) {
			std::terminate();
		}
	}

	template < class T > 
	df_column operator * (const T& val, df_column& a) {
		return a * val;
	}

	bool operator == (df_column& y, df_column& x) {
		if (y.type() != x.type()) {
			return false;
		}
		else if(y.size() != x.size()) {
			return false;
		}
		for(size_t i = 0; i < x.size(); ++i) {
			switch (y.type()) {
				case 0: if((y.get<bool>()(i) != x.get<bool>()(i)) ) {
					return false;
				}
				break;
				case 1: if((y.get<char>()(i) != x.get<char>()(i)) ) {
					return false;
				}
				break;
				case 2: if((y.get<unsigned char>()(i) != x.get<unsigned char>()(i)) ) {
					return false;
				}
				break;
				case 3: if((y.get<short>()(i) != x.get<short>()(i)) ) {
					return false;
				}
				break;
				case 4: if((y.get<unsigned short>()(i) != x.get<unsigned short>()(i)) ) {
					return false;
				}
				break;
				case 5: if((y.get<int>()(i) != x.get<int>()(i)) ) {
					return false;
				}
				break;
				case 6: if((y.get<unsigned int>()(i) != x.get<unsigned int>()(i)) ) {
					return false;
				}
				break;
				case 7: if((y.get<long>()(i) != x.get<long>()(i)) ) {
					return false;
				}
				break;
				case 8: if((y.get<unsigned long>()(i) != x.get<unsigned long>()(i)) ) {
					return false;
				}
				break;
				case 9: if((y.get<long long>()(i) != x.get<long long>()(i)) ) {
					return false;
				}
				break;
				case 10: if((y.get<unsigned long long>()(i) != x.get<unsigned long long>()(i)) ) {
					return false;
				}
				break;
				case 11: if((y.get<float>()(i) != x.get<float>()(i)) ) {
					return false;
				}
				break;
				case 12: if((y.get<double>()(i) != x.get<double>()(i)) ) {
					return false;
				}
				break;
				case 13: if((y.get<long double>()(i) != x.get<long double>()(i)) ) {
					return false;
				}
				break;
				case 14: if((y.get<std::string>()(i) != x.get<std::string>()(i)) ) {
					return false;
				}
				break;
				case 15: if((y.get<std::string*>()(i) != x.get<std::string*>()(i)) ) {
					return false;
				}
				break;
			}
		}
		return true;
	}

	bool operator != (df_column& y, df_column& x) {
		return !(x == y);
	}


	BOOST_UBLAS_INLINE
	void remove (vector<std::string>& v, size_t idx) {
	    for (size_t i = idx; i < v.size() - 1; i++) {
	        v(i) = v(i + 1);
	    }
	    v.resize(v.size() - 1);
	    return;
	}

	class data_frame {
	public:
		// construction and destruction
		
		/// \default constructor
		BOOST_UBLAS_INLINE 
		data_frame () :
			ncol_ (0),
			nrow_ (0) {}

		/// \params: 	
		/// 	column header
		/// 	data_ in the form of columns
		BOOST_UBLAS_INLINE 
		data_frame (const vector<std::string>& headers, const vector<df_column>& data) {
			try {
				// std::cout << "hello" << std::endl;
				/// \exception for unequal headers and columns
				if (headers.size() != data.size()) {
					throw inconsistent_arguments();
				}

				ncol_ = headers.size();
				nrow_ = (ncol_ > 0) ? data(0).size() : 0; 

				/// \exception for unequal no. of rows in data_frame columns
				for(size_t i = 1; i < ncol_; ++i) {
					if (data (i).size() != nrow_) {
						throw differing_rows();
					}
				}	

				/// \initialise the data_ members
				column_headers_ = headers;	
				
				for(size_t i = 0; i < ncol_; ++i) {
					// if a column_name already exists throw an error
					if ( (i != 0) && (data_.find(headers(i))) != data_.end())  {
						throw same_header();
					}	
					data_.insert({headers(i), data(i)});
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
			if(data_.find(header) != data_.end()) {
				return data_ [header];
			}
			/// \create the column (will be furthur used by = operator in df_column)
			column_headers_.resize(ncol_ + 1);
			column_headers_(ncol_) = header;
			++ncol_;
			return data_ [header];
		}
		
		BOOST_UBLAS_INLINE 
		df_column& operator[] (const size_t& i) {
			try {
				/// \given column name already exists
				/// \return the particular column
				if(i < ncol_) {
					return data_ [column_headers_ (i)];
				}
				else if(i > ncol_) {
					throw holes();
				} 
				/// \custom name if no name is set by default
				column_headers_.resize(ncol_ + 1);
				column_headers_(ncol_) = default_name(ncol_); 
				++ncol_;
				return data_ [column_headers_(i)];
			}
			catch(std::exception &e) {
				std::terminate();
			}
		}
		
		
		template < class T > 
		BOOST_UBLAS_INLINE
		vector<T>& column (const std::string& header) {
			return data_[header].get<T>();
		}
		template < class T > 
		BOOST_UBLAS_INLINE
		vector<T>& column(const size_t& i) {
			return data_[column_headers_ (i)].get<T>();
		}

		/* Add row accessors */ 

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
				data_.erase(column_headers_[i]);
				remove(column_headers_, i);
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
				if (data_.find(header) == data_.end()) {
					throw undefined_column_name();
				}
				/// \name is valid so delete the column[$name]
				data_.erase(header);
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
				data_ [column_headers_(i)].print();
			}
		}

		BOOST_UBLAS_INLINE
		vector<std::string>& headers() {
			return column_headers_;
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
		std::map < std::string, df_column > data_;
		size_t ncol_, nrow_;
		vector < std::string> column_headers_;
		
		/// \returns the default column name
		/// \used when the column name is not set by the user
		BOOST_UBLAS_INLINE
		std::string default_name(size_t i) {
			return ("Col-" + boost::lexical_cast<std::string>(i));
		}
	};

	data_frame operator + (data_frame& a, data_frame& b) {
		try {
			if (a.ncol() != b.ncol()) {
				// throw something
			}
			else if (a.nrow() != b.nrow()) {
				// throw something
			}
			vector<std::string> header(a.ncol());
			vector<df_column> col(a.ncol());
			for(size_t i = 0; i < a.ncol(); ++i) {
				if (a.colname(i) != b.colname(i)) {
					// throw something
				}
				header(i) = a.colname(i);
				col(i) = a[i] + b[i];
			}
			return data_frame(header, col);
		}
		catch (std::exception& e) {
			std::terminate();
		}
	}

	data_frame operator - (data_frame& a, data_frame& b) {
		try {
			if (a.ncol() != b.ncol()) {
				// throw something
			}
			else if (a.nrow() != b.nrow()) {
				// throw something
			}
			vector<std::string> header(a.ncol());
			vector<df_column> col(a.ncol());
			for(size_t i = 0; i < a.ncol(); ++i) {
				if (a.colname(i) != b.colname(i)) {
					// throw something
				}
				header(i) = a.colname(i);
				col(i) = a[i] - b[i];
			}
			return data_frame(header, col);
		}
		catch (std::exception& e) {
			std::terminate();
		}
	}

	template < class T >
	data_frame operator + (data_frame& a, const T& val) {
		try {
			vector<std::string> header(a.ncol());
			vector<df_column> col(a.ncol());
			for(size_t i = 0; i < a.ncol(); ++i) {
				header(i) = a.colname(i);
				col(i) = a[i] + val;
			}
			return data_frame(header, col);
		}
		catch (std::exception& e) {
			std::terminate();
		}
	}

	template < class T > 
	data_frame operator + (const T& val, data_frame& a) {
		return a + val;
	}

	template < class T > 
	data_frame operator - (const T& val, data_frame& a) {
		return a + (-val);
	}

	template < class T >
	data_frame operator * (data_frame& a, const T& val) {
		try {
			vector<std::string> header(a.ncol());
			vector<df_column> col(a.ncol());
			for(size_t i = 0; i < a.ncol(); ++i) {
				header(i) = a.colname(i);
				col(i) = a[i] * val;
			}
			return data_frame(header, col);
		}
		catch (std::exception& e) {
			std::terminate();
		}
	}

	template < class T > 
	data_frame operator * (const T& val, data_frame& a) {
		return a * val;
	}

	bool operator == (data_frame& a, data_frame& b) {
		if (a.ncol() != b.ncol()) {
			return false;
		}
		else if(a.nrow() != b.nrow()) {
			return false;
		}
		for(size_t i = 0; i < a.ncol(); ++i) {
			if (a.headers()(i) != b.headers()(i)) {
				return false;
			}
			else if(a[i] != b[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator != (data_frame& a, data_frame& b) {
		return !(a == b);
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
}}}

#endif
