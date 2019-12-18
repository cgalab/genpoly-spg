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
  std::cerr << std::endl;
}

void print_enum(enum edge_t val) {
  switch (val) {
    case E_VALID:
      std::cerr << "E_VALID";
      break;
    case E_INTERSECTION:
      std::cerr << "E_INTERSECTION";
      break;
    case E_COLLINEAR:
      std::cerr << "E_COLLINEAR";
      break;
    case E_NOT_VALID:
      std::cerr << "E_NOT_VALID";
      break;
    default:
      break;
  }
  std::cerr << std::endl;
}

void print_enum(enum intersect_t val) {
  switch (val) {
    case IS_FALSE:
      std::cerr << "IS_FALSE";
      break;
    case IS_VERTEX11:
      std::cerr << "IS_VERTEX11";
      break;
    case IS_VERTEX12:
      std::cerr << "IS_VERTEX12";
      break;
    case IS_VERTEX21:
      std::cerr << "IS_VERTEX21";
      break;
    case IS_VERTEX22:
      std::cerr << "IS_VERTEX22";
      break;
    case IS_TRUE:
      std::cerr << "IS_TRUE";
      break;
    case IS_4P_COLLINEAR:
      std::cerr << "IS_4P_COLLINEAR";
      break;
    case IS_3P_COLLINEAR:
      std::cerr << "IS_3P_COLLINEAR";
      break;
    case IS_SAME_EDGE:
      std::cerr << "IS_SAME_EDGE";
      break;
  }
  std::cerr << std::endl;
}

//used by: opt2b, opt2c, opt2d
// function to update the 'lowest_index' variable if one of the points in either 'e1' or 'e1'
// is lex. lower than current value in 'lowest_index'
void update_lowest_index(Edge e1, Edge e2, unsigned int& lowest_index) {
  if (e1.getLowerLexIdx() < e2.getLowerLexIdx()) {
    if (e1.getLowerLexIdx() < lowest_index) lowest_index = e1.getLowerLexIdx();
  }
  else {
    if (e2.getLowerLexIdx() < lowest_index) lowest_index = e2.getLowerLexIdx();
  }
}

void update_highest_index(Edge e1, Edge e2, unsigned int& highest_index) {
  if (e1.getLowerLexIdx() < e2.getLowerLexIdx()) {
    if (e1.getLowerLexIdx() > highest_index) highest_index = e1.getLowerLexIdx();
  }
  else {
    if (e2.getLowerLexIdx() > highest_index) highest_index = e2.getLowerLexIdx();
  }
}

//used by: opt2b, opt2c, opt2d
// function to update the 'lowest_index' variable if one of the points
// is lex. lower than current value in 'lowest_index'
void update_lowest_index(Point *a, Point *b, Point *c, unsigned int& lowest_index) {
  std::vector<Point*> lex {a, b, c};
  sort(lex.begin(), lex.end(),
    [](Point* p1, Point* p2) -> bool {return (*p1).l < (*p2).l;});
  if ((*lex[0]).l < lowest_index) lowest_index = (*lex[0]).l;
}

// used by: opt2, opt2a
// function to purely remove edge 'e' from 'edgeS' if it's in there and check the 'before' and 'after' edges for intersection.
enum edge_t removeEdgeFromSet(Edge& e, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  Edge before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge>::iterator it;

//  std::cerr << "edge to be removed: " << e << std::endl;
  it = edgeS.find(e);

  if (it != edgeS.end()) {
//    std::cerr << "*it: " << *it << std::endl;
    if (*it != e) {
      std::cerr << "edge to be removed: " << e << std::endl;
      std::cerr << "it : " << *it << std::endl;
      std::cerr << "edges in 'edgeS':" << std::endl;
      for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
      valid = E_NOT_VALID;
    }
    assert(e == *it);

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
//      std::cerr << "before: " << before << ", after: " << after << std::endl;
      isval = checkIntersection(before, after);
      if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (coll4Swap(before, after, edgeS, polygon, points)) {
//          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_INTERSECTION;
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
        std::cerr << "isval: "; print_enum(isval);
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
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
          if (isval == IS_4P_COLLINEAR) {
    //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
            if (coll4Swap(before, after, edgeS, polygon, points)) {
    //          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
              valid = E_COLLINEAR;
            }
          }
          else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//            std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
            eraseEdgeFromSet(before, edgeS);
            eraseEdgeFromSet(after, edgeS);
            flip(before, after, polygon, points);
            valid = E_INTERSECTION;
          } else if (isval >= IS_TRUE) {
            std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
            std::cerr << "isval: "; print_enum(isval);
          }
        }

        break;
      }
    }
  }
  return valid;
}

enum edge_t removeEdgeFromSetOld(Edge& e, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  Edge before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge>::iterator it;

//  std::cerr << "edge to be removed: " << e << std::endl;
  it = edgeS.find(e);

  if (it != edgeS.end()) {
//    std::cerr << "*it: " << *it << std::endl;
    if (*it != e) {
      std::cerr << "edge to be removed: " << e << std::endl;
      std::cerr << "it : " << *it << std::endl;
      std::cerr << "edges in 'edgeS':" << std::endl;
      for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
      valid = E_NOT_VALID;
    }
    assert(e == *it);

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
//      std::cerr << "before: " << before << ", after: " << after << std::endl;
      isval = checkIntersection(before, after);
      if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (coll4SwapOld(before, after, edgeS, polygon, points)) {
//          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_INTERSECTION;
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
        std::cerr << "isval: "; print_enum(isval);
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
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
          if (isval == IS_4P_COLLINEAR) {
    //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
            if (coll4SwapOld(before, after, edgeS, polygon, points)) {
    //          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
              valid = E_COLLINEAR;
            }
          }
          else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//            std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
            eraseEdgeFromSet(before, edgeS);
            eraseEdgeFromSet(after, edgeS);
            flip(before, after, polygon, points);
            valid = E_INTERSECTION;
          } else if (isval >= IS_TRUE) {
            std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
            std::cerr << "isval: "; print_enum(isval);
          }
        }

        break;
      }
    }
  }
  return valid;
}

