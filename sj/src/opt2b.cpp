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
#include "opt2base.h"
// Version of BO-2opt that reverses to the lowest index when an intersection is found, then continues.
enum error opt2b(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  enum error retval = SUCCESS;
	// initialise and create a random permutation for the polygon
	createRandPol(polygon, points, randseed);

	// the point set 'points' now has x/y coordinates as well as
	// original input index of points in 'i' and polygon index in 'v'
	// Now it can be sorted lexicographically, each points lex. index is also stored in 'l'
  // 'lex' is an "event-point-schedule" object.
	std::vector<unsigned int> lex (points.size());
	fill_lex(lex, points); // fill 'lex' with the indexes
//  pdisplay(lex, points);

	// Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index, before, after, lowest_index;
  std::pair<enum edge_t, std::set<Edge>::iterator> val1, val2;
  double val3;
	Point *p1, *p2, *p3;
	Edge e1, e2, old_e1, old_e2;
  bool loop, revert, debug=false;
  std::set<Edge> edgeS; // a sweep-line-status object.

  do {
    (debug) ? std::cerr << "looping" << std::endl : std::cerr;
    loop = false;
    revert = false;
    index = 0;
    lowest_index = polygon.size();
    decrementEdges(index, edgeS);

  	while (index < points.size()) {

      if (index > 18751 && index < 18756) {
        debug = true;
        std::cerr << std::endl << "index: " << index << std::endl;
        std::cerr << "revert: " << ((revert) ? "true" : "false") << std::endl;
        std::cerr << "lowest_index: " << lowest_index << std::endl;
        std::cout << "edges in 'edgeS':" << std::endl;
        for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
      }
      else debug = false;

//      std::cerr << std::endl << "index: " << index << std::endl;
//      std::cerr << "lowest_index: " << lowest_index << std::endl;
//      std::cerr << "loop: " << loop << std::endl;
  		val1.first = E_VALID; val2.first = E_VALID;
  		// get the current point at 'index'
  		p1 = &points[lex[index]];

  		// get the 2 points it is connected to in 'polygon', treating the edge case when the point 'p1' is on the ends
  		before = ((*p1).v + points.size() -1) % points.size();
  		after =  ((*p1).v + points.size() +1) % points.size();

  		p2 = &points[polygon[before]];
  		p3 = &points[polygon[after]];

      // construct the edges
      (debug) ? std::cerr << "p1: " << *p1 << ", p2: "<< *p2 << " < p3: " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl : std::cerr;
      if ((!revert && (*p2 < *p3)) || (revert && (*p3 < *p2))) {  // make sure the earlier edge gets processed first.
        e1 = Edge (p1, p2, index);
        e2 = Edge (p1, p3, index);
        val3 = det(e1, *p3);
      }
      else {
        e1 = Edge (p1, p3, index);
        e2 = Edge (p1, p2, index);
        val3 = det(e1, *p2);
      }

      //process first edge
      if ((revert && (*e1.p1 == *p1)) || (!revert && (*e1.p2 == *p1))) {
        (debug) ? std::cerr << std::endl << "removing e1: " << e1 << std::endl : std::cerr;
        val1.first = removeEdgeFromSetb(e1, lowest_index, edgeS, polygon, points);
        if (val1.first == E_SKIP) {
          // before restarting, make sure e2 wasn't supposed to be removed as well, if so, remove it.
          if ((revert && (*e2.p1 == *p1)) || (!revert && (*e2.p2 == *p1))) {
            removeEdgeFromSetb(e2, lowest_index, edgeS, polygon, points);
          }
          loop=true;revert=true;}
        if (val1.first == E_NOT_VALID) break;
      }
      else {
        // if I am about to process 'e1' I can start by clearing it of any collinearity with 'e2'
        if (fabs(val3) < EPSILON) {
          // the 2 edges are collinear
          (debug) ? std::cerr << "collinear check found a possible match."  << std::endl : std::cerr;
          if ((*p2 < *p1) && (*p3 < *p1)) {
          (debug) ? std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
            if (coll3Swap(p1, p2, p3, edgeS, polygon, points)) {
              update_lowest_index(p1, p2, p3, lowest_index);
              (debug) ? std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
              loop = true;revert = true;continue;
            }
          } //else std::cerr << "false alarm." << std::endl;
        }

        (debug) ? std::cerr << std::endl << "processing e1: " << e1 << std::endl : std::cerr;
        val1 = processEdgeb(e1, lowest_index, edgeS, polygon, points);
        if (val1.first == E_SKIP) {loop=true;revert=true;continue;}
        if (val1.first == E_NOT_VALID) break;
      }

      // process second edge
      if ((revert && (*e2.p1 == *p1)) || (!revert && (*e2.p2 == *p1))) {
        (debug) ? std::cerr << std::endl << "removing e2: " << e2 << std::endl : std::cerr;
        val2.first = removeEdgeFromSetb(e2, lowest_index, edgeS, polygon, points);
        if (val2.first == E_SKIP) {loop=true;revert=true;}
        if (val2.first == E_NOT_VALID) break;
      }
      else {
        (debug) ? std::cerr << std::endl << "processing e2: " << e2 << std::endl : std::cerr;
        val2 = processEdgeb(e2, lowest_index, edgeS, polygon, points);
        if (val2.first == E_SKIP) {
          val1.first = removeEdgeFromSetb(e1, lowest_index, edgeS, polygon, points);
          if (val1.first == E_NOT_VALID) break;
          loop=true;revert=true;continue;
        }
        if (val2.first == E_NOT_VALID) break;
      }

      if (lowest_index == index) {
        revert = false;
        lowest_index = polygon.size();
        continue;
      }
  		if (revert) --index;
      else ++index;
//      std::cerr << "revert: " << ((revert) ? "true" : "false") << std::endl;
//      std::cerr << "lowest_index: " << lowest_index << std::endl;
      if (loop == true) std::cerr << "i: " << index << ", l_i: " << lowest_index << std::endl;
  	}
    if ((val1.first == E_NOT_VALID) || (val2.first == E_NOT_VALID)) {retval=UNEXPECTED_ERROR; break;}
  } while (loop);

	return retval;
}
