#include <vector>
#include <iostream> // for std::endl
#include "point.h"
#include "edge.h"
#include "basicFunctions.h"

double pol_calc_area(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  double Area = 0;
  Point prev, p;
  unsigned int index;

  prev = points[polygon[polygon.size()-1]];
  // for each vertex in the polygon, index is the latter point
  for (index = 0; index < polygon.size(); ++index)
  {
    p = points[polygon[index]];

    Area += p.x * prev.y - p.y * prev.x;
    prev = p;
  }

  Area = Area / 2;
  return Area;
}

bool checkAllIntersections (std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  std::cerr << "checking intersections..." << std::endl;
  unsigned int count = 0;
  bool ret = false;
  Point *previ, *pi, *prevj, *pj;
  Edge ei, ej;
  enum intersect_t retval;

  previ = &points[polygon[polygon.size()-1]];
  for (unsigned int i = 0; i < polygon.size();++i) {
    pi = &points[polygon[i]];
    ei = Edge(previ, pi);
    prevj = &points[polygon[polygon.size()-1]];
    for (unsigned int j = 0; j < polygon.size();++j) {
      pj = &points[polygon[j]];
      ej = Edge(prevj, pj);
      retval = checkIntersection(ei, ej);

      if ((retval == IS_TRUE) || (retval == IS_4P_COLLINEAR) || (retval == IS_3P_COLLINEAR)) {
        std::cerr << "retval: ";
        printEnum(retval);
        std::cerr << ", ei: " << ei << ", ej: " << ej << std::endl;

        ++count;
        ret = true;
      }
      prevj = pj;
    }
    previ = pi;
  }
  std::cerr << "count: " << count << std::endl;
  return ret;
}