// used by opt2b, opt2c (updates lowest_index)
// function to purely remove edge 'e' from 'edgeS' if it's in there and check the 'before' and 'after' edges for intersection.
enum edge_t removeEdgeFromSetb(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  Edge before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge>::iterator it;

  it = edgeS.find(e);

  if (it != edgeS.end()) {
    if (e != *it) {
      std::cerr << "edge to be removed: " << e << std::endl;
      std::cerr << "*it: " << *it << std::endl;
      std::cerr << "edges in 'edgeS':" << std::endl;
      for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) std::cerr << *it1 << std::endl;
      valid = E_NOT_VALID;
    }
    assert(e == *it);

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
//      std::cerr << "before: " << before << ", after: " << after << std::endl;
      isval = checkIntersection(before, after);
      if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (coll4Swap(before, after, edgeS, polygon, points, lowest_index)) {
//          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(before, after, lowest_index);
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
        std::cerr << "isval: "; print_enum(isval);
        valid = E_NOT_VALID;
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
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
          if (isval == IS_4P_COLLINEAR) {
    //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
            if (coll4Swap(before, after, edgeS, polygon, points, lowest_index)) {
    //          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
              valid = E_COLLINEAR;
            }
          }
          else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//            std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
            eraseEdgeFromSet(before, edgeS);
            eraseEdgeFromSet(after, edgeS);
            flip(before, after, polygon, points);
            valid = E_INTERSECTION;
            update_lowest_index(before, after, lowest_index);
          } else if (isval >= IS_TRUE) {
            std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
            std::cerr << "isval: "; print_enum(isval);
            valid = E_NOT_VALID;
          }
        }

        break;
      }
    }
  }
  return valid;
}

// function for opt2f, that needs to use coll3sort2 in its coll4swap..
enum edge_t removeEdgeFromSetf(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  Edge before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge>::iterator it;

  it = edgeS.find(e);

  if (it != edgeS.end()) {
    if (e != *it) {
      std::cerr << "edge to be removed: " << e << std::endl;
      std::cerr << "*it: " << *it << std::endl;
      std::cerr << "edges in 'edgeS':" << std::endl;
      for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) std::cerr << *it1 << std::endl;
      valid = E_NOT_VALID;
    }

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
//      std::cerr << "before: " << before << ", after: " << after << std::endl;
      isval = checkIntersection(before, after);
      if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (coll4Swap4(before, after, edgeS, polygon, points, lowest_index)) {
//          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(before, after, lowest_index);
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
        std::cerr << "removing edge from set(): " << e << std::endl;
        std::cerr << "iterator: " << *it << std::endl;
        std::cerr << "isval: "; print_enum(isval);
        std::cerr << "edges in 'edgeS':" << std::endl;
        for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) std::cerr << *it1 << std::endl;
        valid = E_NOT_VALID;
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
//    std::cerr << "edges in 'edgeS':" << std::endl;
//    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) std::cerr << *it1 << std::endl;

    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
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
          if (isval == IS_4P_COLLINEAR) {
    //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
            if (coll4Swap4(before, after, edgeS, polygon, points, lowest_index)) {
    //          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
              valid = E_COLLINEAR;
            }
          }
          else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//            std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
            eraseEdgeFromSet(before, edgeS);
            eraseEdgeFromSet(after, edgeS);
            flip(before, after, polygon, points);
            valid = E_INTERSECTION;
            update_lowest_index(before, after, lowest_index);
          } else if (isval >= IS_TRUE) {
            std::cerr << "Error!!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
            std::cerr << "removing edge from set(): " << e << std::endl;
            std::cerr << "iterator: " << *it1 << std::endl;
            std::cerr << "isval: "; print_enum(isval);
            std::cerr << "edges in 'edgeS':" << std::endl;
            for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) std::cerr << *it1 << std::endl;
            valid = E_NOT_VALID;
          }
        }

        break;
      }
    }
  }
  return valid;
}

// same as 'f' above with added highest index.
enum edge_t removeEdgeFromSetf(Edge& e, Point *idx, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  Edge before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge>::iterator it;

  it = edgeS.find(e);

  if (it != edgeS.end()) {
    if (e != *it) {
      std::cerr << "edge to be removed: " << e << std::endl;
      std::cerr << "*it: " << *it << std::endl;
      std::cerr << "edges in 'edgeS':" << std::endl;
      for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) std::cerr << *it1 << std::endl;
      valid = E_NOT_VALID;
    }
    assert(e == *it);

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
//      std::cerr << "before: " << before << ", after: " << after << std::endl;
      isval = checkIntersection(before, after);
      if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (coll4Swap2(before, after, idx, edgeS, polygon, points, lowest_index, highest_index)) {
//          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(before, after, lowest_index);
        update_highest_index(before, after, highest_index);
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
        std::cerr << "isval: "; print_enum(isval);
        valid = E_NOT_VALID;
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
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
          if (isval == IS_4P_COLLINEAR) {
    //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
            if (coll4Swap2(before, after, idx, edgeS, polygon, points, lowest_index, highest_index)) {
    //          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
              valid = E_COLLINEAR;
            }
          }
          else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//            std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
            eraseEdgeFromSet(before, edgeS);
            eraseEdgeFromSet(after, edgeS);
            flip(before, after, polygon, points);
            valid = E_INTERSECTION;
            update_lowest_index(before, after, lowest_index);
            update_highest_index(before, after, highest_index);
          } else if (isval >= IS_TRUE) {
            std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
            std::cerr << "isval: "; print_enum(isval);
            valid = E_NOT_VALID;
          }
        }

        break;
      }
    }
  }
  return valid;
}

