#include <vector>
#include <iostream> // for endl and cerr
#include <algorithm>    // for std::sort
#include <assert.h> // for assert
#include <math.h>
#include <cmath> // for fabs()
#include "point.h" // for Point class
#include "pol.h" // for doFlip
#include "edge.h"

enum error verify_point_set(std::vector<Point>& points) {
  enum error retval = SUCCESS;

  std::vector<unsigned int> lex (points.size());
	fill_lex(lex, points);

  //check if any point is coincidental
  Point prev, cur;
  for (unsigned int i=1; i < lex.size();++i) { //-1 so we don't check collinearity of the last point
    prev = points[lex[i-1]];
    cur  = points[lex[i]];
    if (prev == cur) {
      retval = COINCIDENTAL_POINTS;
      std::cerr << "Error: Found 2 points with same coordinates!" << std::endl;
      break;
    }
  }

  if (retval == SUCCESS) {
    retval = COLLINEAR_POINTS;
    // check if all points are collinear
    Edge e = Edge(&points[lex[0]], &points[lex[lex.size()-1]]);
    for (unsigned int i=1; i < lex.size()-1;++i) { //-1 so we don't check collinearity of the last point
      cur = points[lex[i]];
      if(fabs(det(e,cur)) > 0) {
        retval = SUCCESS;
        break;
      }
    }
    if (retval == COLLINEAR_POINTS) std::cerr << "Error: all points are collinear!" << std::endl;
  }
  return retval;
}
/*
// calculates the determinant of the vertices of 'e' and the point p (similar function in edge.cpp for 1 edge and a point)
double det(const Point& pa, const Point& pb, const Point& p) {
	double ans = (p.x * (pa.y - pb.y) - p.y * (pa.x-pb.x) + (pa.x*pb.y - pb.x*pa.y));
	return ans; //(fabs(ans) < EPSILON) ? 0 : ans;
}
*/
// simpler calculates of the determinant where pb is moved to (0.0) and thus it is removed from the other 2 points.
double det(Point& p1, Point& p2, Point& p) {
	Point pa = Point(p1);
	Point pb = Point(p2);
	Point pc = Point(p);
	pa.x = pa.x - pb.x;pa.y = pa.y - pb.y;
	pc.x = pc.x - pb.x;pc.y = pc.y - pb.y;
	double ans = pc.x * pa.y - pc.y * pa.x;
	//std::cerr << "det: " << ans << std::endl;
	//return (fabs(ans) < EPSILON) ? 0 : ans;
	return ans;
}

// function that fills the vector 'ch' with indexes of 'points' set that are the points on the convex get_convex_hull
// input: 'ch' - vector of indexes <unsigned int> into 'points' that are the points on the convex hull
//        'points' - a vector of <Point> points.
void get_convex_hull(std::vector<unsigned int>& ch, std::vector<Point>& points, bool enforceCCWOrder) {
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

      if (fabs(x_j) < EPSILON) a_j = (1 + signbit(y_j)*(-2))*INFINITY;
      else a_j = y_j/x_j;
//      std::cerr << "j: " << j << ", p: " << points[upper[j]] << ", x_j: " << x_j << ", y_j: " << y_j << ", a_j: " << a_j << std::endl;

      // get relative x and y values for lex[i]-upper[j+1]
      if (j+1 < upper.size()) {
        double x_next = points[upper[j+1]].x - points[upper[j]].x;
        double y_next = points[upper[j+1]].y - points[upper[j]].y;
        double a_next;

        if (fabs(x_next) < EPSILON) a_next = (1 + signbit(y_next)*(-2))*INFINITY ;
        else a_next = y_next/x_next;
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

      if (fabs(x_j) < EPSILON) a_j = (1 + signbit(y_j)*(-2))*INFINITY;
      else a_j = y_j/x_j;

//      std::cerr << "j: " << j << ", p: " << points[upper[j]] << ", x_j: " << x_j << ", y_j: " << y_j << ", a_j: " << a_j << std::endl;

      // get relative x and y values for lex[i]-lower[j+1]
      if (j+1 < lower.size()) {
        double x_next = points[lower[j+1]].x - points[lower[j]].x;
        double y_next = points[lower[j+1]].y - points[lower[j]].y;
        double a_next;

        if (fabs(x_next) < EPSILON) a_next = (1 + signbit(y_next)*(-2))*INFINITY;
        else a_next = y_next/x_next;
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

  // make sure of the ordering
	if (enforceCCWOrder) {
    double area = pol_calc_area(ch, points);
    //std::cerr << "area: " << area << std::endl;
    if (area < 0) doFlip(0, ch.size()-1, ch, points);
    //area = pol_calc_area(ch, points);
    //std::cerr << "area: " << area << std::endl;
  }
}

// function to fill a vector of unsigned integers 'ip' with all points not on the convex hull of the point set 'points'
void get_inner_points(std::vector<unsigned int>& ip, std::vector<unsigned int>& ch, std::vector<Point>& points) {
  assert(ip.size() == 0); assert(ch.size() > 0); assert(points.size() > 0);

  std::vector<unsigned int> lex_points (points.size());
  fill_lex(lex_points, points); // fill 'lex' with the 'points' indexes in lex. order
  std::vector<unsigned int> lex_ch (ch.size());
  fill_lex(lex_ch, ch, points); // fill 'lex' with the ch indexes in lex. order

  unsigned int j = 0;
  for (unsigned int i = 0; i < lex_points.size();++i) {
    if (points[lex_points[i]] == points[lex_ch[j]]) {
      ++j;
      continue;
    }
    else {
        ip.push_back(lex_points[i]);
    }
  }
}

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

// comparison function for the sort algorithm in 'star'
bool angleComparator (Point i,Point j) {
  double ai = i.y/i.x;
  double li = sqrt(i.x*i.x + i.y*i.y);
  double aj = j.y/j.x;
  double lj = sqrt(j.x*j.x + j.y*j.y);

  if (fabs(ai - aj) < EPSILON) {
    if (fabs(li - lj) < EPSILON) return false;
    else return (li < lj);
  }
  else return (ai < aj);
  std::cerr << "ERROR!  Fallthrough in angle comparison function!" << std::endl;
  return false;
}
