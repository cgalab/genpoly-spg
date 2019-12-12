#include <vector>
#include <iostream> // for std::endl
#include <assert.h>
#include <algorithm>    // std::sort
#include <math.h>       /* sqrt, INFINITY */
#include "point.h"
#include "edge.h"
#include "basicFunctions.h"
#include "rand.h"
#include "pol.h"

std::random_device rd;  // global variables for random engine from "rand.h"
std::mt19937 mt (rd()); // global variables for random engine from "rand.h"

// adds 'value' at 'insert_at' then shifts and  updates the values in 'polygon' and 'points' to reflect the shift.
// assumes 'insert_at' is lower than 'shift_to'
void collinear_shift(unsigned int value, unsigned int insert_at, unsigned int shift_to, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  unsigned int temp;
  ++shift_to; // so that I can give this function an index, and iterate to that index with a '<'
  for(unsigned int i = insert_at; i != shift_to; ++i) {
    temp = polygon[i];
    polygon[i] = value;
    points[polygon[i]].v = i;
    //std::cerr << "p[" << i << "]: " << points[polygon[i]] << std::endl;
    value = temp;
  }
}

// function to update the 'lowest_index' variable if one of the points
// is lex. lower than current value in 'lowest_index'
void update_lowest_index(Point *a, unsigned int& lowest_index, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  //std::cerr << "update: l_i: " << lowest_index << ", *a.l: " << (*a).l << ", left: " << points[polygon[((*a).v+points.size()-1)%points.size()]].l << ", right: " << points[polygon[((*a).v+points.size()+1)%points.size()]].l << std::endl;
  if ((*a).l < lowest_index) lowest_index = (*a).l;
  if (points[polygon[((*a).v+points.size()-1)%points.size()]].l < lowest_index) lowest_index = points[polygon[((*a).v+points.size()-1)%points.size()]].l;
  if (points[polygon[((*a).v+points.size()+1)%points.size()]].l < lowest_index) lowest_index = points[polygon[((*a).v+points.size()+1)%points.size()]].l;
}

// function to update the 'highest_index' variable if one of the points
// is lex. lower than current value in 'lowest_index'
void update_highest_index(Point *a, unsigned int& highest_index, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  //std::cerr << "update: h_i: " << highest_index << ", *a.l: " << (*a).l << ", left: " << points[polygon[((*a).v+points.size()-1)%points.size()]].l << ", right: " << points[polygon[((*a).v+points.size()+1)%points.size()]].l << std::endl;
  if (highest_index == polygon.size()) highest_index = (*a).l;
  if ((*a).l > highest_index) highest_index = (*a).l;
  if (points[polygon[((*a).v+points.size()-1)%points.size()]].l > highest_index) highest_index = points[polygon[((*a).v+points.size()-1)%points.size()]].l;
  if (points[polygon[((*a).v+points.size()+1)%points.size()]].l > highest_index) highest_index = points[polygon[((*a).v+points.size()+1)%points.size()]].l;
}

// swaps the order of points of the edge in the polygon,
// i.e. v values of the points in the edge 'e' and order of points in 'polygon'.
void polSwap(Edge e, std::vector<unsigned int>& polygon) {
  unsigned int temp = (*e.p1).v;
  (*e.p1).v = (*e.p2).v;
  (*e.p2).v = temp;
  polygon[(*e.p1).v] = (*e.p1).i;
  polygon[(*e.p2).v] = (*e.p2).i;
}
void polSwap(Point* a, Point* b, std::vector<unsigned int>& polygon) {
  unsigned int temp = (*a).v;
  (*a).v = (*b).v;
  (*b).v = temp;
  polygon[(*a).v] = (*a).i;
  polygon[(*b).v] = (*b).i;
}

// function that takes 3 points: a, b, and c that are already assumed collinear and fixes the collinearity
// by sorting them in the polygon such that they are no longer intersecting.
bool collSwap (Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  bool retval = false;

  // sort the points into lo/mid/hi lex order.
  std::vector<Point*> lex {a, b, c};
  sort(lex.begin(), lex.end(),
    [](Point* p1, Point* p2) -> bool {return *p1 < *p2;});
/*
  for (unsigned int i = 0; i < lex.size();++i) {
    std::cerr << "lex[" << i << "]: " << *lex[i] << std::endl;
  }
*/
  // sort the points into lo/mid/hi polygon order.
  std::vector<Point*> vert {a, b, c};
  sort(vert.begin(), vert.end(),
    [&](Point * p1, Point * p2) -> bool {return isPol1Left(p1, p2, polygon.size());});

  unsigned int arr[] = {(*vert[0]).v, (*vert[1]).v, (*vert[2]).v};
/*
  for (unsigned int i = 0; i < 3;++i) {
    std::cerr << "arr[" << i << "]: " << arr[i] << std::endl;
  }
*/
  // we can erase the edges connected to lower and upper vertices from 'edgeS' set.
  eraseVertexFromSet(vert[0], edgeS, polygon, points);
  eraseVertexFromSet(vert[2], edgeS, polygon, points);

  // then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if (*lex[i] != *vert[i]) {
      (*lex[i]).v = arr[i];
      polygon[(*lex[i]).v] = (*lex[i]).i;
      retval = true;
    }
  }

  return retval;
}

// function that takes 3 points: a, b, and c that are already assumed collinear and fixes the collinearity
// by sorting them in the polygon such that they are no longer intersecting.
// older version.
bool coll3Swap(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  bool retval = false;
  double len1=0, len2=0;  // we want to make sure that the length of the polygonal chain defined by a, b and c decreases
  len1 = pol_calc_chain_length(get_lowest_v(a, b, c), get_highest_v(a, b, c), polygon, points);

  // sort the points into lo/mid/hi lex order.
  std::vector<Point*> lex {a, b, c};
  sort(lex.begin(), lex.end(),
    [](Point* p1, Point* p2) -> bool {return *p1 < *p2;});
/*
  for (unsigned int i = 0; i < lex.size();++i) {
    std::cerr << "lex[" << i << "]: " << *lex[i] << std::endl;
  }
*/
  // sort the points into lo/mid/hi polygon order.
  std::vector<Point*> vert {a, b, c};
  sort(vert.begin(), vert.end(),
    [&](Point * p1, Point * p2) -> bool {return isPol1Left(p1, p2, polygon.size());});

  unsigned int arr[] = {(*vert[0]).v, (*vert[1]).v, (*vert[2]).v};
/*
  for (unsigned int i = 0; i < 3;++i) {
    std::cerr << "arr[" << i << "]: " << arr[i] << std::endl;
  }
*/
  // we can erase the edges connected to lower and upper vertices from 'edgeS' set.

  eraseVertexFromSet(vert[2], edgeS, polygon, points);

  // then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if (*lex[i] != *vert[i]) {
      eraseVertexFromSet(lex[i], edgeS, polygon, points);
      eraseVertexFromSet(vert[i], edgeS, polygon, points);
      (*lex[i]).v = arr[i];
      polygon[(*lex[i]).v] = (*lex[i]).i;
      retval = true;
    }
  }

  len2 = pol_calc_chain_length(get_lowest_v(a, b, c), get_highest_v(a, b, c), polygon, points);
  if (len1 < len2) {
//    std::cerr << "len1: " << len1 << ", len2: " << len2 << std::endl;
    doFlip(get_lowest_v(a, b, c), get_highest_v(a, b, c), polygon, points);
    len2 = pol_calc_chain_length(get_lowest_v(a, b, c), get_highest_v(a, b, c), polygon, points);
//    std::cerr << "len1: " << len1 << ", new len2: " << len2 << std::endl;
    retval = false;
    for (unsigned int i = 0; i < lex.size(); ++i) {
      if (*lex[i] != *vert[i]) {retval = true;break;}
    }
  }

//  if (retval) for (unsigned int i = 0; i < lex.size();++i) std::cerr << "point at pol[arr[" << i << "]]: " << points[polygon[arr[i]]] << std::endl;
  return retval;
}

// function that takes 3 points: a, b, and c that are already assumed collinear and fixes the collinearity
// by sorting them in the polygon such that they are no longer intersecting.
// older version.
bool coll3SwapOld(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  bool retval = false;

  // sort the points into lo/mid/hi lex order.
  std::vector<Point*> lex {a, b, c};
  sort(lex.begin(), lex.end(),
    [](Point* p1, Point* p2) -> bool {return *p1 < *p2;});
/*
  for (unsigned int i = 0; i < lex.size();++i) {
    std::cerr << "lex[" << i << "]: " << *lex[i] << std::endl;
  }
*/
  // sort the points into lo/mid/hi polygon order.
  std::vector<Point*> vert {a, b, c};
  sort(vert.begin(), vert.end(),
    [&](Point * p1, Point * p2) -> bool {return isPol1Left(p1, p2, polygon.size());});

  unsigned int arr[] = {(*vert[0]).v, (*vert[1]).v, (*vert[2]).v};
/*
  for (unsigned int i = 0; i < 3;++i) {
    std::cerr << "arr[" << i << "]: " << arr[i] << std::endl;
  }
*/
  // we can erase the edges connected to lower and upper vertices from 'edgeS' set.

  eraseVertexFromSet(vert[2], edgeS, polygon, points);

  // then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if (*lex[i] != *vert[i]) {
      eraseVertexFromSet(lex[i], edgeS, polygon, points);
      eraseVertexFromSet(vert[i], edgeS, polygon, points);
      (*lex[i]).v = arr[i];
      polygon[(*lex[i]).v] = (*lex[i]).i;
      retval = true;
    }
  }

//  if (retval) for (unsigned int i = 0; i < lex.size();++i) std::cerr << "point at pol[arr[" << i << "]]: " << points[polygon[arr[i]]] << std::endl;
  return retval;
}