// same as 'f' above but using coll3sort3.
enum edge_t removeEdgeFromSetg(Edge& e, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  Edge before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge>::iterator it;

  it = edgeS.find(e);

  if (it != edgeS.end()) {
    if (e != *it) {
      std::cerr << "edge to be removed: " << e << std::endl;
      std::cerr << "*it: " << *it << std::endl;
      std::cerr << "edges in 'edgeS':" << std::endl;
      for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) std::cerr << *it1 << std::endl;
      valid = E_NOT_VALID;
    }
    assert(e == *it);

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
//      std::cerr << "before: " << before << ", after: " << after << std::endl;
      isval = checkIntersection(before, after);
      if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (coll4Swap3(before, after, edgeS, polygon, points, lowest_index, highest_index)) {
//          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(before, after, lowest_index);
        update_highest_index(before, after, highest_index);
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
        std::cerr << "isval: "; print_enum(isval);
        valid = E_NOT_VALID;
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
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
          if (isval == IS_4P_COLLINEAR) {
    //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
            if (coll4Swap3(before, after, edgeS, polygon, points, lowest_index, highest_index)) {
    //          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
              valid = E_COLLINEAR;
            }
          }
          else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//            std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
            eraseEdgeFromSet(before, edgeS);
            eraseEdgeFromSet(after, edgeS);
            flip(before, after, polygon, points);
            valid = E_INTERSECTION;
            update_lowest_index(before, after, lowest_index);
            update_highest_index(before, after, highest_index);
          } else if (isval >= IS_TRUE) {
            std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
            std::cerr << "isval: "; print_enum(isval);
            valid = E_NOT_VALID;
          }
        }

        break;
      }
    }
  }
  return valid;
}

// used by opt2b, opt2c (updates lowest_index and highest_index)
// function to purely remove edge 'e' from 'edgeS' if it's in there and check the 'before' and 'after' edges for intersection.
enum edge_t removeEdgeFromSetb(Edge& e, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  Edge before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge>::iterator it;

  it = edgeS.find(e);

  if (it != edgeS.end()) {
    if (e != *it) {
      std::cerr << "edge to be removed: " << e << std::endl;
      std::cerr << "*it: " << *it << std::endl;
      std::cerr << "edges in 'edgeS':" << std::endl;
      for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) std::cerr << *it1 << std::endl;
      valid = E_NOT_VALID;
    }
    assert(e == *it);

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
      //std::cerr << "before: " << before << ", after: " << after << std::endl;
      isval = checkIntersection(before, after);
      if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (coll4Swap(before, after, edgeS, polygon, points, lowest_index, highest_index)) {
//          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(before, after, lowest_index);
        update_highest_index(before, after, highest_index);
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
        std::cerr << "isval: "; print_enum(isval);
        valid = E_NOT_VALID;
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
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
          if (isval == IS_4P_COLLINEAR) {
    //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
            if (coll4Swap(before, after, edgeS, polygon, points, lowest_index, highest_index)) {
    //          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
              valid = E_COLLINEAR;
            }
          }
          else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//            std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
            eraseEdgeFromSet(before, edgeS);
            eraseEdgeFromSet(after, edgeS);
            flip(before, after, polygon, points);
            valid = E_INTERSECTION;
            update_lowest_index(before, after, lowest_index);
            update_highest_index(before, after, highest_index);
          } else if (isval >= IS_TRUE) {
            std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
            std::cerr << "isval: "; print_enum(isval);
            valid = E_NOT_VALID;
          }
        }

        break;
      }
    }
  }
  return valid;
}

// used by: opt2d (calls different flip function)
// function to purely remove edge 'e' from 'edgeS' if it's in there and check the 'before' and 'after' edges for intersection.
enum edge_t removeEdgeFromSetd(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  Edge before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge>::iterator it;

  it = edgeS.find(e);

  if (it != edgeS.end()) {
    if (e != *it) {
      std::cerr << "edge to be removed: " << e << std::endl;
      std::cerr << "*it: " << *it << std::endl;
      std::cerr << "edges in 'edgeS':" << std::endl;
      for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) std::cerr << *it1 << std::endl;
      valid = E_NOT_VALID;
    }
    assert(e == *it);

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
//      std::cerr << "before: " << before << ", after: " << after << std::endl;
      isval = checkIntersection(before, after);
      if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (coll4Swap4(before, after, edgeS, polygon, points, lowest_index)) {
//          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip2(before, after, polygon);
        valid = E_INTERSECTION;
        update_lowest_index(before, after, lowest_index);
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
        std::cerr << "isval: "; print_enum(isval);
        valid = E_NOT_VALID;
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
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
          if (isval == IS_4P_COLLINEAR) {
    //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
            if (coll4Swap4(before, after, edgeS, polygon, points, lowest_index)) {
    //          std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
              valid = E_COLLINEAR;
            }
          }
          else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//            std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
            eraseEdgeFromSet(before, edgeS);
            eraseEdgeFromSet(after, edgeS);
            flip2(before, after, polygon);
            valid = E_INTERSECTION;
            update_lowest_index(before, after, lowest_index);
          } else if (isval >= IS_TRUE) {
            std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
            std::cerr << "isval: "; print_enum(isval);
            valid = E_NOT_VALID;
          }
        }

        break;
      }
    }
  }
  return valid;
}

