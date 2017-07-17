#include <iostream>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace boost { namespace numeric { namespace ublas {

	//! Will serve as an interface class between the 3 proxies and the user.
	template < class T >
	class vector_proxy {
	public:

		typedef typename T::size_type size_type;
        typedef typename T::difference_type difference_type;
        typedef typename T::value_type value_type;
        typedef typename T::const_reference const_reference;
        typedef typename T::reference reference;

		BOOST_UBLAS_INLINE
		vector_proxy();

		BOOST_UBLAS_INLINE
		vector_proxy(const T& proxy): 
			proxy_ (proxy) {}

		BOOST_UBLAS_INLINE
		~vector_proxy() {}

		
#ifndef BOOST_UBLAS_PROXY_CONST_MEMBER
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i) const {
            return proxy_ (i);
        }
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) {
         	return proxy_ (i);
        }

        BOOST_UBLAS_INLINE
        const_reference operator [] (size_type i) const {
         	return proxy_ [i];
        }
        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) {
        	return proxy_ [i];
        }
#else
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) const {
            return proxy_ (i);
        }

        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) const {
            return proxy_ [i];
        }
#endif
	private:	
		T proxy_;
	};
}}}

int main() {

	using namespace boost::numeric::ublas;

	vector <int> v(2);
	v(0) = 0, v(1) = 1;
	vector_range < vector < int > > r(v, range(0, 2));
	vector_proxy< vector_range < vector < int > > > vr(r);
	std::cout << vr(1) << std::endl;
	return 0;
}