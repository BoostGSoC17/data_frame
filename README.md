**Motivation:** R-data.frame / Python Pandas-Library

data_frame is a matrix-like structure where the columns are accessible through string headers and are allowed to have different data types. 

The current version of the library supports 16 data types and used the boost.variant and boost::numeric::ublas::vector to store the column of a data_frame. It will be extended to support more data and vector types. 

Setup Proceedure On Linux
-------------------------
```
  cd /path/to/boost_1_64_0/boost/numeric/ublas 
  git clone https://github.com/BoostGSoC17/dataframe.git
  cd dataframe/test
  sudo ../../../../../b2
  
```
