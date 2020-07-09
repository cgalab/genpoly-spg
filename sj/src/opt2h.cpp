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
#include "rand.h"

// 2opt version that starts in a random index after each time it finds and solves an intersection.
// if it goes cleanly from an index to an end, it updates the interval of random indices to pick.
// if it finds an intersection it updates the lower part of the interval and resets the higher.
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
	unsigned int index, before, after, lowest_index=0, highest_index=points.size()-1, random_index;
  std::pair<enum edge_t, std::set<Edge2>::iterator> val1, val2;
  enum edge_t val1_2=E_VALID, val2_1=E_VALID;
  double val3;
	Point *p1, *p2, *p3;
	Edge2 e1, e2;
  bool loop=true, is_clean=false, revert=false;
//  bool debug=false;
  unsigned int count_intersections=0, count_coll=0, count_total_passes=0;
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

    //reverse_flag = false;
    //index = 0;
    if (loop) {
      loop = false;
    }
    if (highest_index - lowest_index > 2) {
      UniformRandomI(random_index, lowest_index+1, highest_index-1);
      UniformRandomI(revert, 0, 1);
    }
    else {
      if (points.size()-1 - highest_index < lowest_index) revert = true;
      else revert = false;
      if (revert) {
        random_index = points.size()-1;
      }
      else {random_index = 0;}
      lowest_index = 0;
      highest_index = points.size()-1;
    }
    index = random_index;
    edgeS.clear();
//    std::cerr << "new i: " << index << ", loop: " << loop << ", is_clean: " << is_clean << ", revert: " << revert << std::endl;
//    std::cerr << "lowest_index: " << lowest_index << ", highest_index: " << highest_index << std::endl;
  	while (index < points.size()) {
//      if (966 < index && index < 975) {
//        std::cerr << "i: " << index << std::endl;
//        debug = true;
//        std::cerr << "edges in 'edgeS':" << std::endl;
//        for (std::set<Edge2>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
//      }
//      else {debug = false;}

      //if (index == 0 && edgeS.size() > 0) {
      //  std::cerr << "at index 0 edgeS was not 0" << std::endl;
      //  edgeS.clear();
      //}

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

          if (coll3Sort5(p1, p2, p3, edgeS, polygon, points, lowest_index, highest_index)) {
            ++count_coll;
//              (debug) ? std::cerr << "after 3P collsort: e1: " << e1 << ", e2: " << e2 << std::endl : std::cerr;
            loop = true;
            break;
          }
        } //else std::cerr << "no collinearity found." << std::endl;
      }

      //process first edge
      if (!(revert ^ (*p1 == *e1.p1))) {
//        (debug) ? std::cerr << "removing e1: " << e1 << std::endl : std::cerr;
        val1.first = removeEdgeFromSet(e1, edgeS, polygon, points, lowest_index, highest_index);
//        if (debug) {std::cerr << "val1: "; print_enum(val1.first);}
        if (val1.first == E_NOT_VALID) break;
        if (val1.first == E_INTERSECTION) { // intersection found in the removal, skip the rest and restart.
          ++count_intersections;
          // before restarting, make sure e2 wasn't supposed to be removed as well, if so, remove it.
          if (!(revert ^ (*p1 == *e2.p1))) {
            val1_2 = removeEdgeFromSet(e2, edgeS, polygon, points, lowest_index, highest_index);
//            if (debug) {std::cerr << "val1_2: "; print_enum(val1_2);}
            if (val1_2 == E_NOT_VALID) break; // the other conditions would be handled when handling 'e2' properly.  This error though has priority.
          }
          loop=true;
          break;
        }
        // when a collinearity is found, the current index is restarted, but no reversal.
        if (val1.first == E_COLLINEAR) {
          ++count_coll;
          // before restarting, make sure e2 wasn't supposed to be removed as well, if so, remove it.
          if (!(revert ^ (*p1 == *e2.p1))) {
            val1_2 = removeEdgeFromSet(e2, edgeS, polygon, points, lowest_index, highest_index);
//            if (debug) {std::cerr << "val1_2: "; print_enum(val1_2);}
            if (val1_2 == E_NOT_VALID) break; // the other conditions would be handled when handling 'e2' properly.  This error though has priority.
          }
          loop=true;
          break;
        }
      }
      else {
//        (debug) ? std::cerr << "processing e1: " << e1 << std::endl : std::cerr;
        val1 = processEdge(e1, edgeS, polygon, points, lowest_index, highest_index);
//        if (debug) {std::cerr << "val1: "; print_enum(val1.first);}
        if (val1.first == E_NOT_VALID) break;
        if (val1.first == E_INTERSECTION) {
          ++count_intersections;
          loop=true;
          break;
        }
        if (val1.first == E_COLLINEAR) {
          ++count_coll;
          loop=true;
          break;
        }
      }

      // process second edge
      if (!(revert ^ (*p1 == *e2.p1))) {
//        (debug) ? std::cerr << "removing e2: " << e2 << std::endl : std::cerr;
        val2.first = removeEdgeFromSet(e2, edgeS, polygon, points, lowest_index, highest_index);
//        if (debug) {std::cerr << "val2: "; print_enum(val2.first);}
        if (val2.first == E_NOT_VALID) break;
        if (val2.first == E_INTERSECTION) {
          ++count_intersections;
          loop=true;
          break;
        }
        if (val2.first == E_COLLINEAR) {
          ++count_coll;
          loop=true;
          break;
        }
      }
      else {
//        (debug) ? std::cerr << "processing e2: " << e2 << std::endl : std::cerr;
        val2 = processEdge(e2, edgeS, polygon, points, lowest_index, highest_index);
//        if (debug) {std::cerr << "val2: "; print_enum(val2.first);}
        if (val2.first == E_NOT_VALID) break;
        if (val2.first == E_INTERSECTION) {
          ++count_intersections;
          // if e1 was inserted "in front of" the index, it needs to be removed.
          if (revert ^ (*p1 == *e1.p1)) {
//            (debug) ? std::cerr << "removing e1: " << e1 << std::endl : std::cerr;
            val2_1 = removeEdgeFromSet(e1, edgeS, polygon, points, lowest_index, highest_index);
//            if (debug) {std::cerr << "val2_1: "; print_enum(val2_1);}
            if (val2_1 == E_NOT_VALID) break;
          }
          loop=true;
          break;
        }
        if (val2.first == E_COLLINEAR) {
          ++count_coll;
          // when a collinearity is found, the current index is restarted, but no reversal.
          // if e1 was inserted "in front of" the index, it needs to be removed.
          if (revert ^ (*p1 == *e1.p1)) {
//            (debug) ? std::cerr << "removing e1: " << e1 << std::endl : std::cerr;
            val2_1 = removeEdgeFromSet(e1, edgeS, polygon, points, lowest_index, highest_index);
//            if (debug) {std::cerr << "val2_1: "; print_enum(val2_1);}
            if (val2_1 == E_NOT_VALID) break;
          }
          loop=true;
          break;
        }
      }

