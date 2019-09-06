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


void print_enum(enum planesweep_t val) {
  switch (val) {
    case P_CLEAN:
      std::cerr << "P_CLEAN";
      break;
    case P_DIRTY_LEFT:
      std::cerr << "P_DIRTY_LEFT";
      break;
    case P_DIRTY_RIGHT:
      std::cerr << "P_DIRTY_RIGHT";
      break;
    default:
      break;
  }
}


// Version of BO-2opt that reverses to the lowest index when an intersection is found, then continues.
/*
enum error opt2b(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  enum error retval = SUCCESS;
  enum planesweep_t p_status = P_CLEAN;
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
  enum edge_t val1_2, val2_1;
  double val3;
	Point *p1, *p2, *p3;
	Edge e1, e2, old_e1, old_e2;
  bool loop, revert, debug=false;
  std::set<Edge> edgeS; // a sweep-line-status object.

  do {
    //(debug) ? std::cerr << "looping" << std::endl : std::cerr;
    std::cerr << "looping" << std::endl;
    loop = false;
    revert = false;
    index = 0;
    lowest_index = polygon.size();
    decrementEdges(index, edgeS);

  	while (index < points.size()) {

      if (index > 170 && index < 250) {
        debug = true;
        std::cout << "edges in 'edgeS':" << std::endl;
        for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
      }
      else debug = false;

      std::cerr << "i: " << index << ", l_i: " << lowest_index << ", loop: " << loop << ", rev: " << ((revert) ? "T" : "F") << ", p_status: " << p_status << std::endl;
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
      if (p_status == P_DIRTY_RIGHT) { // intersection or collinearity was found, clean any problems before reversing direction
        if (*p2 < *p3) {
          e1 = Edge (p1, p2, index);
          e2 = Edge (p1, p3, index);
          val3 = det(e1, *p3);
        }
        else {
          e1 = Edge (p1, p3, index);
          e2 = Edge (p1, p2, index);
          val3 = det(e1, *p2);
        }
      }
      else if (p_status == P_DIRTY_LEFT) { // intersection or collinearity was found, clean any problems before reversing direction
        if (*p3 < *p2) {
          e1 = Edge (p1, p2, index);
          e2 = Edge (p1, p3, index);
          val3 = det(e1, *p3);
        }
        else {
          e1 = Edge (p1, p3, index);
          e2 = Edge (p1, p2, index);
          val3 = det(e1, *p2);
        }
      }
      else if ((!revert && (*p2 < *p3)) || (revert && (*p3 < *p2))) {  // make sure the earlier edge gets processed first.
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
      //if (((p_status == P_DIRTY_LEFT || revert) && (*e1.p1 == *p1)) || ((p_status == P_DIRTY_RIGHT || !revert) && (*e1.p2 == *p1))) {
      if ( ((p_status == P_CLEAN) && !(revert ^ (*p1 == *e1.p1)))
        || ((p_status == P_DIRTY_RIGHT) && (*p1 == *e1.p2))
        || ((p_status == P_DIRTY_LEFT)  && (*p1 == *e1.p1)) )
      {
        (debug) ? std::cerr << std::endl << "removing e1: " << e1 << std::endl : std::cerr;
        val1.first = removeEdgeFromSetb(e1, lowest_index, edgeS, polygon, points);
        if (val1.first == E_NOT_VALID) break;
        if (val1.first == E_SKIP) { // intersection or collinearity found in the removal, skip the rest and restart.
          // before restarting, make sure e2 wasn't supposed to be removed as well, if so, remove it.
          //if ((revert && (*e2.p1 == *p1)) || (!revert && (*e2.p2 == *p1))) {
          if ( ((p_status == P_CLEAN) && !(revert ^ (*p1 == *e2.p1)))
            || ((p_status == P_DIRTY_RIGHT) && (*p1 == *e2.p2))
            || ((p_status == P_DIRTY_LEFT)  && (*p1 == *e2.p1)) )
          {
            val1_2 = removeEdgeFromSetb(e2, lowest_index, edgeS, polygon, points);
            if (val1_2 == E_NOT_VALID) break;
          }
          loop=true;
          if (p_status == P_CLEAN &&  revert) p_status = P_DIRTY_LEFT;
          if (p_status == P_CLEAN && !revert) p_status = P_DIRTY_RIGHT;
          revert=true;
          continue;
        }
        // to change p_status to P_CLEAN, I need to check both val1.first and val2.first, see below.
      }
      else {
        // if I am about to process 'e1' I can start by clearing it of any collinearity with 'e2'
        if (fabs(val3) == 0) {
          // the 2 edges are collinear
          (debug) ? std::cerr << "collinear check found a possible match."  << std::endl : std::cerr;
          if (((*p2 < *p1) && (*p3 < *p1)) || ((*p1 < *p2) && (*p1 < *p3))) {
          //(debug) ? std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
            std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl;
            if (coll3Swap(p1, p2, p3, edgeS, polygon, points, lowest_index)) {
              //(debug) ? std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
              std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl;
              loop = true;
              if (p_status == P_CLEAN &&  revert) p_status = P_DIRTY_LEFT;
              if (p_status == P_CLEAN && !revert) p_status = P_DIRTY_RIGHT;
              revert=true;
              continue;
            }
          } //else std::cerr << "no collinearity found." << std::endl;
        }

        (debug) ? std::cerr << std::endl << "processing e1: " << e1 << std::endl : std::cerr;
        val1 = processEdgeb(e1, lowest_index, edgeS, polygon, points);
        if (val1.first == E_NOT_VALID) break;
        if (val1.first == E_SKIP) {
          loop=true;
          if (p_status == P_CLEAN &&  revert) p_status = P_DIRTY_LEFT;
          if (p_status == P_CLEAN && !revert) p_status = P_DIRTY_RIGHT;
          revert=true;
          continue;
        }
      }

      // process second edge
      //if ((revert && (*e2.p1 == *p1)) || (!revert && (*e2.p2 == *p1))) {
      if ( ((p_status == P_CLEAN) && !(revert ^ (*p1 == *e2.p1)))
        || ((p_status == P_DIRTY_RIGHT) && (*p1 == *e2.p2))
        || ((p_status == P_DIRTY_LEFT)  && (*p1 == *e2.p1)) )
      {
        (debug) ? std::cerr << std::endl << "removing e2: " << e2 << std::endl : std::cerr;
        val2.first = removeEdgeFromSetb(e2, lowest_index, edgeS, polygon, points);
        if (val2.first == E_NOT_VALID) break;
        if (val2.first == E_SKIP) {
          loop=true;
          if (p_status == P_CLEAN &&  revert) p_status = P_DIRTY_LEFT;
          if (p_status == P_CLEAN && !revert) p_status = P_DIRTY_RIGHT;
          revert=true;
          continue;
        }
      }
      else {
        (debug) ? std::cerr << std::endl << "processing e2: " << e2 << std::endl : std::cerr;
        val2 = processEdgeb(e2, lowest_index, edgeS, polygon, points);
        if (val2.first == E_NOT_VALID) break;
        if (val2.first == E_SKIP) {
          val2_1 = removeEdgeFromSetb(e1, lowest_index, edgeS, polygon, points);
          if (val2_1 == E_NOT_VALID) break;
          loop=true;
          if (p_status == P_CLEAN &&  revert) p_status = P_DIRTY_LEFT;
          if (p_status == P_CLEAN && !revert) p_status = P_DIRTY_RIGHT;
          revert=true;
          continue;
        }
      }

      if ((val1.first == E_VALID) && (val2.first == E_VALID)) p_status = P_CLEAN;

      if ((lowest_index == index) && (p_status == P_CLEAN)) {
        revert = false;
        lowest_index = polygon.size();
        continue;
      }
  		if (revert) --index;
      else ++index;
//      std::cerr << "revert: " << ((revert) ? "true" : "false") << std::endl;
//      std::cerr << "lowest_index: " << lowest_index << std::endl;
//      if (loop == true) std::cerr << "i: " << index << ", l_i: " << lowest_index << std::endl;
  	}
    if ((val1.first == E_NOT_VALID) || (val2.first == E_NOT_VALID)) {retval=UNEXPECTED_ERROR; break;}
  } while (loop);

	return retval;
}
*/

