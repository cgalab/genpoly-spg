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
//        'lex' - all points in 'points' set in lexicographical order
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

    // upper loop
    unsigned int j = 0;
    while (j < upper.size()) {
      // get relative x and y values for lex[i]-upper[j]
      double x_j = x_i - points[upper[j]].x;
      double y_j = y_i - points[upper[j]].y;
      double a_j;

      if (x_j > 0) a_j = y_j/x_j;
      else if (x_j == 0) a_j = 1 + signbit(y_j)*(-2);

      // get relative x and y values for lex[i]-upper[j+1]
      if (j+1 < upper.size()) {
        double x_next = x_i - points[upper[j+1]].x;
        double y_next = y_i - points[upper[j+1]].y;
        double a_next;

        if (x_next > 0) a_next = y_next/x_next;
        else if (x_next == 0) a_next = 1 + signbit(y_j)*(-2) ;

        // if the ratio of i-j is larger than i-(j+1), then lex[i] should be a new upper[j+1]
        if (a_next < a_j) {
          upper[j+1] = lex[i];
          upper.resize(j+1); //removes all values after upper[j+1]
          break;
        }
      }
      else {
        // upper[j+1] doesn't exist, i is automatically the next j+1
        upper.push_back(lex[i]);
      }
      ++j;
    }

    // lower loop
    j = 0;
    while (j < lower.size()) {
      // get relative x and y values for lex[i]-lower[j]
      double x_j = x_i - points[lower[j]].x;
      double y_j = y_i - points[lower[j]].y;
      double a_j;

      if (x_j > 0) a_j = y_j/x_j;
      else if (x_j == 0) a_j = 1 + signbit(y_j)*(-2);

      // get relative x and y values for lex[i]-lower[j+1]
      if (j+1 < lower.size()) {
        double x_next = x_i - points[lower[j+1]].x;
        double y_next = y_i - points[lower[j+1]].y;
        double a_next;

        if (x_next > 0) a_next = y_next/x_next;
        else if (x_next == 0) a_next = 1 + signbit(y_j)*(-2);

        // if the ratio of lex[i]-lower[j] is smaller than lex[i]-lower[j+1], then lex[i] should be a new lower[j+1]
        if (a_next > a_j) {
          lower[j+1] = lex[i];
          lower.resize(j+1); //removes all values after lower[j+1]
          break;
        }
      }
      else {
        // lower[j+1] doesn't exist, i is automatically the next j+1
        lower.push_back(lex[i]);
      }
      ++j;
    }
  }

  // 'upper' and 'lower' both have the same start and end points, to return a CCW c.h.,
  // first add 'lower' to 'ch' then 'upper' minus the ends
  ch.insert(ch.end(), lower.begin(), lower.end());
  ch.insert(ch.end(), std::next(upper.begin()), std::prev(lower.end()));
}

// simple function to display points in a Point vector.
void pdisplay (const std::vector<Point>& p) {
  for (unsigned int i = 0; i < p.size(); i++) {
    std::cout << p[i] << std::endl;
  }
}

// simple function to display points in a Point vector.
void pdisplay (const std::vector<unsigned int> ind, const std::vector<Point>& p) {
  for (unsigned int i = 0; i < p.size(); i++) {
    std::cout << p[ind[i]] << std::endl;
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
