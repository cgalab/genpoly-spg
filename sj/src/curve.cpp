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
/*
// function that accepts a simple polygon and returns a array of a polygon with its holes.
// Input: 'polygon'     : a vector with vertices of 'points' set that is a simple polygon
//        'points'      : a vector of <Point> objects
// Output: 'pol_array'  : an array of polygons, the first index is the outermost simple polygon, the rest are simple holes inside that polygon
enum error holes(std::vector<unsigned int>& polygon, std::vector<Point>& points,std::vector<std::vector<unsigned int>>& pol_array) {

  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (points.size());
  fill_lex(lex, points); // fill 'lex' with the indexes

  std::cerr << "first point: " << points[lex[0]] << ", pol: " << polygon.size() << std::endl;

  // go through all the points in lex. order
  for (unsigned int i = 0; i < lex.size(), ++i) {
    // for point i:  insert it into a set.

  }

  return UNEXPECTED_ERROR;
}
*/
