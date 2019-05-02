#include <iostream> // for endl
#include <vector>
#include <list>
#include <set>
#include <utility> // for std::pair
#include <stdlib.h>  // for abs
#include <algorithm>    // std::sort
#include <iterator> // for std:prev and std::next
#include <assert.h>
#include "basicDefinitions.h"
#include "basicFunctions.h"
#include "point.h"
#include "edge.h"
#include "randomGenerator.h"

// comparison function for sorting pairs by secondary value
bool sortbysec(const std::pair<Point*,double> &a, const std::pair<Point*,double> &b) {
    return (a.second > b.second);
}

bool sortbyint(const int i, const int j) {return (i<j);}

// function to remove edges from 'edgeS' up to and including value of 'index'
void decrementEdges(unsigned int index, std::set<Edge, setComp>& edgeS) {
	std::set<Edge, setComp>::iterator it = edgeS.begin();
	while (it != edgeS.end()) {
		if ((*it).l_idx >= index) it = edgeS.erase(it);
		else ++it;
	}
}

enum edge_t removeEdgeFromSet(Edge& e, std::set<Edge, setComp>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);

// function to remove edges connected to a single vertex from 'edgeS' set.
void eraseVertexFromSet(Point *p1, std::set<Edge, setComp>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  unsigned int before, after;
  Point *p2, *p3;
  Edge e1, e2;

  before = ((*p1).v + points.size() -1) % points.size();
  after =  ((*p1).v + points.size() +1) % points.size();

  p2 = &points[polygon[before]];
  p3 = &points[polygon[after]];

  e1 = Edge(p1, p2);
  e2 = Edge(p1, p3);

  removeEdgeFromSet(e1, edgeS, polygon, points);
  removeEdgeFromSet(e2, edgeS, polygon, points);
}

// swaps the order of points of the edge in the polygon,
// i.e. v values of the points in the edge 'e' and order of points in 'polygon'.
void polSwap(Edge e, std::vector<unsigned int>& polygon) {
  unsigned int temp = (*e.p1).v;
  (*e.p1).v = (*e.p2).v;
  (*e.p2).v = temp;
  polygon[(*e.p1).v] = (*e.p1).i;
  polygon[(*e.p2).v] = (*e.p2).i;
}
void polSwap(Point* a, Point* b, std::vector<unsigned int>& polygon) {
  unsigned int temp = (*a).v;
  (*a).v = (*b).v;
  (*b).v = temp;
  polygon[(*a).v] = (*a).i;
  polygon[(*b).v] = (*b).i;
}

