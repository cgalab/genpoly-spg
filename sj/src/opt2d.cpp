#include <iostream> // for endl
#include <vector>
#include <list>
#include <set>
#include <utility> // for std::pair
#include <cmath>  // for fabs
#include <algorithm>    // std::sort
#include <iterator> // for std:prev and std::next
#include <assert.h>
#include <map>
#include "basicDefinitions.h"
#include "basicFunctions.h"
#include "point.h"
#include "edge.h"
#include "pol.h"
#include "opt2base.h"
#include "elapsed.h"

// Version of BO-2opt that only flips the edges and not the whole chain. //NOT WORKING.
enum error opt2d(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum error retval = SUCCESS;
  double duration = 0;
	// initialise and create a random permutation for the polygon
	createRandPol(polygon, points);

	// the point set 'points' now has x/y coordinates as well as
	// original input index of points in 'i' and polygon index in 'v'
	// Now it can be sorted lexicographically, each points lex. index is also stored in 'l'
  // 'lex' is an "event-point-schedule" object.
	std::vector<unsigned int> lex (points.size());
	fill_lex(lex, points); // fill 'lex' with the indexes

	// Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index, before, after, lowest_index;
  std::pair<enum edge_t, std::set<Edge>::iterator> val1, val2;
  enum edge_t val1_2, val2_1;
  double val3;
	Point *p1, *p2, *p3;
	Edge e1, e2, old_e1, old_e2;
  bool loop, revert;
//  bool debug = false;
  unsigned int count_intersections=0, count_coll=0, count_reversals=0, count_total_passes=0;
  std::set<Edge> edgeS; // a sweep-line-status object.
  //double circumference;
  std::map<double, unsigned int> circ, c_counter;
  std::map<double, unsigned int>::iterator c_it;

  duration = elapsed();
  do {
    ++count_total_passes;
//    (debug) ? std::cerr << "looping" << std::endl : std::cerr;
    //circumference = pol_calc_circumference(polygon, points);
    //c_it = circ.find(circumference);
    //if (c_it != circ.end()) {
//      std::cerr << "c: " << circumference << ", circ[c]: " << circ[circumference] << std::endl;
      //if ((*c_it).second == 4) {std::cerr<<"Error!  Infinite loop!"<<std::endl;retval=INFINITE_LOOP; break;}
      //circ[circumference] = (*c_it).second +1;
    //}
    //else {
      //circ[circumference] = 1;
    //}
//    (debug) ? std::cerr << "looping" << std::endl : std::cerr;
//    std::cerr << "looping" << std::endl;
    loop = false;
    revert = false;
    index = 0;
    lowest_index = polygon.size();
    decrementEdges(edgeS);

//    std::cerr << "New loop" << std::endl;
  	while (index < points.size()) {
//    if (index > 3437 && index < 3503) {
//      debug = true;
//      std::cerr << std::endl << "edges in 'edgeS':" << std::endl;
//      for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
//    }
//    else debug = false;

//    if (debug) {std::cerr << "i: " << index << ", l_i: " << lowest_index << ", c_i: " << collinear_index << ", loop: " << loop << ", rev: " << ((revert) ? "T" : "F") << ", p_status: ";print_enum(p_status); std::cerr << std::endl;}
  		val1.first = E_VALID; val2.first = E_VALID;
  		// get the current point at 'index'
  		p1 = &points[lex[index]];
  		// get the 2 points it is connected to in 'polygon', treating the edge case when the point 'p1' is on the ends
  		before = ((*p1).v + points.size() -1) % points.size();
  		after =  ((*p1).v + points.size() +1) % points.size();

  		p2 = &points[polygon[before]];
  		p3 = &points[polygon[after]];

      // construct the edges
//    (debug) ? std::cerr << "p1: " << *p1 << ", p2: "<< *p2 << " < p3: " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl : std::cerr;
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
      if (!(revert ^ (*p1 == *e1.p1))) {
//        (debug) ? std::cerr << "removing e1: " << e1 << std::endl : std::cerr;
        val1.first = removeEdgeFromSetd(e1, lowest_index, edgeS, polygon, points);
//        (debug) ? std::cerr << "val1: " << val1.first << std::endl : std::cerr;
        if (val1.first == E_NOT_VALID) break;
        if ((val1.first == E_INTERSECTION) || (val1.first == E_COLLINEAR)) {
          if (val1.first == E_INTERSECTION) ++count_intersections;
          else ++count_coll;
          // before restarting, make sure e2 wasn't supposed to be removed as well, if so, remove it.
          if (!(revert ^ (*p1 == *e2.p1))) {
            val1_2 = removeEdgeFromSetd(e2, lowest_index, edgeS, polygon, points);
//            (debug) ? std::cerr << "val1_2: " << val1_2 << std::endl : std::cerr;
              if (val1_2 == E_NOT_VALID) break; // the other conditions would be handled when handling 'e2' properly.  This error though has priority.
          }
          loop=true;revert=true;++count_reversals;continue;}

      }
      else {
        // if I am about to process 'e1' I can start by clearing it of any collinearity with 'e2'
        if (fabs(val3) == 0) {
          // the 2 edges are collinear
          if (((*p2 < *p1) && (*p3 < *p1)) || ((*p1 < *p2) && (*p1 < *p3))) {
//            (debug) ? std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
//            std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl;
            if (coll3Sort2(p1, p2, p3, p1, edgeS, polygon, points, lowest_index)) {
              ++count_coll;
              update_lowest_index(p1, p2, p3, lowest_index);
//              (debug) ? std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
//              std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl;
              loop = true;
              revert = true;
              ++count_reversals;
              continue;
            }
          } //else std::cerr << "no collinearity found." << std::endl;
        }

//        std::cerr << std::endl << "processing e1: " << e1 << std::endl;
        val1 = processEdged(e1, p1, lowest_index, edgeS, polygon, points);
//        (debug) ? std::cerr << "val1: " << val1.first << std::endl : std::cerr;
        if (val1.first == E_NOT_VALID) break;
        if ((val1.first == E_INTERSECTION) || (val1.first == E_COLLINEAR)) {
          if (val1.first == E_INTERSECTION) ++count_intersections;
          else ++count_coll;
          loop=true;revert=true;continue;}
      }

      // process second edge
      if (!(revert ^ (*p1 == *e2.p1))) {
//        (debug) ? std::cerr << "removing e2: " << e2 << std::endl : std::cerr;
        val2.first = removeEdgeFromSetd(e2, lowest_index, edgeS, polygon, points);
//        (debug) ? std::cerr << "val2: " << val2.first << std::endl : std::cerr;
        if (val2.first == E_NOT_VALID) break;
        if ((val2.first == E_INTERSECTION) || (val2.first == E_COLLINEAR)) {
          if (val2.first == E_INTERSECTION) ++count_intersections;
          else ++count_coll;
          loop=true;revert=true;++count_reversals;continue;}
      }
      else {
//        (debug) ? std::cerr << "processing e2: " << e2 << std::endl : std::cerr;
        val2 = processEdged(e2, p1, lowest_index, edgeS, polygon, points);
//        (debug) ? std::cerr << "val2: " << val2.first << std::endl : std::cerr;
        if (val2.first == E_NOT_VALID) break;
        if ((val2.first == E_INTERSECTION) || (val2.first == E_COLLINEAR)) {
          if (val2.first == E_INTERSECTION) ++count_intersections;
          else ++count_coll;
          // if e1 was inserted "in front of" the index, it needs to be removed.
          if (revert ^ (*p1 == *e1.p1)) {
            //std::cerr << "removing e1: " << e1 << std::endl;
            val2_1 = removeEdgeFromSetd(e1, lowest_index, edgeS, polygon, points);
//          (debug) ? std::cerr << "val2_1: " << val2_1 << std::endl : std::cerr;
            if (val2_1 == E_NOT_VALID) break;
          }
          loop=true;revert=true;++count_reversals;continue;
        }
      }

      if (lowest_index == index) {
        lowest_index = points.size();
        revert = false;
        continue;
      }
  		if (revert) --index;
      else ++index;
//      std::cerr << "revert: " << ((revert) ? "true" : "false") << std::endl;
//      std::cerr << "lowest_index: " << lowest_index << std::endl;
  	}
    if ((val1.first == E_NOT_VALID) || (val2.first == E_NOT_VALID)) {retval=UNEXPECTED_ERROR; break;}
  } while (loop);
  duration = elapsed();
  std::cout << "Time elapsed: " << duration << std::endl;
  std::cout << "Total passes: " << count_total_passes << std::endl;
  std::cout << "Intersections: " << count_intersections << std::endl;
  std::cout << "Collinearities: " << count_coll << std::endl;
  std::cout << "Reversals: " << count_reversals << std::endl;

	return retval;
}
