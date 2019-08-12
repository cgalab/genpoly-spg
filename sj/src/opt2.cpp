#include <iostream> // for endl
#include <vector>
#include <list>
#include <set>
#include <utility> // for std::pair
#include <cmath> // for fabs
#include <algorithm>    // std::sort
#include <iterator> // for std:prev and std::next
#include <assert.h>
#include "basicDefinitions.h"
#include "basicFunctions.h"
#include "point.h"
#include "edge.h"
#include "pol.h"
#include "opt2base.h"

enum error opt2(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
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
      if ((*p1 > *p2) && (*p1 > *p3)) {
        e1 = Edge(p1, p2, index);
        e2 = Edge(p1, p3, index);
//        std::cerr << std::endl << "removing: " << e1 << ", and: " << e2 << std::endl;
        val1.first = removeEdgeFromSet(e1, edgeS, polygon, points);
        if (val1.first == E_SKIP) {loop = true;}
        if (val1.first == E_NOT_VALID) break;
        val2.first = removeEdgeFromSet(e2, edgeS, polygon, points);
        if (val2.first == E_SKIP) {loop = true;}
        if (val2.first == E_NOT_VALID) break;
//        std::cerr << "skipping index" << std::endl;
      } else if ((*p1 < *p2) && (*p1 > *p3)) {
        e1 = Edge(p1, p2, index);
        e2 = Edge(p1, p3, index);
        val2.first = removeEdgeFromSet(e2, edgeS, polygon, points);
        if (val2.first == E_SKIP) {loop = true;}
        if (val2.first == E_NOT_VALID) break;
//        std::cerr << std::endl << "removed p3: " << e2 << ", processing: " << e1 << std::endl;
  		  val1 = processEdge(e1, edgeS, polygon, points);
  		  if (val1.first == E_SKIP) {loop = true;continue;}
        if (val1.first == E_NOT_VALID) break;
      } else if ((*p1 > *p2) && (*p1 < *p3) ) {
        e1 = Edge(p1, p3, index);
        e2 = Edge(p1, p2, index);
        val2.first = removeEdgeFromSet(e2, edgeS, polygon, points);
        if (val2.first == E_SKIP) {loop = true;}
        if (val2.first == E_NOT_VALID) break;
//        std::cerr << std::endl << "removed p2: " << e2 << ", processing: " << e1 << std::endl;
  		  val1 = processEdge(e1, edgeS, polygon, points);
  		  if (val1.first == E_SKIP) {loop = true; continue;}
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

        if (fabs(val3) < EPSILON) {
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
        if (val1.first == E_NOT_VALID) break;

  		  //std::cerr << std::endl << "processing e2: " << e2 << std::endl;
  		  val2 = processEdge(e2, edgeS, polygon, points);
  		  if (val2.first == E_SKIP) {
          loop = true;
          edgeS.erase(val1.second);
          continue;
        }
        if (val2.first == E_NOT_VALID) break;
      }

      //std::cout << "edges in 'edgeS':" << std::endl;
      //for (std::set<Edge, setComp>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
  		++index;
  	}
    if ((val1.first == E_NOT_VALID) || (val2.first == E_NOT_VALID)) {retval=UNEXPECTED_ERROR; break;}
  } while (loop);

	return retval;
}