// function that takes 3 points: a, b, and c that are already assumed collinear
// a is assumed to be the lowest point lexicographically as well as the middle point in polygon of the 3 points.
// swaps the points in the polygon so that the lex. order of the points is also the vertex order of the points.
bool collSwap(Point *a, Point *b, Point *c, std::set<Edge, setComp>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  Point *lo, *mid, *hi;

  // sort the points into lo/mid/hi lex order.
  if ((*a < *b) && (*a < *c)) {
    lo = a;
    if (*b < *c) {
      mid = b;
      hi = c;
    } else {
      mid = c;
      hi = b;
    }
  }
  else if ((*b < *a) && (*b < *c)) {
    lo = b;
    if (*a < *c) {
      mid = a;
      hi = c;
    } else {
      mid = c;
      hi = a;
    }
  }
  else if ((*c < *b) && (*c < *a)) {
    lo = c;
    if (*b < *a) {
      mid = b;
      hi = a;
    } else {
      mid = a;
      hi = b;
    }
  }

  // treating the case when vertices are at indices [0,1,...,size()-1]
  if (((*lo).v == 0) && (((*mid).v + (*hi).v) == polygon.size())) {
    eraseVertexFromSet(lo, edgeS, polygon, points);
    eraseVertexFromSet(mid, edgeS, polygon, points);
    eraseVertexFromSet(hi, edgeS, polygon, points);
    polSwap(lo, mid, polygon);
    if ((*lo).v == 1) polSwap(lo, hi, polygon);
    return true;
  }
  //  std::cerr << "lo: " << *lo << ", mid: " << *mid << ", hi: " << *hi << std::endl;
  else if (((*mid).v == 0) && (((*lo).v + (*hi).v) == polygon.size())) {
    if ((*hi).v == 1) return false;
    else {
      eraseVertexFromSet(lo, edgeS, polygon, points);
      eraseVertexFromSet(mid, edgeS, polygon, points);
      eraseVertexFromSet(hi, edgeS, polygon, points);
      polSwap(lo, hi, polygon);
    }
    return true;
  }
  else if (((*hi).v == 0) && (((*lo).v + (*mid).v) == polygon.size())) {
    eraseVertexFromSet(lo, edgeS, polygon, points);
    eraseVertexFromSet(mid, edgeS, polygon, points);
    eraseVertexFromSet(hi, edgeS, polygon, points);
    polSwap(mid, hi, polygon);
    if ((*lo).v == 1) polSwap(lo, hi, polygon);
    return true;
  }

  //treating the case when vertices are at indices [0,...,size()-2,size()-1]
  if (((*lo).v == 0) && (((*mid).v + (*hi).v) == 2*polygon.size()-3)) {
    eraseVertexFromSet(lo, edgeS, polygon, points);
    eraseVertexFromSet(mid, edgeS, polygon, points);
    eraseVertexFromSet(hi, edgeS, polygon, points);
    polSwap(lo, hi, polygon);
    if ((*lo).v == polygon.size()-1) polSwap(lo, mid, polygon);
    return true;
  }
  else if (((*mid).v == 0) && (((*lo).v + (*hi).v) == 2*polygon.size()-3)) {
    eraseVertexFromSet(lo, edgeS, polygon, points);
    eraseVertexFromSet(mid, edgeS, polygon, points);
    eraseVertexFromSet(hi, edgeS, polygon, points);
    polSwap(mid, hi, polygon);
    if ((*lo).v == polygon.size()-1) polSwap(lo, mid, polygon);
    return true;
  }
  else if (((*hi).v == 0) && ((*lo).v+(*mid).v == 2*polygon.size()-3)) {
    if (((*lo).v == polygon.size()-2) && ((*mid).v == polygon.size()-1)) {
      return false;
    }
    if ((*lo).v == polygon.size()-1) {
      eraseVertexFromSet(lo, edgeS, polygon, points);
      eraseVertexFromSet(mid, edgeS, polygon, points);
      eraseVertexFromSet(hi, edgeS, polygon, points);
      polSwap(lo, mid, polygon);
      return true;
    }
  }
  else {
    if ((*hi).v < (*mid).v) {
      eraseVertexFromSet(lo, edgeS, polygon, points);
      eraseVertexFromSet(mid, edgeS, polygon, points);
      eraseVertexFromSet(hi, edgeS, polygon, points);
      polSwap(mid, hi, polygon);
    }
    if ((*mid).v < (*lo).v) {
      eraseVertexFromSet(lo, edgeS, polygon, points);
      eraseVertexFromSet(mid, edgeS, polygon, points);
      eraseVertexFromSet(hi, edgeS, polygon, points);
      polSwap(lo, mid, polygon);
    }
    if ((*hi).v < (*mid).v) {
      eraseVertexFromSet(lo, edgeS, polygon, points);
      eraseVertexFromSet(mid, edgeS, polygon, points);
      eraseVertexFromSet(hi, edgeS, polygon, points);
      polSwap(mid, hi, polygon);
    }
    return true;
  }
  std::cerr << "Unexpected error in 3P collSwap!" << std::endl;
  std::cerr << "lo: " << *lo << ", mid: " << *mid << ", hi: " << *hi << std::endl;
  return false;
}

