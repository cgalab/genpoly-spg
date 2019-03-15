#include "points.h"

// simple function to display points in a Point vector.
void pdisplay (const std::vector<Point>& p) {
  for (unsigned int i = 0; i < p.size(); i++) {
    std::cout << p[i] << std::endl;
  }
}