std::pair<enum edge_t, std::set<Edge>::iterator> processEdge(Edge& e, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge before, after;
  std::pair<std::set<Edge>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge>::iterator> retval2;

  retval = edgeS.insert(e);

  if (*retval.first != e) {
    std::cerr << "e : " << e << std::endl;
    std::cerr << "retval.first : " << *retval.first << std::endl;
	  std::cerr << "retval.second: " << retval.second << std::endl;
    std::cerr << "edges in 'edgeS':" << std::endl;
    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
    retval2.first = valid;
    retval2.second = retval.first;
    return retval2;
   }

  assert(*retval.first == e);

  if (retval.second) {  // successfully inserted edge.
    //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
    if (retval.first != edgeS.begin()) {
      before = *(std::prev(retval.first));
      //std::cerr << "before: " << before << std::endl;
      bef = true;
    }
    //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
    if (retval.first != --edgeS.end()) {
      after  = *(std::next(retval.first));
      //std::cerr << "after : " << after << std::endl;
      af = true;
    }

    // check incidental edge 'before' if it intersects with 'e'
    if (bef) {
      isval = checkIntersection(e, before);
      if (isval < IS_TRUE) {
        //std::cerr << "No intersection." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between e:" << e << " and bef: " << before << std::endl;
        if (coll4Swap(e, before, edgeS, polygon, points)) {
          eraseEdgeFromSet(e, edgeS); // there is a chance that only the 'before' edge needs sorting, so 'e' is never removed from 'edgeS'
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip(e, before, polygon, points);
        valid = E_INTERSECTION;
      }
    }
    // check incidental edge 'after' if it intersects with 'e'
    if (af && (valid == E_VALID)) {
      isval = checkIntersection(e, after);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and aft: " << after << std::endl;
        if (coll4Swap(e, after, edgeS, polygon, points)) {
          eraseEdgeFromSet(e, edgeS); // there is a chance that only the 'after' edge needs sorting, so 'e' is never removed from 'edgeS'
//          std::cerr << "4P coll. after swap: " << e << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)){
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSet(e, edgeS, polygon, points);
        //removeEdgeFromSet(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_INTERSECTION;
      }
      else valid = E_NOT_VALID;
    }

  }
  else {
    // edge already existed in set.
    // this can happen if an index was reset when inserting 'e2' and we're dealing with insertion of e1 again.
//    std::cerr << "Error: Edge already exists in set! : " << *retval.first << std::endl;
    valid = E_NOT_VALID;
  }
  retval2.first = valid;
  retval2.second = retval.first;
  return retval2;
}

std::pair<enum edge_t, std::set<Edge>::iterator> processEdgeOld(Edge& e, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge before, after;
  std::pair<std::set<Edge>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge>::iterator> retval2;

  retval = edgeS.insert(e);

  if (*retval.first != e) {
    std::cerr << "e : " << e << std::endl;
    std::cerr << "retval.first : " << *retval.first << std::endl;
	  std::cerr << "retval.second: " << retval.second << std::endl;
    std::cerr << "edges in 'edgeS':" << std::endl;
    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
    retval2.first = valid;
    retval2.second = retval.first;
    return retval2;
   }

  assert(*retval.first == e);

  if (retval.second) {  // successfully inserted edge.
    //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
    if (retval.first != edgeS.begin()) {
      before = *(std::prev(retval.first));
      //std::cerr << "before: " << before << std::endl;
      bef = true;
    }
    //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
    if (retval.first != --edgeS.end()) {
      after  = *(std::next(retval.first));
      //std::cerr << "after : " << after << std::endl;
      af = true;
    }

    // check incidental edge 'before' if it intersects with 'e'
    if (bef) {
      isval = checkIntersection(e, before);
      if (isval < IS_TRUE) {
        //std::cerr << "No intersection." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between e:" << e << " and bef: " << before << std::endl;
        if (coll4SwapOld(e, before, edgeS, polygon, points)) {
          eraseEdgeFromSet(e, edgeS); // there is a chance that only the 'before' edge needs sorting, so 'e' is never removed from 'edgeS'
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip(e, before, polygon, points);
        valid = E_INTERSECTION;
      }
    }
    // check incidental edge 'after' if it intersects with 'e'
    if (af && (valid == E_VALID)) {
      isval = checkIntersection(e, after);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and aft: " << after << std::endl;
        if (coll4SwapOld(e, after, edgeS, polygon, points)) {
          eraseEdgeFromSet(e, edgeS); // there is a chance that only the 'after' edge needs sorting, so 'e' is never removed from 'edgeS'
//          std::cerr << "4P coll. after swap: " << e << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)){
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSet(e, edgeS, polygon, points);
        //removeEdgeFromSet(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_INTERSECTION;
      }
      else valid = E_NOT_VALID;
    }

  }
  else {
    // edge already existed in set.
    // this can happen if an index was reset when inserting 'e2' and we're dealing with insertion of e1 again.
//    std::cerr << "Error: Edge already exists in set! : " << *retval.first << std::endl;
    valid = E_NOT_VALID;
  }
  retval2.first = valid;
  retval2.second = retval.first;
  return retval2;
}


// updates lowest_index (used by 'b', 'c', and 'f')
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgeb(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge before, after;
  std::pair<std::set<Edge>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge>::iterator> retval2;

  retval = edgeS.insert(e);

  if (*retval.first != e) {
    std::cerr << "retval.first : " << *retval.first << std::endl;
	   std::cerr << "retval.second: " << retval.second << std::endl;
     std::cerr << "edges in 'edgeS':" << std::endl;
     for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
     valid = E_NOT_VALID;
   }

  assert(*retval.first == e);

  //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
  if (retval.first != edgeS.begin()) {
    before = *(std::prev(retval.first));
    //std::cerr << "before: " << before << std::endl;
    bef = true;
  }
  //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
  if (retval.first != --edgeS.end()) {
    after  = *(std::next(retval.first));
    //std::cerr << "after : " << after << std::endl;
    af = true;
  }

  if (retval.second) {  // successfully inserted edge.
    // check incidental edge 'before' if it intersects with 'e'
    if (bef) {
      isval = checkIntersection(e, before);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with before." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and before: " << before << std::endl;
        if (coll4Swap(e, before, edgeS, polygon, points, lowest_index)) {
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip(e, before, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, before, lowest_index);
      }
    }
    // check incidental edge 'after' if it intersects with 'e'
    if (af && (valid == E_VALID)) {
      isval = checkIntersection(e, after);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with before." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and after: " << after << std::endl;
        if (coll4Swap(e, after, edgeS, polygon, points, lowest_index)) {
//          std::cerr << "coll. after swap: " << e << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSetb(e, edgeS, polygon, points);
        //removeEdgeFromSetb(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, after, lowest_index);
      }
    }

  } else {
    // edge already existed in set.
    // this can happen if an index is going backwards through the lex. order of points.
    // but if it's reversing and it hits an edge already in, earlier code should have caught it and removed it.
    std::cerr << "Error: Edge already exists in set!" << std::endl;
    std::cerr << "edge: " << e << ", returned: " << *retval.first << std::endl;
//    std::cerr << "edges in 'edgeS':" << std::endl;
//    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
  }

  retval2.first = valid;
  if (valid != E_VALID) retval2.second = edgeS.end();
  else retval2.second = retval.first;

  return retval2;
}

// updates lowest_index and highest_index
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgeb(Edge& e, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge before, after;
  std::pair<std::set<Edge>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge>::iterator> retval2;

  retval = edgeS.insert(e);
/*
  if (*retval.first != e) {
    std::cerr << "retval.first : " << *retval.first << std::endl;
	   std::cerr << "retval.second: " << retval.second << std::endl;
     std::cerr << "edges in 'edgeS':" << std::endl;
     for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
   }
*/
  assert(*retval.first == e);

  //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
  if (retval.first != edgeS.begin()) {
    before = *(std::prev(retval.first));
    //std::cerr << "before: " << before << std::endl;
    bef = true;
  }
  //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
  if (retval.first != --edgeS.end()) {
    after  = *(std::next(retval.first));
    //std::cerr << "after : " << after << std::endl;
    af = true;
  }

  if (retval.second) {  // successfully inserted edge.
    // check incidental edge 'before' if it intersects with 'e'
    if (bef) {
      isval = checkIntersection(e, before);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with before." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
        std::cerr << "4P collinearity between:" << e << " and before: " << before << std::endl;
    if (coll4Swap(e, before, edgeS, polygon, points, lowest_index, highest_index)) {
          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip(e, before, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, before, lowest_index);
        update_highest_index(e, before, highest_index);
      }
    }
    // check incidental edge 'after' if it intersects with 'e'
    if (af && (valid == E_VALID)) {
      isval = checkIntersection(e, after);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with after." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
        std::cerr << "collinearity between:" << e << " and after: " << after << std::endl;
        if (coll4Swap(e, after, edgeS, polygon, points, lowest_index, highest_index)) {
          std::cerr << "coll. after swap: " << e << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSetb(e, edgeS, polygon, points);
        //removeEdgeFromSetb(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, after, lowest_index);
        update_highest_index(e, after, highest_index);
      }
    }

  } else {
    // edge already existed in set.
    // this can happen if an index is going backwards through the lex. order of points.
    // but if it's reversing and it hits an edge already in, earlier code should have caught it and removed it.
    std::cerr << "Error: Edge already exists in set!" << std::endl;
    std::cerr << "edge: " << e << ", returned: " << *retval.first << std::endl;
//    std::cerr << "edges in 'edgeS':" << std::endl;
//    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
  }

  retval2.first = valid;
  if (valid != E_VALID) retval2.second = edgeS.end();
  else retval2.second = retval.first;

  return retval2;
}

// updates lowest_index and uses a different flip function (used by 'd' and 'g')
std::pair<enum edge_t, std::set<Edge>::iterator> processEdged(Edge& e, Point *idx, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge before, after;
  std::pair<std::set<Edge>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge>::iterator> retval2;

  retval = edgeS.insert(e);

  if (*retval.first != e) {
    std::cerr << "retval.first : " << *retval.first << std::endl;
	   std::cerr << "retval.second: " << retval.second << std::endl;
     std::cerr << "edges in 'edgeS':" << std::endl;
     for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
     valid = E_NOT_VALID;
   }

  assert(*retval.first == e);

  //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
  if (retval.first != edgeS.begin()) {
    before = *(std::prev(retval.first));
    //std::cerr << "before: " << before << std::endl;
    bef = true;
  }
  //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
  if (retval.first != --edgeS.end()) {
    after  = *(std::next(retval.first));
    //std::cerr << "after : " << after << std::endl;
    af = true;
  }

  if (retval.second) {  // successfully inserted edge.
    // check incidental edge 'before' if it intersects with 'e'
    if (bef) {
      isval = checkIntersection(e, before);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with before." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and before: " << before << std::endl;
        if (coll4Swap4(e, before, edgeS, polygon, points, lowest_index)) {
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;

        //preparing for insertion of the edge not connected to 'idx' into 'edgeS'
        std::vector<unsigned int> vertices {(*e.p1).v, (*e.p2).v, (*before.p1).v, (*before.p2).v};

        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip2(e, before, polygon);
        valid = E_INTERSECTION;
        update_lowest_index(e, before, lowest_index);

        //code to insert the edge not connected to 'idx' into 'edgeS'
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
      		if (points[polygon[vertices[i]]].l == (*idx).l) {
      			if (i < 2) new_edge.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);
      			else new_edge.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
      			break;
      		}
      	}
      	if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
//      		std::cerr << "before flip insertion: " << new_edge << std::endl;
//      		std::cerr << "at idx: " << *idx << std::endl;
      		std::pair<std::set<Edge>::iterator,bool> retval;
      		retval = edgeS.insert(new_edge);
      		if (*retval.first != new_edge) {
      			std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      		}
      	}
      }
    }
    // check incidental edge 'after' if it intersects with 'e'
    if (af && (valid == E_VALID)) {
      isval = checkIntersection(e, after);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with after." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and after: " << after << std::endl;
        if (coll4Swap4(e, after, edgeS, polygon, points, lowest_index)) {
//          std::cerr << "coll. after swap: " << e << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;

        //preparing for insertion of the edge not connected to 'idx' into 'edgeS'
        std::vector<unsigned int> vertices {(*e.p1).v, (*e.p2).v, (*after.p1).v, (*after.p2).v};

        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSetb(e, edgeS, polygon, points);
        //removeEdgeFromSetb(after, edgeS, polygon, points);
        flip2(e, after, polygon);
        valid = E_INTERSECTION;
        update_lowest_index(e, after, lowest_index);

        //code to insert the edge not connected to 'idx' into 'edgeS'
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
      		if (points[polygon[vertices[i]]].l == (*idx).l) {
      			if (i < 2) new_edge.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);
      			else new_edge.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
      			break;
      		}
      	}
      	if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
//      		std::cerr << "after flip insertion: " << new_edge << std::endl;
//      		std::cerr << "at idx: " << *idx << std::endl;
      		std::pair<std::set<Edge>::iterator,bool> retval;
      		retval = edgeS.insert(new_edge);
      		if (*retval.first != new_edge) {
      			std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      		}
      	}
      }
    }

  } else {
    // edge already existed in set.
    // this can happen if an index is going backwards through the lex. order of points.
    // but if it's reversing and it hits an edge already in, earlier code should have caught it and removed it.
    std::cerr << "Processing error: Edge already exists in set!" << std::endl;
    std::cerr << "idx: " << *idx << std::endl;
    std::cerr << "edge: " << e << ", returned: " << *retval.first << std::endl;
//    std::cerr << "edges in 'edgeS':" << std::endl;
//    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
  }

  retval2.first = valid;
  if (valid != E_VALID) retval2.second = edgeS.end();
  else retval2.second = retval.first;

  return retval2;
}

