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
        //std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (coll4Swap(before, after, edgeS, polygon, points)) {
          //std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_SKIP;
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_SKIP;
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
        edgeS.erase(it1);
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

//  std::cerr << "edge to be removed: " << e << std::endl;
  it = edgeS.find(e);

  if (it != edgeS.end()) {
//    std::cerr << "*it: " << *it << std::endl;
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
        //std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (coll4Swap(before, after, edgeS, polygon, points)) {
          //std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_SKIP;
          update_lowest_index(before, after, lowest_index);
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip(before, after, polygon, points);
        valid = E_SKIP;
        update_lowest_index(before, after, lowest_index);
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
        edgeS.erase(it1);
        break;
      }
    }
  }
  if (valid == E_SKIP) {
    if (e.getLowerLexIdx() < before.getLowerLexIdx()) lowest_index = e.getLowerLexIdx();
    else lowest_index = before.getLowerLexIdx();
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

//  std::cerr << "edge to be removed: " << e << std::endl;
  it = edgeS.find(e);

  if (it != edgeS.end()) {
//    std::cerr << "*it: " << *it << std::endl;
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
        //std::cerr << "4P collinearity between:" << before << " and " << after << std::endl;
        if (coll4Swap(before, after, edgeS, polygon, points)) {
          //std::cerr << "4P coll. after swap: " << before << " and " << after << std::endl;
          valid = E_SKIP;
          update_lowest_index(before, after, lowest_index);
        }
      }
      else if ((isval == IS_TRUE) || (isval == IS_3P_COLLINEAR)) {
//        std::cerr << "Intersection between 'before': " << before << ", and 'after': " << after << std::endl;
        eraseEdgeFromSet(before, edgeS);
        eraseEdgeFromSet(after, edgeS);
        flip2(before, after, polygon);
        valid = E_SKIP;
        update_lowest_index(before, after, lowest_index);
      } else if (isval >= IS_TRUE) {
        std::cerr << "Error!  Unhandled exception in removal of 'before': " << before << ", and 'after': " << after << std::endl;
      }
    }
  } else {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    // this is technically a crutch because there's a problem with the comparator function.
    for (std::set<Edge>::iterator it1=edgeS.begin(); it1!=edgeS.end(); ++it1) {
      if (*it1 == e) {
        edgeS.erase(it1);
        break;
      }
    }
  }
  if (valid == E_SKIP) {
    if (e.getLowerLexIdx() < before.getLowerLexIdx()) lowest_index = e.getLowerLexIdx();
    else lowest_index = before.getLowerLexIdx();
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
  //std::set<Edge, setComp>::key_compare mycomp = edgeS.key_comp();

  retval = edgeS.insert(e);
/*
  if (*retval.first != e) {
    std::cerr << "retval.first : " << *retval.first << std::endl;
	   std::cerr << "retval.second: " << retval.second << std::endl;
     std::cout << "edges in 'edgeS':" << std::endl;
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
        //std::cerr << "No intersection." << std::endl;
        valid = E_VALID;
      }
      else if (isval == IS_4P_COLLINEAR) {
//        std::cerr << "4P collinearity between:" << e << " and bef: " << before << std::endl;
        if (coll4Swap(e, before, edgeS, polygon, points)) {
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_SKIP;
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip(e, before, polygon, points);
        valid = E_SKIP;
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
//          std::cerr << "4P coll. after swap: " << e << " and " << after << std::endl;
          valid = E_SKIP;
        }
      }
      else {
        //std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSet(e, edgeS, polygon, points);
        //removeEdgeFromSet(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_SKIP;
      }
    }

  }
  else {
    // edge already existed in set.
    // this can happen if an index was reset when inserting 'e2' and we're dealing with insertion of e1 again.
    std::cerr << "Error: Edge already exists in set! : " << *retval.first << std::endl;
    valid = E_NOT_VALID;
  }
  retval2.first = valid;
  retval2.second = retval.first;
  return retval2;
}
// updates lowest_index
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgeb(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
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
     std::cout << "edges in 'edgeS':" << std::endl;
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
//        std::cerr << "4P collinearity between:" << e << " and before: " << before << std::endl;
        if (coll4Swap(e, before, edgeS, polygon, points)) {
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_SKIP;
          update_lowest_index(e, before, lowest_index);
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip(e, before, polygon, points);
        valid = E_SKIP;
        update_lowest_index(e, before, lowest_index);
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
//        std::cerr << "4P collinearity between:" << e << " and aft: " << after << std::endl;
        if (coll4Swap(e, after, edgeS, polygon, points)) {
//          std::cerr << "4P coll. after swap: " << e << " and " << after << std::endl;
          valid = E_SKIP;
          update_lowest_index(e, after, lowest_index);
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSetb(e, edgeS, polygon, points);
        //removeEdgeFromSetb(after, edgeS, polygon, points);
        flip(e, after, polygon, points);
        valid = E_SKIP;
        update_lowest_index(e, after, lowest_index);
      }
    }

  } else {
    // edge already existed in set.
    // this can happen if an index is going backwards through the lex. order of points.
    // but if it's reversing and it hits an edge already in, earlier code should have caught it and removed it.
    std::cerr << "Error: Edge already exists in set!" << std::endl;
    std::cerr << "edge: " << e << ", returned: " << *retval.first << std::endl;
//    std::cout << "edges in 'edgeS':" << std::endl;
//    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
  }

  retval2.first = valid;
  if (valid != E_VALID) retval2.second = edgeS.end();
  else retval2.second = retval.first;

  return retval2;
}
// updates lowest_index and uses a different flip function
std::pair<enum edge_t, std::set<Edge>::iterator> processEdged(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
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
     std::cout << "edges in 'edgeS':" << std::endl;
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
//        std::cerr << "4P collinearity between:" << e << " and before: " << before << std::endl;
        if (coll4Swap(e, before, edgeS, polygon, points)) {
//          std::cerr << "4P coll. after swap: " << e << " and bef: " << before << std::endl;
          valid = E_SKIP;
          update_lowest_index(e, before, lowest_index);
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", before: " << before << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(before, edgeS);
        flip2(e, before, polygon);
        valid = E_SKIP;
        update_lowest_index(e, before, lowest_index);
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
//        std::cerr << "4P collinearity between:" << e << " and aft: " << after << std::endl;
        if (coll4Swap(e, after, edgeS, polygon, points)) {
//          std::cerr << "4P coll. after swap: " << e << " and " << after << std::endl;
          valid = E_SKIP;
          update_lowest_index(e, after, lowest_index);
        }
      }
      else {
//        std::cerr << "Intersection: e: " << e << ", after: " << after << std::endl;
        edgeS.erase(retval.first);
        eraseEdgeFromSet(after, edgeS);
        //removeEdgeFromSetb(e, edgeS, polygon, points);
        //removeEdgeFromSetb(after, edgeS, polygon, points);
        flip2(e, after, polygon);
        valid = E_SKIP;
        update_lowest_index(e, after, lowest_index);
      }
    }

  } else {
    // edge already existed in set.
    // this can happen if an index is going backwards through the lex. order of points.
    // but if it's reversing and it hits an edge already in, earlier code should have caught it and removed it.
    std::cerr << "Error: Edge already exists in set!" << std::endl;
    std::cerr << "edge: " << e << ", returned: " << *retval.first << std::endl;
//    std::cout << "edges in 'edgeS':" << std::endl;
//    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
    valid = E_NOT_VALID;
  }

  retval2.first = valid;
  if (valid != E_VALID) retval2.second = edgeS.end();
  else retval2.second = retval.first;

  return retval2;
}
