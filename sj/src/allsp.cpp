#include <iostream> // for endl
#include <assert.h>
#include <array>
#include "basicDefinitions.h"
#include "basicFunctions.h" // for printEnum
#include "io.h"
#include "point.h"
#include "edge.h"
#include "heap.h"
#include "simple_pol_check.h"
#include "allsp.h"


enum error allsp(std::vector<Point>& points) {
  //unsigned int arr_s = points.size()*points.size();
  //std::vector<std::vector<bool> > intersects;

  //for (unsigned int i = 0; i < arr_s;++i) {
  //  intersects.push_back(std::vector<bool> (arr_s,0));
  //}
  //std::cerr << "intersects: " << intersects.size() << std::endl;

  std::vector<unsigned int> polygon;
  for (unsigned int i = 0; i < points.size();++i) {
    points[i].v = polygon.size();
    polygon.push_back(i);
  }
  bool intersects = false;
  heap2(polygon, polygon.size()-1, intersects, points);


  return SUCCESS;
}

void swap2 (std::vector<unsigned int>& a, int i, int j, bool& intersects, std::vector<Point>& points) {
  unsigned int temp = a[i];
  a[i] = a[j];
  a[j] = temp;
  points[a[i]].v = i;
  points[a[j]].v = j;
  // should check incidental edges to swapped vertices for intersections in 'intersects'
  // i is never smaller than j, j is always 0
  enum intersect_t isval;
  std::vector<Edge> e;
  e.push_back(Edge(&points[a[0]], &points[a[a.size()-1]]));
  e.push_back(Edge(&points[a[0]], &points[a[1]]));
  e.push_back(Edge(&points[a[i]], &points[a[i-1]]));
  e.push_back(Edge(&points[a[i]], &points[a[i+1]]));

  intersects = false;
  for (unsigned int k = 0; k < 3; ++k) {
    for (unsigned int l = k+1; l < e.size(); ++l) {
//      std::cerr << "k: " << k << ", l: " << l << std::endl;
      isval = checkIntersection(e[k], e[l]);
//      printEnum(isval); std::cerr << std::endl;
      if (isval == IS_TRUE || isval == IS_4P_COLLINEAR) {
        intersects = true;
//        std::cerr << "swap: intersection" << std::endl;
//        std::cerr << "e1: " << e[k] << std::endl;
//        std::cerr << "e2: " << e[l] << std::endl;
//        std::cerr << "same edge? " << ((e[k] == e[l]) ? "yes" : "no") << std::endl;
        break;
      }
    }
    if (isval == IS_TRUE || isval == IS_4P_COLLINEAR) break;
  }
}

enum error heap2(std::vector<unsigned int>& polygon, int n, bool& intersects, std::vector<Point>& points) {
  if(n == 1) {
    // (got a new permutation)

    // verify the polygon
    // verify should return a pair of edges if there was an intersection
    // is that enough or do I need a boolean as well for when no intersection?
    // add the intersection into 'intersects'
    if (!intersects) {
      std::pair<E_Edge, enum error> result = simple_pol_check2(polygon, points);
      if (result.second == SUCCESS) {
        std::cerr << "simple polygon: ";
        for(unsigned int i = 0; i < polygon.size(); ++i) std::cerr << polygon[i] << ",";
        std::cerr << "\n";
        char of[255] = "all_simple_";
        enum error returnValue = writeOutFile(of, OF_DAT, true, polygon, points);
        if (returnValue != SUCCESS) std::cerr << "Error writing polygon file!" << std::endl;
      }
      else {
//        std::cerr << "non-simple polygon." << std::endl;
//        std::cerr << "edge 1: " << result.first << std::endl;
//        std::cerr << "edge 2: " << result.first.closest[0] << std::endl;
      }
    }
    //else std::cerr << "swap already found intersection" << std::endl;

    return SUCCESS;
  }
  for(int i = 0;i < (n - 1);i++) {
//    std::cerr << "i: " << i << ", n: " << n << std::endl;
    heap2(polygon, n-1, intersects, points);
    // always swap the first when odd,
    // swap the i-th when even
//    std::cerr << "calling swap, i: " << i << ", n: " << n << std::endl;
    if(n % 2 == 0) swap2(polygon, n-1, i, intersects, points);
    else swap2(polygon, n-1, 0, intersects, points);
  }
  heap2(polygon, n-1, intersects, points);
  return SUCCESS;
}
