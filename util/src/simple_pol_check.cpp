#include <iostream> // for cerr and endl
#include <vector>
#include <cmath>  // for fabs
#include <iterator> // for std:prev and std::next
#include <assert.h>
#include <utility>      // std::pair, std::make_pair
#include "basicDefinitions.h"
#include "basicFunctions.h"
#include "edge.h"

// function to purely remove edge 'e' from 'edgeS' if it's in there and check the 'before' and 'after' edges for intersection.
enum edge_t removeEdgeFromSet(Edge2& e, std::set<Edge2>& edgeS) {
  enum edge_t valid = E_VALID;
  Edge2 before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge2>::iterator it;

  it = edgeS.find(e); // will return edgeS.end() which if edge not found.

  if (it != edgeS.end()) {
//    std::cerr << "edge to be removed: " << e << std::endl;
//    std::cerr << "*it: " << *it << std::endl;
    if (*it == e) {
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
//        std::cerr << "before: " << before << ", after: " << after << std::endl;
        isval = checkIntersection(before, after);
        if (isval >= IS_TRUE) {
//          std::cerr << "Removing: Intersection between: before: " << before << " and after: " << after << std::endl;
          valid = E_NOT_VALID;
        }
      }
    }
    else {
      std::cerr << "Removing: ERROR:  edge found was not the same edge!" << std::endl;
      std::cerr << "edge: " << e << std::endl;
      std::cerr << " *it: " << *it << std::endl;
      std::cerr << "edges in 'edgeS':" << std::endl;
      for (std::set<Edge2>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

    }
  } else {
    // came to the end of the set without finding the edge, can only happen if there was an intersection that went undiscovered.
    std::cerr << "Lost Edge in set: " << e << std::endl;
//    if (it != edgeS.end()) std::cerr << "*it: " << *it << std::endl;
//    std::cerr << "edges in 'edgeS':" << std::endl;
//    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

    for (std::set<Edge2>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
//        std::cerr << "*it1: " << *it1 << std::endl;
        // get edges before and after
        if (it1 != edgeS.begin()) {
          before = *(std::prev(it1));
          bef = true;
        }
        if (it1 != std::prev(edgeS.end())) {
          after  = *(std::next(it1));
          af = true;
        }

        edgeS.erase(it1);

        if (bef && af) {
//          std::cerr << "before: " << before << ", after: " << after << std::endl;
          isval = checkIntersection(before, after);
          if (isval >= IS_TRUE) {
            std::cerr << "Removing: " << e << std::endl;
            std::cerr <<"Intersection found between: before: " << before << " and after: " << after << std::endl;
            valid = E_NOT_VALID;
          }
        }
        break;
      }
    }
  }

  return valid;
}

enum edge_t processEdge(Edge2& e, std::set<Edge2>& edgeS) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge2 before, after;
  std::pair<std::set<Edge2>::iterator,bool> retval;

  retval = edgeS.insert(e);
/*
  if (*retval.first != e) {
    std::cerr << "retval.first : " << *retval.first << std::endl;
	   std::cerr << "retval.second: " << retval.second << std::endl;
     std::cout << "edges in 'edgeS':" << std::endl;
     for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
   }
*/
  if (*retval.first == e) {
    //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
    if (retval.first != edgeS.begin()) {
      before = *(std::prev(retval.first));
//      std::cerr << "before: " << before << std::endl;
      bef = true;
    }
    //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
    if (retval.first != std::prev(edgeS.end())) {
      after = *(std::next(retval.first));
//      std::cerr << "after : " << after << std::endl;
      af = true;
    }

    if (retval.second) {  // successfully inserted edge.
      // check incidental edge 'before' if it intersects with 'e'
      if (bef) {
        isval = checkIntersection(e, before);
        if (isval >= IS_TRUE) {
          std::cerr << "Processing: Intersection found between: " << e << " and " << before << std::endl;
          valid = E_NOT_VALID;
        }
      }
      // check incidental edge 'after' if it intersects with 'e'
      if (af && (valid == E_VALID)) {
        isval = checkIntersection(e, after);
        if (isval >= IS_TRUE) {
          std::cerr << "Processing: Intersection found between: " << e << " and " << after << std::endl;
          valid = E_NOT_VALID;
        }
      }

    } else {
      // edge already existed in set.
      // this can happen if an index is going backwards through the lex. order of points.
      // but if it's reversing and it hits an edge already in, earlier code should have caught it and removed it.
//      std::cerr << "Error: Edge already exists in set!" << std::endl;
//      std::cerr << "edge: " << e << ", returned: " << *retval.first << std::endl;
      valid = E_NOT_VALID;
    }
  }
  else {
    std::cerr << "Error: Iterator did not return same edge." << std::endl;
    std::cerr << "Edge: " << e << std::endl;
    std::cerr << "*it" << *retval.first << std::endl;
    valid = E_NOT_VALID;
  }



  return valid;
}

