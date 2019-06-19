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
#include "pol.h"

// comparison function for sorting pairs by secondary value
bool sortbysec(const std::pair<Point*,double> &a, const std::pair<Point*,double> &b) {
    return (a.second > b.second);
}

bool sortbyint(const int i, const int j) {return (i<j);}

// function to remove edges from 'edgeS' up to and including value of 'index'
void decrementEdges(unsigned int index, std::set<Edge>& edgeS) {
	std::set<Edge>::iterator it = edgeS.begin();
	while (it != edgeS.end()) {
		if ((*it).l_idx >= index) it = edgeS.erase(it);
		else ++it;
	}
}

// this function should be used to guarantee removal of an edge from the 'edgeS' set.
bool eraseEdgeFromSet (Edge e, std::set<Edge>& edgeS) {
  std::set<Edge>::iterator it;

//  std::cerr << "edge being erased: " << e << std::endl;
  it = edgeS.find(e);

  if (it != edgeS.end()) {
    assert(e == *it);
    edgeS.erase(it);
    return true;
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) {
      if (*it == e) {
        edgeS.erase(it);
        return true;
      }
    }
  }
  return false;
}

// function to remove edges connected to a single vertex from 'edgeS' set.
// does not care whether it finds an edge or not, just attempts to remove it.
void eraseVertexFromSet(Point *p1, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  unsigned int before, after;
  Point *p2, *p3;
  Edge e1, e2;

  before = ((*p1).v + points.size() -1) % points.size();
  after =  ((*p1).v + points.size() +1) % points.size();

  p2 = &points[polygon[before]];
  p3 = &points[polygon[after]];

  if (*p2 < *p3) {
    e1 = Edge(p1, p2);
    e2 = Edge(p1, p3);
  } else {
    e1 = Edge(p1, p3);
    e2 = Edge(p1, p2);
  }

//  std::cerr << "erasing vertexes: e1: " << e1 << ", e2: " << e2 << std::endl;
  edgeS.erase(e1); // even if I get the wrong iterator for e1, it's an iterator to e2, so ok to be removed.
  edgeS.erase(e2); // same should be fine with e2.
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
bool collSwap (Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  Point *lo, *mid, *hi;
  //Edge e1, e2;
  //bool be1=false, be2=false;

  assert(*a < *b);
  assert(*a < *c);

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

//  std::cerr << "erasing because of 3PC:";
  // treating the case when vertices are at indices [0,1,...,size()-1]
  if (((*lo).v == 0) && (((*mid).v + (*hi).v) == polygon.size())) {
    eraseVertexFromSet(b, edgeS, polygon, points);
    eraseVertexFromSet(c, edgeS, polygon, points);
    polSwap(lo, mid, polygon);
    if ((*lo).v == 1) polSwap(lo, hi, polygon);
    return true;
  }
  //  std::cerr << "lo: " << *lo << ", mid: " << *mid << ", hi: " << *hi << std::endl;
  else if (((*mid).v == 0) && (((*lo).v + (*hi).v) == polygon.size())) {
    if ((*hi).v == 1) return false;
    else {
      eraseVertexFromSet(b, edgeS, polygon, points);
      eraseVertexFromSet(c, edgeS, polygon, points);
      polSwap(lo, hi, polygon);
    }
    return true;
  }
  else if (((*hi).v == 0) && (((*lo).v + (*mid).v) == polygon.size())) {
    eraseVertexFromSet(b, edgeS, polygon, points);
    eraseVertexFromSet(c, edgeS, polygon, points);
    polSwap(mid, hi, polygon);
    if ((*lo).v == 1) polSwap(lo, hi, polygon);
    return true;
  }

  //treating the case when vertices are at indices [0,...,size()-2,size()-1]
  if (((*lo).v == 0) && (((*mid).v + (*hi).v) == 2*polygon.size()-3)) {
    eraseVertexFromSet(b, edgeS, polygon, points);
    eraseVertexFromSet(c, edgeS, polygon, points);
    polSwap(lo, hi, polygon);
    if ((*lo).v == polygon.size()-1) polSwap(lo, mid, polygon);
    return true;
  }
  else if (((*mid).v == 0) && (((*lo).v + (*hi).v) == 2*polygon.size()-3)) {
    eraseVertexFromSet(b, edgeS, polygon, points);
    eraseVertexFromSet(c, edgeS, polygon, points);
    polSwap(mid, hi, polygon);
    if ((*lo).v == polygon.size()-1) polSwap(lo, mid, polygon);
    return true;
  }
  else if (((*hi).v == 0) && ((*lo).v+(*mid).v == 2*polygon.size()-3)) {
    if (((*lo).v == polygon.size()-2) && ((*mid).v == polygon.size()-1)) {
      return false; // no erasing of edges as no change in order was made.
    }
    if ((*lo).v == polygon.size()-1) {
      eraseVertexFromSet(b, edgeS, polygon, points);
      eraseVertexFromSet(c, edgeS, polygon, points);
      polSwap(lo, mid, polygon);
      return true;
    }
  }
  else {
    eraseVertexFromSet(b, edgeS, polygon, points);
    eraseVertexFromSet(c, edgeS, polygon, points);
    if ((*hi).v < (*mid).v) {polSwap(mid, hi, polygon);}
    if ((*mid).v < (*lo).v) {polSwap(lo, mid, polygon);}
    if ((*hi).v < (*mid).v) {polSwap(mid, hi, polygon);}
    return true;
  }
  std::cerr << "Unexpected error in 3P collSwap!" << std::endl;
  std::cerr << "lo: " << *lo << ", mid: " << *mid << ", hi: " << *hi << std::endl;
  return false;
}

// Function that takes 2 edges, e1 and e2 that both intersect and are collinear
bool collSwap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
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
  //std::cerr << "erasing because of 4PC: e1: " << e1 << ", e2: " << e2 << std::endl;
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
  if (!d1 &&  d2 && (rd1 < 0) && (rd2 == 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    polSwap(e2.p1, e1.p1, polygon);
    polSwap(e2.p1, e2.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 < 0) && (rd2 == 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    polSwap(e1.p1, e2.p1, polygon);
    polSwap(e2.p1, e2.p2, polygon);
    return true;
  }

  std::cerr << "ERROR in 4P collSwap: fallthrough!" << std::endl;
  std::cerr << "e1: " << e1 << ", e2: " << e2 << std::endl;
  std::cerr << "edge collswap: rd1: " << rd1 << ", rd2: " << rd2 << std::endl;
  std::cerr << "d1: " << ((d1) ? "true" : "false") << ", d2: " << ((d2) ? "true" : "false") << std::endl;
  return true;
}

// function to purely remove edge 'e' from 'edgeS' if it's in there and check the 'before' and 'after' edges for intersection.
enum edge_t removeEdgeFromSet(Edge& e, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  Edge before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge>::iterator it;

//  std::cerr << "edge to be removed: " << e << std::endl;
  it = edgeS.find(e);

  if (it != edgeS.end()) {
//    std::cerr << "*it: " << *it << std::endl;
    assert(e == *it);

    // get edges before and after
    if (it != edgeS.begin()) {
      before = *(std::prev(it));
      bef = true;
    }
    if (it != std::prev(edgeS.end())) {
      after  = *(std::next(it));
      af = true;
    }

//    std::cerr << "removing edge from set(): " << *it << std::endl;
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
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_SKIP;
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
        edgeS.erase(it1);
        break;
      }
    }
  }
  return valid;
}

