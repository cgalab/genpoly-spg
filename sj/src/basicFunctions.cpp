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