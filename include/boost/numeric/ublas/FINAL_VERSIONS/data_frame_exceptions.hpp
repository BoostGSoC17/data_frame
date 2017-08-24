//          Copyright(C) Rishabh Arora 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Exceptions for data_frame class
// Used for any version string / variant / variadic

#ifndef _DATA_FRAME_EXCEPTIONS_HPP
#define _DATA_FRAME_EXCEPTIONS_HPP

#include <exception>

struct inconsistent_arguments : public std::exception {
   const char * what () const throw () {
      return "inconsistent data_frame constructor arguments";
   }
};

struct differing_rows : public std::exception {
   const char * what () const throw () {
      return "differing number of rows";
   }
};

struct same_header : public std::exception {
   const char * what () const throw () {
      return "data_frame columns not allowed to have same header";
   }
};

struct holes : public std::exception {
   const char * what () const throw () {
      return "new columns will leave holes after existing columns";
   }
};

struct undefined_column_header : public std::exception {
   const char * what () const throw () {
      return "column header not defined";
   }
};

struct unequal_rows : public std::exception {
   const char * what () const throw () {
      return "unequal number of rows in the expression";
   }
};

struct unequal_columns : public std::exception {
   const char * what () const throw () {
      return "unequal number of columns in the expression";
   }
};

struct column_header_mismatch : public std::exception {
   const char * what () const throw () {
      return "column header mismatch";
   }
};

#endif