// Function that takes 2 edges, e1 and e2 that both intersect and are collinear
bool collSwap (Edge& e1, Edge& e2, std::set<Edge, setComp>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  bool d1, d2;
  double rd1 = reldist(e1, *e2.p1);
  double rd2 = reldist(e1, *e2.p2);

  if (((*e1.p1).v == 0) && (*e1.p2).v == polygon.size()-1)
    d1 = false;
  else if (((*e1.p2).v == 0) && (*e1.p1).v == polygon.size()-1)
    d1 = true;
  else {
    d1 = ((int)(*e1.p1).v - (int)(*e1.p2).v) < 0; // true  means edge order in poly is : [...p1p2...]
  }

  if (((*e2.p1).v == 0) && (*e2.p2).v == polygon.size()-1)
    d2 = false;
  else if (((*e2.p2).v == 0) && (*e2.p1).v == polygon.size()-1)
    d2 = true;
  else {
    d2 = ((int)(*e2.p1).v - (int)(*e2.p2).v) < 0; // false means edge order in poly is : [...p2p1...]
  }


  if ((rd1 > 1) && (rd2 > 1)) return false;
  if ((rd1 < 0) && (rd2 < 0)) return false;
  if ( d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p2, e2.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    return true;
  }
  if (!d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    polSwap(e1.p2, e2.p2, polygon);
    return true;
  }

  if ( d1 &&  d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p1, e1.p1, polygon);
    return true;
  }
  if (!d1 && !d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p1, e1.p1, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }

  if ( d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }
  if (!d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    return true;
  }

  if ( d1 &&  d2 && (rd1 < 0) && (rd2 > 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e1.p2, e2.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 < 0) && (rd2 > 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 < 0) && (rd2 > 1)) {
    if ((*e1.p1).v + (*e1.p2).v < (*e2.p1).v + (*e2.p2).v) {
      eraseVertexFromSet(e1.p1, edgeS, polygon, points);
      eraseVertexFromSet(e2.p1, edgeS, polygon, points);
      eraseVertexFromSet(e1.p2, edgeS, polygon, points);
      eraseVertexFromSet(e2.p2, edgeS, polygon, points);
      polSwap(e1.p1, e2.p1, polygon);
      polSwap(e1.p2, e1.p1, polygon);
      polSwap(e1.p2, e2.p2, polygon);
      return true;
    } else {
      eraseVertexFromSet(e1.p1, edgeS, polygon, points);
      eraseVertexFromSet(e2.p2, edgeS, polygon, points);
      eraseVertexFromSet(e1.p2, edgeS, polygon, points);
      eraseVertexFromSet(e2.p1, edgeS, polygon, points);
      polSwap(e1.p1, e2.p2, polygon);
      polSwap(e1.p1, e1.p2, polygon);
      polSwap(e2.p1, e2.p2, polygon);
      return true;
    }

  }
  if (!d1 && !d2 && (rd1 < 0) && (rd2 > 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p1, e1.p1, polygon);
    return true;
  }
  std::cerr << "ERROR in 4P collSwap!" << std::endl;
  std::cerr << "edge collswap: rd1: " << rd1 << ", dp2: " << rd2 << std::endl;
  std::cerr << "d1: " << ((d1) ? "true" : "false") << ", d2: " << ((d2) ? "true" : "false") << std::endl;
  return true;
}

// function to purely remove edge 'e' from 'edgeS' if it's in there and check the 'before' and 'after' edges for intersection.
enum edge_t removeEdgeFromSet(Edge& e, std::set<Edge, setComp>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  Edge before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge, setComp>::iterator it;

  std::cerr << "edge being removed: " << e << std::endl;
  it = edgeS.find(e);
  if (it != edgeS.end()) {
    std::cerr << "edge found: " << *it << std::endl;
    // get edges before and after
    if (it != edgeS.begin()) {
      before = *(std::prev(it));
      bef = true;
    }
    if (it != std::prev(edgeS.end())) {
      after  = *(std::next(it));
      af = true;
    }

    std::cerr << "it check: " << *it << std::endl;
    edgeS.erase(it);

    if (bef && af) {
      //std::cerr << "before: " << before << ", after: " << after << std::endl;
      isval = checkIntersection(before, after);
      if (isval == IS_4P_COLLINEAR) {
        //std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (collSwap(before, after, edgeS, polygon, points)) {
          //std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_SKIP;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
        //std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        edgeS.erase(before);
        edgeS.erase(after);
        flip(before, after, polygon, points);
        valid = E_SKIP;
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
      }
    }
  }
  return valid;
}

