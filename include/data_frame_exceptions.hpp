
// Exceptions for data_frame class
// Used for any version string / variant / variadic

#ifndef _DATA_FRAME_EXCEPTIONS_HPP
#define _DATA_FRAME_EXCEPTIONS_HPP

#include <exception>

struct inconsistent_arguments : public std::exception {
   const char * what () const throw () {
      return "inconsistent constructor arguments";
   }
};
struct same_header : public std::exception {
   const char * what () const throw () {
      return "data_frame columns not allowed to have same header";
   }
};
struct undefined_column_header : public std::exception {
   const char * what () const throw () {
      return "";
   }
};
struct holes : public std::exception {
   const char * what () const throw () {
      return "new columns will leave holes after existing columns";
   }
};
struct type_not_supported : public std::exception {
   const char * what () const throw () {
      return "type not supported";
   }
};
struct undefined_index : public std::exception {
   const char * what () const throw () {
      return "undefined column index";
   }
};
struct differing_rows : public std::exception {
   const char * what () const throw () {
      return "differing number of rows";
   }
};
struct bad_header_type : public std::exception {
   const char * what () const throw () {
      return "invalid column header type";
   }
};
struct column_type_mismatch : public std::exception {
   const char * what () const throw () {
      return "";
   }
};
struct undefined_column_name : public std::exception {
   const char * what () const throw () {
      return "";
   }
};
#endif