#include <iostream> // for endl
#include <vector>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"


enum error curve(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  ++randseed;
  std::cerr << "hello world: pol.size: " << polygon.size() << "points: " << points[0] << ", seed: " << randseed << std::endl;
  return UNEXPECTED_ERROR;
}
