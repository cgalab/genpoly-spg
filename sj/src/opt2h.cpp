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
enum error opt2h(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum error retval = SUCCESS;
  double duration = 0;
	// initialise and create a random permutation for the polygon
	createRandPol(polygon, points);

	// the point set 'points' now has x/y coordinates as well as
	// original input index of points in 'i' and polygon index in 'v'
	// Now it can be sorted lexicographically
	std::vector<unsigned int> lex (points.size());
	fill_lex(lex, points); // fill 'lex' with the indices

  // A test to see if saving 'dirty' lex indices and randomly selecting from that
  // is faster than a purely random selection of an index.
//  std::set<unsigned int> dirty;
//  std::set<unsigned int>::iterator ret;
//  ret = dirty.begin();
//  for (unsigned int i = 0; i < polygon.size(); ++i) ret = dirty.insert(ret, i);

	// Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index, before, after, lowest_index;
  enum edge_t val1=E_VALID, val2=E_VALID;
  enum edge_t val1_2=E_VALID, val2_1=E_VALID;
  double val3;
	Point *p1, *p2, *p3;
	Edge2 e1, e2;
  bool loop, revert, reverse_flag;
//  bool debug=true;
  unsigned int count_intersections=0, count_coll=0, count_reversals=0, count_total_passes=0;
  std::set<Edge2> edgeS; // a set of edges.
//  double circumference;
//  std::map<double, unsigned int> circ, c_counter;
//  std::map<double, unsigned int>::iterator c_it;
//  std::cerr << "polygon:" << std::endl;
//  pdisplay(polygon, points);

  duration = elapsed();
  do {
    ++count_total_passes;
//    (debug) ? std::cerr << "looping" << std::endl : std::cerr;
//    circumference = pol_calc_circumference(polygon, points);
//    c_it = circ.find(circumference);
//    std::cerr << "c: " << circumference << std::endl;
//    if (c_it != circ.end()) {
//      std::cerr << "circ[c]: " << circ[circumference] << std::endl;
//      if ((*c_it).second == MAX_NO_OF_LOOPS) {std::cerr<<"Error!  Infinite loop!"<<std::endl;retval=INFINITE_LOOP; break;}
//      circ[circumference] = (*c_it).second +1;
//    }
//    else {
//      circ[circumference] = 1;
//    }

    loop = false;
    reverse_flag = false;
    revert = false;
    lowest_index = points.size();
    index = 0;
    edgeS.clear();

  	while (index < points.size()) {
//      std::cerr << "i: " << index << std::endl;
//      if (966 < index && index < 975) {
//        std::cerr << "i: " << index << std::endl;
//        debug = true;
//        std::cerr << "edges in 'edgeS':" << std::endl;
//        for (std::set<Edge2>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
//      }
//      else {debug = false;}

  		val1 = E_VALID;
      val2 = E_VALID;
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
      if ((!revert && (*p2 < *p3)) || (revert && (*p3 < *p2))) {  // make sure the earlier edge gets processed first.
        e1 = Edge2 (p1, p2);
        e2 = Edge2 (p1, p3);
        val3 = e1.cdet(*p3);
      }
      else {
        e1 = Edge2 (p1, p3);
        e2 = Edge2 (p1, p2);
        val3 = e1.cdet(*p2);
      }

      // clear the edges of any collinearity.  if they are collinear, and '>o' or 'o<', they need to be processed
      // having this check after processing the first edge means '>o' would never be caught.
      if (fabs(val3) == 0) {
        // the 2 edges are collinear
        if (((*p2 < *p1) && (*p3 < *p1)) || ((*p1 < *p2) && (*p1 < *p3))) {
//            (debug) ? std::cerr << "before sort: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;

          if (coll3Sort4(p1, p2, p3, polygon, points, lowest_index)) {
            ++count_coll;
//              (debug) ? std::cerr << "after 3P collsort: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
            loop = true;
            reverse_flag = true;
            continue;
          }
        } //else std::cerr << "no collinearity found." << std::endl;
      }

      //process first edge
      if (!(revert ^ (*p1 == *e1.p1))) {
//        (debug) ? std::cerr << "removing e1: " << e1 << std::endl : std::cerr;
        val1 = removeEdgeFromSeth(e1, lowest_index, edgeS, polygon, points);
//        if (debug) {std::cerr << "val1: "; print_enum(val1);}
        if (val1 == E_NOT_VALID) break;
        if (val1 == E_INTERSECTION) { // intersection found in the removal, skip the rest and restart.
          ++count_intersections;
          // before restarting, make sure e2 wasn't supposed to be removed as well, if so, remove it.
          if (!(revert ^ (*p1 == *e2.p1))) {
            val1_2 = removeEdgeFromSeth(e2, lowest_index, edgeS, polygon, points);
//            if (debug) {std::cerr << "val1_2: "; print_enum(val1_2);}
            if (val1_2 == E_NOT_VALID) break; // the other conditions would be handled when handling 'e2' properly.  This error though has priority.
          }
          loop=true;
          reverse_flag=true;
          continue;
        }
        // when a collinearity is found, the current index is restarted, but no reversal.
        if (val1 == E_COLLINEAR) {
          ++count_coll;
          // before restarting, make sure e2 wasn't supposed to be removed as well, if so, remove it.
          if (!(revert ^ (*p1 == *e2.p1))) {
            val1_2 = removeEdgeFromSeth(e2, lowest_index, edgeS, polygon, points);
//            if (debug) {std::cerr << "val1_2: "; print_enum(val1_2);}
            if (val1_2 == E_NOT_VALID) break; // the other conditions would be handled when handling 'e2' properly.  This error though has priority.
          }
          loop=true;
          reverse_flag=true;
          continue;
        }
      }
      else {
//        (debug) ? std::cerr << "processing e1: " << e1 << std::endl : std::cerr;
        val1 = processEdgeh(e1, lowest_index, edgeS, polygon, points);
//        if (debug) {std::cerr << "val1: "; print_enum(val1);}
        if (val1 == E_NOT_VALID) break;
        if (val1 == E_INTERSECTION) {++count_intersections;loop=true;reverse_flag=true;continue;}
        if (val1 == E_COLLINEAR) {++count_coll;loop=true;reverse_flag=true;continue;}
      }

      // process second edge
      if (!(revert ^ (*p1 == *e2.p1))) {
//        (debug) ? std::cerr << "removing e2: " << e2 << std::endl : std::cerr;
        val2 = removeEdgeFromSeth(e2, lowest_index, edgeS, polygon, points);
//        if (debug) {std::cerr << "val2: "; print_enum(val2);}
        if (val2 == E_NOT_VALID) break;
        if (val2 == E_INTERSECTION) {++count_intersections;loop=true;reverse_flag=true;continue;}
        if (val2 == E_COLLINEAR) {++count_coll;loop=true;reverse_flag=true;continue;}
      }
      else {
//        (debug) ? std::cerr << "processing e2: " << e2 << std::endl : std::cerr;
        val2 = processEdgeh(e2, lowest_index, edgeS, polygon, points);
//        if (debug) {std::cerr << "val2: "; print_enum(val2);}
        if (val2 == E_NOT_VALID) break;
        if (val2 == E_INTERSECTION) {
          ++count_intersections;
          // if e1 was inserted "in front of" the index, it needs to be removed.
          if (revert ^ (*p1 == *e1.p1)) {
//            (debug) ? std::cerr << "removing e1: " << e1 << std::endl : std::cerr;
            val2_1 = removeEdgeFromSeth(e1, lowest_index, edgeS, polygon, points);
//            if (debug) {std::cerr << "val2_1: "; print_enum(val2_1);}
            if (val2_1 == E_NOT_VALID) break;
          }
          loop=true;reverse_flag=true;continue;
        }
        if (val2 == E_COLLINEAR) {
          ++count_coll;
          // when a collinearity is found, the current index is restarted, but no reversal.
          // if e1 was inserted "in front of" the index, it needs to be removed.
          if (revert ^ (*p1 == *e1.p1)) {
//            (debug) ? std::cerr << "removing e1: " << e1 << std::endl : std::cerr;
            val2_1 = removeEdgeFromSeth(e1, lowest_index, edgeS, polygon, points);
//            if (debug) {std::cerr << "val2_1: "; print_enum(val2_1);}
            if (val2_1 == E_NOT_VALID) break;
          }
          loop=true;
          reverse_flag=true;
          continue;
        }
      }
      // all cases where either val1 or val2 != E_VALID should be handled in somekind of a 'continue' in above code.

      if (lowest_index == index) {
        lowest_index = points.size();
        // 3P collinearity could set the reverse flag, and lowest_index as current index,
        // but not need to reverse, so it does not need to repeat the current index as it got here without any problems.
        if (revert) {
          reverse_flag = false;
          revert = false;
          continue;
        }
        else reverse_flag = false;
      }
      // the reverse_flag assures that sorting of the points in advancing direction finishes before the index is reversed.
      if (reverse_flag && !revert) {
        ++count_reversals;
        revert = true;
        continue;
      }
  		if (revert) --index;
      else ++index;
//      std::cerr << "revert: " << ((revert) ? "true" : "false") << std::endl;
//      std::cerr << "lowest_index: " << lowest_index << std::endl;
//      if (loop == true) std::cerr << "i: " << index << ", l_i: " << lowest_index << std::endl;
  	}
    if ((val1 == E_NOT_VALID) || (val2 == E_NOT_VALID)) {retval=UNEXPECTED_ERROR; break;}
  } while (loop);
  duration = elapsed();
  std::cout << "Time elapsed: " << duration << std::endl;
  std::cout << "Total passes: " << count_total_passes << std::endl;
  std::cout << "Intersections: " << count_intersections << std::endl;
  std::cout << "Collinearities: " << count_coll << std::endl;
  std::cout << "Reversals: " << count_reversals << std::endl;

	return retval;
}