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

void update_lowest_index(Edge2 e1, Edge2 e2, unsigned int& lowest_index) {
  if (e1.getLowerLexIdx() < e2.getLowerLexIdx()) {
    if (e1.getLowerLexIdx() < lowest_index) lowest_index = e1.getLowerLexIdx();
  }
  else {
    if (e2.getLowerLexIdx() < lowest_index) lowest_index = e2.getLowerLexIdx();
  }
}

void update_highest_index(Edge e1, Edge e2, unsigned int& highest_index) {
  unsigned int e1_higher = e1.getHigherLexIdx();
  unsigned int e2_higher = e2.getHigherLexIdx();
  if (e1_higher > e2_higher) {
    if (e1_higher > highest_index) highest_index = e1_higher;
  }
  else {
    if (e2_higher > highest_index) highest_index = e2_higher;
  }
}

void update_highest_index(Edge2 e1, Edge2 e2, unsigned int& highest_index) {
  unsigned int e1_higher = e1.getHigherLexIdx();
  unsigned int e2_higher = e2.getHigherLexIdx();
  if (e1_higher > e2_higher) {
    if (e1_higher > highest_index) highest_index = e1_higher;
  }
  else {
    if (e2_higher > highest_index) highest_index = e2_higher;
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

// used by: opt2
// function to purely remove edge 'e' from 'edgeS' if it's in there and check the 'before' and 'after' edges for intersection.
enum edge_t removeEdgeFromSet(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  Edge2 before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge2>::iterator it, ibef, iaft;

//  std::cerr << "edge to be removed: " << e << std::endl;
  it = edgeS.find(e);

  if (e != *it) {
    // returned an edge that is different from the one we want to find
    // if the edge in 'it' doesn't cross the LSL (linesweep line) 'it' can still intersect 'e',
    //   verify then fix it and return "E_INTERSECTION"
    // if the edge in 'it' crosses the LSL, it intersects 'e', verify, fix and return "E_INTERSECTION"
    // - we don't find 'e' in this scenario, but as it intersected, we'll remove 'it',
    // 'e' is still technically in 'edgeS' but the points will change vertex indices, and 'e' will have non-incidental vertices
    // which means 'e' gets removed in future vertex validations (see while loop above).
    // if 'e' doesn't intersect 'it' then 'it' should not have returned 'it' but 'edgeS.end()'

    isval = checkIntersection(e, *it);
    if (isval < IS_TRUE) {
      std::cerr << "Removal: find returned a different edge that did not intersect!" << std::endl;
      std::cerr << "e:   " << e << std::endl;
      std::cerr << "*it: " << *it << std::endl;
      valid = E_NOT_VALID;
    }
    else {
      if ((isval == IS_4P_COLLINEAR) || (isval == IS_3P_COLLINEAR)) {
        Edge2 temp_e (*it);
        coll4Swap5(e, temp_e, edgeS, polygon, points);
//        std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
        valid = E_COLLINEAR;
      }
      else if (isval == IS_TRUE) {
        Edge2 temp_e = Edge2(*it);
        flip(e, temp_e, polygon, points);
        softEraseEdgeFromSet(e, edgeS);
        softEraseEdgeFromSet(temp_e, edgeS);
        valid = E_INTERSECTION;
      }
    }
  }
  else {
    // 'e' and 'it' are the same edge, check and clear incidental 'before' and 'after' edges in 'edgeS'.

    // get edges before and after
    if (it != edgeS.begin()) {bef=true;ibef = std::prev(it);}
    if (it != std::prev(edgeS.end())) {af=true;iaft  = std::next(it);}

    // no need to test 'e' against 'ibef' or 'iaft' independently because
    // there's no chance they haven't seen each other, either when either pair of edges
    // were first inserted, or when edges between either pair was removed, and this pair of 'ibef'/'iaft'
    // would have been checked, so only test 'ibef' against 'iaft'.
    if (bef && af) {
      isval = checkIntersection(*ibef, *iaft);
      if ((isval == IS_4P_COLLINEAR)  || (isval == IS_3P_COLLINEAR)) {
  //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        Edge2 t_bef = Edge2((*ibef).p1, (*ibef).p2);
        Edge2 t_aft = Edge2((*iaft).p1, (*iaft).p2);
        coll4Swap5(t_bef, t_aft, edgeS, polygon, points);
  //        std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
        valid = E_COLLINEAR;
      }
      else if (isval == IS_TRUE) {
  //        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        Edge2 t_bef = Edge2((*ibef).p1, (*ibef).p2);
        Edge2 t_aft = Edge2((*iaft).p1, (*iaft).p2);
        flip(t_bef, t_aft, polygon, points);
        edgeS.erase(ibef);
        edgeS.erase(iaft);
        valid = E_INTERSECTION;
      }
    }
    edgeS.erase(it);
  }
  return valid;
}

// used by:
// function to purely remove edge 'e' from 'edgeS' if it's in there and check the 'before' and 'after' edges for intersection.
enum edge_t removeEdgeFromSet(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index) {
  enum edge_t valid = E_VALID;
  Edge2 before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge2>::iterator it, ibef, iaft;

//  std::cerr << "edge to be removed: " << e << std::endl;
  it = edgeS.find(e);

  if (e != *it) {
    // returned an edge that is different from the one we want to find
    // if the edge in 'it' doesn't cross the LSL (linesweep line) 'it' can still intersect 'e',
    //   verify then fix it and return "E_INTERSECTION"
    // if the edge in 'it' crosses the LSL, it intersects 'e', verify, fix and return "E_INTERSECTION"
    // - we don't find 'e' in this scenario, but as it intersected, we'll remove 'it',
    // 'e' is still technically in 'edgeS' but the points will change vertex indices, and 'e' will have non-incidental vertices
    // which means 'e' gets removed in future vertex validations (see while loop above).
    // if 'e' doesn't intersect 'it' then 'it' should not have returned 'it' but 'edgeS.end()'

    isval = checkIntersection(e, *it);
    if (isval < IS_TRUE) {
      std::cerr << "Removal: find returned a different edge that did not intersect!" << std::endl;
      std::cerr << "e:   " << e << std::endl;
      std::cerr << "*it: " << *it << std::endl;
      valid = E_NOT_VALID;
    }
    else {
      if ((isval == IS_4P_COLLINEAR) || (isval == IS_3P_COLLINEAR)) {
        Edge2 temp_e (*it);
        coll4Swap5(e, temp_e, edgeS, polygon, points, lowest_index);
//        std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
        valid = E_COLLINEAR;
      }
      else if (isval == IS_TRUE) {
        Edge2 temp_e = Edge2(*it);
        flip(e, temp_e, polygon, points);
        update_lowest_index(e, temp_e, lowest_index);
        softEraseEdgeFromSet(e, edgeS);
        softEraseEdgeFromSet(temp_e, edgeS);
        valid = E_INTERSECTION;
      }
    }
  }
  else {
    // 'e' and 'it' are the same edge, check and clear incidental 'before' and 'after' edges in 'edgeS'.

    // get edges before and after
    if (it != edgeS.begin()) {bef=true;ibef = std::prev(it);}
    if (it != std::prev(edgeS.end())) {af=true;iaft  = std::next(it);}

    // no need to test 'e' against 'ibef' or 'iaft' independently because
    // there's no chance they haven't seen each other, either when either pair of edges
    // were first inserted, or when edges between either pair was removed, and this pair of 'ibef'/'iaft'
    // would have been checked, so only test 'ibef' against 'iaft'.
    if (bef && af) {
      isval = checkIntersection(*ibef, *iaft);
      if ((isval == IS_4P_COLLINEAR)  || (isval == IS_3P_COLLINEAR)) {
  //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        Edge2 t_bef = Edge2((*ibef).p1, (*ibef).p2);
        Edge2 t_aft = Edge2((*iaft).p1, (*iaft).p2);
        coll4Swap5(t_bef, t_aft, edgeS, polygon, points, lowest_index);
  //        std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
        valid = E_COLLINEAR;
      }
      else if (isval == IS_TRUE) {
  //        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        Edge2 t_bef = Edge2((*ibef).p1, (*ibef).p2);
        Edge2 t_aft = Edge2((*iaft).p1, (*iaft).p2);
        flip(t_bef, t_aft, polygon, points);
        update_lowest_index(t_bef, t_aft, lowest_index);
        edgeS.erase(ibef);
        edgeS.erase(iaft);
        valid = E_INTERSECTION;
      }
    }
    edgeS.erase(it);
  }
  return valid;
}

// used by:
// function to purely remove edge 'e' from 'edgeS' if it's in there and check the 'before' and 'after' edges for intersection.
enum edge_t removeEdgeFromSet(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index) {
  enum edge_t valid = E_VALID;
  Edge2 before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge2>::iterator it, ibef, iaft;

//  std::cerr << "edge to be removed: " << e << std::endl;
  it = edgeS.find(e);

  if (e != *it) {
    // returned an edge that is different from the one we want to find
    // if the edge in 'it' doesn't cross the LSL (linesweep line) 'it' can still intersect 'e',
    //   verify then fix it and return "E_INTERSECTION"
    // if the edge in 'it' crosses the LSL, it intersects 'e', verify, fix and return "E_INTERSECTION"
    // - we don't find 'e' in this scenario, but as it intersected, we'll remove 'it',
    // 'e' is still technically in 'edgeS' but the points will change vertex indices, and 'e' will have non-incidental vertices
    // which means 'e' gets removed in future vertex validations (see while loop above).
    // if 'e' doesn't intersect 'it' then 'it' should not have returned 'it' but 'edgeS.end()'

    isval = checkIntersection(e, *it);
    if (isval < IS_TRUE) {
      std::cerr << "Removal: find returned a different edge that did not intersect!" << std::endl;
      std::cerr << "e:   " << e << std::endl;
      std::cerr << "*it: " << *it << std::endl;
      valid = E_NOT_VALID;
    }
    else {
      if ((isval == IS_4P_COLLINEAR) || (isval == IS_3P_COLLINEAR)) {
        Edge2 temp_e (*it);
        coll4Swap5(e, temp_e, edgeS, polygon, points, lowest_index, highest_index);
//        std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
        valid = E_COLLINEAR;
      }
      else if (isval == IS_TRUE) {
        Edge2 temp_e = Edge2(*it);
        flip(e, temp_e, polygon, points);
        update_lowest_index(e, temp_e, lowest_index);
        update_highest_index(e, temp_e, highest_index);
        softEraseEdgeFromSet(e, edgeS);
        softEraseEdgeFromSet(temp_e, edgeS);
        valid = E_INTERSECTION;
      }
    }
  }
  else {
    // 'e' and 'it' are the same edge, check and clear incidental 'before' and 'after' edges in 'edgeS'.

    // get edges before and after
    if (it != edgeS.begin()) {bef=true;ibef = std::prev(it);}
    if (it != std::prev(edgeS.end())) {af=true;iaft  = std::next(it);}

    // no need to test 'e' against 'ibef' or 'iaft' independently because
    // there's no chance they haven't seen each other, either when either pair of edges
    // were first inserted, or when edges between either pair was removed, and this pair of 'ibef'/'iaft'
    // would have been checked, so only test 'ibef' against 'iaft'.
    if (bef && af) {
      isval = checkIntersection(*ibef, *iaft);
      if ((isval == IS_4P_COLLINEAR)  || (isval == IS_3P_COLLINEAR)) {
  //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        Edge2 t_bef = Edge2((*ibef).p1, (*ibef).p2);
        Edge2 t_aft = Edge2((*iaft).p1, (*iaft).p2);
        coll4Swap5(t_bef, t_aft, edgeS, polygon, points);
  //        std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
        valid = E_COLLINEAR;
      }
      else if (isval == IS_TRUE) {
  //        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        Edge2 t_bef = Edge2((*ibef).p1, (*ibef).p2);
        Edge2 t_aft = Edge2((*iaft).p1, (*iaft).p2);
        flip(t_bef, t_aft, polygon, points);
        update_lowest_index(t_bef, t_aft, lowest_index);
        update_highest_index(t_bef, t_aft, highest_index);
        edgeS.erase(ibef);
        edgeS.erase(iaft);
        valid = E_INTERSECTION;
      }
    }
    edgeS.erase(it);
  }
  return valid;
}

/*
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
*/
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

// function for opt2e.
// BEHAVIOUR:
// erases edge from set, checks before/after edges for intersection, if collinear does the coll4swap4,
// if intersects, flips before and after and reinserts an edge if it passes through the next indices linesweep line
// (because 2opte continues to the next index without restarting.)
enum edge_t removeEdgeFromSete(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
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

        //preparing for insertion of the edge not connected to 'idx' into 'edgeS'
        //std::vector<unsigned int> vertices {(*before.p1).v, (*before.p2).v, (*after.p1).v, (*after.p2).v};

        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(before, after, lowest_index);
/*
        //code to insert the new edge not connected to 'idx' into 'edgeS' if it crosses the linesweep line.
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
      		if (points[polygon[vertices[i]]].l == (*idx).l) {
      			if (i < 2) new_edge.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);
      			else new_edge.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
      			break;
      		}
      	}

      	if (((*new_edge.p1).l < (*idx).l) && ((*idx).l+1 < (*new_edge.p2).l)) {
      		std::cerr << "'before/after' flip insertion: " << new_edge << std::endl;
      		std::cerr << "at idx: " << *idx << std::endl;
          std::cerr << "edges in 'edgeS':" << std::endl;
          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

      		std::pair<std::set<Edge>::iterator,bool> retval;
          retval.first = edgeS.find(new_edge);
          if (retval.first == edgeS.end()) {
            retval = edgeS.insert(new_edge);
            std::cerr << "edge added to 'edgeS':" << std::endl;
            for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

        		if (*retval.first != new_edge) {
        			std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
        		}
          }
      	}
*/
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

// function for opt2f, that needs to use coll3sort2 in its coll4swap..
enum edge_t removeEdgeFromSetf(Edge& e, Point *idx, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
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

        //preparing for insertion of the edge not connected to 'idx' into 'edgeS'
        std::vector<unsigned int> vertices {(*before.p1).v, (*before.p2).v, (*after.p1).v, (*after.p2).v};

        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(before, after, lowest_index);

        //code to insert the new edge not connected to 'idx' into 'edgeS' if it crosses the linesweep line.
        Edge new_edge1, new_edge2;
        new_edge1.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
        new_edge2.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);

        if (((*new_edge1.p1).l < (*idx).l) && ((*idx).l < (*new_edge1.p2).l)) {
//          std::cerr << "'before/after' flip insertion: " << new_edge1 << std::endl;
//          std::cerr << "at idx: " << *idx << std::endl;
//          std::cerr << "edges in 'edgeS':" << std::endl;
//          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

          std::pair<std::set<Edge>::iterator,bool> retval;
          retval = edgeS.insert(new_edge1);
//          std::cerr << "edge added to 'edgeS':" << std::endl;
//          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
          if (*retval.first != new_edge1) {
            std::cerr << "Error!  inserting: " << new_edge1 << ", returned: " << *retval.first << std::endl;
          }
        }

        if (((*new_edge2.p1).l < (*idx).l) && ((*idx).l < (*new_edge2.p2).l)) {
//          std::cerr << "'before/after' flip insertion: " << new_edge2 << std::endl;
//          std::cerr << "at idx: " << *idx << std::endl;
//          std::cerr << "edges in 'edgeS':" << std::endl;
//          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

          std::pair<std::set<Edge>::iterator,bool> retval;
          retval = edgeS.insert(new_edge2);
//          std::cerr << "edge added to 'edgeS':" << std::endl;
//          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
          if (*retval.first != new_edge2) {
            std::cerr << "Error!  inserting: " << new_edge2 << ", returned: " << *retval.first << std::endl;
          }
        }

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
            //preparing for insertion of the edge not connected to 'idx' into 'edgeS'
            std::vector<unsigned int> vertices {(*before.p1).v, (*before.p2).v, (*after.p1).v, (*after.p2).v};

            eraseEdgeFromSet(before, edgeS);
            eraseEdgeFromSet(after, edgeS);
            flip(before, after, polygon, points);
            valid = E_INTERSECTION;
            update_lowest_index(before, after, lowest_index);

            //code to insert the new edge not connected to 'idx' into 'edgeS' if it crosses the linesweep line.
            Edge new_edge1, new_edge2;
            new_edge1.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
            new_edge2.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);

            if (((*new_edge1.p1).l < (*idx).l) && ((*idx).l < (*new_edge1.p2).l)) {
    //          std::cerr << "'before/after' flip insertion: " << new_edge1 << std::endl;
    //          std::cerr << "at idx: " << *idx << std::endl;
    //          std::cerr << "edges in 'edgeS':" << std::endl;
    //          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

              std::pair<std::set<Edge>::iterator,bool> retval;
              retval = edgeS.insert(new_edge1);
    //          std::cerr << "edge added to 'edgeS':" << std::endl;
    //          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
              if (*retval.first != new_edge1) {
                std::cerr << "Error!  inserting: " << new_edge1 << ", returned: " << *retval.first << std::endl;
              }
            }

            if (((*new_edge2.p1).l < (*idx).l) && ((*idx).l < (*new_edge2.p2).l)) {
    //          std::cerr << "'before/after' flip insertion: " << new_edge2 << std::endl;
    //          std::cerr << "at idx: " << *idx << std::endl;
    //          std::cerr << "edges in 'edgeS':" << std::endl;
    //          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

              std::pair<std::set<Edge>::iterator,bool> retval;
              retval = edgeS.insert(new_edge2);
    //          std::cerr << "edge added to 'edgeS':" << std::endl;
    //          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
              if (*retval.first != new_edge2) {
                std::cerr << "Error!  inserting: " << new_edge2 << ", returned: " << *retval.first << std::endl;
              }
            }
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

// function for opt2h.
enum edge_t removeEdgeFromSeth(Edge2& e, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;

  Edge2 before, after;
  bool bef=false, af=false;
  enum intersect_t isval;
  std::set<Edge2>::iterator it, ibef, iaft;

  // find the edge, erase invalid edges, or if the edge wasn't found, return.
  do {
    it = edgeS.find(e);
    if (it != edgeS.end()) {
      // if the vertex indices are not incidental, remove the edge in it' and find 'e' again
      if (((*it).getPHigh() - (*it).getPLow() == 1) || (((*it).getPHigh() == polygon.size()-1) && ((*it).getPLow() == 0))) {
        break;
      }
      else edgeS.erase(it);
    }
    else return E_VALID; // edge not found, if it's hidden, it'll be found and handled in the future.
  } while (true);

  if (e != *it) {
    // returned an edge that is different from the one we want to find
    // if the edge in 'it' doesn't cross the LSL (linesweep line) 'it' can still intersect 'e',
    //   verify then fix it and return "E_INTERSECTION"
    // if the edge in 'it' crosses the LSL, it intersects 'e', verify, fix and return "E_INTERSECTION"
    // - we don't find 'e' in this scenario, but as it intersected, we'll remove 'it',
    // 'e' is still technically in 'edgeS' but the points will change vertex indices, and 'e' will have non-incidental vertices
    // which means 'e' gets removed in future vertex validations (see while loop above).
    // if 'e' doesn't intersect 'it' then 'it' should not have returned 'it' but 'edgeS.end()'

    isval = checkIntersection(e, *it);
    if (isval < IS_TRUE) {
      std::cerr << "Removal: find returned a different edge that did not intersect!" << std::endl;
      std::cerr << "e:   " << e << std::endl;
      std::cerr << "*it: " << *it << std::endl;
      valid = E_NOT_VALID;
    }
    else {
      if ((isval == IS_4P_COLLINEAR) || (isval == IS_3P_COLLINEAR)) {
        Edge2 temp_e (*it);
        coll4Swap5(e, temp_e, edgeS, polygon, points, lowest_index, highest_index);
//        std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
        valid = E_COLLINEAR;
      }
      else if (isval == IS_TRUE) {
        Edge2 temp_e = Edge2(*it);
        flip(e, temp_e, polygon, points);
        update_lowest_index(e, temp_e, lowest_index);
        update_highest_index(e, temp_e, highest_index);
        valid = E_INTERSECTION;
      }
      edgeS.erase(it);
    }
  }
  else {
    // 'e' and 'it' are the same edge, check and clear incidental 'before' and 'after' edges in 'edgeS'.

    // get edges before and after
    do { // validate the edge.
      if (it != edgeS.begin()) {
        ibef = std::prev(it);
        if (((*ibef).getPHigh() - (*ibef).getPLow() == 1) || (((*ibef).getPHigh() == polygon.size()-1) && ((*ibef).getPLow() == 0))) {
          bef = true;
          break;
        }
        else { // edge isn't valid, remove and check next one.
          edgeS.erase(ibef);
        }
      }
      else break;
    } while (true);

    do {
      if (it != std::prev(edgeS.end())) {
        iaft  = std::next(it);
        if (((*iaft).getPHigh() - (*iaft).getPLow() == 1) || (((*iaft).getPHigh() == polygon.size()-1) && ((*iaft).getPLow() == 0))) {
          af = true;
          break;
        }
        else {
          edgeS.erase(iaft);
        }
      }
      else break;
    } while (true);

    // no need to test 'e' against 'ibef' or 'iaft' independently because
    // there's no chance they haven't seen each other, either when either pair of edges
    // were first inserted, or when edges between either pair was removed, and this pair of 'ibef'/'iaft'
    // would have been checked, so only test 'ibef' against 'iaft'.
    if (bef && af) {
      isval = checkIntersection(*ibef, *iaft);
      if ((isval == IS_4P_COLLINEAR)  || (isval == IS_3P_COLLINEAR)) {
  //        std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        Edge2 t_bef = Edge2((*ibef).p1, (*ibef).p2);
        Edge2 t_aft = Edge2((*iaft).p1, (*iaft).p2);
        coll4Swap5(t_bef, t_aft, edgeS, polygon, points, lowest_index, highest_index);
  //        std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
        valid = E_COLLINEAR;
        edgeS.erase(ibef);
        edgeS.erase(iaft);
      }
      else if (isval == IS_TRUE) {
  //        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        Edge2 t_bef = Edge2((*ibef).p1, (*ibef).p2);
        Edge2 t_aft = Edge2((*iaft).p1, (*iaft).p2);
        flip(t_bef, t_aft, polygon, points);
        update_lowest_index(*ibef, *iaft, lowest_index);
        update_highest_index(*ibef, *iaft, highest_index);
        valid = E_INTERSECTION;
        edgeS.erase(ibef);
        edgeS.erase(iaft);
      }
    }
    edgeS.erase(it);
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

std::pair<enum edge_t, std::set<Edge2>::iterator> processEdge(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge2 before, after;
  std::set<Edge2>::iterator it, it_bef, it_aft;
  std::pair<std::set<Edge2>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge2>::iterator> retval2;

//  std::cerr << "edge to be inserted: " << e << std::endl;
  retval = edgeS.insert(e);

  if (*retval.first != e) {
    std::cerr << "e : " << e << std::endl;
    std::cerr << "retval.first : " << *retval.first << std::endl;
	  std::cerr << "retval.second: " << retval.second << std::endl;
    std::cerr << "edges in 'edgeS':" << std::endl;
    for (std::set<Edge2>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
    retval2.first = valid;
    retval2.second = retval.first;
    return retval2;
  }

  //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
  if (retval.first != edgeS.begin()) {
    it_bef = std::prev(retval.first);
    before = *it_bef;
    //std::cerr << "before: " << before << std::endl;
    bef = true;
  }
  //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
  if (retval.first != --edgeS.end()) {
    it_aft = std::next(retval.first);
    after  = *it_aft;
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
    else if ((isval == IS_4P_COLLINEAR) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "4P collinearity between e:" << e << " and bef: " << before << std::endl;
      if (coll4Swap5(e, before, edgeS, polygon, points)) {
//        std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
        valid = E_COLLINEAR;
      }
    }
    else {
//      std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
      softEraseEdgeFromSet(e, edgeS);
      softEraseEdgeFromSet(before, edgeS);
      flip(e, before, polygon, points);
      valid = E_INTERSECTION;
    }
  }
  // check incidental edge 'after' if it intersects with 'e'
  if (af && (valid == E_VALID)) {
    isval = checkIntersection(e, after);
    if (isval < IS_TRUE) {
//      std::cerr << "No intersection." << std::endl;
      valid = E_VALID;
    }
    else if ((isval == IS_4P_COLLINEAR) || (isval == IS_3P_COLLINEAR)) {
//      std::cerr << "4P collinearity between:" << e << " and aft: " << after << std::endl;
      if (coll4Swap5(e, after, edgeS, polygon, points)) {
//        std::cerr << "4P coll. after swap: " << e << " and " << after << std::endl;
        valid = E_COLLINEAR;
      }
    }
    else if (isval == IS_TRUE) {
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
      softEraseEdgeFromSet(e, edgeS);
      softEraseEdgeFromSet(after, edgeS);
      flip(e, after, polygon, points);
      valid = E_INTERSECTION;
    }
    else valid = E_NOT_VALID;
  }

  retval2.first = valid;
  retval2.second = retval.first;
  return retval2;
}

std::pair<enum edge_t, std::set<Edge2>::iterator> processEdge(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge2 before, after;
  std::set<Edge2>::iterator it, it_bef, it_aft;
  std::pair<std::set<Edge2>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge2>::iterator> retval2;

//  std::cerr << "edge to be inserted: " << e << std::endl;
  retval = edgeS.insert(e);

  if (*retval.first != e) {
    std::cerr << "e : " << e << std::endl;
    std::cerr << "retval.first : " << *retval.first << std::endl;
	  std::cerr << "retval.second: " << retval.second << std::endl;
    std::cerr << "edges in 'edgeS':" << std::endl;
    for (std::set<Edge2>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
    retval2.first = valid;
    retval2.second = retval.first;
    return retval2;
  }

  //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
  if (retval.first != edgeS.begin()) {
    it_bef = std::prev(retval.first);
    before = *it_bef;
    //std::cerr << "before: " << before << std::endl;
    bef = true;
  }
  //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
  if (retval.first != --edgeS.end()) {
    it_aft = std::next(retval.first);
    after  = *it_aft;
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
    else if ((isval == IS_4P_COLLINEAR) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "4P collinearity between e:" << e << " and bef: " << before << std::endl;
      if (coll4Swap5(e, before, edgeS, polygon, points, lowest_index)) {
//        std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
        valid = E_COLLINEAR;
      }
    }
    else {
//      std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
      flip(e, before, polygon, points);
      update_lowest_index(e, before, lowest_index);
      softEraseEdgeFromSet(e, edgeS);
      softEraseEdgeFromSet(before, edgeS);
      valid = E_INTERSECTION;
    }
  }
  // check incidental edge 'after' if it intersects with 'e'
  if (af && (valid == E_VALID)) {
    isval = checkIntersection(e, after);
    if (isval < IS_TRUE) {
//      std::cerr << "No intersection." << std::endl;
      valid = E_VALID;
    }
    else if ((isval == IS_4P_COLLINEAR) || (isval == IS_3P_COLLINEAR)) {
//      std::cerr << "4P collinearity between:" << e << " and aft: " << after << std::endl;
      if (coll4Swap5(e, after, edgeS, polygon, points, lowest_index)) {
//        std::cerr << "4P coll. after swap: " << e << " and " << after << std::endl;
        valid = E_COLLINEAR;
      }
    }
    else if (isval == IS_TRUE) {
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
      flip(e, after, polygon, points);
      update_lowest_index(e, after, lowest_index);
      softEraseEdgeFromSet(e, edgeS);
      softEraseEdgeFromSet(after, edgeS);
      valid = E_INTERSECTION;
    }
    else valid = E_NOT_VALID;
  }

  retval2.first = valid;
  retval2.second = retval.first;
  return retval2;
}

std::pair<enum edge_t, std::set<Edge2>::iterator> processEdge(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge2 before, after;
  std::set<Edge2>::iterator it, it_bef, it_aft;
  std::pair<std::set<Edge2>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge2>::iterator> retval2;

//  std::cerr << "edge to be inserted: " << e << std::endl;
  retval = edgeS.insert(e);

  if (*retval.first != e) {
    std::cerr << "e : " << e << std::endl;
    std::cerr << "retval.first : " << *retval.first << std::endl;
	  std::cerr << "retval.second: " << retval.second << std::endl;
    std::cerr << "edges in 'edgeS':" << std::endl;
    for (std::set<Edge2>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
    retval2.first = valid;
    retval2.second = retval.first;
    return retval2;
  }

  //std::cerr << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
  if (retval.first != edgeS.begin()) {
    it_bef = std::prev(retval.first);
    before = *it_bef;
    //std::cerr << "before: " << before << std::endl;
    bef = true;
  }
  //std::cerr << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
  if (retval.first != --edgeS.end()) {
    it_aft = std::next(retval.first);
    after  = *it_aft;
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
    else if ((isval == IS_4P_COLLINEAR) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "4P collinearity between e:" << e << " and bef: " << before << std::endl;
      if (coll4Swap5(e, before, edgeS, polygon, points, lowest_index, highest_index)) {
//        std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
        valid = E_COLLINEAR;
      }
    }
    else {
//      std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
      flip(e, before, polygon, points);
      update_lowest_index(e, before, lowest_index);
      update_highest_index(e, before, highest_index);
      softEraseEdgeFromSet(e, edgeS);
      softEraseEdgeFromSet(before, edgeS);
      valid = E_INTERSECTION;
    }
  }
  // check incidental edge 'after' if it intersects with 'e'
  if (af && (valid == E_VALID)) {
    isval = checkIntersection(e, after);
    if (isval < IS_TRUE) {
//      std::cerr << "No intersection." << std::endl;
      valid = E_VALID;
    }
    else if ((isval == IS_4P_COLLINEAR) || (isval == IS_3P_COLLINEAR)) {
//      std::cerr << "4P collinearity between:" << e << " and aft: " << after << std::endl;
      if (coll4Swap5(e, after, edgeS, polygon, points, lowest_index, highest_index)) {
//        std::cerr << "4P coll. after swap: " << e << " and " << after << std::endl;
        valid = E_COLLINEAR;
      }
    }
    else if (isval == IS_TRUE) {
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
      flip(e, after, polygon, points);
      update_lowest_index(e, after, lowest_index);
      update_highest_index(e, after, highest_index);
      softEraseEdgeFromSet(e, edgeS);
      softEraseEdgeFromSet(after, edgeS);
      valid = E_INTERSECTION;
    }
    else valid = E_NOT_VALID;
  }

  retval2.first = valid;
  retval2.second = retval.first;
  return retval2;
}


/*
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
*/

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
// as '2opte' advances to next index no matter what, the re-insertion of the new edge after a flip must be checked against the next index.
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgee(Edge& e, Point *idx, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge before, after;
  std::pair<std::set<Edge>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge>::iterator> retval2;

  retval = edgeS.insert(e);

/*
  // Edge comparison changes test:
  // this was done to analyse why the edges weren't always ordered the same way in 'edgeS'
  // final result was that it can't be done with intersecting edges.. you will always have the
  // comparison give a relative comparison and not an absolute.
  bool b_old, b_new;
  int counter = 0;
  b_old = e < *edgeS.begin();
  b_new = b_old;
  for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) {
    b_new = e < *it;
    if (b_old != b_new) {
      ++counter;
    }
    b_old = b_new;
  }
  if (counter > 1) {
    std::cerr << "Error!  edge comparison problem!" << std::endl;
    std::cerr << "e: " << e << std::endl;
    std::cerr << "edges in 'edgeS':" << std::endl;
    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << " : " << ((e < *it) ? "true" : "false") << std::endl;
    exit(1);
  }
*/

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
        //std::vector<unsigned int> vertices {(*e.p1).v, (*e.p2).v, (*before.p1).v, (*before.p2).v};

        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip(e, before, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, before, lowest_index);
/*
        //code to insert the edge not connected to 'idx' into 'edgeS'
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
      		if (points[polygon[vertices[i]]].l == (*idx).l) {
      			if (i < 2) new_edge.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);
      			else new_edge.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
      			break;
      		}
      	}

      	if (((*new_edge.p1).l < (*idx).l) && ((*idx).l+1 < (*new_edge.p2).l)) {
      		std::cerr << "'before' flip insertion: " << new_edge << std::endl;
      		std::cerr << "at idx: " << *idx << std::endl;
          std::cerr << "edges in 'edgeS':" << std::endl;
          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

      		std::pair<std::set<Edge>::iterator,bool> retval;
          retval.first = edgeS.find(new_edge);
          if (retval.first == edgeS.end()) {
            retval = edgeS.insert(new_edge);
            std::cerr << "edge added to 'edgeS':" << std::endl;
            for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
        		if (*retval.first != new_edge) {
        			std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
        		}
          }
      	}
*/
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
        //std::vector<unsigned int> vertices {(*e.p1).v, (*e.p2).v, (*after.p1).v, (*after.p2).v};
//        std::cerr << "vertices before flip:" << std::endl;
//        for (unsigned int i = 0; i < vertices.size();++i) std::cerr << "i:" << i << " : " << vertices[i] << std::endl;

        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSetb(e, edgeS, polygon, points);
        //removeEdgeFromSetb(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, after, lowest_index);
/*
        //code to insert the edge not connected to 'idx' into 'edgeS'
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
      		if (points[polygon[vertices[i]]].l == (*idx).l) {
      			if (i < 2) new_edge.set(points[polygon[vertices[2]]], points[polygon[vertices[3]]]);
      			else new_edge.set(points[polygon[vertices[0]]], points[polygon[vertices[1]]]);
      			break;
      		}
      	}

      	if (((*new_edge.p1).l < (*idx).l) && ((*idx).l+1 < (*new_edge.p2).l)) {
      		std::cerr << "'after' flip insertion: " << new_edge << std::endl;
      		std::cerr << "at idx: " << *idx << std::endl;
          std::cerr << "edges in 'edgeS':" << std::endl;
          for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

      		std::pair<std::set<Edge>::iterator,bool> retval;
          retval.first = edgeS.find(new_edge);
          if(retval.first == edgeS.end()) {
            retval = edgeS.insert(new_edge);
            std::cerr << "edge added to 'edgeS':" << std::endl;
            for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

      		  if (*retval.first != new_edge) {
      			  std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      		  }
          }
      	}
*/
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
//        std::cerr << "after flip: e: " << e << ", after: " << after << std::endl;

        //code to insert the edge not connected to 'idx' into 'edgeS'
      	Edge new_edge;
      	for (unsigned int i = 0; i < vertices.size();++i) {
//          std::cerr << "point: " << points[polygon[vertices[i]]] << std::endl;
//          std::cerr << "idx: " << *idx << std::endl;
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

// for reversal algorithm opt2f, when it finds an intersection, it inserts the flipped edge that does not connect to the current point.
// as the edge connected to the current point is handled in the index restart.
std::pair<enum edge_t, std::set<Edge2>::iterator> processEdgeh(Edge2& e, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  Edge2 before, after;
  std::pair<std::set<Edge2>::iterator,bool> retval;
  std::pair<enum edge_t, std::set<Edge2>::iterator> retval2;

  retval = edgeS.insert(e);

  if (*retval.first != e) {
    std::cerr << "retval.first : " << *retval.first << std::endl;
	   std::cerr << "retval.second: " << retval.second << std::endl;
     std::cerr << "edges in 'edgeS':" << std::endl;
     for (std::set<Edge2>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
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
        if (coll4Swap5(e, before, edgeS, polygon, points, lowest_index, highest_index)) {
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;

        edgeS.erase(retval.first); //erasing 'e'
        softEraseEdgeFromSet(before, edgeS);
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
//        std::cerr << "4P collinearity between:" << e << " and after: " << after << std::endl;
        if (coll4Swap5(e, after, edgeS, polygon, points, lowest_index, highest_index)) {
//          std::cerr << "coll. after swap: " << e << " and " << after << std::endl;
          valid = E_COLLINEAR;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;

        // preparing for insertion of the edge not connected to 'idx' into 'edgeS'
        // the vertices do not change, the lower 2 vertices are still connected together
        // and the higher 2 vertices are still connected together.
        std::vector<unsigned int> vertices {(*e.p1).v, (*e.p2).v, (*after.p1).v, (*after.p2).v};
//        std::cerr << "vertices before flip:" << std::endl;
//        for (unsigned int i = 0; i < vertices.size();++i) std::cerr << "i:" << i << " : " << vertices[i] << std::endl;

        edgeS.erase(retval.first); // erasing 'e'
        softEraseEdgeFromSet(after, edgeS);
        flip(e, after, polygon, points);
        valid = E_INTERSECTION;
        update_lowest_index(e, after, lowest_index);
        update_highest_index(e, after, highest_index);
//        std::cerr << "after flip: e: " << e << ", after: " << after << std::endl;
      }
    }

  } else {
    // edge already existed in set.
    // this can happen if an index is going backwards through the lex. order of points.
    // but if it's reversing and it hits an edge already in, earlier code should have caught it and removed it.
    std::cerr << "Processing error: Edge already exists in set!" << std::endl;
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

/*
// for reversal algorithm opt2f, when it finds an intersection, it inserts the flipped edge that does not connect to the current point.
// as the edge connected to the current point is handled in the index restart.
enum edge_t processEdgeh(Edge2& e, unsigned int& lowest_index, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  enum edge_t valid = E_VALID;
  enum intersect_t isval;
  bool bef = false, af = false;
  std::set<Edge2>::iterator ibef, iaft;
  std::pair<std::set<Edge2>::iterator,bool> retval;

  // inserting will either i) return 'e' or ii) return an edge that intersects with 'e' that could either:
  // 1) be a real edge, or 2) be invalid (points not incidental in polygon).
  do { // clear out invalid edges
    retval = edgeS.insert(e);
    if (((*retval.first).getPHigh() - (*retval.first).getPLow() == 1) || (((*retval.first).getPHigh() == polygon.size()-1) && ((*retval.first).getPLow() == 0))) {
      break;
    }
    else edgeS.erase(retval.first);
  } while (true);


  if (*retval.first != e) {
    isval = checkIntersection(e, *retval.first);
    if (isval < IS_TRUE) {
      std::cerr << "Insertion: insert returned a different edge that did not intersect!" << std::endl;
      std::cerr << "e:   " << e << std::endl;
      std::cerr << "*it: " << *retval.first << std::endl;

      if (retval.first != edgeS.begin()) {
        ibef = std::prev(retval.first);
        std::cerr << "edge before insertion: " << *ibef << std::endl;
        isval = checkIntersection(e, *ibef);
        std::cerr << "intersect check: "; printEnum(isval); std::cerr << std::endl;
      }

      if (retval.first != std::prev(edgeS.end())) {
        iaft  = std::next(retval.first);
        std::cerr << "edge after insertion: " << *ibef << std::endl;
        isval = checkIntersection(e, *iaft);
        std::cerr << "intersect check: "; printEnum(isval); std::cerr << std::endl;
      }
      std::cerr << "edges in 'edgeS':" << std::endl;
      for (std::set<Edge2>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

      valid = E_NOT_VALID;
    }
    else {
      if ((isval == IS_4P_COLLINEAR) || (isval == IS_3P_COLLINEAR)) {
        Edge2 t_ret = Edge2((*retval.first).p1, (*retval.first).p2);
        coll4Swap5(e, t_ret, edgeS, polygon, points, lowest_index);
//        std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
        valid = E_COLLINEAR;
      }
      else if (isval == IS_TRUE) {
        Edge2 t_ret = Edge2((*retval.first).p1, (*retval.first).p2);
        flip(e, t_ret, polygon, points);
        update_lowest_index(e, *retval.first, lowest_index);
        valid = E_INTERSECTION;
      }
      edgeS.erase(retval.first);
    }
  }
  else {
    // same edge, check if 'e' intersects neigbours in 'edgeS'

    // get the edge before 'e' and validate it.
    do {
      if (retval.first != edgeS.begin()) {
        ibef = std::prev(retval.first);
        if (((*ibef).getPHigh() - (*ibef).getPLow() == 1) || (((*ibef).getPHigh() == polygon.size()-1) && ((*ibef).getPLow() == 0))) {
          bef = true;
          break;
        }
        else { // edge isn't valid, remove and check next one.
          edgeS.erase(ibef);
        }
      }
      else break;
    } while (true);

    if (bef) {
      isval = checkIntersection(e, *ibef);
      if (isval < IS_TRUE) {
//        std::cerr << "No intersection with before." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and before: " << before << std::endl;
        Edge2 t_bef = Edge2((*ibef).p1, (*ibef).p2);
        coll4Swap5(e, t_bef, edgeS, polygon, points, lowest_index);
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
        valid = E_COLLINEAR;
        edgeS.erase(ibef);
        edgeS.erase(retval.first);
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
//        std::cerr << "idx: " << *idx << std::endl;
        Edge2 t_bef = Edge2((*ibef).p1, (*ibef).p2);
        flip(e, t_bef, polygon, points);
        update_lowest_index(e, *ibef, lowest_index);
        valid = E_INTERSECTION;
        edgeS.erase(ibef);
        edgeS.erase(retval.first);
      }
    }

    if (valid == E_VALID) { // 'e' and 'before' didn't intersect, safe to check 'e' and 'after'

      do { // get and validate the edge after 'e'
        if (retval.first != std::prev(edgeS.end())) {
          iaft  = std::next(retval.first);
          if (((*iaft).getPHigh() - (*iaft).getPLow() == 1) || (((*iaft).getPHigh() == polygon.size()-1) && ((*iaft).getPLow() == 0))) {
            af = true;
            break;
          }
          else {
            edgeS.erase(iaft);
          }
        }
        else break;
      } while (true);

      if (af) {
        isval = checkIntersection(e, *iaft);
        if (isval < IS_TRUE) {
  //        std::cerr << "No intersection with before." << std::endl;
          valid = E_VALID;
        }
        else if (isval == IS_4P_COLLINEAR) {
  //        std::cerr << "4P collinearity between:" << e << " and before: " << before << std::endl;
          Edge2 t_aft = Edge2((*iaft).p1, (*iaft).p2);
          coll4Swap5(e, t_aft, edgeS, polygon, points, lowest_index);
  //        std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_COLLINEAR;
          edgeS.erase(iaft);
          edgeS.erase(retval.first);
        }
        else {
  //        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
  //        std::cerr << "idx: " << *idx << std::endl;
          Edge2 t_aft = Edge2((*iaft).p1, (*iaft).p2);
          flip(e, t_aft, polygon, points);
          update_lowest_index(e, *iaft, lowest_index);
          valid = E_INTERSECTION;
          edgeS.erase(iaft);
          edgeS.erase(retval.first);
        }
      }
    }
  }
  return valid;
}
*/