// Function that verifies whether a point set is simple.
// Input: a vector of <Point> which is a point set in a simple polygon order.
// Assumption:  all points in point set is a part of the same polygon.
enum error simple_pol_check(std::vector<Point>& points) {
  enum error retval = SUCCESS;

  std::vector<unsigned int> polygon;
  for (unsigned int i = 0; i < points.size(); ++i) polygon.push_back(i);

  std::vector<unsigned int> lex (points.size());
	fill_lex(lex, points); // fill 'lex' with the indexes
//  pdisplay(lex, points);

  // Given a lexicographical sort, we can go through the vector and check for intersections.
	unsigned int index=0, before=0, after=0;
  enum edge_t val1 = E_VALID, val2 = E_VALID;
  double val3=1.0;
	Point *p1, *p2, *p3;
	Edge2 e1, e2;
  std::set<Edge2> edgeS; // a sweep-line-status object.

  while (index < points.size()) {
//      std::cerr << std::endl << "i: " << index << std::endl;
//      std::cout << "edges in 'edgeS':" << std::endl;
//      for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

    // get the current point at 'index'
    p1 = &points[lex[index]];

    // get the 2 points it is connected to in 'polygon', treating the edge case when the point 'p1' is on the ends
    before = ((*p1).v + polygon.size() -1) % polygon.size();
    after =  ((*p1).v + polygon.size() +1) % polygon.size();

    p2 = &points[polygon[before]];
    p3 = &points[polygon[after]];

    // construct the edges
//    std::cerr << "p1: " << *p1 << ", p2: "<< *p2 << " < p3: " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl;
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

    //process first edge
    if (*e1.p2 == *p1) {
//      std::cerr << std::endl << "removing e1: " << e1 << std::endl;
      val1 = removeEdgeFromSet(e1, edgeS);
      if (val1 != E_VALID) {
        retval = UNEXPECTED_ERROR; break;}
    }
    else {
      // if the earlier edge was allowed, then the later edge is also allowed
      if (fabs(val3) == 0) {
//        std::cerr << "Intersection found: collinearity between: " << e1 << " and " << e2 << std::endl;
        retval = UNEXPECTED_ERROR; break;}
//      std::cerr << std::endl << "processing e1: " << e1 << std::endl;
      val1 = processEdge(e1, edgeS);
      if (val1 != E_VALID) {retval = UNEXPECTED_ERROR; break;}
    }

    // process second edge
    if (*e2.p2 == *p1) {
//      std::cerr << std::endl << "removing e2: " << e2 << std::endl;
      val2 = removeEdgeFromSet(e2, edgeS);
      if (val2 != E_VALID) {retval = UNEXPECTED_ERROR; break;}
    }
    else {
//      std::cerr << std::endl << "processing e2: " << e2 << std::endl;
      val2 = processEdge(e2, edgeS);
      if (val2 != E_VALID) {retval = UNEXPECTED_ERROR; break;}
    }

    ++index;
  }

  return retval;
}

enum error simple_pol_check(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum error retval = SUCCESS;

  std::vector<unsigned int> lex (points.size());
	fill_lex(lex, points); // fill 'lex' with the indexes
//  pdisplay(lex, points);

  // Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index=0, before=0, after=0;
  enum edge_t val1 = E_VALID, val2 = E_VALID;
  double val3=1.0;
	Point *p1, *p2, *p3;
	Edge2 e1, e2;
  std::set<Edge2> edgeS; // a sweep-line-status object.

  while (index < points.size()) {
//      std::cerr << std::endl << "i: " << index << std::endl;
//      std::cout << "edges in 'edgeS':" << std::endl;
//      for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

    // get the current point at 'index'
    p1 = &points[lex[index]];

    // get the 2 points it is connected to in 'polygon', treating the edge case when the point 'p1' is on the ends
    before = ((*p1).v + polygon.size() -1) % polygon.size();
    after =  ((*p1).v + polygon.size() +1) % polygon.size();

    p2 = &points[polygon[before]];
    p3 = &points[polygon[after]];

    // construct the edges
//    std::cerr << "p1: " << *p1 << ", p2: "<< *p2 << " < p3: " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl;
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

    //process first edge
    if (*e1.p2 == *p1) {
//      std::cerr << std::endl << "removing e1: " << e1 << std::endl;
      val1 = removeEdgeFromSet(e1, edgeS);
      if (val1 != E_VALID) {
        retval = UNEXPECTED_ERROR; break;}
    }
    else {
      // if the earlier edge was allowed, then the later edge is also allowed
      if (fabs(val3) == 0) {
//        std::cerr << "Intersection found: collinearity between: " << e1 << " and " << e2 << std::endl;
        retval = UNEXPECTED_ERROR; break;}
//      std::cerr << std::endl << "processing e1: " << e1 << std::endl;
      val1 = processEdge(e1, edgeS);
      if (val1 != E_VALID) {retval = UNEXPECTED_ERROR; break;}
    }

    // process second edge
    if (*e2.p2 == *p1) {
//      std::cerr << std::endl << "removing e2: " << e2 << std::endl;
      val2 = removeEdgeFromSet(e2, edgeS);
      if (val2 != E_VALID) {retval = UNEXPECTED_ERROR; break;}
    }
    else {
//      std::cerr << std::endl << "processing e2: " << e2 << std::endl;
      val2 = processEdge(e2, edgeS);
      if (val2 != E_VALID) {retval = UNEXPECTED_ERROR; break;}
    }

    ++index;
  }

  return retval;
}

