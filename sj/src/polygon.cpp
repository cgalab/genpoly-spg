#include <vector>
#include "point.h"

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