std::pair<enum edge_t, std::set<Edge>::iterator> processEdge(Edge& e, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge before, after;
  std::pair<std::set<Edge>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge>::iterator> retval2;
  //std::set<Edge, setComp>::key_compare mycomp = edgeS.key_comp();

  retval = edgeS.insert(e);
/*
  if (*retval.first != e) {
    std::cerr << "retval.first : " << *retval.first << std::endl;
	   std::cerr << "retval.second: " << retval.second << std::endl;
     std::cout << "edges in 'edgeS':" << std::endl;
     for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
   }
*/
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
//        std::cerr << "4P collinearity between:" << e << " and bef: " << before << std::endl;
        if (collSwap(e, before, edgeS, polygon, points)) {
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_SKIP;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip(e, before, polygon, points);
        valid = E_SKIP;
      }
    }
    // check incidental edge 'after' if it intersects with 'e'
    if (af && (valid == E_VALID)) {
      isval = checkIntersection(e, after);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and aft: " << after << std::endl;
        if (collSwap(e, after, edgeS, polygon, points)) {
//          std::cerr << "4P coll. after swap: " << e << " and " << after << std::endl;
          valid = E_SKIP;
        }
      }
      else {
        //std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSet(e, edgeS, polygon, points);
        //removeEdgeFromSet(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_SKIP;
      }
    }

  } //else {
    // edge already existed in set.
    // this can happen if an index was reset when inserting 'e2' and we're dealing with insertion of e1 again.
  //}
  retval2.first = valid;
  retval2.second = retval.first;
  return retval2;
}

