#ifndef __BASIC_DEFINITIONS_H_
#define __BASIC_DEFINITIONS_H_

// largest number a double can take that works with ++/-- operation.
#define DBL_MAX_ITER_INT 9007199254740992
#define EPSILON 1e-15

#define Swap(X,Y)  do{ __typeof__ (X) _T = X; X = Y; Y = _T; }while(0)

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
  NO_ARGUMENTS  = 9,
  ALGORITHM_UNDEFINED = 10,
  ALGORITHM_IDLE = 11,
  RUN_TESTS = 12
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
  OF_PERM,
  OF_POLY,
  OF_DAT
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

enum intersect_t {
  IS_FALSE = 0,
  IS_VERTEX11 = 1,
  IS_VERTEX12 = 2,
  IS_VERTEX21 = 3,
  IS_VERTEX22 = 4,
  IS_TRUE = 50,  // value 10 is required for a easy recognized non-vertex intersections in a sum
  IS_COLLINEAR = 51,
  IS_SAME_EDGE = 52
};

enum edge_t {
  E_VALID,
  E_SKIP,
  E_NOT_VALID
};

#endif
