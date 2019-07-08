#include <vector>
#include <iostream> // for std::endl
#include <assert.h>
#include "point.h"
#include "edge.h"
#include "basicFunctions.h"
#include "rand.h"

double pol_calc_area(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  double Area = 0;
  Point prev, p;
  unsigned int index;

  prev = points[polygon[polygon.size()-1]];
  // for each vertex in the polygon, index is the latter point
  for (index = 0; index < polygon.size(); ++index)
  {
    p = points[polygon[index]];

    Area += p.y * prev.x - p.x * prev.y;
    prev = p;
  }

  Area = Area / 2;
  return Area;
}

// function that checks if the points in 'points' given by indexes in 'polygon'
// (can be a subset of 'points' of minimum size 3) is 2 dimensional.
bool is_2D(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  assert(polygon.size() > 2);
  Edge e = Edge (&points[polygon[0]], &points[polygon[1]]);
  Point p;

  for (unsigned int i = 2; i < polygon.size(); ++i) {
    p = points[polygon[i]];
    if (det(e,p) > 0) return true;
  }
  return false;
}

// function to safely get the proper direction of ascension in polygon
// returns true if going from 'p2' to 'p1' is ascending the polygon.
bool is_ascending(I_Edge e) {
  if (e.l2ch) {
    if ((*e.p1).v == 0) {
      if ((*e.p2).v == 1) return true;
      else return false;
    }
    if ((*e.p2).v == 0) {
      if ((*e.p1).v == 1) return false;
      else return true;
    }
    if ((*e.p1).v < (*e.p2).v) return true;
    return false;
  }
  else {
    if ((*e.p1).v == 0) {
      if ((*e.p2).v == 1) return false;
      else return true;
    }
    if ((*e.p2).v == 0) {
      if ((*e.p1).v == 1) return true;
      else return false;
    }
    if ((*e.p1).v < (*e.p2).v) return false;
    return true;
  }
  // should never reach the end
  assert(false);
}

// function to get the lower index distance from one index to another in a cyclic index, such as a polygon
unsigned int get_lower_cyclic_difference(unsigned int a, unsigned int b, unsigned int cycle) {
  unsigned int diff1, diff2;

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

// function to get a length of a chain in a cyclic index, such as a polygon
// given a certain 'lower' index and a 'higher' index and a 'cycle' which is the polygon size.
// INPUTS:
//    'par'       : pair of 'I_Edge's which define an inner polygonal chain
//    'ascending' : boolean that defines to ascend through the polygonal index
//                  from 'par.first' to 'par.second' or not
// OUTPUT: length of the inner chain
unsigned int get_chain_length(std::pair<I_Edge,I_Edge> par, unsigned int cycle) {
  unsigned int a, b, l;

  // 'a' is 'par.first' ch point 'v' value, 'b' is 'par.second' ch point 'v' value
  a = (par.first.l2ch ? (*par.first.p1).v : (*par.first.p2).v);
  b = (par.second.l2ch ? (*par.second.p1).v : (*par.second.p2).v);

  if (is_ascending(par.first)) { // ascending from a to b
    // there's a cycle reset inside the chain
    // -1 because we assume a and b are on the convex hull, so one points needs to be deducted.
    if (b < a) l = b + cycle -a -1;
    else l = b -a -1;
  }
  else { // descending from a to b
    if (a < b) l = a + cycle -b -1; // there's a cycle reset inside the chain
    else l = a -b -1;
  }
  return l;
}

// function that checks if the chain defined by the 2 edges in 'par' of
// the polygon in 'polygon' of the points in 'points' is 2 dimensional.
bool is_2D(std::pair<I_Edge,I_Edge> par, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  assert(polygon.size() > 2);

  unsigned int a, b, l;
  bool ascending;
  int it;
  Edge e;
  Point p;

  // 'a' is 'par.first' ch point 'v' value, 'b' is 'par.second' ch point 'v' value
  a = (par.first.l2ch ? (*par.first.p1).v : (*par.first.p2).v);
  b = (par.second.l2ch ? (*par.second.p1).v : (*par.second.p2).v);

  // get direction from 'a' to 'b' through the inner chain, whether it is ascending through the index or descending
  ascending = is_ascending(par.first);
  std::cerr << "a: " << a << ", b: " << b << ", ascending: " << (ascending ? "true" : "false") << std::endl;
  // get length of the inner chain
  l = get_chain_length(par, polygon.size());
  std::cerr << "length: " << l << std::endl;
  if (l < 3) return false;

  if (ascending) it = 1;
  else it = -1;

  e = Edge (&points[polygon[(a + it) % polygon.size()]], &points[polygon[(a + 2*it) % polygon.size()]]);
  unsigned int i = (a + 3*it)%polygon.size();

  while (i != b) {
    p = points[polygon[i]];
    if (det(e,p) > 0) return true;
    i = (ascending ? (i+1)%polygon.size() : (i-1)%polygon.size());
  }
  return false;
}

// function that fills the vector 'ch' with indexes of 'points' set that are the points on the convex get_convex_hull
// input: 'ch' - vector of indexes <unsigned int> into 'points' that are the points on the convex hull
//        'points' - a vector of <Point> points.
void get_convex_hull(std::vector<unsigned int>& ch, std::vector<Point>& points, bool enforceCCWOrder=false) {
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

// creates a random polygon from a given pointset and random seed.
void createRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
	if (randseed) mt.seed(randseed);

	unsigned int randpos, temp;
	polygon.resize(points.size());

	for(unsigned int i = 0; i < polygon.size();++i) {
		polygon[i] = points[i].i;
	}

  unsigned int i = polygon.size();
	do {
    --i;
		UniformRandomI(randpos, 0, i);
		temp = polygon[i];
		polygon[i] = polygon[randpos];
		polygon[randpos] = temp;
		points[polygon[i]].v = i;
	} while (i!= 0);

  //std::cerr << "polygon: " << std::endl;
  //pdisplay(polygon, points);
}

// function to create a random polygon where the convex hull points are in relative CCW order.
void createCHRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
	if (randseed) mt.seed(randseed);
	unsigned int randpos;

	// start with getting all c.h. points.
	std::vector<unsigned int> ch;
	get_convex_hull(ch, points, true);
  //std::cerr << "ch: " << std::endl;
  //pdisplay(ch, points);
  // get all inner points.
  std::vector<unsigned int> ip;
	get_inner_points(ip, ch, points);

	polygon.resize(points.size());

  unsigned int i = polygon.size();
	do {
    --i;
		UniformRandomI(randpos, 0, ch.size()+ip.size()-1);
    if (randpos < ch.size()) {
      polygon[i] = ch[ch.size()-1]; // using always the last point make sure the c.h. is orderly distributed in the polygon.
      points[polygon[i]].v = i;
      ch.pop_back();
    }
    else {
      randpos = randpos - ch.size();
      polygon[i] = ip[randpos];
      ip[randpos] = ip[ip.size()-1];
      points[polygon[i]].v = i;
      ip.pop_back();
    }
	} while (i != 0) ;

  //std::cerr << "polygon: " << std::endl;
  //pdisplay(polygon, points);
}


