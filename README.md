**Motivation:** R-data.frame / Python Pandas-Library

data_frame is a matrix-like structure where the columns are accessible through string headers and are allowed to have different data types. 

Library is *header only*. The current version (v1.0) of the library supports 16 data types and uses the boost.variant and boost::numeric::ublas::vector to store the column of a data_frame. It will be extended to support more data and vector types. 

**Features Supported:**
- column subsetting.
- generating statistical summaries. 
- binary operations on columns and data_frames. (\+/\-/\*).
- Access and modification operations. 

Setup Proceedure On Linux
-------------------------
```
  cd /path/to/boost_directory/boost/numeric/ublas            # switch to your boost home directory.
  git clone https://github.com/BoostGSoC17/dataframe.git     
  cd dataframe/test                                             
  sudo ../../../../../b2                                     # To run tests.       
  
```
