#include <iostream> // for endl
#include <vector>
#include <assert.h>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"


enum error curve(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  ++randseed;

  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (points.size());
  fill_lex(lex, points); // fill 'lex' with the indexes

  std::cerr << "first point: " << points[lex[0]] << ", pol: " << polygon.size() << std::endl;



  return UNEXPECTED_ERROR;
}