// function to return the pairs of edges that are the beginning of a polygonal chain
// that ends in incidental convex hull points.
void get_inner_chains_to_ch(std::vector< std::pair<I_Edge,I_Edge> >& ends, std::vector<unsigned int>& ch, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  Point prev, p, next;
  unsigned int diff;
  bool is_left, inner;

  for (unsigned int i = 0; i < ch.size(); ++i) {
    prev = points[ch[(ch.size() + i - 1) % ch.size()]];
    p = points[ch[i]];
    next = points[ch[(ch.size() + i + 1) % ch.size()]];

    // get the difference in index distance between 'prev' and 'p'
    diff = get_lower_cyclic_difference(p.v, prev.v, polygon.size());
//    std::cerr << "diff: " << diff << std::endl;
    //diff = 1 means the 2 c.h. points are connected by an edge.
    if (diff > 1) {
//      std::cerr << "prev: " << prev << std::endl;
//      std::cerr << "p: " << p << std::endl;
//      std::cerr << "next: " << next << std::endl;

      // 'p' and 'prev' create a 'inner' polygonal chain and an 'outer' polygonal chain.
      // if 'next' is inside the 'inner' p. chain, then the inner curve defined by the 2 c.h. points is the 'outer' p. chain
      if (p.v > prev.v) {
        is_left = true;
        // check if 'next' is either lower than 'p' and higher than 'next'
        if ((next.v < p.v) && (next.v > prev.v)) inner = false; //do not use the inner boundary between 'p' and 'prev'
        else inner = true;
      }
      else {
        is_left = false;
        if ((next.v > p.v) && (next.v < prev.v)) inner = false; //do not use the inner boundary between 'p' and 'prev'
        else inner = true;
      }

      // create the edges
      I_Edge e1, e2;
      if (is_left ^ inner) {
        e1 = I_Edge (&points[polygon[prev.v]], &points[polygon[(polygon.size() + prev.v - 1) % polygon.size()]]);
        e2 = I_Edge (&points[polygon[p.v]], &points[polygon[(polygon.size() + p.v + 1) % polygon.size()]]);
      }
      else {
        e1 = I_Edge (&points[polygon[prev.v]], &points[polygon[(polygon.size() + prev.v + 1) % polygon.size()]]);
        e2 = I_Edge (&points[polygon[p.v]], &points[polygon[(polygon.size() + p.v - 1) % polygon.size()]]);
      }
      // set the l2ch boolean of the edges
      if (*e1.p1 == prev) e1.l2ch = true;
      if (*e2.p1 == p) e2.l2ch = true;
//      std::cerr << "e1: " << e1 << ", e2: " << e2 << std::endl;
      std::pair<I_Edge, I_Edge> par (e1, e2);
      ends.push_back(par);
    }
  }
}

// really slow version to check whether a polygon has an intersection.
// Better to use a form of the line-sweep to check.
bool checkAllIntersections (std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  std::cerr << "intersections: ";
  unsigned int count = 0;
  bool ret = false;
  Point *previ, *pi, *prevj, *pj;
  Edge ei, ej;
  enum intersect_t retval;

  previ = &points[polygon[polygon.size()-1]];
  for (unsigned int i = 0; i < polygon.size();++i) {
    pi = &points[polygon[i]];
    ei = Edge(previ, pi);
    prevj = &points[polygon[polygon.size()-1]];
    for (unsigned int j = 0; j < polygon.size();++j) {
      pj = &points[polygon[j]];
      ej = Edge(prevj, pj);
      retval = checkIntersection(ei, ej);

      if ((retval == IS_TRUE) || (retval == IS_4P_COLLINEAR) || (retval == IS_3P_COLLINEAR)) {
        std::cerr << "retval: ";
        printEnum(retval);
        std::cerr << ", ei: " << ei << ", ej: " << ej << std::endl;

        ++count;
        ret = true;
      }
      prevj = pj;
    }
    previ = pi;
  }
  std::cerr << count << std::endl;
  return ret;
}
