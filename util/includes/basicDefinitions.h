#ifndef __BASIC_DEFINITIONS_H_
#define __BASIC_DEFINITIONS_H_

// largest number a double can take that works with ++/-- operation.
#define DBL_MAX_ITER_INT 9007199254740992

// return errors
enum error {
  SUCCESS = 0,
  UNEXPECTED_ERROR = 1,
  NO_IN_FILE = 2,
  NO_OUT_FILE = 3,
  NO_ALGORITHM = 4,
  READ_ERROR_IFILE = 5,
  READ_ERROR_OFILE = 6,
  READ_ERROR_IFORMAT = 7,
  READ_ERROR_OFORMAT = 8,
  NO_ARGUMENTS  =9
};

// the algorithms that should be selectable.
enum alg_t {
  A_UNDEFINED,
  A_IDLE,
  A_2OPT
};

// the output format of the result.
enum out_format_t {
  OF_UNDEFINED,
  OF_PERM
};

// the input format of the point set.
enum in_format_t {
  IF_UNDEFINED,
  IF_POINTS,
  IF_POLY,
  IF_COMP
};

enum point_format_t {
  PF_UNDEFINED,
  PF_SIGNED_INT,
  PF_UNSIGNED_INT,
  PF_FLOAT,
  PF_DOUBLE,
  PF_LONG_DOUBLE,
  PF_BIG
};

#endif