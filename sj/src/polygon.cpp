#include <vector>
#include "point.h"

double pol_calc_area(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  double Area = 0;
  Point p1, p2;
  unsigned int index;

  // for each vertex in the polygon, index is the latter point
  for (index = 1; index < polygon.size(); ++index)
  {
    p1 = points[polygon[index-1]];
    p2 = points[polygon[index]];

    Area += p1.x * p2.y - p1.y * p2.x;
  }
  // edge between last point and first point
  p1 = points[polygon[0]];
  p2 = points[polygon[polygon.size()-1]];

  Area += p1.x * p2.y - p1.y * p2.x;
  Area = Area /2;

  return Area;
}