// function that checks all the polygons in sph for simplicity.
enum error simple_pol_check(std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points) {
  enum error retval = SUCCESS;

  //make sure all points are correctly labeled to its polygon.
  for (unsigned int i = 0; i < sph.size(); ++i) {
    for (unsigned int j = 0; j < sph[i].size(); ++j) {
      points[sph[i][j]].p = i;
    }
  }

  std::vector<unsigned int> lex (points.size());
	fill_lex(lex, points); // fill 'lex' with the indexes
//  pdisplay(lex, points);

  // Given a lexicographical sort, we can go through the vector and check for intersections.
	unsigned int index=0, before=0, after=0;
  enum edge_t val1 = E_VALID, val2 = E_VALID;
  double val3=1.0;
	Point *p1, *p2, *p3;
	Edge2 e1, e2;
  std::set<Edge2> edgeS; // a sweep-line-status object.

  while (index < points.size()) {
//      std::cerr << std::endl << "i: " << index << std::endl;
//      std::cout << "edges in 'edgeS':" << std::endl;
//      for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

    // get the current point at 'index'
    p1 = &points[lex[index]];

    // get the 2 points it is connected to in 'polygon', treating the edge case when the point 'p1' is on the ends
    before = ((*p1).v + sph[(*p1).p].size() -1) % sph[(*p1).p].size();
    after =  ((*p1).v + sph[(*p1).p].size() +1) % sph[(*p1).p].size();

    p2 = &points[sph[(*p1).p][before]];
    p3 = &points[sph[(*p1).p][after]];

    // construct the edges
//    std::cerr << "p1: " << *p1 << ", p2: "<< *p2 << " < p3: " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl;
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
//    std::cerr << "e1: " << e1 << std::endl;
//    std::cerr << "e2: " << e2 << std::endl;

    //process first edge
    if (*e1.p2 == *p1) {
//      std::cerr << std::endl << "removing e1: " << e1 << std::endl;
      val1 = removeEdgeFromSet(e1, edgeS);
      if (val1 != E_VALID) {
        std::cerr << "removing: " << e1 << ", found an intersection!" << std::endl;
        retval = UNEXPECTED_ERROR;
        break;}
    }
    else {
      // if the earlier edge was allowed, then the later edge is also allowed
      if (fabs(val3) == 0) {
        std::cerr << "Intersection found: collinearity between: " << e1 << " and " << e2 << std::endl;
        retval = UNEXPECTED_ERROR;
        break;}
//      std::cerr << std::endl << "processing e1: " << e1 << std::endl;
      val1 = processEdge(e1, edgeS);
      if (val1 != E_VALID) {
        std::cerr << "processing: " << e1 << ", found an intersection!" << std::endl;
        retval = UNEXPECTED_ERROR;
        break;}
    }

    // process second edge
    if (*e2.p2 == *p1) {
//      std::cerr << std::endl << "removing e2: " << e2 << std::endl;
      val2 = removeEdgeFromSet(e2, edgeS);
      if (val2 != E_VALID) {
        std::cerr << "removing: " << e2 << ", found an intersection!" << std::endl;
        retval = UNEXPECTED_ERROR;
        break;}
    }
    else {
//      std::cerr << std::endl << "processing e2: " << e2 << std::endl;
      val2 = processEdge(e2, edgeS);
      if (val2 != E_VALID) {
        std::cerr << "processing: " << e2 << ", found an intersection!" << std::endl;
        retval = UNEXPECTED_ERROR;
        break;}
    }

    ++index;
  }

  return retval;
}
