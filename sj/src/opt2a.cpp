#include <iostream> // for endl
#include <vector>
#include <list>
#include <set>
#include <utility> // for std::pair
#include <cmath>  // for fabs
#include <algorithm>    // std::sort
#include <iterator> // for std:prev and std::next
#include <assert.h>
#include "basicDefinitions.h"
#include "basicFunctions.h"
#include "point.h"
#include "edge.h"
#include "pol.h"

// function to purely remove edge 'e' from 'edgeS' if it's in there and check the 'before' and 'after' edges for intersection.
enum edge_t removeEdgeFromSeta(Edge& e, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
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

std::pair<enum edge_t, std::set<Edge>::iterator> processEdgea(Edge& e, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
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
  assert(e == *retval.first);
//  std::cerr << "inserted: " << e << std::endl;
//  std::cerr << "returned: " << *retval.first << std::endl;
//  std::cerr << "equality check: " << ((e == *retval.first) ? "yes, equal." : "no, not equal.") << std::endl;

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
        //removeEdgeFromSeta(e, edgeS, polygon, points);
        //removeEdgeFromSeta(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_SKIP;
      }
    }

  }
  else {
    // edge already existed in set.
    // this can happen if an index was reset when inserting 'e2' and we're dealing with insertion of e1 again.
    std::cerr << "Error: Edge already exists in set! : " << *retval.first << std::endl;
    valid = E_NOT_VALID;
  }
  retval2.first = valid;
  retval2.second = retval.first;
  return retval2;
}


// here we restart the index we look for after every found and fixed intersection.
enum error opt2a(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  enum error retval = SUCCESS;
	// initialise and create a random permutation for the polygon
	createRandPol(polygon, points, randseed);
  //createCHRandPol(polygon, points, randseed);
  // .
  //pdisplay(polygon, points);
  //assert(1 == 0);

	// the point set 'points' now has x/y coordinates as well as
	// original input index of points in 'i' and polygon index in 'v'
	// Now it can be sorted lexicographically
	std::vector<unsigned int> lex (points.size());
	fill_lex(lex, points); // fill 'lex' with the indexes

	// Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index=0, before, after;
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
    index = 0;
    decrementEdges(index, edgeS);

    //std::cerr << "New loop" << std::endl;
  	while (index < points.size()) {

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
      if ((*p2 < *p1) && (*p3 < *p1)) {
        e1 = Edge(p1, p2, index);
        e2 = Edge(p1, p3, index);
//        std::cerr << std::endl << "removing: " << e1 << ", and: " << e2 << std::endl;
        val1.first = removeEdgeFromSeta(e1, edgeS, polygon, points);
        if (val1.first == E_SKIP) {loop = true;}
        if (val1.first == E_NOT_VALID) break;
        val2.first = removeEdgeFromSeta(e2, edgeS, polygon, points);
        if (val2.first == E_SKIP) {loop = true;}
        if (val2.first == E_NOT_VALID) break;
//        std::cerr << "skipping index" << std::endl;
      } else if ((*p1 < *p2) && (*p3 < *p1)) {
        e1 = Edge(p1, p2, index);
        e2 = Edge(p1, p3, index);
        val2.first = removeEdgeFromSeta(e2, edgeS, polygon, points);
        if (val2.first == E_SKIP) {loop = true;}
//        std::cerr << std::endl << "removed p3: " << e2 << ", processing: " << e1 << std::endl;
  		  val1 = processEdgea(e1, edgeS, polygon, points);
  		  if (val1.first == E_SKIP) {loop = true;}
        if (val1.first == E_NOT_VALID) break;
      } else if ((*p2 < *p1) && (*p1 < *p3) ) {
        e1 = Edge(p1, p3, index);
        e2 = Edge(p1, p2, index);
        val2.first = removeEdgeFromSeta(e2, edgeS, polygon, points);
        if (val2.first == E_SKIP) {loop = true;}
//        std::cerr << std::endl << "removed p2: " << e2 << ", processing: " << e1 << std::endl;
  		  val1 = processEdgea(e1, edgeS, polygon, points);
  		  if (val1.first == E_SKIP) {loop = true;}
        if (val1.first == E_NOT_VALID) break;
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
//        std::cerr << "det3: " << val3 << std::endl;
        if (fabs(val3) < EPSILON) {
          // the 2 edges are collinear
          //std::cerr << "collinear check found a possible match."  << std::endl;
          if ((*p1 < *p2) && (*p1 < *p3)) {
            //std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl;
            old_e1 = e1;
            old_e2 = e2;
            if (collSwap(p1, p2, p3, edgeS, polygon, points)) {
//              std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl;
              loop = true;
              break;
            }
          } //else std::cerr << "false alarm." << std::endl;
        }

  		  //std::cerr << std::endl << "processing e1: " << e1 << std::endl;
  		  val1 = processEdgea(e1, edgeS, polygon, points);
  		  if (val1.first == E_SKIP) loop = true;
        if (val1.first == E_NOT_VALID) break;

  		  //std::cerr << std::endl << "processing e2: " << e2 << std::endl;
  		  val2 = processEdgea(e2, edgeS, polygon, points);
  		  if (val2.first == E_SKIP) loop = true;
        if (val2.first == E_NOT_VALID) break;
      }

//      std::cout << "edges in 'edgeS':" << std::endl;
//      for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

      if (loop) {
        break;
      }

  		++index;
  	}
    if ((val1.first == E_NOT_VALID) || (val2.first == E_NOT_VALID)) {retval=UNEXPECTED_ERROR; break;}
  } while (loop);

	return retval;
}