bool coll3Sort(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  std::vector<Point> cp;
  unsigned int lower, upper, index;
  double value;
  bool changed=false;

  // add point 'a' to cp, set lower/upper as 'a.v'
  cp.emplace_back((*a));
  lower = (*a).v; upper = (*a).v;
  // add point 'b' to cp, update lower/upper if necessary
  cp.emplace_back((*b));
  if (isPol1Left(upper, (*b).v, points.size())) upper = (*b).v;
  if (isPol1Left((*b).v, lower, points.size())) lower = (*b).v;
  // add point 'c' to cp, update lower/upper if necessary
  cp.emplace_back((*c));
  if (isPol1Left(upper, (*c).v, points.size())) upper = (*c).v;
  if (isPol1Left((*c).v, lower, points.size())) lower = (*c).v;
  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
//  std::cerr << "after 3 first points: upper: " << upper << ", lower: " << lower << std::endl;

  index = (upper+1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      upper = points[polygon[index]].v;
      index = (index+1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after upper:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;
  // do {add points from lower in the chain, update 'lower'} while (new point is collinear with points in vector)
  index = (lower+points.size()-1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      lower = points[polygon[index]].v;
      index = (index+points.size()-1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after lower:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;
  // sort the vector
  sort(cp.begin(), cp.end());
//  std::cerr << "after sort:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // from 'lower' to 'upper': add the sorted points in cp in order.
  upper = (upper+1) % points.size();
  index = 0;
  for(unsigned int v = lower; v != upper; v = (v+1)%points.size()) {
    if (points[cp[index].i].v != v) {
      changed = true;
      eraseVertexFromSet(&points[polygon[v]], edgeS, polygon, points);
      eraseVertexFromSet(&points[cp[index].i], edgeS, polygon, points);
      polygon[v] = cp[index].i;
      points[polygon[v]].v = v;
    }
    ++index;
  }

//  for(unsigned int v = lower; v != upper; v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  return changed;
}

// updates 'lowest_index' as well.
bool coll3Sort(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index) {
  std::vector<Point> cp;
  unsigned int lower, upper, index;
  double value;
  bool changed=false;

  // add point 'a' to cp, set lower/upper as 'a.v'
  cp.emplace_back((*a));
  lower = (*a).v; upper = (*a).v;
  // add point 'b' to cp, update lower/upper if necessary
  cp.emplace_back((*b));
  if (isPol1Left(upper, (*b).v, points.size())) upper = (*b).v;
  if (isPol1Left((*b).v, lower, points.size())) lower = (*b).v;
  // add point 'c' to cp, update lower/upper if necessary
  cp.emplace_back((*c));
  if (isPol1Left(upper, (*c).v, points.size())) upper = (*c).v;
  if (isPol1Left((*c).v, lower, points.size())) lower = (*c).v;
  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
//  std::cerr << "after 3 first points: upper: " << upper << ", lower: " << lower << std::endl;
  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
  index = (upper+1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      upper = points[polygon[index]].v;
      index = (index+1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after upper:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;
  // do {add points from lower in the chain, update 'lower'} while (new point is collinear with points in vector)
  index = (lower+points.size()-1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      lower = points[polygon[index]].v;
      index = (index+points.size()-1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after lower:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;
  // sort the vector
  sort(cp.begin(), cp.end());
//  std::cerr << "after sort:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // from 'lower' to 'upper': add the sorted points in cp in order.
  upper = (upper+1) % points.size();
  index = 0;
  for(unsigned int v = lower; v != upper; v = (v+1)%points.size()) {
    if (points[cp[index].i].v != v) {
      changed = true;
      //update_lowest_index(&points[polygon[v]], lowest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[v]], edgeS, polygon, points);
      //update_lowest_index(&points[cp[index].i], lowest_index, polygon, points);
      eraseVertexFromSet(&points[cp[index].i], edgeS, polygon, points);
      polygon[v] = cp[index].i;
      points[polygon[v]].v = v;
      if (points[polygon[v]].l < lowest_index) lowest_index = points[polygon[v]].l;
    }
    ++index;
  }

//  for(unsigned int v = lower; v != upper; v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  return changed;
}

// updates 'lowest_index' and 'highest_index'.
bool coll3Sort(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index) {
  std::vector<Point> cp;
  unsigned int lower, upper, index;
  double value;
  bool changed=false;

  // add point 'a' to cp, set lower/upper as 'a.v'
  cp.emplace_back((*a));
  lower = (*a).v; upper = (*a).v;
  // add point 'b' to cp, update lower/upper if necessary
  cp.emplace_back((*b));
  if (isPol1Left(upper, (*b).v, points.size())) upper = (*b).v;
  if (isPol1Left((*b).v, lower, points.size())) lower = (*b).v;
  // add point 'c' to cp, update lower/upper if necessary
  cp.emplace_back((*c));
  if (isPol1Left(upper, (*c).v, points.size())) upper = (*c).v;
  if (isPol1Left((*c).v, lower, points.size())) lower = (*c).v;
  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
//  std::cerr << "after 3 first points: upper: " << upper << ", lower: " << lower << std::endl;
  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
  index = (upper+1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      upper = points[polygon[index]].v;
      index = (index+1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after upper:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;
  // do {add points from lower in the chain, update 'lower'} while (new point is collinear with points in vector)
  index = (lower+points.size()-1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      lower = points[polygon[index]].v;
      index = (index+points.size()-1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after lower:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;
  // sort the vector
  sort(cp.begin(), cp.end());
//  std::cerr << "after sort:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // from 'lower' to 'upper': add the sorted points in cp in order.
  upper = (upper+1) % points.size();
  index = 0;
  for(unsigned int v = lower; v != upper; v = (v+1)%points.size()) {
    if (points[cp[index].i].v != v) {
      changed = true;
      //update_lowest_index(&points[polygon[v]], lowest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[v]], edgeS, polygon, points);
      //update_lowest_index(&points[cp[index].i], lowest_index, polygon, points);
      eraseVertexFromSet(&points[cp[index].i], edgeS, polygon, points);
      polygon[v] = cp[index].i;
      points[polygon[v]].v = v;
      if (points[polygon[v]].l < lowest_index) lowest_index = points[polygon[v]].l;
      if (points[polygon[v]].l > highest_index) highest_index = points[polygon[v]].l;
    }
    ++index;
  }

//  for(unsigned int v = lower; v != upper; v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  return changed;
}

// updates 'lowest_index', also inserts the new edges (at the ends of the sorted collinear edges) not connected to 'idx'
// if they cross the "line sweeping the plane" at idx.
bool coll3Sort(Point *a, Point *b, Point *c, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index) {
  std::vector<Point> cp;
  unsigned int lower, upper, index;
  double value;
  bool changed=false;

  // add point 'a' to cp, set lower/upper as 'a.v'
  cp.emplace_back((*a));
  lower = (*a).v; upper = (*a).v;
  // add point 'b' to cp, update lower/upper if necessary
  cp.emplace_back((*b));
  if (isPol1Left(upper, (*b).v, points.size())) upper = (*b).v;
  if (isPol1Left((*b).v, lower, points.size())) lower = (*b).v;
  // add point 'c' to cp, update lower/upper if necessary
  cp.emplace_back((*c));
  if (isPol1Left(upper, (*c).v, points.size())) upper = (*c).v;
  if (isPol1Left((*c).v, lower, points.size())) lower = (*c).v;
  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
//  std::cerr << "after 3 first points: upper: " << upper << ", lower: " << lower << std::endl;
  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
  index = (upper+1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      upper = points[polygon[index]].v;
      index = (index+1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after upper:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;
  // do {add points from lower in the chain, update 'lower'} while (new point is collinear with points in vector)
  index = (lower+points.size()-1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      lower = points[polygon[index]].v;
      index = (index+points.size()-1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after lower:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;
  // sort the vector
  sort(cp.begin(), cp.end());
//  std::cerr << "after sort:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // from 'lower' to 'upper': add the sorted points in cp in order.
  upper = (upper+1) % points.size();
  index = 0;
  for(unsigned int v = lower; v != upper; v = (v+1)%points.size()) {
    if (points[cp[index].i].v != v) {
      changed = true;
      update_lowest_index(&points[polygon[v]], lowest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[v]], edgeS, polygon, points);
      update_lowest_index(&points[cp[index].i], lowest_index, polygon, points);
      eraseVertexFromSet(&points[cp[index].i], edgeS, polygon, points);
      polygon[v] = cp[index].i;
      points[polygon[v]].v = v;
      //if (points[polygon[v]].l < lowest_index) lowest_index = points[polygon[v]].l;
    }
    ++index;
  }

//  for(unsigned int v = lower; v != upper; v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  // code to check the edges on the ends of the collinear points and add them if they cross the 'l_idx' x coordinate.
  if (changed) {
//    std::cerr << "current index: " << *idx << std::endl;
    Edge new_edge1, new_edge2;
    new_edge1.set(points[polygon[lower]], points[polygon[(lower+points.size()-1)%points.size()]]);
    new_edge2.set(points[polygon[upper]], points[polygon[(upper+1)%points.size()]]);
    std::pair<std::set<Edge>::iterator,bool> retval;

    if (((*new_edge1.p1).l < (*idx).l) && ((*idx).l < (*new_edge1.p2).l)) {
//      std::cerr << "inserting1: " << new_edge1 << std::endl;
      retval = edgeS.insert(new_edge1);
      if (*retval.first != new_edge1) {
        std::cerr << "Error!  inserting: " << new_edge1 << ", returned: " << *retval.first << std::endl;
      }
      if ((*new_edge1.p1).l < lowest_index) lowest_index = (*new_edge1.p1).l;
    }
    if (((*new_edge2.p1).l < (*idx).l) && ((*idx).l < (*new_edge2.p2).l)) {
//      std::cerr << "inserting2: " << new_edge2 << std::endl;
      retval = edgeS.insert(new_edge2);
      if (*retval.first != new_edge2) {
        std::cerr << "Error!  inserting: " << new_edge2 << ", returned: " << *retval.first << std::endl;
      }
      if ((*new_edge2.p1).l < lowest_index) lowest_index = (*new_edge2.p1).l;
    }
  }

  return changed;
}

// Same as above, but updates 'highest_index' as well.
bool coll3Sort(Point *a, Point *b, Point *c, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index) {
  std::vector<Point> cp;
  unsigned int lower, upper, index;
  double value;
  bool changed=false;

  // add point 'a' to cp, set lower/upper as 'a.v'
  cp.emplace_back((*a));
  lower = (*a).v; upper = (*a).v;
  // add point 'b' to cp, update lower/upper if necessary
  cp.emplace_back((*b));
  if (isPol1Left(upper, (*b).v, points.size())) upper = (*b).v;
  if (isPol1Left((*b).v, lower, points.size())) lower = (*b).v;
  // add point 'c' to cp, update lower/upper if necessary
  cp.emplace_back((*c));
  if (isPol1Left(upper, (*c).v, points.size())) upper = (*c).v;
  if (isPol1Left((*c).v, lower, points.size())) lower = (*c).v;
  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
//  std::cerr << "after 3 first points: upper: " << upper << ", lower: " << lower << std::endl;
  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
  index = (upper+1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      upper = points[polygon[index]].v;
      index = (index+1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after upper:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;
  // do {add points from lower in the chain, update 'lower'} while (new point is collinear with points in vector)
  index = (lower+points.size()-1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      lower = points[polygon[index]].v;
      index = (index+points.size()-1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after lower:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;
  // sort the vector
  sort(cp.begin(), cp.end());
//  std::cerr << "after sort:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // from 'lower' to 'upper': add the sorted points in cp in order.
  upper = (upper+1) % points.size();
  index = 0;
  for(unsigned int v = lower; v != upper; v = (v+1)%points.size()) {
    if (points[cp[index].i].v != v) {
      changed = true;
      update_lowest_index(&points[polygon[v]], lowest_index, polygon, points);
      update_highest_index(&points[polygon[v]], highest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[v]], edgeS, polygon, points);
      update_lowest_index(&points[cp[index].i], lowest_index, polygon, points);
      update_highest_index(&points[cp[index].i], highest_index, polygon, points);
      eraseVertexFromSet(&points[cp[index].i], edgeS, polygon, points);
      polygon[v] = cp[index].i;
      points[polygon[v]].v = v;
      //if (points[polygon[v]].l < lowest_index) lowest_index = points[polygon[v]].l;
    }
    ++index;
  }

//  for(unsigned int v = lower; v != upper; v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  // code to check the edges on the ends of the collinear points and add them if they cross the 'l_idx' x coordinate.
  if (changed) {
//    std::cerr << "current index: " << *idx << std::endl;
    Edge new_edge1, new_edge2;
    new_edge1.set(points[polygon[lower]], points[polygon[(lower+points.size()-1)%points.size()]]);
    new_edge2.set(points[polygon[upper]], points[polygon[(upper+1)%points.size()]]);
    std::pair<std::set<Edge>::iterator,bool> retval;

    if (((*new_edge1.p1).l < (*idx).l) && ((*idx).l < (*new_edge1.p2).l)) {
//      std::cerr << "inserting1: " << new_edge1 << std::endl;
      retval = edgeS.insert(new_edge1);
      if (*retval.first != new_edge1) {
        std::cerr << "Error!  inserting: " << new_edge1 << ", returned: " << *retval.first << std::endl;
      }
      if ((*new_edge1.p1).l < lowest_index) lowest_index = (*new_edge1.p1).l;
    }
    if (((*new_edge2.p1).l < (*idx).l) && ((*idx).l < (*new_edge2.p2).l)) {
//      std::cerr << "inserting2: " << new_edge2 << std::endl;
      retval = edgeS.insert(new_edge2);
      if (*retval.first != new_edge2) {
        std::cerr << "Error!  inserting: " << new_edge2 << ", returned: " << *retval.first << std::endl;
      }
      if ((*new_edge2.p1).l < lowest_index) lowest_index = (*new_edge2.p1).l;
    }
  }

  return changed;
}

// updates 'lowest_index', also inserts the new edges (at the ends of the sorted collinear edges) not connected to 'idx'
// if they cross the "line sweeping the plane" at idx.
// added a check for the total length between the vertex connected to the beginning and the vertex connected to the end
// picks the shortest version, if both equal, pick a random.
bool coll3Sort2(Point *a, Point *b, Point *c, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index) {
  std::vector<Point> cp;
  unsigned int lower, upper, index;
  double value;
  bool changed=false;

  // add point 'a' to cp, set lower/upper as 'a.v'
  cp.emplace_back((*a));
  lower = (*a).v; upper = (*a).v;
  // add point 'b' to cp, update lower/upper if necessary
  cp.emplace_back((*b));
  if (isPol1Left(upper, (*b).v, points.size())) upper = (*b).v;
  if (isPol1Left((*b).v, lower, points.size())) lower = (*b).v;
  // add point 'c' to cp, update lower/upper if necessary
  cp.emplace_back((*c));
  if (isPol1Left(upper, (*c).v, points.size())) upper = (*c).v;
  if (isPol1Left((*c).v, lower, points.size())) lower = (*c).v;
//  std::cerr << "after 3 first points: lower: " << lower << ", upper: " << upper << std::endl;

  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
  index = (upper+1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      upper = points[polygon[index]].v;
      index = (index+1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after upper:" << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // do {add points from lower in the chain, update 'lower'} while (new point is collinear with points in vector)
  index = (points.size()+lower-1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      lower = points[polygon[index]].v;
      index = (index+points.size()-1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after lower:" << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // sort the vector
  sort(cp.begin(), cp.end());
//  std::cerr << "after sort:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // from 'lower' to 'upper': add the sorted points in cp in order.
  index = 0;
  // first check if any have been changed
  for(unsigned int v = lower; v != (upper+1)%points.size(); v = (v+1)%points.size()) {
    if (points[cp[index].i].v != v) {
      changed = true;
    }
    ++index;
  }
  index = 0;
  if (changed) {
    for(unsigned int v = lower; v != (upper+1)%points.size(); v = (v+1)%points.size()) {
//      std::cerr << "erasing: " << points[polygon[v]] << ", and: " << points[cp[index].i] << std::endl;
      update_lowest_index(&points[polygon[v]], lowest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[v]], edgeS, polygon, points);
      update_lowest_index(&points[cp[index].i], lowest_index, polygon, points);
      eraseVertexFromSet(&points[cp[index].i], edgeS, polygon, points);
      polygon[v] = cp[index].i;
      points[polygon[v]].v = v;
      //if (points[polygon[v]].l < lowest_index) lowest_index = points[polygon[v]].l;
      ++index;
    }
  }


//  for(unsigned int v = (points.size()+lower-1)%points.size(); v != (upper+2)%points.size(); v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  // To make sure that the circumference of the polygon is always shrinking, check 2 things:
  // 1) check the distance between [lower-1,lower] + [upper,upper+1]
  // 2) check the distance between [lower-1,upper] + [lower,upper+1]
  // if 2) is smaller than 1), flip the points from ower to upper
  // if 1) is equal to 2), pick randomly between the 2.
  double dist11 = get_length(points[polygon[(points.size()+lower-1)%points.size()]], points[polygon[lower]]);
  double dist12 = get_length(points[polygon[upper]], points[polygon[(upper+1)%points.size()]]);
  double dist21 = get_length(points[polygon[(points.size()+lower-1)%points.size()]], points[polygon[upper]]);
  double dist22 = get_length(points[polygon[lower]], points[polygon[(upper+1)%points.size()]]);
//  std::cerr << "d1: " << dist11+dist12 << ", d2: " << dist21+dist22 << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
  bool flip;
  if ((dist11+dist12) == (dist21 + dist22)) {
    UniformRandomI(flip, 0, 1);
  }
  else if ((dist21+dist22) < (dist11+dist12)) flip = true;
  else flip = false;

  if (flip) {
    doFlip(lower, upper, polygon, points);
  }

//  for(unsigned int v = (points.size()+lower-1)%points.size(); v != (upper+2)%points.size(); v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  // code to check the edges on the ends of the collinear points and add them if they cross the 'l_idx' x coordinate.
  if (changed) {
//    std::cerr << "current index: " << *idx << std::endl;
    Edge new_edge1, new_edge2;
    new_edge1.set(points[polygon[lower]], points[polygon[(lower+points.size()-1)%points.size()]]);
    new_edge2.set(points[polygon[upper]], points[polygon[(upper+1)%points.size()]]);
    std::pair<std::set<Edge>::iterator,bool> retval;

    if (((*new_edge1.p1).l < (*idx).l) && ((*idx).l < (*new_edge1.p2).l)) {
//      std::cerr << "inserting1: " << new_edge1 << std::endl;
      retval = edgeS.insert(new_edge1);
      if (*retval.first != new_edge1) {
        std::cerr << "Error!  inserting: " << new_edge1 << ", returned: " << *retval.first << std::endl;
      }
      if ((*new_edge1.p1).l < lowest_index) lowest_index = (*new_edge1.p1).l;
    }
    if (((*new_edge2.p1).l < (*idx).l) && ((*idx).l < (*new_edge2.p2).l)) {
//      std::cerr << "inserting2: " << new_edge2 << std::endl;
      retval = edgeS.insert(new_edge2);
      if (*retval.first != new_edge2) {
        std::cerr << "Error!  inserting: " << new_edge2 << ", returned: " << *retval.first << std::endl;
      }
      if ((*new_edge2.p1).l < lowest_index) lowest_index = (*new_edge2.p1).l;
    }
  }

  return changed;
}

// updates 'lowest_index', also inserts the new edges (at the ends of the sorted collinear edges) not connected to 'idx'
// if they cross the "line sweeping the plane" at idx.
// added a check for the total length between the vertex connected to the beginning and the vertex connected to the end
// picks the shortest version, if both equal, pick a random.
bool coll3Sort2(Point *a, Point *b, Point *c, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index) {
  std::vector<Point> cp;
  unsigned int lower, upper, index;
  double value;
  bool changed=false;

  // add point 'a' to cp, set lower/upper as 'a.v'
  cp.emplace_back((*a));
  lower = (*a).v; upper = (*a).v;
  // add point 'b' to cp, update lower/upper if necessary
  cp.emplace_back((*b));
  if (isPol1Left(upper, (*b).v, points.size())) upper = (*b).v;
  if (isPol1Left((*b).v, lower, points.size())) lower = (*b).v;
  // add point 'c' to cp, update lower/upper if necessary
  cp.emplace_back((*c));
  if (isPol1Left(upper, (*c).v, points.size())) upper = (*c).v;
  if (isPol1Left((*c).v, lower, points.size())) lower = (*c).v;
//  std::cerr << "after 3 first points: lower: " << lower << ", upper: " << upper << std::endl;

  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
  index = (upper+1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      upper = points[polygon[index]].v;
      index = (index+1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after upper:" << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // do {add points from lower in the chain, update 'lower'} while (new point is collinear with points in vector)
  index = (points.size()+lower-1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      lower = points[polygon[index]].v;
      index = (index+points.size()-1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after lower:" << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // sort the vector
  sort(cp.begin(), cp.end());
//  std::cerr << "after sort:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // from 'lower' to 'upper': add the sorted points in cp in order.
  index = 0;
  // first check if any have been changed
  for(unsigned int v = lower; v != (upper+1)%points.size(); v = (v+1)%points.size()) {
    if (points[cp[index].i].v != v) {
      changed = true;
      break;
    }
    ++index;
  }
  index = 0;
  if (changed) {
    for(unsigned int v = lower; v != (upper+1)%points.size(); v = (v+1)%points.size()) {
//      std::cerr << "erasing: " << points[polygon[v]] << ", and: " << points[cp[index].i] << std::endl;
      update_lowest_index(&points[polygon[v]], lowest_index, polygon, points);
      update_highest_index(&points[polygon[v]], highest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[v]], edgeS, polygon, points);
      update_lowest_index(&points[cp[index].i], lowest_index, polygon, points);
      update_highest_index(&points[cp[index].i], highest_index, polygon, points);
      eraseVertexFromSet(&points[cp[index].i], edgeS, polygon, points);
      polygon[v] = cp[index].i;
      points[polygon[v]].v = v;
      //if (points[polygon[v]].l < lowest_index) lowest_index = points[polygon[v]].l;
      ++index;
    }
  }

//  for(unsigned int v = (points.size()+lower-1)%points.size(); v != (upper+2)%points.size(); v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  // To make sure that the circumference of the polygon is always shrinking, check 2 things:
  // 1) check the distance between [lower-1,lower] + [upper,upper+1]
  // 2) check the distance between [lower-1,upper] + [lower,upper+1]
  // if 2) is smaller than 1), flip the points from ower to upper
  // if 1) is equal to 2), pick randomly between the 2.
  double dist11 = get_length(points[polygon[(points.size()+lower-1)%points.size()]], points[polygon[lower]]);
  double dist12 = get_length(points[polygon[upper]], points[polygon[(upper+1)%points.size()]]);
  double dist21 = get_length(points[polygon[(points.size()+lower-1)%points.size()]], points[polygon[upper]]);
  double dist22 = get_length(points[polygon[lower]], points[polygon[(upper+1)%points.size()]]);
//  std::cerr << "d1: " << dist11+dist12 << ", d2: " << dist21+dist22 << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
  bool flip;
  if ((dist11+dist12) == (dist21 + dist22)) {
    UniformRandomI(flip, 0, 1);
  }
  else if ((dist21+dist22) < (dist11+dist12)) flip = true;
  else flip = false;

  if (flip) {
    doFlip(lower, upper, polygon, points);
  }

//  for(unsigned int v = (points.size()+lower-1)%points.size(); v != (upper+2)%points.size(); v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  // code to check the edges on the ends of the collinear points and add them if they cross the 'l_idx' x coordinate.
  if (changed) {
//    std::cerr << "current index: " << *idx << std::endl;
    Edge new_edge1, new_edge2;
    new_edge1.set(points[polygon[lower]], points[polygon[(lower+points.size()-1)%points.size()]]);
    new_edge2.set(points[polygon[upper]], points[polygon[(upper+1)%points.size()]]);
    std::pair<std::set<Edge>::iterator,bool> retval;

    if (((*new_edge1.p1).l < (*idx).l) && ((*idx).l < (*new_edge1.p2).l)) {
//      std::cerr << "inserting1: " << new_edge1 << std::endl;
      retval = edgeS.insert(new_edge1);
      if (*retval.first != new_edge1) {
        std::cerr << "Error!  inserting: " << new_edge1 << ", returned: " << *retval.first << std::endl;
      }
      if ((*new_edge1.p1).l < lowest_index) lowest_index = (*new_edge1.p1).l;
    }
    if (((*new_edge2.p1).l < (*idx).l) && ((*idx).l < (*new_edge2.p2).l)) {
//      std::cerr << "inserting2: " << new_edge2 << std::endl;
      retval = edgeS.insert(new_edge2);
      if (*retval.first != new_edge2) {
        std::cerr << "Error!  inserting: " << new_edge2 << ", returned: " << *retval.first << std::endl;
      }
      if ((*new_edge2.p1).l < lowest_index) lowest_index = (*new_edge2.p1).l;
    }
  }

  return changed;
}

// same as coll3sort2 except it doesn't add the edge that doesn't start or end in the current index.
bool coll3Sort3(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index) {
  std::vector<Point> cp;
  unsigned int lower, upper, index;
  double value;
  bool changed=false;

  // add point 'a' to cp, set lower/upper as 'a.v'
  cp.emplace_back((*a));
  lower = (*a).v; upper = (*a).v;
  // add point 'b' to cp, update lower/upper if necessary
  cp.emplace_back((*b));
  if (isPol1Left(upper, (*b).v, points.size())) upper = (*b).v;
  if (isPol1Left((*b).v, lower, points.size())) lower = (*b).v;
  // add point 'c' to cp, update lower/upper if necessary
  cp.emplace_back((*c));
  if (isPol1Left(upper, (*c).v, points.size())) upper = (*c).v;
  if (isPol1Left((*c).v, lower, points.size())) lower = (*c).v;
//  std::cerr << "after 3 first points: lower: " << lower << ", upper: " << upper << std::endl;

  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
  index = (upper+1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      upper = points[polygon[index]].v;
      index = (index+1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after upper:" << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // do {add points from lower in the chain, update 'lower'} while (new point is collinear with points in vector)
  index = (points.size()+lower-1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      lower = points[polygon[index]].v;
      index = (index+points.size()-1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after lower:" << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // sort the vector
  sort(cp.begin(), cp.end());
//  std::cerr << "after sort:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  //update the lowest_index
  if (cp[0].l < lowest_index) lowest_index = cp[0].l;

  // from 'lower' to 'upper': add the sorted points in cp in order.
  index = 0;
  // first check if any have been changed
  for(unsigned int v = lower; v != (upper+1)%points.size(); v = (v+1)%points.size()) {
    if (points[cp[index].i].v != v) {
      changed = true;
      break;
    }
    ++index;
  }
  index = 0;
  if (changed) {
    for(unsigned int v = lower; v != (upper+1)%points.size(); v = (v+1)%points.size()) {
//      std::cerr << "erasing: " << points[polygon[v]] << ", and: " << points[cp[index].i] << std::endl;
      eraseVertexFromSet(&points[polygon[v]], edgeS, polygon, points);
      eraseVertexFromSet(&points[cp[index].i], edgeS, polygon, points);
      polygon[v] = cp[index].i;
      points[polygon[v]].v = v;
      ++index;
    }
  }

//  for(unsigned int v = (points.size()+lower-1)%points.size(); v != (upper+2)%points.size(); v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  // To make sure that the circumference of the polygon is always shrinking, check 2 things:
  // 1) check the distance between [lower-1,lower] + [upper,upper+1]
  // 2) check the distance between [lower-1,upper] + [lower,upper+1]
  // if 2) is smaller than 1), flip the points from ower to upper
  // if 1) is equal to 2), pick randomly between the 2.
  double dist11 = get_length(points[polygon[(points.size()+lower-1)%points.size()]], points[polygon[lower]]);
  double dist12 = get_length(points[polygon[upper]], points[polygon[(upper+1)%points.size()]]);
  double dist21 = get_length(points[polygon[(points.size()+lower-1)%points.size()]], points[polygon[upper]]);
  double dist22 = get_length(points[polygon[lower]], points[polygon[(upper+1)%points.size()]]);
//  std::cerr << "d1: " << dist11+dist12 << ", d2: " << dist21+dist22 << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
  bool flip;
  if ((dist11+dist12) == (dist21 + dist22)) {
    UniformRandomI(flip, 0, 1);
  }
  else if ((dist21+dist22) < (dist11+dist12)) flip = true;
  else flip = false;

  if (flip) {
    doFlip(lower, upper, polygon, points);
  }

//  for(unsigned int v = (points.size()+lower-1)%points.size(); v != (upper+2)%points.size(); v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  return changed;
}

// same as coll3sort2 except it doesn't add the edge that doesn't start or end in the current index.
bool coll3Sort3(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index) {
  std::vector<Point> cp;
  unsigned int lower, upper, index;
  double value;
  bool changed=false;

  // add point 'a' to cp, set lower/upper as 'a.v'
  cp.emplace_back((*a));
  lower = (*a).v; upper = (*a).v;
  // add point 'b' to cp, update lower/upper if necessary
  cp.emplace_back((*b));
  if (isPol1Left(upper, (*b).v, points.size())) upper = (*b).v;
  if (isPol1Left((*b).v, lower, points.size())) lower = (*b).v;
  // add point 'c' to cp, update lower/upper if necessary
  cp.emplace_back((*c));
  if (isPol1Left(upper, (*c).v, points.size())) upper = (*c).v;
  if (isPol1Left((*c).v, lower, points.size())) lower = (*c).v;
//  std::cerr << "after 3 first points: lower: " << lower << ", upper: " << upper << std::endl;

  // do {add points from higher in the chain, update 'upper'} while (new point is collinear with points in vector)
  index = (upper+1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      upper = points[polygon[index]].v;
      index = (index+1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after upper:" << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // do {add points from lower in the chain, update 'lower'} while (new point is collinear with points in vector)
  index = (points.size()+lower-1)%points.size();
  do {
    value = det(points[polygon[lower]], points[polygon[upper]], points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      lower = points[polygon[index]].v;
      index = (index+points.size()-1) % points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after lower:" << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // sort the vector
  sort(cp.begin(), cp.end());
//  std::cerr << "after sort:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // from 'lower' to 'upper': add the sorted points in cp in order.
  index = 0;
  // first check if any have been changed
  for(unsigned int v = lower; v != (upper+1)%points.size(); v = (v+1)%points.size()) {
    if (points[cp[index].i].v != v) {
      changed = true;
      break;
    }
    ++index;
  }
  index = 0;
  if (changed) {
    for(unsigned int v = lower; v != (upper+1)%points.size(); v = (v+1)%points.size()) {
//      std::cerr << "erasing: " << points[polygon[v]] << ", and: " << points[cp[index].i] << std::endl;
      update_lowest_index(&points[polygon[v]], lowest_index, polygon, points);
      update_highest_index(&points[polygon[v]], highest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[v]], edgeS, polygon, points);
      update_lowest_index(&points[cp[index].i], lowest_index, polygon, points);
      update_highest_index(&points[cp[index].i], highest_index, polygon, points);
      eraseVertexFromSet(&points[cp[index].i], edgeS, polygon, points);
      polygon[v] = cp[index].i;
      points[polygon[v]].v = v;
      //if (points[polygon[v]].l < lowest_index) lowest_index = points[polygon[v]].l;
      ++index;
    }
  }

//  for(unsigned int v = (points.size()+lower-1)%points.size(); v != (upper+2)%points.size(); v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  // To make sure that the circumference of the polygon is always shrinking, check 2 things:
  // 1) check the distance between [lower-1,lower] + [upper,upper+1]
  // 2) check the distance between [lower-1,upper] + [lower,upper+1]
  // if 2) is smaller than 1), flip the points from ower to upper
  // if 1) is equal to 2), pick randomly between the 2.
  double dist11 = get_length(points[polygon[(points.size()+lower-1)%points.size()]], points[polygon[lower]]);
  double dist12 = get_length(points[polygon[upper]], points[polygon[(upper+1)%points.size()]]);
  double dist21 = get_length(points[polygon[(points.size()+lower-1)%points.size()]], points[polygon[upper]]);
  double dist22 = get_length(points[polygon[lower]], points[polygon[(upper+1)%points.size()]]);
//  std::cerr << "d1: " << dist11+dist12 << ", d2: " << dist21+dist22 << std::endl;
//  std::cerr << "lower: " << lower << ", upper: " << upper << std::endl;
  bool flip;
  if ((dist11+dist12) == (dist21 + dist22)) {
    UniformRandomI(flip, 0, 1);
  }
  else if ((dist21+dist22) < (dist11+dist12)) flip = true;
  else flip = false;

  if (flip) {
    doFlip(lower, upper, polygon, points);
  }

//  for(unsigned int v = (points.size()+lower-1)%points.size(); v != (upper+2)%points.size(); v = (v+1)%points.size()) std::cerr << "p[p[" << v << "]]: " << points[polygon[v]] << std::endl;

  return changed;
}


/*
bool coll3Swap(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  bool retval = false;
  Point p1, p2, p3;
  p1.set(*a); p2.set(*b); p3.set(*c);

  // sort the points into lo/mid/hi lex order.
  std::vector<Point> lex {p1, p2, p3};
  sort(lex.begin(), lex.end());//,

  std::cerr << "3swap:" << std::endl;
  for (unsigned int i = 0; i < lex.size();++i) {
    std::cerr << "lex[" << i << "]: " << lex[i] << std::endl;
  }

  // sort the points into lo/mid/hi polygon order.
  std::vector<Point*> vert {a, b, c};
  sort(vert.begin(), vert.end(),
    [&](Point * p1, Point * p2) -> bool {return isPol1Left(p1, p2, polygon.size());});

  unsigned int arr[] = {(*vert[0]).v, (*vert[1]).v, (*vert[2]).v};

  for (unsigned int i = 0; i < 3;++i) {
    std::cerr << "arr[" << i << "]: " << arr[i] << std::endl;
  }

  // then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if ((lex[i]).v != arr[i]) {
      eraseVertexFromSet(&points[lex[i].i], edgeS, polygon, points);
      eraseVertexFromSet(&points[polygon[arr[i]]], edgeS, polygon, points);
      points[lex[i].i].v = arr[i];
      polygon[arr[i]] = lex[i].i;
      retval = true;
    }
  }

  if (retval) {
    for (unsigned int i = 0; i < lex.size();++i) {
      std::cerr << "point at pol[arr[i]]: " << points[polygon[arr[i]]] << std::endl;
    }
  }

  return retval;
}
*/
// 'b' version for the reversing version of A_2OPT_B
// function that takes 3 points: a, b, and c that are already assumed collinear and fixes the collinearity
// by sorting them in the polygon such that they are no longer intersecting.
bool coll3Swap(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index) {
  bool retval = false;
  Point p1, p2, p3;
  p1.set(*a); p2.set(*b); p3.set(*c);

  // sort the points into lo/mid/hi lex order.
  std::vector<Point> lex {p1, p2, p3};
  sort(lex.begin(), lex.end());//,

//  std::cerr << "3swap:" << std::endl;
//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "lex[" << i << "]: " << lex[i] << std::endl;

  // sort the points into lo/mid/hi polygon order.
  std::vector<Point*> vert {a, b, c};
  sort(vert.begin(), vert.end(),
    [&](Point * p1, Point * p2) -> bool {return isPol1Left(p1, p2, polygon.size());});

  unsigned int arr[] = {(*vert[0]).v, (*vert[1]).v, (*vert[2]).v};

//  for (unsigned int i = 0; i < 3;++i) std::cerr << "arr[" << i << "]: " << arr[i] << std::endl;

  // then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if ((lex[i]).v != arr[i]) {
      eraseVertexFromSet(&points[lex[i].i], edgeS, polygon, points);
      update_lowest_index(&points[lex[i].i], lowest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[arr[i]]], edgeS, polygon, points);
      update_lowest_index(&points[polygon[arr[i]]], lowest_index, polygon, points);
      points[lex[i].i].v = arr[i];
      polygon[arr[i]] = lex[i].i;
      retval = true;
    }
  }

//  if (retval) for (unsigned int i = 0; i < lex.size();++i) std::cerr << "point at pol[arr[" << i << "]]: " << points[polygon[arr[i]]] << std::endl;
  return retval;
}

bool coll3Swap(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index) {
  bool retval = false;
  Point p1, p2, p3;
  p1.set(*a); p2.set(*b); p3.set(*c);

  // sort the points into lo/mid/hi lex order.
  std::vector<Point> lex {p1, p2, p3};
  sort(lex.begin(), lex.end());//,
/*
  std::cerr << "3swap:" << std::endl;
  for (unsigned int i = 0; i < lex.size();++i) {
    std::cerr << "lex[" << i << "]: " << lex[i] << std::endl;
  }
*/
  // sort the points into lo/mid/hi polygon order.
  std::vector<Point*> vert {a, b, c};
  sort(vert.begin(), vert.end(),
    [&](Point * p1, Point * p2) -> bool {return isPol1Left(p1, p2, polygon.size());});

  unsigned int arr[] = {(*vert[0]).v, (*vert[1]).v, (*vert[2]).v};
/*
  for (unsigned int i = 0; i < 3;++i) {
    std::cerr << "arr[" << i << "]: " << arr[i] << std::endl;
  }
*/

  // then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if ((lex[i]).v != arr[i]) {
      eraseVertexFromSet(&points[lex[i].i], edgeS, polygon, points);
      update_lowest_index(&points[lex[i].i], lowest_index, polygon, points);
      update_highest_index(&points[lex[i].i], highest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[arr[i]]], edgeS, polygon, points);
      update_lowest_index(&points[polygon[arr[i]]], lowest_index, polygon, points);
      update_highest_index(&points[polygon[arr[i]]], highest_index, polygon, points);
      points[lex[i].i].v = arr[i];
      polygon[arr[i]] = lex[i].i;
      retval = true;
    }
  }
/*
  for (unsigned int i = 0; i < lex.size();++i) {
    std::cerr << "point at pol[arr[i]]: " << points[polygon[arr[i]]] << std::endl;
  }
*/
  return retval;
}

// Coll4swap takes 2 edges that both intersect and are collinear, sets the lowest lex. point as the lowest of the vertex indices and
// shifts the rest of the points so they are incidental in the polygon in their lex. order
bool coll4Swap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  bool retval = false;
  bool end_edge;

  if (*e1.p1 == *e2.p1) return coll3Swap(e1.p1, e1.p2, e2.p2, edgeS, polygon, points);
  if (*e1.p1 == *e2.p2) return coll3Swap(e1.p1, e1.p2, e2.p1, edgeS, polygon, points);
  if (*e1.p2 == *e2.p1) return coll3Swap(e1.p1, e1.p2, e2.p2, edgeS, polygon, points);
  if (*e1.p2 == *e2.p2) return coll3Swap(e1.p1, e1.p2, e2.p1, edgeS, polygon, points);

  // sort the points into lex order.
  std::vector<Point*> lex {e1.p1, e1.p2, e2.p1, e2.p2};
  sort(lex.begin(), lex.end(),
    [](Point* p1, Point* p2) -> bool {return *p1 < *p2;});

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "lex[" << i << "]: " << *lex[i] << std::endl;

  // sort the points into polygon order.
  std::vector<unsigned int> vert {(*e1.p1).v, (*e1.p2).v, (*e2.p1).v, (*e2.p2).v};
  sort(vert.begin(), vert.end());

  if ((e1.getPLow().v == 0 && e1.getPHigh().v == points.size()-1) || (e2.getPLow().v == 0 && e2.getPHigh().v == points.size()-1)) {
    end_edge = true;
  }
  else end_edge = false;
  if (end_edge) {
    vert.pop_back();
    vert.insert(vert.begin(), points.size()-1);
  }

  // then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if ((*lex[i]).v != vert[i]) {
      eraseVertexFromSet(lex[i], edgeS, polygon, points);
      eraseVertexFromSet(&points[polygon[vert[i]]], edgeS, polygon, points);
      (*lex[i]).v = vert[i];
      polygon[(*lex[i]).v] = (*lex[i]).i;
      retval = true;
    }
  }

//  if (retval) for (unsigned int i = 0; i < lex.size();++i) std::cerr << "point at pol[arr["<<i<<"]]: " << points[polygon[arr[i]]] << std::endl;

  return retval;
}

bool coll4SwapOld(Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  bool retval = false;
  bool end_edge;

  if (*e1.p1 == *e2.p1) return coll3SwapOld(e1.p1, e1.p2, e2.p2, edgeS, polygon, points);
  if (*e1.p1 == *e2.p2) return coll3SwapOld(e1.p1, e1.p2, e2.p1, edgeS, polygon, points);
  if (*e1.p2 == *e2.p1) return coll3SwapOld(e1.p1, e1.p2, e2.p2, edgeS, polygon, points);
  if (*e1.p2 == *e2.p2) return coll3SwapOld(e1.p1, e1.p2, e2.p1, edgeS, polygon, points);

  // sort the points into lex order.
  std::vector<Point*> lex {e1.p1, e1.p2, e2.p1, e2.p2};
  sort(lex.begin(), lex.end(),
    [](Point* p1, Point* p2) -> bool {return *p1 < *p2;});

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "lex[" << i << "]: " << *lex[i] << std::endl;

  // sort the points into polygon order.
  std::vector<unsigned int> vert {(*e1.p1).v, (*e1.p2).v, (*e2.p1).v, (*e2.p2).v};
  sort(vert.begin(), vert.end());

  if ((e1.getPLow().v == 0 && e1.getPHigh().v == points.size()-1) || (e2.getPLow().v == 0 && e2.getPHigh().v == points.size()-1)) {
    end_edge = true;
  }
  else end_edge = false;
  if (end_edge) {
    vert.pop_back();
    vert.insert(vert.begin(), points.size()-1);
  }

  // then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if ((*lex[i]).v != vert[i]) {
      eraseVertexFromSet(lex[i], edgeS, polygon, points);
      eraseVertexFromSet(&points[polygon[vert[i]]], edgeS, polygon, points);
      (*lex[i]).v = vert[i];
      polygon[(*lex[i]).v] = (*lex[i]).i;
      retval = true;
    }
  }

//  if (retval) for (unsigned int i = 0; i < lex.size();++i) std::cerr << "point at pol[arr["<<i<<"]]: " << points[polygon[arr[i]]] << std::endl;

  return retval;
}

// updates the 'lowestindex'
// Function that takes 2 edges, e1 and e2 that both intersect and are collinear
bool coll4Swap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index) {
  bool retval = false;
  bool end_edge; // boolean for if one edge pairs [0,points.size()-1] together

//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << std::endl;
  if (*e1.p1 == *e2.p1) {
//    std::cerr << "hi1"<<std::endl;
    eraseEdgeFromSet(e1, edgeS);
    eraseEdgeFromSet(e2, edgeS);
    return coll3Sort(e1.p1, e1.p2, e2.p2, edgeS, polygon, points, lowest_index);
  }
  if (*e1.p1 == *e2.p2) {
//    std::cerr << "hi2"<<std::endl;
    eraseEdgeFromSet(e1, edgeS);
    eraseEdgeFromSet(e2, edgeS);
    return coll3Sort(e1.p1, e1.p2, e2.p1, edgeS, polygon, points, lowest_index);
  }
  if (*e1.p2 == *e2.p1) {
//    std::cerr << "hi3"<<std::endl;
    eraseEdgeFromSet(e1, edgeS);
    eraseEdgeFromSet(e2, edgeS);
    return coll3Sort(e1.p1, e1.p2, e2.p2, edgeS, polygon, points, lowest_index);
  }
  if (*e1.p2 == *e2.p2) {
//    std::cerr << "hi4"<<std::endl;
    eraseEdgeFromSet(e1, edgeS);
    eraseEdgeFromSet(e2, edgeS);
    return coll3Sort(e1.p1, e1.p2, e2.p1, edgeS, polygon, points, lowest_index);
  }

  Point p1, p2, p3, p4;
  p1.set(*e1.p1); p2.set(*e1.p2); p3.set(*e2.p1); p4.set(*e2.p2);

  if ((e1.getPLow().v == 0 && e1.getPHigh().v == points.size()-1) || (e2.getPLow().v == 0 && e2.getPHigh().v == points.size()-1)) {
    end_edge = true;
  }
  else end_edge = false;

  // sort the points into lex order.
  std::vector<Point> lex {p1, p2, p3, p4};
  sort(lex.begin(), lex.end());
    //[](Point* p1, Point* p2) -> bool {std::cerr<<"*p1: "<<*p1<<", *p2: "<<*p2<<std::endl;return *p1 < *p2;});

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "lex[" << i << "]: " << lex[i] << std::endl;

  // sort the points into polygon order.
  std::vector<unsigned int> vert {(*e1.p1).v, (*e1.p2).v, (*e2.p1).v, (*e2.p2).v};
  sort(vert.begin(), vert.end());//,
  //[&](Point * p1, Point * p2) -> bool {return isPolLeft(p1, p2, polygon.size());});
  if (end_edge) {
    vert.pop_back();
    vert.insert(vert.begin(), points.size()-1);
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "vert[" << i << "]: " << vert[i] << std::endl;

//   then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if (lex[i].v != vert[i]) {
      eraseVertexFromSet(&points[lex[i].i], edgeS, polygon, points);
      //update_lowest_index(&points[lex[i].i], lowest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[vert[i]]], edgeS, polygon, points);
      //update_lowest_index(&points[polygon[vert[i]]], lowest_index, polygon, points);
      points[lex[i].i].v = vert[i];
      polygon[vert[i]] = lex[i].i;
      retval = true;
      if (points[polygon[vert[i]]].l < lowest_index) lowest_index = polygon[vert[i]];
    }
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "point at pol[vert[i]]: " << points[polygon[vert[i]]] << std::endl;
//  std::cerr << std::endl;

  return retval;
}

// updates the 'lowestindex'
// Function that takes 2 edges, e1 and e2 that both intersect and are collinear
bool coll4Swap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index) {
  bool retval = false;

  if (*e1.p1 == *e2.p1) {
//    std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p1: " << *e2.p1 << std::endl;
    return coll3Swap(e1.p1, e1.p2, e2.p2, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p1 == *e2.p2) {
//    std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p2: " << *e2.p2 << std::endl;
    return coll3Swap(e1.p1, e1.p2, e2.p1, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p2 == *e2.p1) {
//    std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p1: " << *e2.p1 << std::endl;
    return coll3Swap(e1.p1, e1.p2, e2.p2, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p2 == *e2.p2) {
//    std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p2: " << *e2.p2 << std::endl;
    return coll3Swap(e1.p1, e1.p2, e2.p1, edgeS, polygon, points, lowest_index, highest_index);
  }

  Point p1, p2, p3, p4;
  p1.set(*e1.p1); p2.set(*e1.p2); p3.set(*e2.p1); p4.set(*e2.p2);

  // sort the points into lex order.
  std::vector<Point> lex {p1, p2, p3, p4};
  sort(lex.begin(), lex.end());
    //[](Point* p1, Point* p2) -> bool {std::cerr<<"*p1: "<<*p1<<", *p2: "<<*p2<<std::endl;return *p1 < *p2;});
/*
  for (unsigned int i = 0; i < lex.size();++i) {
    std::cerr << "lex[" << i << "]: " << lex[i] << std::endl;
  }
*/
  // sort the points into polygon order.
  std::vector<unsigned int> vert {(*e1.p1).v, (*e1.p2).v, (*e2.p1).v, (*e2.p2).v};
  sort(vert.begin(), vert.end());//,
  //[&](Point * p1, Point * p2) -> bool {return isPolLeft(p1, p2, polygon.size());});
  if ((vert[0] == 0) && vert[vert.size()-1] == points.size()-1) {
    vert.pop_back();
    vert.insert(vert.begin(), points.size()-1);
  }

  // then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if (lex[i].v != vert[i]) {
      eraseVertexFromSet(&points[lex[i].i], edgeS, polygon, points);
      update_lowest_index(&points[lex[i].i], lowest_index, polygon, points);
      update_highest_index(&points[lex[i].i], highest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[vert[i]]], edgeS, polygon, points);
      update_lowest_index(&points[polygon[vert[i]]], lowest_index, polygon, points);
      update_highest_index(&points[polygon[vert[i]]], highest_index, polygon, points);
      points[lex[i].i].v = vert[i];
      polygon[vert[i]] = lex[i].i;
      retval = true;
    }
  }
/*
  for (unsigned int i = 0; i < lex.size();++i) {
    std::cerr << "point at pol[arr[i]]: " << points[polygon[arr[i]]] << std::endl;
  }
*/
  return retval;
}

// updates the 'lowestindex', forwards 'idx' to coll3sort if necessary.
// Function that takes 2 edges, e1 and e2 that both intersect and are collinear
bool coll4Swap (Edge& e1, Edge& e2, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index) {
  bool retval = false;
  bool end_edge; // boolean for if one edge pairs [0,points.size()-1] together

//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << std::endl;
  if (*e1.p1 == *e2.p1) {
    //std::cerr << "hi1"<<std::endl;
    return coll3Sort(e1.p1, e1.p2, e2.p2, idx, edgeS, polygon, points, lowest_index);
  }
  if (*e1.p1 == *e2.p2) {
    //std::cerr << "hi2"<<std::endl;
    return coll3Sort(e1.p1, e1.p2, e2.p1, idx, edgeS, polygon, points, lowest_index);
  }
  if (*e1.p2 == *e2.p1) {
    //std::cerr << "hi3"<<std::endl;
    return coll3Sort(e1.p1, e1.p2, e2.p2, idx, edgeS, polygon, points, lowest_index);
  }
  if (*e1.p2 == *e2.p2) {
    //std::cerr << "hi4"<<std::endl;
    return coll3Sort(e1.p1, e1.p2, e2.p1, idx, edgeS, polygon, points, lowest_index);
  }

  Point p1, p2, p3, p4;
  p1.set(*e1.p1); p2.set(*e1.p2); p3.set(*e2.p1); p4.set(*e2.p2);

  // boolean for dealing with an edge between the 2 ends of the polygon.
  // It changes how the sorting should be done in regards to the polygon.
  if ((e1.getPLow().v == 0 && e1.getPHigh().v == points.size()-1) || (e2.getPLow().v == 0 && e2.getPHigh().v == points.size()-1)) {
    end_edge = true;
  }
  else end_edge = false;

  // sort the points into lex order.
  std::vector<Point> lex {p1, p2, p3, p4};
  sort(lex.begin(), lex.end());

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "lex[" << i << "]: " << lex[i] << std::endl;

  // sort the points into polygon order.
  std::vector<unsigned int> vert {(*e1.p1).v, (*e1.p2).v, (*e2.p1).v, (*e2.p2).v};
  sort(vert.begin(), vert.end());

  // the 2 polygonal ends form an edge, and as long as either the lex. lowest 2 points or the lex. highest 2 points
  // form that edge, we assure there's no overlap of the 2 new edges.
  if (end_edge) {
    vert.pop_back();
    vert.insert(vert.begin(), points.size()-1);
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "vert[" << i << "]: " << vert[i] << std::endl;

//   then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if (lex[i].v != vert[i]) {
      eraseVertexFromSet(&points[lex[i].i], edgeS, polygon, points);
      //update_lowest_index(&points[lex[i].i], lowest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[vert[i]]], edgeS, polygon, points);
      //update_lowest_index(&points[polygon[vert[i]]], lowest_index, polygon, points);
      points[lex[i].i].v = vert[i];
      polygon[vert[i]] = lex[i].i;
      retval = true;
      if (points[polygon[vert[i]]].l < lowest_index) lowest_index = polygon[vert[i]];
    }
  }

  // code to add the edge not connected to current index back into 'edgeS'
  if (retval) {
    Edge new_edge;
    for (unsigned int i = 0; i < lex.size();++i) {
      if (lex[i].l == (*idx).l) {
        if (i < 2) new_edge.set(points[lex[2].i], points[lex[3].i]);
        else new_edge.set(points[lex[0].i], points[lex[1].i]);
        break;
      }
    }
    if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
      std::cerr << "c4s inserting: " << new_edge << std::endl;
      std::cerr << "at idx: " << *idx << std::endl;
      std::pair<std::set<Edge>::iterator,bool> retval;
      retval = edgeS.insert(new_edge);
      if (*retval.first != new_edge) {
        std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      }
    }
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "point at pol[vert[i]]: " << points[polygon[vert[i]]] << std::endl;

  return retval;
}

// Same as above, but updates 'highest_index' as well.
bool coll4Swap (Edge& e1, Edge& e2, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index) {
  bool retval = false;
  bool end_edge; // boolean for if one edge pairs [0,points.size()-1] together

//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << std::endl;
  if (*e1.p1 == *e2.p1) {
    //std::cerr << "hi1"<<std::endl;
    return coll3Sort(e1.p1, e1.p2, e2.p2, idx, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p1 == *e2.p2) {
    //std::cerr << "hi2"<<std::endl;
    return coll3Sort(e1.p1, e1.p2, e2.p1, idx, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p2 == *e2.p1) {
    //std::cerr << "hi3"<<std::endl;
    return coll3Sort(e1.p1, e1.p2, e2.p2, idx, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p2 == *e2.p2) {
    //std::cerr << "hi4"<<std::endl;
    return coll3Sort(e1.p1, e1.p2, e2.p1, idx, edgeS, polygon, points, lowest_index, highest_index);
  }

  Point p1, p2, p3, p4;
  p1.set(*e1.p1); p2.set(*e1.p2); p3.set(*e2.p1); p4.set(*e2.p2);

  // boolean for dealing with an edge between the 2 ends of the polygon.
  // It changes how the sorting should be done in regards to the polygon.
  if ((e1.getPLow().v == 0 && e1.getPHigh().v == points.size()-1) || (e2.getPLow().v == 0 && e2.getPHigh().v == points.size()-1)) {
    end_edge = true;
  }
  else end_edge = false;

  // sort the points into lex order.
  std::vector<Point> lex {p1, p2, p3, p4};
  sort(lex.begin(), lex.end());

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "lex[" << i << "]: " << lex[i] << std::endl;

  // sort the points into polygon order.
  std::vector<unsigned int> vert {(*e1.p1).v, (*e1.p2).v, (*e2.p1).v, (*e2.p2).v};
  sort(vert.begin(), vert.end());

  // the 2 polygonal ends form an edge, and as long as either the lex. lowest 2 points or the lex. highest 2 points
  // form that edge, we assure there's no overlap of the 2 new edges.
  if (end_edge) {
    vert.pop_back();
    vert.insert(vert.begin(), points.size()-1);
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "vert[" << i << "]: " << vert[i] << std::endl;

//   then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if (lex[i].v != vert[i]) {
      eraseVertexFromSet(&points[lex[i].i], edgeS, polygon, points);
      //update_lowest_index(&points[lex[i].i], lowest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[vert[i]]], edgeS, polygon, points);
      //update_lowest_index(&points[polygon[vert[i]]], lowest_index, polygon, points);
      points[lex[i].i].v = vert[i];
      polygon[vert[i]] = lex[i].i;
      retval = true;
      if (points[polygon[vert[i]]].l < lowest_index) lowest_index = polygon[vert[i]];
      if (points[polygon[vert[i]]].l > highest_index) highest_index = polygon[vert[i]];
    }
  }

  // code to add the edge not connected to current index back into 'edgeS'
  if (retval) {
    Edge new_edge;
    for (unsigned int i = 0; i < lex.size();++i) {
      if (lex[i].l == (*idx).l) {
        if (i < 2) new_edge.set(points[lex[2].i], points[lex[3].i]);
        else new_edge.set(points[lex[0].i], points[lex[1].i]);
        break;
      }
    }
    if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
      std::cerr << "c4s inserting: " << new_edge << std::endl;
      std::cerr << "at idx: " << *idx << std::endl;
      std::pair<std::set<Edge>::iterator,bool> retval;
      retval = edgeS.insert(new_edge);
      if (*retval.first != new_edge) {
        std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      }
    }
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "point at pol[vert[i]]: " << points[polygon[vert[i]]] << std::endl;

  return retval;
}

// same as coll4swap but uses coll3Sort2
bool coll4Swap2 (Edge& e1, Edge& e2, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index) {
  bool retval = false;
  bool end_edge; // boolean for if one edge pairs [0,points.size()-1] together

//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << std::endl;
  if (*e1.p1 == *e2.p1) {
    //std::cerr << "hi1"<<std::endl;
    return coll3Sort2(e1.p1, e1.p2, e2.p2, idx, edgeS, polygon, points, lowest_index);
  }
  if (*e1.p1 == *e2.p2) {
    //std::cerr << "hi2"<<std::endl;
    return coll3Sort2(e1.p1, e1.p2, e2.p1, idx, edgeS, polygon, points, lowest_index);
  }
  if (*e1.p2 == *e2.p1) {
    //std::cerr << "hi3"<<std::endl;
    return coll3Sort2(e1.p1, e1.p2, e2.p2, idx, edgeS, polygon, points, lowest_index);
  }
  if (*e1.p2 == *e2.p2) {
    //std::cerr << "hi4"<<std::endl;
    return coll3Sort2(e1.p1, e1.p2, e2.p1, idx, edgeS, polygon, points, lowest_index);
  }

  Point p1, p2, p3, p4;
  p1.set(*e1.p1); p2.set(*e1.p2); p3.set(*e2.p1); p4.set(*e2.p2);

  // boolean for dealing with an edge between the 2 ends of the polygon.
  // It changes how the sorting should be done in regards to the polygon.
  if ((e1.getPLow().v == 0 && e1.getPHigh().v == points.size()-1) || (e2.getPLow().v == 0 && e2.getPHigh().v == points.size()-1)) {
    end_edge = true;
  }
  else end_edge = false;

  // sort the points into lex order.
  std::vector<Point> lex {p1, p2, p3, p4};
  sort(lex.begin(), lex.end());

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "lex[" << i << "]: " << lex[i] << std::endl;

  // sort the points into polygon order.
  std::vector<unsigned int> vert {(*e1.p1).v, (*e1.p2).v, (*e2.p1).v, (*e2.p2).v};
  sort(vert.begin(), vert.end());

  // the 2 polygonal ends form an edge, and as long as either the lex. lowest 2 points or the lex. highest 2 points
  // form that edge, we assure there's no overlap of the 2 new edges.
  if (end_edge) {
    vert.pop_back();
    vert.insert(vert.begin(), points.size()-1);
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "vert[" << i << "]: " << vert[i] << std::endl;

//   then assign the proper 'vert' order to the 'lex' ordered points
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if (lex[i].v != vert[i]) {
      eraseVertexFromSet(&points[lex[i].i], edgeS, polygon, points);
      //update_lowest_index(&points[lex[i].i], lowest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[vert[i]]], edgeS, polygon, points);
      //update_lowest_index(&points[polygon[vert[i]]], lowest_index, polygon, points);
      points[lex[i].i].v = vert[i];
      polygon[vert[i]] = lex[i].i;
      retval = true;
      if (points[polygon[vert[i]]].l < lowest_index) lowest_index = polygon[vert[i]];
    }
  }

  // code to add the edge not connected to current index back into 'edgeS'
  if (retval) {
    Edge new_edge;
    for (unsigned int i = 0; i < lex.size();++i) {
      if (lex[i].l == (*idx).l) {
        if (i < 2) new_edge.set(points[lex[2].i], points[lex[3].i]);
        else new_edge.set(points[lex[0].i], points[lex[1].i]);
        break;
      }
    }
    if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
      std::cerr << "c4s inserting: " << new_edge << std::endl;
      std::cerr << "at idx: " << *idx << std::endl;
      std::pair<std::set<Edge>::iterator,bool> retval;
      retval = edgeS.insert(new_edge);
      if (*retval.first != new_edge) {
        std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      }
    }
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "point at pol[vert[i]]: " << points[polygon[vert[i]]] << std::endl;

  return retval;
}

// same as coll4swap but uses coll3Sort2
bool coll4Swap2 (Edge& e1, Edge& e2, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index) {
  bool retval = false;
  bool end_edge; // boolean for if one edge pairs [0,points.size()-1] together

//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << std::endl;
  if (*e1.p1 == *e2.p1) {
    //std::cerr << "hi1"<<std::endl;
    return coll3Sort2(e1.p1, e1.p2, e2.p2, idx, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p1 == *e2.p2) {
    //std::cerr << "hi2"<<std::endl;
    return coll3Sort2(e1.p1, e1.p2, e2.p1, idx, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p2 == *e2.p1) {
    //std::cerr << "hi3"<<std::endl;
    return coll3Sort2(e1.p1, e1.p2, e2.p2, idx, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p2 == *e2.p2) {
    //std::cerr << "hi4"<<std::endl;
    return coll3Sort2(e1.p1, e1.p2, e2.p1, idx, edgeS, polygon, points, lowest_index, highest_index);
  }

  Point p1, p2, p3, p4;
  p1.set(*e1.p1); p2.set(*e1.p2); p3.set(*e2.p1); p4.set(*e2.p2);

  // boolean for dealing with an edge between the 2 ends of the polygon.
  // It changes how the sorting should be done in regards to the polygon.
  if ((e1.getPLow().v == 0 && e1.getPHigh().v == points.size()-1) || (e2.getPLow().v == 0 && e2.getPHigh().v == points.size()-1)) {
    end_edge = true;
  }
  else end_edge = false;

  // sort the points into lex order.
  std::vector<Point> lex {p1, p2, p3, p4};
  sort(lex.begin(), lex.end());

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "lex[" << i << "]: " << lex[i] << std::endl;

  // sort the points into polygon order.
  std::vector<unsigned int> vert {(*e1.p1).v, (*e1.p2).v, (*e2.p1).v, (*e2.p2).v};
  sort(vert.begin(), vert.end());

  // the 2 polygonal ends form an edge, and as long as either the lex. lowest 2 points or the lex. highest 2 points
  // form that edge, we assure there's no overlap of the 2 new edges.
  if (end_edge) {
    vert.pop_back();
    vert.insert(vert.begin(), points.size()-1);
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "vert[" << i << "]: " << vert[i] << std::endl;

//   then assign the proper 'vert' order to the 'lex' ordered points
  bool changed = false;
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if (lex[i].v != vert[i]) {changed=true;break;}
  }
  if (changed) {
    for (unsigned int i = 0; i < lex.size(); ++i) {
      eraseVertexFromSet(&points[lex[i].i], edgeS, polygon, points);
      //update_lowest_index(&points[lex[i].i], lowest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[vert[i]]], edgeS, polygon, points);
      //update_lowest_index(&points[polygon[vert[i]]], lowest_index, polygon, points);
      points[lex[i].i].v = vert[i];
      polygon[vert[i]] = lex[i].i;
      retval = true;
      if (points[polygon[vert[i]]].l < lowest_index) lowest_index = polygon[vert[i]];
      if (points[polygon[vert[i]]].l > highest_index) highest_index = polygon[vert[i]];
    }
  }

  // code to add the edge not connected to current index back into 'edgeS'
  if (retval) {
    Edge new_edge;
    for (unsigned int i = 0; i < lex.size();++i) {
      if (lex[i].l == (*idx).l) {
        if (i < 2) new_edge.set(points[lex[2].i], points[lex[3].i]);
        else new_edge.set(points[lex[0].i], points[lex[1].i]);
        break;
      }
    }
    if (((*new_edge.p1).l < (*idx).l) && ((*idx).l < (*new_edge.p2).l)) {
      std::cerr << "c4s inserting: " << new_edge << std::endl;
      std::cerr << "at idx: " << *idx << std::endl;
      std::pair<std::set<Edge>::iterator,bool> retval;
      retval = edgeS.insert(new_edge);
      if (*retval.first != new_edge) {
        std::cerr << "Error!  inserting: " << new_edge << ", returned: " << *retval.first << std::endl;
      }
    }
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "point at pol[vert[i]]: " << points[polygon[vert[i]]] << std::endl;

  return retval;
}

//same as above but uses coll3sort3 and doesn't add the edge not connected to current index back in.
bool coll4Swap3 (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index) {
  bool retval = false;
  bool end_edge; // boolean for if one edge pairs [0,points.size()-1] together

//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << std::endl;
  if (*e1.p1 == *e2.p1) {
    //std::cerr << "hi1"<<std::endl;
    return coll3Sort3(e1.p1, e1.p2, e2.p2, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p1 == *e2.p2) {
    //std::cerr << "hi2"<<std::endl;
    return coll3Sort3(e1.p1, e1.p2, e2.p1, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p2 == *e2.p1) {
    //std::cerr << "hi3"<<std::endl;
    return coll3Sort3(e1.p1, e1.p2, e2.p2, edgeS, polygon, points, lowest_index, highest_index);
  }
  if (*e1.p2 == *e2.p2) {
    //std::cerr << "hi4"<<std::endl;
    return coll3Sort3(e1.p1, e1.p2, e2.p1, edgeS, polygon, points, lowest_index, highest_index);
  }

  Point p1, p2, p3, p4;
  p1.set(*e1.p1); p2.set(*e1.p2); p3.set(*e2.p1); p4.set(*e2.p2);

  // boolean for dealing with an edge between the 2 ends of the polygon.
  // It changes how the sorting should be done in regards to the polygon.
  if ((e1.getPLow().v == 0 && e1.getPHigh().v == points.size()-1) || (e2.getPLow().v == 0 && e2.getPHigh().v == points.size()-1)) {
    end_edge = true;
  }
  else end_edge = false;

  // sort the points into lex order.
  std::vector<Point> lex {p1, p2, p3, p4};
  sort(lex.begin(), lex.end());

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "lex[" << i << "]: " << lex[i] << std::endl;

  // sort the points into polygon order.
  std::vector<unsigned int> vert {(*e1.p1).v, (*e1.p2).v, (*e2.p1).v, (*e2.p2).v};
  sort(vert.begin(), vert.end());

  // the 2 polygonal ends form an edge, and as long as either the lex. lowest 2 points or the lex. highest 2 points
  // form that edge, we assure there's no overlap of the 2 new edges.
  if (end_edge) {
    vert.pop_back();
    vert.insert(vert.begin(), points.size()-1);
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "vert[" << i << "]: " << vert[i] << std::endl;

//   then assign the proper 'vert' order to the 'lex' ordered points
  bool changed = false;
  for (unsigned int i = 0; i < lex.size(); ++i) {
    if (lex[i].v != vert[i]) {changed=true;break;}
  }
  if (changed) {
    for (unsigned int i = 0; i < lex.size(); ++i) {
      eraseVertexFromSet(&points[lex[i].i], edgeS, polygon, points);
      //update_lowest_index(&points[lex[i].i], lowest_index, polygon, points);
      eraseVertexFromSet(&points[polygon[vert[i]]], edgeS, polygon, points);
      //update_lowest_index(&points[polygon[vert[i]]], lowest_index, polygon, points);
      points[lex[i].i].v = vert[i];
      polygon[vert[i]] = lex[i].i;
      retval = true;
      if (points[polygon[vert[i]]].l < lowest_index) lowest_index = polygon[vert[i]];
      if (points[polygon[vert[i]]].l > highest_index) highest_index = polygon[vert[i]];
    }
  }

//  for (unsigned int i = 0; i < lex.size();++i) std::cerr << "point at pol[vert[i]]: " << points[polygon[vert[i]]] << std::endl;

  return retval;
}

// same as coll4swap but uses coll3Sort2, and does a distance metric to decide where to add the edges into the polygonal chains.
bool coll4Swap4 (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index) {
  std::vector<Point> cp;
  double value;
  unsigned int lower1, upper1, lower2, upper2; // this is where the lowest lex. point will be placed in the polygon.
  unsigned int index;

//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p1: " << *e1.p1 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p1: " << *e2.p1 << ", is: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << std::endl;
//  std::cerr << "same point: e1p2: " << *e1.p2 << " == e2p2: " << *e2.p2 << ", is: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << std::endl;
    if (*e1.p1 == *e2.p1) {
//      std::cerr << "hi1"<<std::endl;
      return coll3Sort3(e1.p1, e1.p2, e2.p2, edgeS, polygon, points, lowest_index);
    }
    if (*e1.p1 == *e2.p2) {
//      std::cerr << "hi2"<<std::endl;
      return coll3Sort3(e1.p1, e1.p2, e2.p1, edgeS, polygon, points, lowest_index);
    }
    if (*e1.p2 == *e2.p1) {
//      std::cerr << "hi3"<<std::endl;
      return coll3Sort3(e1.p1, e1.p2, e2.p2, edgeS, polygon, points, lowest_index);
    }
    if (*e1.p2 == *e2.p2) {
//      std::cerr << "hi4"<<std::endl;
      return coll3Sort3(e1.p1, e1.p2, e2.p1, edgeS, polygon, points, lowest_index);
    }


  // add points in 'e1' to 'cp' and any extra collinear points in the polygon
  cp.emplace_back(*e1.p1);
  cp.emplace_back(*e1.p2);

  // get the starting upper and lower vertex indices for the 2 chains
  lower1 = e1.getVLow();
  upper1 = e1.getVHigh();
  lower2 = e2.getVLow();
  upper2 = e2.getVHigh();

  // do {add points from higher in the chain from 'e1'} while (new point is collinear with 'e1' and not part of the other chain)
  index = (upper1+1)%points.size();
  do {
    if (index == lower2) break;
    value = det(e1, points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      upper1 = index;
      index = (index+1)%points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after 'e1' upper:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // do {add points from lower in the chain from 'e1'} while (new point is collinear with 'e1' and not part of the other chain)
  index = (points.size()+lower1-1)%points.size();
  do {
    if (index == upper2) break;
    value = det(e1, points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      lower1 = index;
      index = (points.size()+index-1)%points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after 'e1' lower:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // add points in 'e2' to 'cp' and any extra collinear points in the polygon
  cp.emplace_back(*e2.p1);
  cp.emplace_back(*e2.p2);

  // do {add points from higher in the chain from 'e2'} while (new point is collinear with 'e2')
  index = (upper2+1)%points.size();
  do {
    if (index == lower1) break;
    value = det(e2, points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      upper2 = index;
      index = (index+1)%points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after 'e2' upper:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // do {add points from lower in the chain from 'e2'} while (new point is collinear with 'e2')
  index = (points.size()+lower2-1)%points.size();
  do {
    if (index == upper1) break;
    value = det(e2, points[polygon[index]]);
    if (value == 0) {
      cp.emplace_back(points[polygon[index]]);
      lower2 = index;
      index = (points.size()+index-1)%points.size();
    }
    else break;
  } while (true);
//  std::cerr << "after 'e2' lower:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // sort the vector
  sort(cp.begin(), cp.end());
//  std::cerr << "after sort:" << std::endl;
//  for (unsigned int i = 0; i<cp.size();++i) std::cerr << "cp[" << i << "]: " << cp[i] << std::endl;

  // update the lowest_index if necessary
  if (cp[0].l < lowest_index) lowest_index = cp[0].l;

  //in case the 2 chains are continuous, sort the lower/upper indices among themselves.
//  std::cerr << "lower1: " << lower1 << ", upper1: " << upper1 << ", lower2: " << lower2 << ", upper2: " << upper2 << std::endl;
  if (isPol1Left(upper1, lower2, points.size())) {
    if (upper1 < lower2) {
      unsigned int temp1 = lower1;
      unsigned int temp2 = upper1;
      lower1 = lower2;
      upper1 = upper2;
      lower2 = temp1;
      upper2 = temp2;
    }
  }
  if (isPol1Left(upper2, lower1, points.size())) {
    if (upper2 < lower1) {
      unsigned int temp1 = lower1;
      unsigned int temp2 = upper1;
      lower1 = lower2;
      upper1 = upper2;
      lower2 = temp1;
      upper2 = temp2;
    }
  }
//  std::cerr << "lower1: " << lower1 << ", upper1: " << upper1 << ", lower2: " << lower2 << ", upper2: " << upper2 << std::endl;


  // go through the chains from lower to upper and add the point index into that index.
  index = 0;
  for (unsigned int i=lower1; i != (upper1+1)%points.size(); i=(i+1)%points.size()) {
//    std::cerr << "index: " << index << ", i: " << i << std::endl;
    eraseVertexFromSet(&points[cp[index].i], edgeS, polygon, points);
    eraseVertexFromSet(&points[polygon[i]], edgeS, polygon, points);
    points[cp[index].i].v = i;
    polygon[i] = cp[index].i;
    index++;
  }
//  std::cerr << "next loop" << std::endl;
  // we can continue with the same index position in 'cp'
  for (unsigned int i=lower2; i != (upper2+1)%points.size(); i=(i+1)%points.size()) {
//    std::cerr << "index: " << index << ", i: " << i << std::endl;
    eraseVertexFromSet(&points[cp[index].i], edgeS, polygon, points);
    eraseVertexFromSet(&points[polygon[i]], edgeS, polygon, points);
    points[cp[index].i].v = i;
    polygon[i] = cp[index].i;
    index++;
  }

//  std::cerr << "chain 1:" << std::endl;
//  pdisplay(lower1, upper1, polygon, points);
//  std::cerr << "chain 2:" << std::endl;
//  pdisplay(lower2, upper2, polygon, points);

  return true;
}

// Function that takes 2 edges, e1 and e2 that both intersect and are collinear
bool collSwap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  bool d1, d2;
  double rd1 = reldist(e1, *e2.p1);
  double rd2 = reldist(e1, *e2.p2);

  if (((*e1.p1).v == 0) && (*e1.p2).v == polygon.size()-1)
    d1 = false;
  else if (((*e1.p2).v == 0) && (*e1.p1).v == polygon.size()-1)
    d1 = true;
  else {
    d1 = ((int)(*e1.p1).v - (int)(*e1.p2).v) < 0; // true  means edge order in poly is : [...p1p2...]
  }

  if (((*e2.p1).v == 0) && (*e2.p2).v == polygon.size()-1)
    d2 = false;
  else if (((*e2.p2).v == 0) && (*e2.p1).v == polygon.size()-1)
    d2 = true;
  else {
    d2 = ((int)(*e2.p1).v - (int)(*e2.p2).v) < 0; // false means edge order in poly is : [...p2p1...]
  }

  if ((rd1 > 1) && (rd2 > 1)) return false;
  if ((rd1 < 0) && (rd2 < 0)) return false;
  //std::cerr << "erasing because of 4PC: e1: " << e1 << ", e2: " << e2 << std::endl;
  if ( d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p2, e2.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    return true;
  }
  if (!d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    polSwap(e1.p2, e2.p2, polygon);
    return true;
  }

  if ( d1 &&  d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p1, e1.p1, polygon);
    return true;
  }
  if (!d1 && !d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p1, e1.p1, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }

  if ( d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }
  if (!d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    return true;
  }

  if ( d1 &&  d2 && (rd1 < 0) && (rd2 > 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    eraseVertexFromSet(e1.p2, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e1.p2, e2.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 < 0) && (rd2 > 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 < 0) && (rd2 > 1)) {
    if ((*e1.p1).v + (*e1.p2).v < (*e2.p1).v + (*e2.p2).v) {
      eraseVertexFromSet(e1.p1, edgeS, polygon, points);
      eraseVertexFromSet(e2.p1, edgeS, polygon, points);
      eraseVertexFromSet(e1.p2, edgeS, polygon, points);
      eraseVertexFromSet(e2.p2, edgeS, polygon, points);
      polSwap(e1.p1, e2.p1, polygon);
      polSwap(e1.p2, e1.p1, polygon);
      polSwap(e1.p2, e2.p2, polygon);
      return true;
    } else {
      eraseVertexFromSet(e1.p1, edgeS, polygon, points);
      eraseVertexFromSet(e2.p2, edgeS, polygon, points);
      eraseVertexFromSet(e1.p2, edgeS, polygon, points);
      eraseVertexFromSet(e2.p1, edgeS, polygon, points);
      polSwap(e1.p1, e2.p2, polygon);
      polSwap(e1.p1, e1.p2, polygon);
      polSwap(e2.p1, e2.p2, polygon);
      return true;
    }

  }
  if (!d1 && !d2 && (rd1 < 0) && (rd2 > 1)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p2, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p1, e1.p1, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 < 0) && (fabs(rd2 - 1) < EPSILON)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    polSwap(e2.p1, e1.p1, polygon);
    polSwap(e2.p1, e2.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 < 0) && (fabs(rd2 - 1) < EPSILON)) {
    eraseVertexFromSet(e1.p1, edgeS, polygon, points);
    eraseVertexFromSet(e2.p1, edgeS, polygon, points);
    polSwap(e1.p1, e2.p1, polygon);
    polSwap(e2.p1, e2.p2, polygon);
    return true;
  }

  std::cerr << "ERROR in 4P collSwap: fallthrough!" << std::endl;
  std::cerr << "e1: " << e1 << ", e2: " << e2 << std::endl;
  std::cerr << "edge collswap: rd1: " << rd1 << ", rd2: " << rd2 << std::endl;
  std::cerr << "d1: " << ((d1) ? "true" : "false") << ", d2: " << ((d2) ? "true" : "false") << std::endl;
  return true;
}

// function to calculate the circumference of the polygon
double pol_calc_circumference(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  double circumference=0, x, y;
  Point prev, p;

  prev = points[polygon[polygon.size()-1]];
  for (unsigned int i = 0; i < polygon.size(); ++i) {
    p = points[polygon[i]];
    x = p.x - prev.x;
    y = p.y - prev.y;
    circumference = circumference + sqrt(x*x + y*y);
    prev = p;
  }
  return circumference;
}

// function to calculate the area of a polygon given by points in 'points' and the permutation in 'polygon'.
double pol_calc_area(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  double Area = 0;
  Point prev, p;
  unsigned int index;

  prev = points[polygon[polygon.size()-1]];
  // for each vertex in the polygon, index is the latter point
  for (index = 0; index < polygon.size(); ++index) {
    p = points[polygon[index]];

    Area += p.y * prev.x - p.x * prev.y;
    prev = p;
  }

  Area = Area / 2;
  return Area;
}

// function to calculate the area of a polygon given by 'points'.
// assumption: the vector is already in the order of the polygon.
double pol_calc_area(std::vector<Point>& points) {
  double Area = 0;
  Point prev, p;
  unsigned int index;

  prev = points[points.size()-1];
  // for each vertex in the polygon, index is the latter point
  for (index = 0; index < points.size(); ++index) {
    p = points[index];

    Area += p.y * prev.x - p.x * prev.y;
    prev = p;
  }

  Area = Area / 2;
  return Area;
}

// function that calculates the sum of the edges from the 2 edges connected to vertex 'start' to the 2 edges connected to vertex 'stop'
double pol_calc_chain_length(unsigned int start, unsigned int stop, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  double length = 0;

  if (is_ascending(start, stop, points.size())) {
    //std::cerr << "asc." << std::endl;
    for (unsigned int i = start; i != (stop+2)%points.size(); i = (i+1)%points.size()) {
      //std::cerr << "i-1: " << (i+points.size()-1)%points.size() << ", i: " << i << ", length: " << get_length(points[polygon[(i+points.size()-1)%points.size()]], points[polygon[i]]) << std::endl;
      length = length + get_length(points[polygon[(i+points.size()-1)%points.size()]], points[polygon[i]]);
    }
  }
  else {
//    std::cerr << "desc." << std::endl;
    for (unsigned int i = start; i != (stop+points.size()-2)%points.size(); i = (i+points.size()-1)%points.size()) {
//      std::cerr << "i+1: " << (i+points.size()+1)%points.size() << ", i: " << i << ", length: " << get_length(points[polygon[(i+1)%points.size()]], points[polygon[i]]) << std::endl;
      length = length + get_length(points[polygon[(i+1)%points.size()]], points[polygon[i]]);
    }
  }

  return length;
}

// function that checks if the points in 'points' given by indexes in 'polygon'
// (can be a subset of 'points' of minimum size 3) is 2 dimensional.
bool is_2D(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  assert(polygon.size() > 2);
  Edge e = Edge (&points[polygon[0]], &points[polygon[1]]);
  Point p;

  for (unsigned int i = 2; i < polygon.size(); ++i) {
    p = points[polygon[i]];
    if (det(e,p) > 0) return true;
  }
  return false;
}

// function to safely get the proper direction of ascension in polygon
// returns true if going from 'p2' to 'p1' is ascending the polygon.
bool is_ascending(I_Edge e) {
  if (e.l2ch) {
    if ((*e.p1).v == 0) {
      if ((*e.p2).v == 1) return true;
      else return false;
    }
    if ((*e.p2).v == 0) {
      if ((*e.p1).v == 1) return false;
      else return true;
    }
    if ((*e.p1).v < (*e.p2).v) return true;
    return false;
  }
  else {
    if ((*e.p1).v == 0) {
      if ((*e.p2).v == 1) return false;
      else return true;
    }
    if ((*e.p2).v == 0) {
      if ((*e.p1).v == 1) return true;
      else return false;
    }
    if ((*e.p1).v < (*e.p2).v) return false;
    return true;
  }
  // should never reach the end
  assert(false);
}

// function that takes 2 vertex indices in 'polygon': 'start' and 'stop'.
// as there are 2 paths from 'start' to 'stop', return if the shorter path is ascending from 'start' to 'stop',
//  i.e. ++i%polygon.size() would work in a for loop.
bool is_ascending(unsigned int start, unsigned int stop, unsigned int cycle) {
  unsigned int left, right; // 'stop' is considered the center, and 'left' is the number of vertices to 'stop's left until you hit 'start' and v.v.

  if (start < stop) {
    left = stop-start;
    right = start+cycle-stop;
  }
  else {
    left = start-stop;
    right = stop+cycle-start;
  }

  if (left < right) return true; // ascending from 'start' to 'stop' is a shorter distance.
  else return false;
}

// function to get the lower index distance from one index to another in a cyclic index, such as a polygon
unsigned int get_lower_cyclic_difference(unsigned int start, unsigned int stop, unsigned int cycle) {
  unsigned int left, right; // 'stop' is considered the center, and 'left' is the number of vertices to 'stop's left until you hit 'start' and v.v.

  if (start < stop) {
    left = stop-start;
    right = start+cycle-stop;
  }
  else {
    left = start-stop;
    right = stop+cycle-start;
  }

  if (left < right) return left;
  else return right;
}

// function to get a length of a chain in a cyclic index, such as a polygon
// given a certain 'lower' index and a 'higher' index and a 'cycle' which is the polygon size.
// INPUTS:
//    'par'       : pair of 'I_Edge's which define an inner polygonal chain
//    'ascending' : boolean that defines to ascend through the polygonal index
//                  from 'par.first' to 'par.second' or not
// OUTPUT: length of the inner chain
unsigned int get_chain_length(Ends ends, unsigned int cycle) {
  unsigned int a, b, l;

  // 'a' is 'par.first' ch point 'v' value, 'b' is 'par.second' ch point 'v' value
  a = (ends.par.first.l2ch ? (*ends.par.first.p1).v : (*ends.par.first.p2).v);
  b = (ends.par.second.l2ch ? (*ends.par.second.p1).v : (*ends.par.second.p2).v);

  if (is_ascending(ends.par.first)) { // ascending from a to b
    // there's a cycle reset inside the chain
    // -1 because we assume a and b are on the convex hull, so one points needs to be deducted.
    if (b < a) l = b + cycle -a -1;
    else l = b -a -1;
  }
  else { // descending from a to b
    if (a < b) l = a + cycle -b -1; // there's a cycle reset inside the chain
    else l = a -b -1;
  }
  return l;
}

// function that checks if the chain defined by the 2 edges in 'par' of
// the polygon in 'polygon' of the points in 'points' is 2 dimensional.
bool is_2D(Ends ends, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  assert(polygon.size() > 2);

  unsigned int a, b, l;
  bool ascending;
  int it;
  Edge e;
  Point p;

  // 'a' is 'par.first' ch point 'v' value, 'b' is 'par.second' ch point 'v' value
  a = (ends.par.first.l2ch ? (*ends.par.first.p1).v : (*ends.par.first.p2).v);
  b = (ends.par.second.l2ch ? (*ends.par.second.p1).v : (*ends.par.second.p2).v);

  // get direction from 'a' to 'b' through the inner chain, whether it is ascending through the index or descending
  ascending = is_ascending(ends.par.first);
//  std::cerr << "a: " << a << ", b: " << b << ", ascending: " << (ascending ? "true" : "false") << std::endl;
  if (ascending) it = 1;
  else it = -1;

  // get length of the inner chain
  l = get_chain_length(ends, polygon.size());
//  std::cerr << "length: " << l << std::endl;
  if (l < 3) return false;

  e = Edge (&points[polygon[(polygon.size() + a + it) % polygon.size()]], &points[polygon[(polygon.size() + b - it) % polygon.size()]]);
//  std::cerr << "inner edge: " << e << std::endl;
  unsigned int i = (polygon.size() + a + 2*it) % polygon.size();
  b = b-it; // small reduction so that I don't have to calculate (b-it) each iteration in next while loop.
//  std::cerr << "inner indices: i: " << i << ", b: " << b << std::endl;

  while (i != b) {
    p = points[polygon[i]];
//    std::cerr << "p: " << p << ", det: " << det(e,p) << std::endl;
    if (det(e,p) != 0) return true;
    i = (i + it) % polygon.size();
  }
  return false;
}

// function that checks if the chain defined by the 2 edges in 'e' and 'e.closest' of
// the polygon in 'polygon' of the points in 'points' is 2 dimensional.
// An inner polygonal chain will have the c.h. points as [0] and [last]
// A hole will have no c.h. points in it.
// An inner polygonal chain can have [0,last] as one edge and the rest of the chain as the other
// A hole must be 2D on both sides of the edges.
bool is_2D(E_Edge candidate, std::vector<unsigned int>& polygon, std::vector<Point>& points, bool is_hole) {
  std::cerr << "=== is_2D function ===" << std::endl;
  if (polygon.size() < 3) return false;

  unsigned int a, b;
  bool is2d = false;
  Edge e1, e2, e3, e4;
  Point p;

  // get the edge that is lex. lower in the polygon (easier to compare to [0,last] later)
  if (candidate.getVLow() < candidate.closest[0].getVLow()) {
    e1 = Edge(candidate.p1, candidate.p2);
    e2 = Edge(candidate.closest[0].p1, candidate.closest[0].p2);
  }
  else {
    e1 = Edge(candidate.closest[0].p1, candidate.closest[0].p2);
    e2 = Edge(candidate.p1, candidate.p2);
  }
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;

  if (is_hole) {
    // this is a hole, so both chains on each side of 'e1' and 'e2' must be 2D
    // chain1: from higher vertex of e1 to lower vertex of e2
    a = e1.getVHigh();
    b = e2.getVLow();
    std::cerr << "a: " << a << ", b: " << b << std::endl;
    if ((b-a) < 2) {
      std::cerr << "not 2D" << std::endl;
      return false;
    }

    e3 = Edge(&points[polygon[(a)%polygon.size()]], &points[polygon[(a+1)%polygon.size()]]);
    std::cerr << "e3: " << e3 << std::endl;
    for (unsigned int i = (a+2)%polygon.size(); i != (b+1)%polygon.size(); i = (i+1)%polygon.size()) {
      p = points[polygon[i]];
      std::cerr << "p: " << p << std::endl;
      std::cerr << "det: " << det(e3,p) << ", det != 0: " << ((det(e3,p) != 0) ? "true" : "false") << std::endl;
      if (det(e3,p) != 0) {
        is2d = true;
        break;
      }
    }
    if (!is2d) return false; // if the earlier chain wasn't 2D, then we skip the rest.

    // next check if the other part of the chain is 2D
    a = e1.getVLow();
    b = e2.getVHigh();
    std::cerr << "a: " << a << ", b: " << b << ", diff: " << (polygon.size()+a-b)%polygon.size() << std::endl;
    if (2 > (polygon.size()+a-b)%polygon.size()) return false; // calc. includes both endpoints, so has to be 5 or more

    e4 = Edge(&points[polygon[b%polygon.size()]], &points[polygon[(b+1)%polygon.size()]]);
    std::cerr << "e4: " << e4 << std::endl;
    for (unsigned int i = (b+2)%polygon.size(); i != (a+1)%polygon.size(); i = (i+1)%polygon.size()) {
      std::cerr << "i: " << i << std::endl;
      p = points[polygon[i]];
      std::cerr << "p: " << p << std::endl;
      if (det(e4,p) != 0) {
        return true;
      }
    }
  }
  else {
    // c.h. points are [0] and [last], if e2 is the c.h. edge, it should be the edge with [0]
    if ((e2.getVHigh() == 0) && (e2.getVLow() == polygon.size()-1)) {
      std::cerr << "one edge is the c.h. edge." << std::endl;
      // e2 is the c.h. edge.  rest of the points can be tested against e1.
      for (unsigned int i = 1; i < polygon.size()-2; ++i) {
        p = points[polygon[i]];
        if (det(e1,p) != 0) return true;
      }
    }
    else {
      // otherwise we check the chain between e1 and e2.
      a = e1.getVHigh();
      b = e2.getVLow();
      std::cerr << "a: " << a << ", b: " << b << std::endl;
      if ((b-a) < 2) {
        std::cerr << "not 2D." << std::endl;
        return false;
      }

      e3 = Edge(&points[polygon[(a)%polygon.size()]], &points[polygon[(a+1)%polygon.size()]]);
      std::cerr << "e3: " << e3 << std::endl;
      for (unsigned int i = (a+2)%polygon.size(); i != (b+1)%polygon.size(); i = (i+1)%polygon.size()) {
        p = points[polygon[i]];
//        std::cerr << "p: " << p << std::endl;
//        std::cerr << "det: " << det(e3,p) << ", det != 0: " << ((det(e3,p) != 0) ? "true" : "false") << std::endl;
        if (det(e3,p) != 0) {
          return true;
        }
      }
    }
  }
  return false;
}


// Temporary 'slow' function to fill 'hole' vector with the hole given by 'end' in polygon 'polygon'
void get_hole_and_new_pol(std::vector<unsigned int>& hole, std::vector<unsigned int>& new_polygon, E_Edge& e, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  std::cerr << "=== get_hole_and_new_pol function ===" << std::endl;
  assert(hole.size() == 0);
  assert(new_polygon.size() == 0);

  std::vector<unsigned int> pol1, pol2;
  bool inside = false;
  Edge e1 = Edge(e.p1, e.p2);
  Edge e2 = Edge(e.closest[e.closest.size()-1].p1, e.closest[e.closest.size()-1].p2);

  std::cerr << "edge 1: " << e1 << std::endl;
  std::cerr << "edge 2: " << e2 << std::endl;
//  std::cerr << "current polygon: " << std::endl;
//  pdisplay(polygon, points);

  Point p, prev;
  prev = points[polygon[polygon.size()-1]];
  for (unsigned int i = 0; i < polygon.size(); ++i) {
//    std::cerr << "i: " << i << ", polygon[i]: " << polygon[i] << ", point: " << points[polygon[i]];
    p = points[polygon[i]];
    Edge i_e = Edge (&prev, &p);

    if (i_e == e1 || i_e == e2) inside = !inside;

    if (inside) pol1.push_back(p.i);//std::cerr<<", inside"<<std::endl;}
    else pol2.push_back(p.i);//std::cerr<<", not inside"<<std::endl;}
    prev = p;
  }

  // to make sure "polygon" is the polygon, find the polygon with lower x-coordinate of the 2 polygons.
  double xmin = INFINITY;
  if (pol1.size() < pol2.size()) {
    for (unsigned int i = 0; i < pol1.size(); ++i) {
      if (points[pol1[i]].x < xmin) xmin = points[pol1[i]].x;
    }
    for (unsigned int i = 0; i < pol2.size(); ++i) {
      if (points[pol2[i]].x < xmin) {
        hole = pol1;
        new_polygon = pol2;
        return;
      }
    }
    hole = pol2;
    new_polygon = pol1;
  }
  else {
    for (unsigned int i = 0; i < pol2.size(); ++i) {
      if (points[pol2[i]].x < xmin) xmin = points[pol2[i]].x;
    }
    for (unsigned int i = 0; i < pol1.size(); ++i) {
      if (points[pol1[i]].x < xmin) {
        hole = pol2;
        new_polygon = pol1;
        return;
      }
    }
    hole = pol1;
    new_polygon = pol2;
  }
//  std::cerr << "polygon:" << std::endl;
//  pdisplay(new_polygon, points);
//  std::cerr << "hole:" << std::endl;
//  pdisplay(hole, points);
}

// same as above, except it accepts a boolean 'is_hole' which, if false,
// changes the edges, if either is the edge of the c.h. which is [0,last]
void get_hole_and_new_pol(std::vector<unsigned int>& hole, std::vector<unsigned int>& new_polygon, E_Edge& e, std::vector<unsigned int>& polygon, std::vector<Point>& points, bool is_hole) {
  std::cerr << "=== get_hole_and_new_pol function ===" << std::endl;
  assert(hole.size() == 0);
  assert(new_polygon.size() == 0);

  std::vector<unsigned int> pol1, pol2;
  bool inside = false;
  Edge e1, e2;
  if (!is_hole && ((e.getVLow() == polygon.size()-1 && e.getVHigh() == 0) || (e.closest[0].getVLow() == polygon.size()-1 && e.closest[0].getVHigh() == 0))) {
    e1 = Edge(&points[polygon[0]], &points[polygon[1]]);
    e2 = Edge(&points[polygon[polygon.size()-2]], &points[polygon[polygon.size()-1]]);
  }
  else {
    e1 = Edge(e.p1, e.p2);
    e2 = Edge(e.closest[e.closest.size()-1].p1, e.closest[e.closest.size()-1].p2);
  }
  std::cerr << "edge 1: " << e1 << std::endl;
  std::cerr << "edge 2: " << e2 << std::endl;
//  std::cerr << "current polygon: " << std::endl;
//  pdisplay(polygon, points);

  Point p, prev;
  prev = points[polygon[polygon.size()-1]];
  for (unsigned int i = 0; i < polygon.size(); ++i) {
//    std::cerr << "i: " << i << ", polygon[i]: " << polygon[i] << ", point: " << points[polygon[i]];
    p = points[polygon[i]];
    Edge i_e = Edge (&prev, &p);

    if (i_e == e1 || i_e == e2) inside = !inside;

    if (inside) pol1.push_back(p.i);//std::cerr<<", inside"<<std::endl;}
    else pol2.push_back(p.i);//std::cerr<<", not inside"<<std::endl;}
    prev = p;
  }

  // to make sure "polygon" is the polygon, find the polygon with a c.h. point.
  Point end = points[polygon[0]];
  if (pol1.size() < pol2.size()) {
    for (unsigned int i = 0; i < pol1.size(); ++i) {
      if (points[pol1[i]] == end) {
        hole = pol2;
        new_polygon = pol1;
        return;
      }
    }
    hole = pol1;
    new_polygon = pol2;
  }
  else {
    for (unsigned int i = 0; i < pol2.size(); ++i) {
      if (points[pol2[i]] == end) {
        hole = pol1;
        new_polygon = pol2;
        return;
      }
    }
    hole = pol2;
    new_polygon = pol1;
  }
  std::cerr << "polygon:" << std::endl;
  pdisplay(new_polygon, points);
  std::cerr << "hole:" << std::endl;
  pdisplay(hole, points);
}

void get_new_inner_polygon(Ends& end, std::vector<unsigned int>& new_inner_polygon, std::vector<unsigned int>& new_polygon, std::vector<Point>& points) {
  std::cerr << "end: " << end << std::endl;
  std::cerr << "nip: " << new_inner_polygon.size() << ", np: " << new_polygon.size() << "points: " << points.size() << std::endl;
}

// function to fill the 'inner_polygon' vector with the inner polygonal chain of 'polygon' defined by 'ends'
// This includes the points on the convex hull as they are a candidate to be used to create the hole.
bool get_inner_chain_polygon(std::vector<unsigned int>& inner_polygon, Ends& ends, std::vector<unsigned int>& polygon) {
  assert(polygon.size() > 2);

  unsigned int a, b, l;
  bool ascending;
  int it;
  Edge e;
  Point p;

  // 'a' is 'par.first' ch point 'v' value, 'b' is 'par.second' ch point 'v' value
  a = (ends.par.first.l2ch ? (*ends.par.first.p1).v : (*ends.par.first.p2).v);
  b = (ends.par.second.l2ch ? (*ends.par.second.p1).v : (*ends.par.second.p2).v);

  // get direction from 'a' to 'b' through the inner chain, whether it is ascending through the index or descending
  ascending = is_ascending(ends.par.first);
//  std::cerr << "a: " << a << ", b: " << b << ", ascending: " << (ascending ? "true" : "false") << std::endl;

  l = get_chain_length(ends, polygon.size());
//  std::cerr << "length: " << l << std::endl;
  if (l < 3) return false;

  if (ascending) it = 1;
  else it = -1;

  unsigned int i = a;
  b = (polygon.size() + b + it) % polygon.size(); // c.h. point allowed to be a part of the inner polygon.
  while (i != b) {
    inner_polygon.push_back(polygon[i]);
    i = (polygon.size() + i + it) % polygon.size();
  }
  return true;
}

// function to fill 'inner_polygon' with the points defined by 'ends' from the polygon in 'inner_polygon' with points in 'points'
// inner_polygon is not changed.
void get_inner_chain_points(std::vector<Point>& inner_points, std::vector<unsigned int>& inner_polygon, std::vector<Point>& points) {
  assert(inner_polygon.size() > 2);

  Point p;

  for (unsigned int i = 0; i < inner_polygon.size(); ++i) {
//    std::cerr << "inner_polygon[i]: " << inner_polygon[i] << std::endl;
    p = Point(points[inner_polygon[i]]);
    p.setI(i);
    p.setV(i);
    p.setL(0);
//    std::cerr << p << std::endl;
    inner_points.push_back(p);
  }
//  std::cerr << "points after inner chain:" << std::endl;
//  pdisplay(inner_points);
//  std::cerr << "polygon after inner chain:" << std::endl;
//  pdisplay(inner_polygon, inner_points);
}

// creates a random polygon from a given pointset and random seed.
void createRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
	if (randseed) mt.seed(randseed);

	unsigned int randpos, temp;
	polygon.resize(points.size());

	for(unsigned int i = 0; i < polygon.size();++i) {
		polygon[i] = points[i].i;
	}

  unsigned int i = polygon.size();
	do {
    --i;
		UniformRandomI(randpos, 0, i);
		temp = polygon[i];
		polygon[i] = polygon[randpos];
		polygon[randpos] = temp;
		points[polygon[i]].v = i;
	} while (i!= 0);

  //std::cerr << "polygon: " << std::endl;
  //pdisplay(polygon, points);
}

// function to create a random polygon where the convex hull points are in relative CCW order.
void createCHRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
	if (randseed) mt.seed(randseed);
	unsigned int randpos;

	// start with getting all c.h. points.
	std::vector<unsigned int> ch;
	get_convex_hull(ch, points);
  //std::cerr << "ch: " << std::endl;
  //pdisplay(ch, points);
  // get all inner points.
  std::vector<unsigned int> ip;
	get_inner_points(ip, ch, points);

	polygon.resize(points.size());

  unsigned int i = polygon.size();
	do {
    --i;
		UniformRandomI(randpos, 0, ch.size()+ip.size()-1);
    if (randpos < ch.size()) {
      polygon[i] = ch[ch.size()-1]; // using always the last point make sure the c.h. is orderly distributed in the polygon.
      points[polygon[i]].v = i;
      ch.pop_back();
    }
    else {
      randpos = randpos - ch.size();
      polygon[i] = ip[randpos];
      ip[randpos] = ip[ip.size()-1];
      points[polygon[i]].v = i;
      ip.pop_back();
    }
	} while (i != 0) ;

  //std::cerr << "polygon: " << std::endl;
  //pdisplay(polygon, points);
}


// function to return the pairs of edges that are the beginning of a polygonal chain
// that ends in incidental convex hull points.
void get_inner_chains_to_ch(std::vector<Ends>& ends, std::vector<unsigned int>& ch, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  Point prev, p, next;
  unsigned int diff;
  bool is_left, inner;
//  std::cerr << "ch: " << std::endl;
//  pdisplay(ch, points);

  for (unsigned int i = 0; i < ch.size(); ++i) {
    prev = points[ch[(ch.size() + i - 1) % ch.size()]];
    p = points[ch[i]];
    next = points[ch[(ch.size() + i + 1) % ch.size()]];

    // get the difference in index distance between 'prev' and 'p'
    // it's just a quick check to make sure that the 2 convex hull points aren't connected by a single edge.
    diff = get_lower_cyclic_difference(p.v, prev.v, polygon.size());
//    std::cerr << "diff: " << diff << std::endl;
    //diff = 1 means the 2 c.h. points are connected by an edge.
    if (diff > 1) {
//      std::cerr << "prev: " << prev << std::endl;
//      std::cerr << "p: " << p << std::endl;
//      std::cerr << "next: " << next << std::endl;

      // 'p' and 'prev' create a "inner" polygonal chain and an "outer" polygonal chain.
      // [...outer...,'prev',...inner...,'p',...outer...] as an example
      // if 'next' c.h. points is inside the 'inner' p. chain, then the inner curve defined by the 2 c.h. points is the 'outer' p. chain
      // and vice versa.
      if (prev.v < p.v) {
        is_left = true;
        // check if 'next' is either lower than 'p' and higher than 'next'
        if ((prev.v < next.v) && (next.v < p.v)) inner = false; //do not use the inner boundary between 'p' and 'prev'
        else inner = true;
      }
      else {
        is_left = false;
        if ((p.v < next.v) && (next.v < prev.v)) inner = false; //do not use the inner boundary between 'p' and 'prev'
        else inner = true;
      }

      // create the edges
      I_Edge e1, e2;
      if (is_left ^ inner) {
        e1 = I_Edge (&points[polygon[prev.v]], &points[polygon[(polygon.size() + prev.v - 1) % polygon.size()]]);
        e2 = I_Edge (&points[polygon[p.v]], &points[polygon[(polygon.size() + p.v + 1) % polygon.size()]]);
      }
      else {
        e1 = I_Edge (&points[polygon[prev.v]], &points[polygon[(polygon.size() + prev.v + 1) % polygon.size()]]);
        e2 = I_Edge (&points[polygon[p.v]], &points[polygon[(polygon.size() + p.v - 1) % polygon.size()]]);
      }
      // set the l2ch boolean of the edges
      if (*e1.p1 == prev) e1.l2ch = true;
      if (*e2.p1 == p) e2.l2ch = true;
//      std::cerr << "Edges: e1: " << e1 << ", e2: " << e2 << std::endl;
      Ends par (e1, e2);
//      std::cerr << "Ends: " << par << std::endl;
      ends.push_back(par);
    }
  }
}

// Function to return if "in" orientation of the closed edge that makes the hole,
// i.e. edge(a,b) given a hole defined by a vector of points: [a,...,b],
// matches the "in" orientation of the rest of the edges.
// "true" means the hole is a correctly oriented hole
// "false" means the hole is flipped and a part of the outer simple polygon could intersect the hole.
bool check_in_orientation (E_Edge e1, std::vector<Point>& points) {
  std::cerr << "=== check_in_orientation ===" << std::endl;
  E_Edge e;
  // check if e1 or e1.closest[0] is the edge with the first point in 'points'
  if ((*e1.p1 == points[0]) || (*e1.p2 == points[0])) e = e1;
  else e = e1.closest[0];
  std::cerr << "e: " << e << std::endl;
  bool use_p1;
  if (*e1.p1 == points[0]) use_p1 = true;
  else use_p1 = false;
  Point p1 = points[1];
  Point p2 = points[points.size()-1];

  // get the angle between 'e' and p1;
  double angle_e = get_angle(e, p1, use_p1);
  std::cerr << "angle_e: " << angle_e << std::endl;
  // get the convex angle
  double angle_c = get_angle(e, p2, use_p1);
  std::cerr << "angle_c: " << angle_c << std::endl;

  // if the hole is flipped, all points lie inside the c.h. of the 2 edges that make the hole
  // which means if fabs(angle2) is smaller than fabs(angle1) all is good.
  std::cerr << "a_c < a_e: " << ((fabs(angle_c) < fabs(angle_e)) ? "true" : "false") << std::endl;
  return (fabs(angle_c) < fabs(angle_e));
}

// really slow version to check whether a polygon has an intersection.
// Better to use a form of the line-sweep to check.
bool checkAllIntersections (std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  unsigned int count = 0;
  bool ret = false;
  Point *previ, *pi, *prevj, *pj;
  Edge ei, ej;
  enum intersect_t retval;

  previ = &points[polygon[polygon.size()-1]];
  for (unsigned int i = 0; i < polygon.size();++i) {
    pi = &points[polygon[i]];
    ei = Edge(previ, pi);
    prevj = &points[polygon[polygon.size()-1]];
    for (unsigned int j = 0; j < polygon.size();++j) {
      pj = &points[polygon[j]];
      ej = Edge(prevj, pj);
      retval = checkIntersection(ei, ej);

      if ((retval == IS_TRUE) || (retval == IS_4P_COLLINEAR) || (retval == IS_3P_COLLINEAR)) {
        std::cerr << "Intersection: "; printEnum(retval);
        std::cerr << ", ei: " << ei << ", ej: " << ej << std::endl;

        ++count;
        ret = true;
      }
      prevj = pj;
    }
    previ = pi;
  }
  std::cerr << "Number of intersections: "<< count << std::endl;
  return ret;
}
