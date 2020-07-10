#ifndef __BASIC_DEFINITIONS_H_
#define __BASIC_DEFINITIONS_H_

// largest number a double can take that works with ++/-- operation.
#define DBL_MAX_ITER_INT 9007199254740992
#define EPSILON 1e-69
#define MAX_NO_OF_LOOPS 4 // this is the largest number of times a 2opt B.O. loop can return the same circumference.  Not total loops.
#define PI 3.14159265358979338462643383279502884L

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
  RUN_TESTS = 12,
  ERR_AREA_NEGATIVE = 13,
  ERR_AREA_NOT_BETTER = 14,
  TOO_FEW_INNER_POINTS_FOR_HOLE = 15,
  INNER_POINTS_COLLINEAR = 16,
  NO_VERIFY_FILE = 17,
  RUN_SIMPLE_CHECK = 18,
  RUN_LONG_CHECK = 19,
  CONVERT_FORMAT = 20,
  COINCIDENTAL_POINTS = 21,
  COLLINEAR_POINTS = 22,
  INTERSECTING_POINTS = 23,
  INFINITE_LOOP = 24,
  SIZE_NOT_EQUAL = 25,
  HOLES_INADEQUATE = 26,
  HELP = 27,
  WRITE_ERROR = 28
};

// the algorithms that should be selectable.
enum alg_t {
  A_UNDEFINED,
  A_IDLE,
  A_2OPT,
  A_2OPT_A,
  A_2OPT_B,
  A_2OPT_C,
  A_2OPT_D,
  A_2OPT_E,
  A_2OPT_F,
  A_2OPT_G,
  A_2OPT_H,
  A_CURVE,
  A_STAR,
  A_ALLSP,
  A_CONVERT_FORMAT,
  A_VERIFY,
  A_VERIFY_LONG,
  A_POLSLOPE,
  A_PNTSLOPE,
  A_EDGELIST
};

// the output format of the result.
enum out_format_t {
  OF_UNDEFINED,
  OF_PERM,
  OF_POLY,
  OF_DAT,
  OF_PURE,
  OF_PURE_AND_PERM,
  OF_LINE
};

// the input format of the point set.
enum in_format_t {
  IF_UNDEFINED,
  IF_POINTS,
  IF_POLY,
  IF_COMP,
  IF_LINE,
  IF_DAT
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
  IS_TRUE = 50,  // value 50 is required for a easy recognized non-vertex intersections in a sum
  IS_4P_COLLINEAR = 51,
  IS_3P_COLLINEAR = 52,
  IS_SAME_EDGE = 53
};

enum edge_t {
  E_VALID,
  E_INTERSECTION,
  E_COLLINEAR,
  E_NOT_VALID
};

enum planesweep_t {
  P_CLEAN,
  P_DIRTY_LEFT,
  P_DIRTY_RIGHT
};

#endif
