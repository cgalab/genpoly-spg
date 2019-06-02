#ifndef __BASICFUNCTIONS_H_
#define __BASICFUNCTIONS_H_

#include <vector>
#include <math.h> // for signbit
#include <iostream> // for endl
#include "point.h"
#include "edge.h"



struct compObject{
  double t;
  unsigned int lower_idx;
};

// comparison class for the set of edges in 'opt2.cpp'
struct setComp {
private:
public:
  compObject& o;

  setComp(compObject& O) : o(O) {}
  bool operator() (const Edge& lhs, const Edge& rhs) const {
    // compares 2 edges at x-coordinate 't'
    // have to catch 5 cases, general case, t is in same beginning point, same end point, 3P collinear and 4P collinear
    // all 4 specific cases happen when y value is the same for lhs and rhs.
    // *Problem 1: if the comparison was true at p1, it might be false at p2 if there is
    //            an intersection between the first comparison at p1 and at p2 when the edge will be removed.
    // possible solution: always compare at higher p1 value(?)
//    std::cerr << "lhs: " << lhs << " < rhs: " << rhs << std::endl;
    Yval yl, yr;
    double s;
    // comparison always starts at lex. higher of the P1 points.
    if ((*lhs.p1 == *rhs.p1) && (*lhs.p2 == *rhs.p2)) return false;
    if (*lhs.p1 == *rhs.p1) {
      if (*lhs.p2 < *rhs.p2) s = (*lhs.p2).x;
      else s = (*rhs.p2).x;
    }
    else if (*lhs.p1 < *rhs.p1) s = (*rhs.p1).x;
    else s = (*lhs.p1).x;
    //std::cerr << ", s: " << s << std::endl;
    yl = getYatX(lhs, s);
    yr = getYatX(rhs, s);

    if (yl == yr) {
      double det1, det2;
      det1 = det(lhs, *rhs.p1);
      det2 = det(lhs, *rhs.p2);

      // 4P coll.
      if ((abs(det1) < EPSILON) && (abs(det2) < EPSILON)) {
//        std::cerr << "4Pc: " << std::endl;
        if (*lhs.p1 == *rhs.p1) return *lhs.p2 < *rhs.p2;
//        std::cerr << ((*lhs.p1 < *rhs.p1) ? "true" : "false") << std::endl;
        return *lhs.p1 < *rhs.p1;
      }
      // 3P coll. pick the lower of the p2s
      else {
//        std::cerr << "3Pc: ";
        if (*lhs.p2 < *rhs.p2) s = (*lhs.p2).x;
        else s = (*rhs.p2).x;
        yl = getYatX(lhs, s);
        yr = getYatX(rhs, s);
//        std::cerr << ((yl < yr) ? "true" : "false") << std::endl;
        return yl < yr;
      }
    }
    else {
//      std::cerr << "gen: " << yl << " < " << yr << " : " << ((yl < yr) ? "true" : "false") << std::endl;
      return yl < yr; // general case:
    }
    std::cerr << "ERROR: unexpected fallthrough in comparison! lhs: " << lhs << ",rhs: " << rhs << std::endl;
    return false;
  }
};

enum error getSP(std::vector<unsigned int>& polygon, std::vector<Point>& points, enum alg_t alg, unsigned int randseed);
void printEnum(enum intersect_t val);
void test();

#endif