enum edge_t processEdge(Edge& e, std::set<Edge, setComp>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge before, after;
  std::pair<std::set<Edge, setComp>::iterator,bool> retval;
  //std::set<Edge, setComp>::key_compare mycomp = edgeS.key_comp();

  retval = edgeS.insert(e);

  std::cerr << "retval.first : " << *retval.first << std::endl;
	std::cerr << "retval.second: " << retval.second << std::endl;
  assert(*retval.first == e);

  //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
  if (retval.first != edgeS.begin()) {
    before = *(std::prev(retval.first));
    //std::cerr << "before: " << before << std::endl;
    bef = true;
  }
  //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
  if (retval.first != --edgeS.end()) {
    after  = *(std::next(retval.first));
    //std::cerr << "after : " << after << std::endl;
    af = true;
  }

  if (retval.second) {  // successfully inserted edge.
    // check incidental edge 'before' if it intersects with 'e'
    if (bef) {
      isval = checkIntersection(e, before);
      if (isval < IS_TRUE) {
        //std::cerr << "No intersection." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
        //std::cerr << "4P collinearity between:" << e << " and " << before << std::endl;
        if (collSwap(e, before, edgeS, polygon, points)) {
          //std::cerr << "4P coll. after swap: " << e << " and " << before << std::endl;
          valid = E_SKIP;
        }
      }
      else {
        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
        removeEdgeFromSet(e, edgeS, polygon, points);
        removeEdgeFromSet(before, edgeS, polygon, points);
        flip(e, before, polygon, points);
        valid = E_SKIP;
      }
    }
    // check incidental edge 'after' if it intersects with 'e'
    if (af && (valid == E_VALID)) {
      isval = checkIntersection(e, after);
      if (isval < IS_TRUE) {
        //std::cerr << "No intersection." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
        //std::cerr << "4P collinearity between:" << e << " and " << after << std::endl;
        if (collSwap(e, after, edgeS, polygon, points)) {
          //std::cerr << "4P coll. after swap: " << e << " and " << after << std::endl;
          valid = E_SKIP;
        }
      }
      else {
        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
        removeEdgeFromSet(e, edgeS, polygon, points);
        removeEdgeFromSet(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_SKIP;
      }
    }

  } else {
    // edge already existed in set.
    // remove edge and check if incidental edges intersect
    //std::cerr << "'e' found in 'edgeS', removing." << std::endl;
    edgeS.erase(retval.first);
    if (bef && af) {
      isval = checkIntersection(before, after);
      if (isval == IS_4P_COLLINEAR) {
        //std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (collSwap(before, after, edgeS, polygon, points)) {
          //std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_SKIP;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
        //std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        removeEdgeFromSet(before, edgeS, polygon, points);
        removeEdgeFromSet(after, edgeS, polygon, points);
        flip(before, after, polygon, points);
        valid = E_SKIP;
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
      }
    }
  }
  return valid;
}

enum error opt2(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
	// initialise and create a random permutation for the polygon
	createRandPol(polygon, points);

	// the point set 'points' now has x/y coordinates as well as
	// original input index of points in 'i' and polygon index in 'v'
	// Now it can be sorted lexicographically
	std::vector<unsigned int> lex (points.size());
	for(unsigned int i = 0; i < points.size(); ++i)
		lex[i] = points[i].i;

	// lex contains a vector of 'points' indexes sorted lexicographically
	std::sort(lex.begin(), lex.end(), lexComp(points));
  unsigned int counter = 0;
  for (std::vector<unsigned int>::iterator it = lex.begin(); it != lex.end(); ++it) {
    points[(*it)].l = counter;
    ++counter;
  }

	// Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index=0, before, after;
  unsigned int max_so_far, max_count, old_index;
	//double d_idx;
  compObject comp;
	enum edge_t val1, val2;
  double val3;
	Point *p1, *p2, *p3;
	Edge e1, e2, old_e1, old_e2;
  bool loop;
	std::set<Edge, setComp> edgeS(comp); // a set of edges.

  do {
    loop = false;
    comp.lower_idx = 0;
    max_so_far = 0;
    old_index = 0;
    max_count = 0;
    index = 0;
    decrementEdges(index, edgeS);

    //std::cerr << "New loop" << std::endl;
  	while (index < points.size()) {

  		if (max_so_far < index) {
        max_so_far = index;
        max_count = 0;
      }
      if ((index == 0) && (old_index == max_so_far)) {
        ++max_count;
        if (max_count > 20)
          std::cerr << "max: " << max_so_far << ", counter: " << max_count << std::endl;
      }

      //std::cerr << std::endl << "index: " << index << std::endl;
  		val1 = E_VALID; val2 = E_VALID;
  		// get the current point at 'index'
  		p1 = &points[lex[index]];
  		comp.t = (*p1).x; // the x index used as the comparison.

  		// get the 2 points it is connected to in 'polygon', treating the edge case when the point 'p1' is on the ends
  		before = ((*p1).v + points.size() -1) % points.size();
  		after =  ((*p1).v + points.size() +1) % points.size();

  		p2 = &points[polygon[before]];
  		p3 = &points[polygon[after]];

      // From the current index you do not want to add edges to a lower lex point,
      // so only process both edges if p2 and p3 are higher lex. order.
      // BUT, the edges need to be checked if they are in 'edgeS' and if so, remove them and check BEFORE and AFTER edges.
      if ((*p1 > *p2) && (*p1 > *p3)) {
        e1 = Edge(p1, p2, index);
        e2 = Edge(p1, p3, index);
        val1 = removeEdgeFromSet(e1, edgeS, polygon, points);
        if (val1 == E_SKIP) {loop = true; continue;}
        val2 = removeEdgeFromSet(e2, edgeS, polygon, points);
        if (val2 == E_SKIP) {loop = true; continue;}
        //std::cerr << "skipping index" << std::endl;
      } else if ((*p1 < *p2) && (*p1 > *p3)) {
        e1 = Edge (p1, p2, index);
        e2 = Edge(p1, p3, index);
        val2 = removeEdgeFromSet(e2, edgeS, polygon, points);
        if (val2 == E_SKIP) {loop = true; continue;}
        std::cerr << "skipping p3: " << *p3 << ", processing: " << e1 << std::endl;
  		  val1 = processEdge(e1, edgeS, polygon, points);
  		  if (val1 == E_SKIP) {
          loop = true;
          continue;
        }
      } else if ((*p1 > *p2) && (*p1 < *p3) ) {
        e1 = Edge(p1, p3, index);
        e2 = Edge(p1, p2, index);
        val2 = removeEdgeFromSet(e2, edgeS, polygon, points);
        if (val2 == E_SKIP) {loop = true; continue;}
        std::cerr << "skipping p2: " << *p2 << ", processing: " << e1 << std::endl;
  		  val1 = processEdge(e1, edgeS, polygon, points);
  		  if (val1 == E_SKIP) {loop = true; continue;}
      } else {
        // construct the edges
        std::cerr << *p2 << " < " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl;
  		  if (*p2 < *p3) {  // make sure the earlier edge gets processed first.
  			  e1 = Edge (p1, p2, index);
  			  e2 = Edge (p1, p3, index);
          val3 = det(e1, *p3);
  		  }
  		  else {
  			  e1 = Edge (p1, p3, index);
  			  e2 = Edge (p1, p2, index);
          val3 = det(e1, *p2);
  		  }

        if (val3 == 0) {
          // the 2 edges are collinear
          //std::cerr << "collinear check found a possible match."  << std::endl;
          if ((*p1 < *p2) && (*p1 < *p3)) {
            //std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl;
            old_e1 = e1;
            old_e2 = e2;
            if (collSwap(p1, p2, p3, edgeS, polygon, points)) {
              //std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl;
              loop = true;
              continue;
            }
          } //else std::cerr << "false alarm." << std::endl;
        }

  		  std::cerr << "processing e1: " << e1 << std::endl;
  		  val1 = processEdge(e1, edgeS, polygon, points);
  		  if (val1 == E_SKIP) {
          loop = true;
          continue; // swapping invalidates 'e2' so start again from the lower index before processing 'e2'
        }

  		  std::cerr << std::endl << "processing e2: " << e2 << std::endl;
  		  val2 = processEdge(e2, edgeS, polygon, points);
  		  if (val2 == E_SKIP) {
          loop = true;
          continue;
        }
      }

      //std::cout << "edges in 'edgeS':" << std::endl;
      //for (std::set<Edge, setComp>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
  		index++;
      old_index = index;
  	}
  } while (loop);

	return SUCCESS;
}