// for reversal algorithm opt2f, when it finds an intersection, it inserts the flipped edge that does not connect to the current point.
// as the edge connected to the current point is handled in the index restart.
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgef(Edge& e, Point *idx, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge before, after;
  std::pair<std::set<Edge>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge>::iterator> retval2;

  retval = edgeS.insert(e);

  if (*retval.first != e) {
    std::cerr << "retval.first : " << *retval.first << std::endl;
	   std::cerr << "retval.second: " << retval.second << std::endl;
     std::cerr << "edges in 'edgeS':" << std::endl;
     for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
     valid = E_NOT_VALID;
   }

  //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
  if (retval.first != edgeS.begin()) {
    before = *(std::prev(retval.first));
    //std::cerr << "before: " << before << std::endl;
    bef = true;
  }
  //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
  if (retval.first != --edgeS.end()) {
    after  = *(std::next(retval.first));
    //std::cerr << "after : " << after << std::endl;
    af = true;
  }

  if (retval.second) {  // successfully inserted edge.
    // check incidental edge 'before' if it intersects with 'e'
    if (bef) {
      isval = checkIntersection(e, before);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with before." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and before: " << before << std::endl;
        if (coll4Swap4(e, before, edgeS, polygon, points, lowest_index)) {
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
//        std::cerr << "idx: " << *idx << std::endl;

        //preparing for insertion of the edge not connected to 'idx' into 'edgeS'
        std::vector<unsigned int> vertices {(*e.p1).v, (*e.p2).v, (*before.p1).v, (*before.p2).v};

        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip(e, before, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, before, lowest_index);

        //code to insert the edge not connected to 'idx' into 'edgeS'
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
      		if (points[polygon[vertices[i]]].l == (*idx).l) {
      			if (i < 2) new_edge.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);
      			else new_edge.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
      			break;
      		}
      	}

      	if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
//      		std::cerr << "'before' flip insertion: " << new_edge << std::endl;
//      		std::cerr << "at idx: " << *idx << std::endl;
//          std::cerr << "edges in 'edgeS':" << std::endl;
//          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

      		std::pair<std::set<Edge>::iterator,bool> retval;
          retval.first = edgeS.find(new_edge);
          if (retval.first == edgeS.end()) {
            retval = edgeS.insert(new_edge);
//            std::cerr << "edge added to 'edgeS':" << std::endl;
//            for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
        		if (*retval.first != new_edge) {
        			std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
        		}
          }
      	}
      }
    }
    // check incidental edge 'after' if it intersects with 'e'
    if (af && (valid == E_VALID)) {
      isval = checkIntersection(e, after);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with after." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and after: " << after << std::endl;
        if (coll4Swap4(e, after, edgeS, polygon, points, lowest_index)) {
//          std::cerr << "coll. after swap: " << e << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
//        std::cerr << "idx: " << *idx << std::endl;

        // preparing for insertion of the edge not connected to 'idx' into 'edgeS'
        // the vertices do not change, the lower 2 vertices are still connected together
        // and the higher 2 vertices are still connected together.
        std::vector<unsigned int> vertices {(*e.p1).v, (*e.p2).v, (*after.p1).v, (*after.p2).v};
//        std::cerr << "vertices before flip:" << std::endl;
//        for (unsigned int i = 0; i < vertices.size();++i) std::cerr << "i:" << i << " : " << vertices[i] << std::endl;

        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSetb(e, edgeS, polygon, points);
        //removeEdgeFromSetb(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, after, lowest_index);

        //code to insert the edge not connected to 'idx' into 'edgeS'
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
      		if (points[polygon[vertices[i]]].l == (*idx).l) {
      			if (i < 2) new_edge.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);
      			else new_edge.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
      			break;
      		}
      	}

      	if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
