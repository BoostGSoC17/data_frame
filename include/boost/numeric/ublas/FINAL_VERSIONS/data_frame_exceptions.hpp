//          Copyright(C) Rishabh Arora 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Exceptions for data_frame class
// Used for any version string / variant / variadic

#ifndef _DATA_FRAME_EXCEPTIONS_HPP
#define _DATA_FRAME_EXCEPTIONS_HPP

#include <exception>

/*! \brief Contains various exceptions encountered when dealing with data_frames.
 *  Triggering an exception leads to termination of program at runtime.
 */

//! \brief checks compatibility of sizes of data_frame constructor arguments.
struct inconsistent_arguments : public std::exception {
   const char * what () const throw () {
      return "inconsistent data_frame constructor arguments";
   }
};

//! \brief number of rows in a data_frame should be same.
struct differing_rows : public std::exception {
   const char * what () const throw () {
      return "differing number of rows";
   }
};

//! \brief thrown if 2 columns have the same header.
struct same_header : public std::exception {
   const char * what () const throw () {
      return "data_frame columns not allowed to have same header";
   }
};

//! \brief thrown if data_frame structure has gap. (a columns is empty)
struct holes : public std::exception {
   const char * what () const throw () {
      return "new columns will leave holes after existing columns";
   }
};

//! \brief thrown if column header mentioned doesn't exist.
struct undefined_column_header : public std::exception {
   const char * what () const throw () {
      return "column header not defined";
   }
};

//! \brief thrown if a data_frame expression has unequal number of rows.
struct unequal_rows : public std::exception {
   const char * what () const throw () {
      return "unequal number of rows in the expression";
   }
};

//! \brief thrown if a data_frame expression has unequal number of columns.
struct unequal_columns : public std::exception {
   const char * what () const throw () {
      return "unequal number of columns in the expression";
   }
};

//! \brief thrown if a data_frame expression doesn't have the same columns.
struct column_header_mismatch : public std::exception {
   const char * what () const throw () {
      return "column header mismatch";
   }
};

#endif