enum error opt2(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
	// initialise and create a random permutation for the polygon
	//createRandPol(polygon, points, randseed);
  createCHRandPol(polygon, points, randseed);
  //pdisplay(polygon, points);
  //assert(1 == 0);

	// the point set 'points' now has x/y coordinates as well as
	// original input index of points in 'i' and polygon index in 'v'
	// Now it can be sorted lexicographically
	std::vector<unsigned int> lex (points.size());
	fill_lex(lex, points); // fill 'lex' with the indexes

	// Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index=0, before, after;
  unsigned int max_so_far, max_count, old_index;
	//double d_idx;
  //compObject comp;
	//std::pair<enum edge_t, std::set<Edge, setComp>::iterator> val1, val2;
  std::pair<enum edge_t, std::set<Edge>::iterator> val1, val2;
  double val3;
	Point *p1, *p2, *p3;
	Edge e1, e2, old_e1, old_e2;
  bool loop;
	//std::set<Edge, setComp> edgeS(comp); // a set of edges.
  std::set<Edge> edgeS; // a set of edges.


  do {
    loop = false;
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

//      std::cerr << std::endl << "index: " << index << std::endl;
  		val1.first = E_VALID; val2.first = E_VALID;
  		// get the current point at 'index'
  		p1 = &points[lex[index]];

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
//        std::cerr << std::endl << "removing: " << e1 << ", and: " << e2 << std::endl;
        val1.first = removeEdgeFromSet(e1, edgeS, polygon, points);
        if (val1.first == E_SKIP) {loop = true;}
        val2.first = removeEdgeFromSet(e2, edgeS, polygon, points);
        if (val2.first == E_SKIP) {loop = true;}
//        std::cerr << "skipping index" << std::endl;
      } else if ((*p1 < *p2) && (*p1 > *p3)) {
        e1 = Edge(p1, p2, index);
        e2 = Edge(p1, p3, index);
        val2.first = removeEdgeFromSet(e2, edgeS, polygon, points);
        if (val2.first == E_SKIP) {loop = true;}
//        std::cerr << std::endl << "removed p3: " << e2 << ", processing: " << e1 << std::endl;
  		  val1 = processEdge(e1, edgeS, polygon, points);
  		  if (val1.first == E_SKIP) {loop = true;continue;}
      } else if ((*p1 > *p2) && (*p1 < *p3) ) {
        e1 = Edge(p1, p3, index);
        e2 = Edge(p1, p2, index);
        val2.first = removeEdgeFromSet(e2, edgeS, polygon, points);
        if (val2.first == E_SKIP) {loop = true;}
//        std::cerr << std::endl << "removed p2: " << e2 << ", processing: " << e1 << std::endl;
  		  val1 = processEdge(e1, edgeS, polygon, points);
  		  if (val1.first == E_SKIP) {loop = true; continue;}
      } else {
        // construct the edges
        //std::cerr << "p1: " << *p1 << ", p2: "<< *p2 << " < p3: " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl;
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
//        std::cerr << "processing e1: " << e1 << ", and e2: " << e2 << std::endl;

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

  		  //std::cerr << std::endl << "processing e1: " << e1 << std::endl;
  		  val1 = processEdge(e1, edgeS, polygon, points);
  		  if (val1.first == E_SKIP) {loop = true;continue;}

  		  //std::cerr << std::endl << "processing e2: " << e2 << std::endl;
  		  val2 = processEdge(e2, edgeS, polygon, points);
  		  if (val2.first == E_SKIP) {
          loop = true;
          edgeS.erase(val1.second);
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