//      		std::cerr << "'after' flip insertion: " << new_edge << std::endl;
//      		std::cerr << "at idx: " << *idx << std::endl;
//          std::cerr << "edges in 'edgeS':" << std::endl;
//          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

      		std::pair<std::set<Edge>::iterator,bool> retval;
          retval.first = edgeS.find(new_edge);
          if(retval.first == edgeS.end()) {
            retval = edgeS.insert(new_edge);
//            std::cerr << "edge added to 'edgeS':" << std::endl;
//            for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

      		  if (*retval.first != new_edge) {
      			  std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      		  }
          }
      	}
      }
    }

  } else {
    // edge already existed in set.
    // this can happen if an index is going backwards through the lex. order of points.
    // but if it's reversing and it hits an edge already in, earlier code should have caught it and removed it.
    std::cerr << "Processing error: Edge already exists in set!" << std::endl;
    std::cerr << "idx: " << *idx << std::endl;
    std::cerr << "edge: " << e << ", returned: " << *retval.first << std::endl;
//    std::cerr << "edges in 'edgeS':" << std::endl;
//    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
  }

  retval2.first = valid;
  if (valid != E_VALID) retval2.second = edgeS.end();
  else retval2.second = retval.first;

  return retval2;
}

// same as above, but also updates 'highest_index'.
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgef(Edge& e, Point *idx, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge before, after;
  std::pair<std::set<Edge>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge>::iterator> retval2;

  retval = edgeS.insert(e);

  if (*retval.first != e) {
    std::cerr << "retval.first : " << *retval.first << std::endl;
	   std::cerr << "retval.second: " << retval.second << std::endl;
     std::cerr << "edges in 'edgeS':" << std::endl;
     for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
     valid = E_NOT_VALID;
   }

  assert(*retval.first == e);

  //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
  if (retval.first != edgeS.begin()) {
    before = *(std::prev(retval.first));
    //std::cerr << "before: " << before << std::endl;
    bef = true;
  }
  //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
  if (retval.first != --edgeS.end()) {
    after  = *(std::next(retval.first));
    //std::cerr << "after : " << after << std::endl;
    af = true;
  }

  if (retval.second) {  // successfully inserted edge.
    // check incidental edge 'before' if it intersects with 'e'
    if (bef) {
      isval = checkIntersection(e, before);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with before." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and before: " << before << std::endl;
        if (coll4Swap2(e, before, idx, edgeS, polygon, points, lowest_index, highest_index)) {
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;

        //preparing for insertion of the edge not connected to 'idx' into 'edgeS'
        std::vector<unsigned int> vertices {(*e.p1).v, (*e.p2).v, (*before.p1).v, (*before.p2).v};

        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip(e, before, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, before, lowest_index);
        update_highest_index(e, before, highest_index);

        //code to insert the edge not connected to 'idx' into 'edgeS'
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
      		if (points[polygon[vertices[i]]].l == (*idx).l) {
      			if (i < 2) new_edge.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);
      			else new_edge.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
      			break;
      		}
      	}
      	if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
//      		std::cerr << "before flip insertion: " << new_edge << std::endl;
//      		std::cerr << "at idx: " << *idx << std::endl;
      		std::pair<std::set<Edge>::iterator,bool> retval;
      		retval = edgeS.insert(new_edge);
      		if (*retval.first != new_edge) {
      			std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      		}
      	}
      }
    }
    // check incidental edge 'after' if it intersects with 'e'
    if (af && (valid == E_VALID)) {
      isval = checkIntersection(e, after);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with after." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and after: " << after << std::endl;
        if (coll4Swap2(e, after, idx, edgeS, polygon, points, lowest_index, highest_index)) {
//          std::cerr << "coll. after swap: " << e << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;

        //preparing for insertion of the edge not connected to 'idx' into 'edgeS'
        std::vector<unsigned int> vertices {(*e.p1).v, (*e.p2).v, (*after.p1).v, (*after.p2).v};

        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSetb(e, edgeS, polygon, points);
        //removeEdgeFromSetb(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, after, lowest_index);
        update_highest_index(e, after, highest_index);

        //code to insert the edge not connected to 'idx' into 'edgeS'
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
      		if (points[polygon[vertices[i]]].l == (*idx).l) {
      			if (i < 2) new_edge.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);
      			else new_edge.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
      			break;
      		}
      	}
      	if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
