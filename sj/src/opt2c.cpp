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

// Version of BO-2opt that reverses to a lower index as well as saves the sweep-line status in case no intersections occurred going back.

// Have a max_back index that the algorithm goes to before moving forward again.
enum error opt2c(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  enum error retval = SUCCESS;
	// initialise and create a random permutation for the polygon
	createRandPol(polygon, points, randseed);

	// the point set 'points' now has x/y coordinates as well as
	// original input index of points in 'i' and polygon index in 'v'
	// Now it can be sorted lexicographically, each points lex. index is also stored in 'l'
  // 'lex' is an "event-point-schedule" object.
	std::vector<unsigned int> lex (points.size());
	fill_lex(lex, points); // fill 'lex' with the indexes

	// Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index, old_index, before, after, lowest_index;
  std::pair<enum edge_t, std::set<Edge>::iterator> val1, val2;
  double val3;
	Point *p1, *p2, *p3;
	Edge e1, e2, old_e1, old_e2;
  bool loop, revert, rev_found;
  std::set<Edge> edgeS; // a sweep-line-status object.
  std::set<Edge> edgeS_old; // the old sweep line.

  do {
    loop = false;
    revert = false;
    rev_found = false;
    index = 0; old_index = 0;
    lowest_index = polygon.size();
    decrementEdges(index, edgeS);

//    std::cerr << "New loop" << std::endl;
  	while (index < points.size()) {
/*      if (index > 32576 && index < 32579) {
        std::cerr << "index: " << index << std::endl;
        std::cout << "edges in 'edgeS':" << std::endl;
        for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
      }
*/

//      std::cerr << std::endl << "index: " << index << std::endl;
  		val1.first = E_VALID; val2.first = E_VALID;
  		// get the current point at 'index'
  		p1 = &points[lex[index]];

  		// get the 2 points it is connected to in 'polygon', treating the edge case when the point 'p1' is on the ends
  		before = ((*p1).v + points.size() -1) % points.size();
  		after =  ((*p1).v + points.size() +1) % points.size();

  		p2 = &points[polygon[before]];
  		p3 = &points[polygon[after]];

      // construct the edges
//      std::cerr << "p1: " << *p1 << ", p2: "<< *p2 << " < p3: " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl;
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
//        std::cerr << std::endl << "removing e1: " << e1 << std::endl;
        val1.first = removeEdgeFromSetb(e1, lowest_index, edgeS, polygon, points);
        if ((val1.first == E_INTERSECTION) || (val1.first == E_COLLINEAR)) {
          // before restarting, make sure e2 wasn't supposed to be removed as well, if so, remove it.
          if ((revert && (*e2.p1 == *p1)) || (!revert && (*e2.p2 == *p1))) {
            val2.first = removeEdgeFromSetb(e2, lowest_index, edgeS, polygon, points);
            if (val2.first == E_NOT_VALID) break;
          }
          if (revert) rev_found = true;
          else {edgeS_old = edgeS;rev_found=false;old_index=index;}
          loop=true;revert=true;}
        if (val1.first == E_NOT_VALID) break;
      }
      else {
        // if I am about to process 'e1' I can start by clearing it of any collinearity with 'e2'
        if (fabs(val3) < EPSILON) {
          // the 2 edges are collinear
//          std::cerr << "collinear check found a possible match."  << std::endl;
          if ((*p2 < *p1) && (*p3 < *p1)) {
//            std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl;
            if (coll3Swap(p1, p2, p3, edgeS, polygon, points)) {
              update_lowest_index(p1, p2, p3, lowest_index);
//              std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl;
              if (revert) rev_found = true;
              else {edgeS_old = edgeS;rev_found=false;old_index=index;}
              loop = true;revert = true;continue;
            }
          } //else std::cerr << "false alarm." << std::endl;
        }

//        std::cerr << std::endl << "processing e1: " << e1 << std::endl;
        val1 = processEdgeb(e1, lowest_index, edgeS, polygon, points);
        if ((val1.first == E_INTERSECTION) || (val1.first == E_COLLINEAR)) {
          if(revert)rev_found=true;
          else {edgeS_old = edgeS;rev_found=false;old_index=index;}
          loop=true;revert=true;continue;}
        if (val1.first == E_NOT_VALID) break;
      }

      // process second edge
      if ((revert && (*e2.p1 == *p1)) || (!revert && (*e2.p2 == *p1))) {
//        std::cerr << std::endl << "removing e2: " << e2 << std::endl;
        val2.first = removeEdgeFromSetb(e2, lowest_index, edgeS, polygon, points);
        if (val2.first == E_NOT_VALID) break;
        if ((val2.first == E_INTERSECTION) || (val2.first == E_COLLINEAR)) {
          if(revert)rev_found=true;
          else {edgeS_old = edgeS;rev_found=false;old_index=index;}
          loop=true;revert=true;}
      }
      else {
//        std::cerr << std::endl << "processing e2: " << e2 << std::endl;
        val2 = processEdgeb(e2, lowest_index, edgeS, polygon, points);
        if (val2.first == E_NOT_VALID) break;
        if ((val2.first == E_INTERSECTION) || (val2.first == E_COLLINEAR)) {
          val1.first = removeEdgeFromSetb(e1, lowest_index, edgeS, polygon, points);
          if (val1.first == E_NOT_VALID) break;
          if(revert)rev_found=true;
          else {edgeS_old = edgeS;rev_found=false;old_index=index;}
          loop=true;revert=true;continue;
        }
      }

      if (lowest_index == index) {
        revert = false;
        lowest_index = polygon.size();
        if (!rev_found) {
          index = old_index;
          edgeS = edgeS_old;
        }
        continue;
      }
  		if (revert) --index;
      else ++index;
//      std::cerr << "revert: " << ((revert) ? "true" : "false") << std::endl;
//      std::cerr << "lowest_index: " << lowest_index << std::endl;
  	}
    if ((val1.first == E_NOT_VALID) || (val2.first == E_NOT_VALID)) {retval=UNEXPECTED_ERROR; break;}
  } while (loop);

	return retval;
}
