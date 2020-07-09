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

// 2 opt that doesn't restart in the same point if a intersection or a collinearity is found, but runs through the whole thing then repeats.
enum error opt2e(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum error retval = SUCCESS;
  double duration = 0;
	// initialise and create a random permutation for the polygon
	createRandPol(polygon, points);

	// the point set 'points' now has x/y coordinates as well as
	// original input index of points in 'i' and polygon index in 'v'
	// Now it can be sorted lexicographically
	std::vector<unsigned int> lex (points.size());
	fill_lex(lex, points); // fill 'lex' with the indices

	// Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index=0, before, after;
  std::pair<enum edge_t, std::set<Edge2>::iterator> val1, val2;
  enum edge_t val1_2, val2_1;
  double val3;
	Point *p1, *p2, *p3;
	Edge2 e1, e2, old_e1, old_e2;
  bool loop = false, e1_found;
//  bool debug=false;
  unsigned int count_intersections=0, count_coll=0, count_total_passes=0;
  std::set<Edge2> edgeS; // a set of edges.
  double circumference;
  std::map<double, unsigned int> circ, c_counter;
  std::map<double, unsigned int>::iterator c_it;

  duration = elapsed();
  do {
    ++count_total_passes;
//    (debug) ? std::cerr << "looping" << std::endl : std::cerr;
    circumference = pol_calc_perimeter(polygon, points);
    c_it = circ.find(circumference);
//    std::cerr << "c: " << circumference << std::endl;
    if (c_it != circ.end()) {
//      std::cerr << "circ[c]: " << circ[circumference] << std::endl;
      if ((*c_it).second == MAX_NO_OF_LOOPS) {std::cerr<<"Error!  Infinite loop!"<<std::endl;retval=INFINITE_LOOP; break;}
      circ[circumference] = (*c_it).second +1;
    }
    else {
      circ[circumference] = 1;
    }

    loop = false;
    index = 0;
    edgeS.clear();

  	while (index < points.size()) {
//      std::cerr << "i: " << index << std::endl;
//      if (966 < index && index < 975) {
//        std::cerr << "i: " << index << std::endl;
//        debug = true;
//        std::cerr << "edges in 'edgeS':" << std::endl;
//        for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
//      }
//      else {debug = false;}

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
        e1 = Edge2 (p1, p2);
        e2 = Edge2 (p1, p3);
        val3 = e1.cdet(*p3);
      }
      else {
        e1 = Edge2 (p1, p3);
        e2 = Edge2 (p1, p2);
        val3 = e1.cdet(*p2);
      }

//      (debug) ? std::cerr << "*e1.p2: " << *e1.p2 << ", *p1: " << *p1 << ", same: " << ((*e1.p2 == *p1) ? "true" : "false") << std::endl : std::cerr;

      // to catch collinearities when '>o' you need to check det before removing the first edge.
      if (fabs(val3 == 0)) {
        if (((*e1.p1 == *p1) && (*e2.p1 == *p1)) || ((*e1.p2 == *p1) && (*e2.p2 == *p1))) {
      //            (debug) ? std::cerr << "Collinearity: before swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
          if (coll3Sort5(p1, p2, p3, edgeS, polygon, points)) {
            ++count_coll;
      //            (debug) ? std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
            loop = true;
            continue;
          }
        }
      }

      //process first edge
      e1_found=false;
      if (*e1.p2 == *p1) {
//        (debug) ? std::cerr << "removing e1: " << e1 << std::endl : std::cerr;
        val1.first = removeEdgeFromSet(e1, edgeS, polygon, points);
//        if (debug) {std::cerr << "val1: "; print_enum(val1.first);}
        if (val1.first == E_NOT_VALID) break;
        if ((val1.first == E_INTERSECTION) || (val1.first == E_COLLINEAR)) {
          if (val1.first == E_INTERSECTION) ++count_intersections;
          else ++count_coll;
          // before restarting, make sure e2 wasn't supposed to be removed as well, if so, remove it.
          if (*e2.p2 == *p1) {
            val1_2 = removeEdgeFromSet(e2, edgeS, polygon, points);
//            if (debug) {std::cerr << "val1_2: "; print_enum(val1_2);}
            if (val1_2 == E_NOT_VALID) break;
          }
          loop=true;
          e1_found=true;
        }
        else if (val1.first != E_VALID) {std::cerr << "val1: "; print_enum(val1.first);loop=true;}
      }
      else {

//        (debug) ? std::cerr << "processing e1: " << e1 << std::endl : std::cerr;
        val1 = processEdge(e1, edgeS, polygon, points);
//        if (debug) {std::cerr << "val1: "; print_enum(val1.first);}
        if (val1.first == E_NOT_VALID) break;
        if ((val1.first == E_INTERSECTION) || (val1.first == E_COLLINEAR)) {
          if (val1.first == E_INTERSECTION) ++count_intersections;
          else ++count_coll;
          loop=true;e1_found=true;
        }
        else if (val1.first != E_VALID) {std::cerr << "val1: "; print_enum(val1.first);loop=true;}
      }

      if(!e1_found) {
        // process second edge
        if (*e2.p2 == *p1) {
//          (debug) ? std::cerr << "removing e2: " << e2 << std::endl : std::cerr;
          val2.first = removeEdgeFromSet(e2, edgeS, polygon, points);
//          if (debug) {std::cerr << "val2: "; print_enum(val2.first);}
          if (val2.first == E_NOT_VALID) break;
          if ((val2.first == E_INTERSECTION) || (val2.first == E_COLLINEAR)) {
            if (val2.first == E_INTERSECTION) ++count_intersections;
            else ++count_coll;
            loop=true;
          } // if this happens, e1 was guaranteed removed as e1 < e2 and e2.p2 > e2.p1 > e1.p1
          else if (val2.first != E_VALID) {std::cerr << "val2: "; print_enum(val2.first);loop=true;}
        }
        else {
//          (debug) ? std::cerr << "processing e2: " << e2 << std::endl : std::cerr;
          val2 = processEdge(e2, edgeS, polygon, points);
//          if (debug) {std::cerr << "val2: "; print_enum(val2.first);}
          if (val2.first == E_NOT_VALID) break;
          if ((val2.first == E_INTERSECTION) || (val2.first == E_COLLINEAR)) {
            if (val2.first == E_INTERSECTION) ++count_intersections;
            else ++count_coll;
//            (debug) ? std::cerr << "need to remove e1" << std::endl : std::cerr;
            val2_1 = removeEdgeFromSet(e1, edgeS, polygon, points);
//            if (debug) {std::cerr << "val2_1: "; print_enum(val2_1);}
            if (val2_1 == E_NOT_VALID) break;
            loop=true;
          }
          else if (val2.first != E_VALID) {std::cerr << "val2: "; print_enum(val2.first);loop=true;}
        }
      }

  		++index;
  	}
    if ((val1.first == E_NOT_VALID) || (val2.first == E_NOT_VALID)) {retval=UNEXPECTED_ERROR; break;}
  } while (loop);
  duration = elapsed();
  std::cout << "Time elapsed: " << duration << std::endl;
  std::cout << "Total passes: " << count_total_passes << std::endl;
  std::cout << "Intersections: " << count_intersections << std::endl;
  std::cout << "Collinearities: " << count_coll << std::endl;

	return retval;
}
