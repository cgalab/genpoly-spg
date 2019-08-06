#include <vector>
#include <iostream> // for endl and cerr
#include <algorithm>    // for std::sort
#include <assert.h> // for assert
#include <math.h>
#include "point.h" // for Point class


// check for if p1 is a 'left' vertex compared to p2
bool isPolLeft(Point *p1, Point *p2, unsigned int cycle) {
  unsigned int lo, hi, left, right;
  bool p1_left;
  if ((*p1).v < (*p2).v) {
    lo = (*p1).v;
    hi = (*p2).v;
    p1_left = true;
  } else {
    lo = (*p2).v;
    hi = (*p1).v;
    p1_left = false;
  }
  left = lo;
  right = lo + cycle;

  if (hi - left < right - hi) {
    if (p1_left) return false;
    else return true;
  }
  else {
    if (p1_left) return true;
    else return false;
  }
}

// check for if p1 is the incidental 'left' vertex compared to p2
bool isPol1Left(Point *p1, Point *p2, unsigned int cycle) {
  if ((*p1).v == 0) {
    if ((*p2).v == 1) return true;
  }
  if ((*p2).v == 0) {
    if ((*p1).v == cycle-1) return true;
  }
  if (((*p1).v < (*p2).v) && (abs((int)((*p1).v - (*p2).v)) == 1)) return true;
  return false;
}

// Input:
// lex:     vector for unsigned integers referencing indexes in point set 'points'
// points:  vector of 'Point'
// Method:
//  Function fills 'lex' with the lexicographgically sorted indexes of 'points'
//  and adds the lexicographical index of each point into '.l' property
// Assumptions: given lex vector is the same size as points vector.
void fill_lex(std::vector<unsigned int>& lex, std::vector<Point>& points) {

  assert(lex.size() == points.size());

  for(unsigned int i = 0; i < points.size(); ++i)
		lex[i] = i;
	// sort 'lex' based on lexicographical order of 'points'
	std::sort(lex.begin(), lex.end(), lexComp(points));
  // save the lex index into each point in 'points'
  unsigned int counter = 0;
  for (std::vector<unsigned int>::iterator it = lex.begin(); it != lex.end(); ++it) {
    points[(*it)].l = counter;
    ++counter;
  }
}

void fill_lex(std::vector<unsigned int>& lex, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  assert(lex.size() == polygon.size());
  for(unsigned int i = 0; i < polygon.size(); ++i)
    lex[i] = polygon[i];

  // sort 'lex' based on lexicographical order of 'points'
	std::sort(lex.begin(), lex.end(), lexComp(points));
}

// simple function to display points in a Point vector.
void pdisplay (const std::vector<Point>& p) {
  for (unsigned int i = 0; i < p.size(); i++) {
    std::cerr << p[i] << std::endl;
  }
}

// simple function to display points in a Point vector.
void pdisplay (const std::vector<unsigned int>& ind, const std::vector<Point>& p) {
  for (unsigned int i = 0; i < ind.size(); i++) {
    std::cerr << p[ind[i]] << std::endl;
  }
}

double getXmin(const std::vector<Point>& p) {
  double xmin = INFINITY;
  for (unsigned int i = 0; i < p.size(); i++) {
    if (p[i].x < xmin) xmin = p[i].x;
  }
  return xmin;
}

double getXmax(const std::vector<Point>& p) {
  double xmax = 0;
  for (unsigned int i = 0; i < p.size(); i++) {
    if (p[i].x > xmax) xmax = p[i].x;
  }
  return xmax;
}

double getYmin(const std::vector<Point>& p) {
  double ymin = INFINITY;
  for (unsigned int i = 0; i < p.size(); i++) {
    if (p[i].y < ymin) ymin = p[i].y;
  }
  return ymin;
}

double getYmax(const std::vector<Point>& p) {
  double ymax = 0;
  for (unsigned int i = 0; i < p.size(); i++) {
    if (p[i].y > ymax) ymax = p[i].y;
  }
  return ymax;
}
