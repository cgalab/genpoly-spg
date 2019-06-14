#include <vector>
#include <iostream> // for endl
#include <algorithm>    // for std::sort
#include <assert.h> // for assert
#include <math.h>
#include "point.h" // for Point class


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

// function that fills the vector 'ch' with indexes of 'points' set that are the points on the convex get_convex_hull
// input: 'ch' - vector of indexes <unsigned int> into 'points' that are the points on the convex hull
//        'points' - a vector of <Point> points.
void get_convex_hull(std::vector<unsigned int>& ch, std::vector<Point>& points) {

  assert(ch.size() == 0);

  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (points.size());
  fill_lex(lex, points); // fill 'lex' with the indexes

  std::vector<unsigned int> lower;
  std::vector<unsigned int> upper;

  lower.push_back(lex[0]);
  upper.push_back(lex[0]);

  // go through all the points
  for (unsigned int i = 1; i < points.size(); ++i) {
    // go through the lower points and check if the new point has a higher angle than the next point in lower.
    double x_i = points[lex[i]].x;
    double y_i = points[lex[i]].y;
//    std::cerr << "i: " << i << ", p: " << points[lex[i]] << std::endl;

//    std::cerr << "upper loop" << std::endl;
    unsigned int j = 0;
    while (j < upper.size()) {
      // get relative x and y values for lex[i]-upper[j]
      double x_j = x_i - points[upper[j]].x;
      double y_j = y_i - points[upper[j]].y;
      double a_j;

      if (x_j > 0) a_j = y_j/x_j;
      else if (x_j == 0) a_j = (1 + signbit(y_j)*(-2))*INFINITY;
//      std::cerr << "j: " << j << ", p: " << points[upper[j]] << ", x_j: " << x_j << ", y_j: " << y_j << ", a_j: " << a_j << std::endl;

      // get relative x and y values for lex[i]-upper[j+1]
      if (j+1 < upper.size()) {
        double x_next = points[upper[j+1]].x - points[upper[j]].x;
        double y_next = points[upper[j+1]].y - points[upper[j]].y;
        double a_next;

        if (x_next > 0) a_next = y_next/x_next;
        else if (x_next == 0) a_next = (1 + signbit(y_next)*(-2))*INFINITY ;
//        std::cerr << "upper: next point: " << points[upper[j+1]] << ", x_next: " << x_next << ", y_next: " << y_next << ", a_next: " << a_next << std::endl;

        // if the ratio of i-j is larger than i-(j+1), then lex[i] should be a new upper[j+1]
        if (a_next < a_j) {
//          std::cerr << "upper[j+1]: " << points[upper[j+1]] << ", replaced with: " << points[lex[i]] << std::endl;
          upper[j+1] = lex[i];
          upper.resize(j+2); //removes all values after upper[j+1]
//          std::cerr << "upper.size: " << upper.size() << std::endl;
          break;
        }
      }
      else {
        // upper[j+1] doesn't exist, i is automatically the next j+1
//        std::cerr << "i pushed to upper." << std::endl;
        upper.push_back(lex[i]);
        break;
      }
      ++j;
    }

//    std::cerr << "lower loop" << std::endl;
    j = 0;
    while (j < lower.size()) {
      // get relative x and y values for lex[i]-lower[j]
      double x_j = x_i - points[lower[j]].x;
      double y_j = y_i - points[lower[j]].y;
      double a_j;

      if (x_j > 0) a_j = y_j/x_j;
      else if (x_j == 0) a_j = (1 + signbit(y_j)*(-2))*INFINITY;
//      std::cerr << "j: " << j << ", p: " << points[upper[j]] << ", x_j: " << x_j << ", y_j: " << y_j << ", a_j: " << a_j << std::endl;

      // get relative x and y values for lex[i]-lower[j+1]
      if (j+1 < lower.size()) {
        double x_next = points[lower[j+1]].x - points[lower[j]].x;
        double y_next = points[lower[j+1]].y - points[lower[j]].y;
        double a_next;

        if (x_next > 0) a_next = y_next/x_next;
        else if (x_next == 0) a_next = (1 + signbit(y_next)*(-2))*INFINITY;
//        std::cerr << "lower: next point: " << points[upper[j+1]] << ", x_next: " << x_next << ", y_next: " << y_next << ", a_next: " << a_next << std::endl;

        // if the ratio of lex[i]-lower[j] is smaller than lex[i]-lower[j+1], then lex[i] should be a new lower[j+1]
        if (a_next > a_j) {
//          std::cerr << "lower[j+1]: " << points[lower[j+1]] << ", replaced with: " << points[lex[i]] << std::endl;
          lower[j+1] = lex[i];
          lower.resize(j+2); //removes all values after lower[j+1]
//          std::cerr << "lower.size: " << lower.size() << std::endl;
          break;
        }
      }
      else {
        // lower[j+1] doesn't exist, i is automatically the next j+1
//        std::cerr << "i pushed to lower." << std::endl;
        lower.push_back(lex[i]);
        break;
      }
      ++j;
    }
  }
  // add the last lex. point if it's missing.
  if (points[lex[lex.size()-1]] != points[upper[upper.size()-1]])
    upper.push_back(lex[lex.size()-1]);
  if (points[lex[lex.size()-1]] != points[lower[lower.size()-1]])
    lower.push_back(lex[lex.size()-1]);

//  std::cerr << "upper: " << std::endl;
//  pdisplay(upper, points);
//  std::cerr << "lower: " << std::endl;
//  pdisplay(lower, points);
  // 'upper' and 'lower' both have the same start point, to return a CCW c.h.,
  // first add 'lower' to 'ch' then 'upper' minus the start
  ch.insert(ch.end(), lower.begin(), lower.end());
  ch.insert(ch.end(), std::next(upper.rbegin()), std::prev(upper.rend()));
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
/*
// function to get the orientation of b relative to a in a cyclic chain, such as a polygon
void is_left(unsigned int a, unsigned int b, unsigned int cycle) {

}
*/
// function to get the distance from one index to another in a cyclic index, such as a polygon
unsigned int get_cyclic_difference(unsigned int a, unsigned int b, unsigned int cycle) {
  int diff1, diff2;

  if (a < b) {
    diff1 = b-a;
    diff2 = a+cycle-b;
  }
  else {
    diff1 = a-b;
    diff2 = b+cycle-a;
  }
  if (diff1 < diff2) return diff1;
  else return diff2;
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
