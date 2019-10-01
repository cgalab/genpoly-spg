#include <iostream> // for endl
#include <vector>
#include <list>
#include <set>
#include <utility> // for std::pair
#include <cmath> // for fabs
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

// 2opt that was used for the competition.
enum error opt2(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  enum error retval = SUCCESS;
  double duration = 0;
	// initialise and create a random permutation for the polygon
	createRandPol(polygon, points, randseed);
  //createCHRandPol(polygon, points, randseed);
  //pdisplay(polygon, points);
  //assert(0);

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
  enum edge_t val1_2, val2_1;
  double val3;
	Point *p1, *p2, *p3;
	Edge e1, e2, old_e1, old_e2;
  bool loop = false;
  //bool debug=false;
  std::set<Edge> edgeS; // a set of edges.
  //double circumference;
  std::map<double, unsigned int> circ, c_counter;
  std::map<double, unsigned int>::iterator c_it;

  duration = elapsed();
  do {
//    (debug) ? std::cerr << "looping" << std::endl : std::cerr;
    //circumference = pol_calc_circumference(polygon, points);
    //c_it = circ.find(circumference);
    //std::cerr << "c: " << circumference << ", circ[c]: " << circ[circumference] << std::endl;
    //if (c_it != circ.end()) {
//      std::cerr << "c: " << circumference << ", circ[c]: " << circ[circumference] << std::endl;
      //if ((*c_it).second == MAX_NO_OF_LOOPS) {std::cerr<<"Error!  Infinite loop!"<<std::endl;retval=INFINITE_LOOP; break;}
      //circ[circumference] = (*c_it).second +1;
    //}
    //else {
      //circ[circumference] = 1;
    //}

    loop = false;
    index = 0;
    decrementEdges(edgeS);

  	while (index < points.size()) {
      //std::cerr << "i: " << index << std::endl;
      //if (index > 205 && index < 207) {
        //std::cerr << std::endl << "i: " << index << std::endl;
        //debug = true;
        //std::cerr << "edges in 'edgeS':" << std::endl;
        //for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
      //}
      //else {debug = false;}

      //std::cerr << "polygon:" << std::endl;
      //pdisplay(polygon, points);
  		val1.first = E_VALID; val2.first = E_VALID;
  		// get the current point at 'index'
  		p1 = &points[lex[index]];

  		// get the 2 points it is connected to in 'polygon', treating the edge case when the point 'p1' is on the ends
  		before = ((*p1).v + points.size() -1) % points.size();
  		after =  ((*p1).v + points.size() +1) % points.size();

  		p2 = &points[polygon[before]];
  		p3 = &points[polygon[after]];

      // construct the edges
//      (debug) ? std::cerr << "p1: " << *p1 << ", p2: "<< *p2 << " < p3: " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl : std::cerr;
//      std::cerr << "p1: " << *p1 << ", p2: "<< *p2 << " < p3: " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl;
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

      if (fabs(val3 == 0)) {
        if (((*e1.p1 == *p1) && (*e2.p1 == *p1)) || ((*e1.p2 == *p1) && (*e2.p2 == *p1))) { // no idea why but this is very important for solving collinearities
//          (debug) ? std::cerr << "Collinearity: before swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
          if (coll3SwapOld(p1, p2, p3, edgeS, polygon, points)) {
//            (debug) ? std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
            loop = true;
            continue;
          }
        }
      }

//      (debug) ? std::cerr << "*e1.p2: " << *e1.p2 << ", *p1: " << *p1 << ", same: " << ((*e1.p2 == *p1) ? "true" : "false") << std::endl : std::cerr;

      //process first edge
      if (*e1.p2 == *p1) {
//        (debug) ? std::cerr << "removing e1: " << e1 << std::endl : std::cerr;
        val1.first = removeEdgeFromSetOld(e1, edgeS, polygon, points);
//        (debug) ? std::cerr << "val1.first: " : std::cerr; if (debug) print_enum(val1.first);
        if (val1.first == E_NOT_VALID) break;
        if ((val1.first == E_INTERSECTION) || (val1.first == E_COLLINEAR)) {
          // before restarting, make sure e2 wasn't supposed to be removed as well, if so, remove it.
          if (*e2.p2 == *p1) {
            val1_2 = removeEdgeFromSetOld(e2, edgeS, polygon, points);
//            (debug) ? std::cerr << "val1_2: " : std::cerr; if (debug) print_enum(val1_2);
            if (val1_2 == E_NOT_VALID) break;
          }
          loop=true;continue;
        }
      }
      else {
        // Only if the first edge has to be added do we have to check for collinearity, as then both edges have to be added.

//        (debug) ? std::cerr << "processing e1: " << e1 << std::endl : std::cerr;
        val1 = processEdgeOld(e1, edgeS, polygon, points);
//        (debug) ? std::cerr << "val1.first: " : std::cerr; if (debug) print_enum(val1.first);
        if (val1.first == E_NOT_VALID) break;
        if ((val1.first == E_INTERSECTION) || (val1.first == E_COLLINEAR)) {loop=true;continue;}
      }

      // process second edge
      if (*e2.p2 == *p1) {
//        (debug) ? std::cerr << "removing e2: " << e2 << std::endl : std::cerr;
        val2.first = removeEdgeFromSetOld(e2, edgeS, polygon, points);
//        (debug) ? std::cerr << "val2.first: " : std::cerr; if (debug) print_enum(val2.first);
        if (val2.first == E_NOT_VALID) break;
        if ((val2.first == E_INTERSECTION) || (val2.first == E_COLLINEAR)) {loop=true;continue;} // if this happens, e1 was guaranteed removed as e1 < e2 and e2.p2 > e2.p1 > e1.p1
      }
      else {
//        (debug) ? std::cerr << "processing e2: " << e2 << std::endl : std::cerr;
        val2 = processEdgeOld(e2, edgeS, polygon, points);
//        (debug) ? std::cerr << "val2.first: " : std::cerr; if (debug) print_enum(val2.first);
        if (val2.first == E_NOT_VALID) break;
        if ((val2.first == E_INTERSECTION) || (val2.first == E_COLLINEAR)) {
          val2_1 = removeEdgeFromSetOld(e1, edgeS, polygon, points);
//          (debug) ? std::cerr << "val2_1: " : std::cerr; if (debug) print_enum(val2_1);
          if (val2_1 == E_NOT_VALID) break;
          loop=true;continue;
        }
      }

  		++index;
  	}
    if ((val1.first == E_NOT_VALID) || (val2.first == E_NOT_VALID)) {retval=UNEXPECTED_ERROR; break;}
  } while (loop);
  duration = elapsed();
  std::cout << "Time elapsed: " << duration << std::endl;

	return retval;
}
