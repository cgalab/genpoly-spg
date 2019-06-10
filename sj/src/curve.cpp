#include <iostream> // for endl
#include <vector>
#include <assert.h>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"
#include "curve.h"


enum error curve(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  ++randseed;

  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (points.size());
  fill_lex(lex, points); // fill 'lex' with the indexes

  std::cerr << "first point: " << points[lex[0]] << ", pol: " << polygon.size() << std::endl;


  return UNEXPECTED_ERROR;
}

// function that accepts a simple polygon and returns a array of a polygon with its holes.
// Input: 'polygon'     : a vector with vertices of 'points' set that is a simple polygon
//        'points'      : a vector of <Point> objects
// Output: 'pol_array'  : an array of polygons, the first index is the outermost simple polygon, the rest are simple holes inside that polygon
enum error holes(std::vector<std::vector<unsigned int>>& sph, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  std::vector<s_curve> sc;
  Point *m, *l, *r;
  bool isll, isrl;
  unsigned int count_open=0, count_cont=0, count_close=0;

  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (points.size());
  fill_lex(lex, points); // fill 'lex' with the indexes

  std::cerr << "sph: " << sph.size() << std::endl;

  // go through all the points in lex. order
  for (unsigned int i = 0; i < lex.size(); ++i) {
    m = &points[lex[i]];
    l = &points[polygon[(points.size() + (*m).v - 1) % points.size()]];
    r = &points[polygon[(points.size() + (*m).v + 1) % points.size()]];
    //std::cerr << "m: " << *m << ", l: " << *l << ", r: " << *r << std::endl;

    // check for '>o', '-o-', 'o<' condition
    (*l < *m) ? isll = true : isll = false;
    (*r < *m) ? isrl = true : isrl = false;

    if (isll && isrl) { // '>o'
      ++count_open;
    }
    else if (isll ^ isrl) { // 'o<'
      ++count_cont;
    }
    else { // '-o-'
      ++count_close;
    }
  }

  std::cerr << "opened: " << count_open << ", continued: " << count_cont << ", closed: " << count_close << std::endl;

  return SUCCESS;
}