enum error opt2b(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  enum error retval = SUCCESS;
  enum planesweep_t p_status = P_CLEAN;
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
	unsigned int index, before, after, lowest_index, highest_index;
  std::pair<enum edge_t, std::set<Edge>::iterator> val1, val2;
  enum edge_t val1_2, val2_1;
  double val3;
	Point *p1, *p2, *p3;
	Edge e1, e2, old_e1, old_e2;
  bool loop, revert;
  bool debug=false;
  std::set<Edge> edgeS; // a sweep-line-status object.

  do {
//    (debug) ? std::cerr << "looping" << std::endl : std::cerr;
    std::cerr << "looping" << std::endl;
    loop = false;
    revert = false;
    index = 0;
    lowest_index = polygon.size();
    highest_index = polygon.size();
    decrementEdges(index, edgeS);

  	while (index < points.size()) {

      if (index == 438 || index == 678) {
//      if (debug) {
        debug = true;
        std::cout << std::endl << "edges in 'edgeS':" << std::endl;
        for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
      }
      else debug = false;

      std::cerr << "i: " << index << ", l_i: " << lowest_index << ", h_i: " << highest_index << ", loop: " << loop << ", rev: " << ((revert) ? "T" : "F") << ", p_status: ";print_enum(p_status); std::cerr << std::endl;
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
      if (p_status == P_DIRTY_RIGHT) { // intersection or collinearity was found, clean any problems before reversing direction
        if (*p2 < *p3) {
          e1 = Edge (p1, p2, index);
          e2 = Edge (p1, p3, index);
          val3 = det(e1, *p3);
        }
        else {
          e1 = Edge (p1, p3, index);
          e2 = Edge (p1, p2, index);
          val3 = det(e1, *p2);
        }
      }
      else if (p_status == P_DIRTY_LEFT) { // intersection or collinearity was found, clean any problems before reversing direction
        if (*p3 < *p2) {
          e1 = Edge (p1, p2, index);
          e2 = Edge (p1, p3, index);
          val3 = det(e1, *p3);
        }
        else {
          e1 = Edge (p1, p3, index);
          e2 = Edge (p1, p2, index);
          val3 = det(e1, *p2);
        }
      }
      else if ((!revert && (*p2 < *p3)) || (revert && (*p3 < *p2))) {  // make sure the earlier edge gets processed first.
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
      //if ((revert && (*e1.p1 == *p1)) || !revert && (*e1.p2 == *p1)) {
      //if (!(revert ^ (*p1 == *e1.p1)))
      if ( ((p_status == P_CLEAN) && !(revert ^ (*p1 == *e1.p1)))
        || ((p_status == P_DIRTY_RIGHT) && (*p1 == *e1.p2))
        || ((p_status == P_DIRTY_LEFT)  && (*p1 == *e1.p1)) )
      {
        (debug) ? std::cerr << "removing e1: " << e1 << std::endl : std::cerr;
        val1.first = removeEdgeFromSetb(e1, lowest_index, highest_index, edgeS, polygon, points);
        if (val1.first == E_NOT_VALID) break;
        if ((val1.first == E_INTERSECTION) || (val1.first == E_COLLINEAR)) { // intersection found in the removal, skip the rest and restart.
          // before restarting, make sure e2 wasn't supposed to be removed as well, if so, remove it.
          //if ((revert && (*e2.p1 == *p1)) || (!revert && (*e2.p2 == *p1)))
          //if (!(revert ^ (*p1 == *e2.p1)))
          if ( ((p_status == P_CLEAN) && !(revert ^ (*p1 == *e2.p1)))
            || ((p_status == P_DIRTY_RIGHT) && (*p1 == *e2.p2))
            || ((p_status == P_DIRTY_LEFT)  && (*p1 == *e2.p1)) )
          {
            val1_2 = removeEdgeFromSetb(e2, lowest_index, highest_index, edgeS, polygon, points);
            if (val1_2 == E_NOT_VALID) break;
          }
          if (val1.first == E_COLLINEAR) {
            if (p_status == P_CLEAN &&  revert) p_status = P_DIRTY_LEFT;
            if (p_status == P_CLEAN && !revert) p_status = P_DIRTY_RIGHT;
          }
          loop=true;
          revert=true;
          continue;
        }
      }
      else {
        // if I am about to process 'e1' I can start by clearing it of any collinearity with 'e2'
        if (fabs(val3) == 0) {
          // the 2 edges are collinear
          (debug) ? std::cerr << "collinear check found a possible match."  << std::endl : std::cerr;
          if (((*p2 < *p1) && (*p3 < *p1)) || ((*p1 < *p2) && (*p1 < *p3))) {
            (debug) ? std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
            //std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl;
            if (coll3Swap(p1, p2, p3, edgeS, polygon, points, lowest_index, highest_index)) {
              (debug) ? std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
              //std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl;
              loop = true;
              if (p_status == P_CLEAN &&  revert) p_status = P_DIRTY_LEFT;
              if (p_status == P_CLEAN && !revert) p_status = P_DIRTY_RIGHT;
              revert=true;
              continue;
            }
          } //else std::cerr << "no collinearity found." << std::endl;
        }

        (debug) ? std::cerr << "processing e1: " << e1 << std::endl : std::cerr;
        val1 = processEdgeb(e1, lowest_index, highest_index, edgeS, polygon, points);
        if (val1.first == E_NOT_VALID) break;
        if (val1.first == E_COLLINEAR) {
          loop=true;
          if (p_status == P_CLEAN &&  revert) p_status = P_DIRTY_LEFT;
          if (p_status == P_CLEAN && !revert) p_status = P_DIRTY_RIGHT;
          revert=true;
          continue;
        }
        if (val1.first == E_INTERSECTION) {
          loop=true;
          revert=true;
          continue;
        }
      }

      // process second edge
      //if ((revert && (*e2.p1 == *p1)) || (!revert && (*e2.p2 == *p1))) {
      if ( ((p_status == P_CLEAN) && !(revert ^ (*p1 == *e2.p1)))
        || ((p_status == P_DIRTY_RIGHT) && (*p1 == *e2.p2))
        || ((p_status == P_DIRTY_LEFT)  && (*p1 == *e2.p1)) )
      {
        (debug) ? std::cerr << "removing e2: " << e2 << std::endl : std::cerr;
        val2.first = removeEdgeFromSetb(e2, lowest_index, highest_index, edgeS, polygon, points);
        if (val2.first == E_NOT_VALID) break;
        if (val2.first == E_COLLINEAR) {
          loop=true;
          if (p_status == P_CLEAN &&  revert) p_status = P_DIRTY_LEFT;
          if (p_status == P_CLEAN && !revert) p_status = P_DIRTY_RIGHT;
          revert=true;
          continue;
        }
        if (val2.first == E_INTERSECTION) {
          loop=true;
          revert=true;
          continue;
        }
      }
      else {
        (debug) ? std::cerr << "processing e2: " << e2 << std::endl : std::cerr;
        val2 = processEdgeb(e2, lowest_index, highest_index, edgeS, polygon, points);
        if (val2.first == E_NOT_VALID) break;
        if (val2.first == E_INTERSECTION) {
          // if e1 was inserted "in front of" the index, it needs to be removed.
          //if ( ((p_status == P_CLEAN) && !(revert ^ (*p1 == *e1.p1)))
          //  || ((p_status == P_DIRTY_RIGHT) && (*p1 == *e1.p1))
          //  || ((p_status == P_DIRTY_LEFT)  && (*p1 == *e1.p2)) )
          //{
          //std::cerr << "removing e1: " << e1 << std::endl;
          val2_1 = removeEdgeFromSetb(e1, lowest_index, highest_index, edgeS, polygon, points);
          if (val2_1 == E_NOT_VALID) break;
          //}
          loop=true;
          revert=true;
          continue;
        }
        // the index can repeat in case of collinearity
        if (val2.first == E_COLLINEAR) {
          if (p_status == P_CLEAN &&  revert) p_status = P_DIRTY_LEFT;
          if (p_status == P_CLEAN && !revert) p_status = P_DIRTY_RIGHT;
          loop = true;
          revert = true;
          continue;
        }
      }
      // all cases where either val1 or val2 != E_VALID should be handled in somekind of a 'continue' in above code.

      //if ((val1.first == E_VALID) && (val2.first == E_VALID)) p_status = P_CLEAN;
      if ((lowest_index == index) && ((p_status == P_DIRTY_LEFT) || (p_status == P_CLEAN))) {
        revert = false;
        lowest_index = polygon.size();
        p_status = P_CLEAN;
        continue;
      }
      if ((highest_index == index) && (p_status == P_DIRTY_RIGHT)) {
        revert = true;
        highest_index = polygon.size();
        p_status = P_CLEAN;
        continue;
      }
      if (p_status == P_DIRTY_RIGHT) ++index;
      else if (p_status == P_DIRTY_LEFT) --index;
  		else if (revert) --index;
      else ++index;
//      std::cerr << "revert: " << ((revert) ? "true" : "false") << std::endl;
//      std::cerr << "lowest_index: " << lowest_index << std::endl;
//      if (loop == true) std::cerr << "i: " << index << ", l_i: " << lowest_index << std::endl;
  	}
    if ((val1.first == E_NOT_VALID) || (val2.first == E_NOT_VALID)) {retval=UNEXPECTED_ERROR; break;}
  } while (loop);

	return retval;
}
