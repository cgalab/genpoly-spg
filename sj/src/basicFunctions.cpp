#include <vector>
#include "opt2.h"

enum error getSP(std::vector<unsigned int>& polygon, std::vector<Point>& points, enum alg_t alg) {
  enum error returnValue;
  switch(alg) {
    case A_UNDEFINED:
      returnValue = ALGORITHM_UNDEFINED;
      break;
    case A_IDLE:
      returnValue = ALGORITHM_IDLE;
      break;
    case A_2OPT:
      returnValue = opt2(polygon, points);
      break;
    default:
      returnValue = UNEXPECTED_ERROR;
      break;
  }
  return returnValue;
}

void printEnum(enum intersect_t val) {
  switch (val) {
    case IS_FALSE:
      std::cerr << "IS_FALSE";
      break;
    case IS_VERTEX11:
      std::cerr << "IS_VERTEX11";
      break;
    case IS_VERTEX12:
      std::cerr << "IS_VERTEX12";
      break;
    case IS_VERTEX21:
      std::cerr << "IS_VERTEX21";
      break;
    case IS_VERTEX22:
      std::cerr << "IS_VERTEX22";
      break;
    case IS_TRUE:
      std::cerr << "IS_TRUE";
      break;
    case IS_4P_COLLINEAR:
      std::cerr << "IS_4P_COLLINEAR";
      break;
    case IS_3P_COLLINEAR:
      std::cerr << "IS_3P_COLLINEAR";
      break;
    case IS_SAME_EDGE:
      std::cerr << "IS_SAME_EDGE";
    default:
      break;
  }
}