//      		std::cerr << "after flip insertion: " << new_edge << std::endl;
//      		std::cerr << "at idx: " << *idx << std::endl;
      		std::pair<std::set<Edge>::iterator,bool> retval;
      		retval = edgeS.insert(new_edge);
      		if (*retval.first != new_edge) {
      			std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      		}
      	}
      }
    }

  } else {
    // edge already existed in set.
    // this can happen if an index is going backwards through the lex. order of points.
    // but if it's reversing and it hits an edge already in, earlier code should have caught it and removed it.
    std::cerr << "Processing error: Edge already exists in set!" << std::endl;
    std::cerr << "idx: " << *idx << std::endl;
    std::cerr << "edge: " << e << ", returned: " << *retval.first << std::endl;
//    std::cerr << "edges in 'edgeS':" << std::endl;
//    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
  }

  retval2.first = valid;
  if (valid != E_VALID) retval2.second = edgeS.end();
  else retval2.second = retval.first;

  return retval2;
}

// same as above, but also updates 'highest_index', and uses coll3sort3.
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgeg(Edge& e, Point *idx, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge before, after;
  std::pair<std::set<Edge>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge>::iterator> retval2;

  retval = edgeS.insert(e);

  if (*retval.first != e) {
    std::cerr << "retval.first : " << *retval.first << std::endl;
	   std::cerr << "retval.second: " << retval.second << std::endl;
     std::cerr << "edges in 'edgeS':" << std::endl;
     for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
     valid = E_NOT_VALID;
   }

  assert(*retval.first == e);

  //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
  if (retval.first != edgeS.begin()) {
    before = *(std::prev(retval.first));
    //std::cerr << "before: " << before << std::endl;
    bef = true;
  }
  //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
  if (retval.first != --edgeS.end()) {
    after  = *(std::next(retval.first));
    //std::cerr << "after : " << after << std::endl;
    af = true;
  }

  if (retval.second) {  // successfully inserted edge.
    // check incidental edge 'before' if it intersects with 'e'
    if (bef) {
      isval = checkIntersection(e, before);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with before." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and before: " << before << std::endl;
        if (coll4Swap3(e, before, edgeS, polygon, points, lowest_index, highest_index)) {
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;

        //preparing for insertion of the edge not connected to 'idx' into 'edgeS'
        std::vector<unsigned int> vertices {(*e.p1).v, (*e.p2).v, (*before.p1).v, (*before.p2).v};

        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip(e, before, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, before, lowest_index);
        update_highest_index(e, before, highest_index);

        //code to insert the edge not connected to 'idx' into 'edgeS'
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
      		if (points[polygon[vertices[i]]].l == (*idx).l) {
      			if (i < 2) new_edge.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);
      			else new_edge.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
      			break;
      		}
      	}
      	if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
//      		std::cerr << "before flip insertion: " << new_edge << std::endl;
//      		std::cerr << "at idx: " << *idx << std::endl;
      		std::pair<std::set<Edge>::iterator,bool> retval;
      		retval = edgeS.insert(new_edge);
      		if (*retval.first != new_edge) {
      			std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      		}
      	}
      }
    }
    // check incidental edge 'after' if it intersects with 'e'
    if (af && (valid == E_VALID)) {
      isval = checkIntersection(e, after);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with after." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and after: " << after << std::endl;
        if (coll4Swap3(e, after, edgeS, polygon, points, lowest_index, highest_index)) {
//          std::cerr << "coll. after swap: " << e << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;

        //preparing for insertion of the edge not connected to 'idx' into 'edgeS'
        std::vector<unsigned int> vertices {(*e.p1).v, (*e.p2).v, (*after.p1).v, (*after.p2).v};

        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSetb(e, edgeS, polygon, points);
        //removeEdgeFromSetb(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, after, lowest_index);
        update_highest_index(e, after, highest_index);

        //code to insert the edge not connected to 'idx' into 'edgeS'
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
      		if (points[polygon[vertices[i]]].l == (*idx).l) {
      			if (i < 2) new_edge.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);
      			else new_edge.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
      			break;
      		}
      	}
      	if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
//      		std::cerr << "after flip insertion: " << new_edge << std::endl;
//      		std::cerr << "at idx: " << *idx << std::endl;
      		std::pair<std::set<Edge>::iterator,bool> retval;
      		retval = edgeS.insert(new_edge);
      		if (*retval.first != new_edge) {
      			std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      		}
      	}
      }
    }

  } else {
    // edge already existed in set.
    // this can happen if an index is going backwards through the lex. order of points.
    // but if it's reversing and it hits an edge already in, earlier code should have caught it and removed it.
    std::cerr << "Processing error: Edge already exists in set!" << std::endl;
    std::cerr << "idx: " << *idx << std::endl;
    std::cerr << "edge: " << e << ", returned: " << *retval.first << std::endl;
//    std::cerr << "edges in 'edgeS':" << std::endl;
//    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
  }

  retval2.first = valid;
  if (valid != E_VALID) retval2.second = edgeS.end();
  else retval2.second = retval.first;

  return retval2;
}