//      std::cerr << "i: " << index << ", loop: " << ((loop) ? "T" : "F") << ", revert: " << ((revert) ? "T" : "F") << std::endl;
//      std::cerr << "random_index: " << random_index << ", lowest_index: " << lowest_index << ", highest_index: " << highest_index << std::endl;
      //if (loop == true) std::cerr << "i: " << index << ", l_i: " << lowest_index << std::endl;

      if (revert) {
        if (index == lowest_index) {
          // if this was a validation check.
          if (random_index == points.size()-1) is_clean = true;
          // if you got to the lowest index from random index without an intersection, then update lowest_index
          if (lowest_index < random_index) lowest_index = random_index;
//          std::cerr << "new lowest index: " << lowest_index << std::endl;
          break;
        }
        else --index;
      }
      else if (index == highest_index) {
        // if this was a validation check.
        if (random_index == 0) is_clean = true;
        // if you got to the highest index from random index without an intersection, then update highest_index
        if (random_index < highest_index) highest_index = random_index;
//        std::cerr << "new highest index: " << highest_index << std::endl;
        break;
      }
      else ++index;


  	}// end of while loop.
    if (loop) {is_clean = false;}
    if ((val1.first == E_NOT_VALID) || (val2.first == E_NOT_VALID)) {retval=UNEXPECTED_ERROR; break;}
  } while (!is_clean);
  duration = elapsed();
  std::cout << "Time elapsed: " << duration << std::endl;
  std::cout << "Total passes: " << count_total_passes << std::endl;
  std::cout << "Intersections: " << count_intersections << std::endl;
  std::cout << "Collinearities: " << count_coll << std::endl;

	return retval;
}
