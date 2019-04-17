#include <vector>
#include <math.h> // for signbit
#include <iostream> // for endl
#include "point.h"
#include "edge.h"

#ifndef __BASICFUNCTIONS_H_
#define __BASICFUNCTIONS_H_



class lexComp {
public:
    const std::vector<Point>& value_vector;

    lexComp(const std::vector<Point>& val_vec):
      value_vector(val_vec) {}

    bool operator()(int i, int j)
    {
      if (value_vector[i].x < value_vector[j].x) return true;
      else if ((value_vector[i].x == value_vector[j].x) && (value_vector[i].y < value_vector[j].y)) return true;
      else return false;
    }
};

struct compObject{
  double t;
  Edge lhs;
  Edge rhs;
  enum intersect_t isect;
  unsigned int l_idx;
};

// comparison class for the set of edges in 'opt2.cpp'
struct setComp {
private:
public:
  compObject& o;

  setComp(compObject& O) : o(O) {}
  bool operator() (const Edge& lhs, const Edge& rhs) const {
    std::cout << "lhs: " << lhs << " < rhs: " << rhs << std::endl;
    double det1;

    if (*lhs.p1 == *rhs.p1) {     // if same starting point
      det1 = det(lhs, *rhs.p2);
      if (det1 == 0) {            // if lhs and rhs.p2 collinear
        std::cerr << "same p1, coll: "<< *lhs.p2 << " < " << *rhs.p2 << " : " << ((*lhs.p2 < *rhs.p2)? "true" : "false") << std::endl;
        return *lhs.p2 < *rhs.p2;
      }
      bool detsign = !signbit(det1);
      std::cerr << "same p1, lhs < rhs : " << ((detsign)? "true" : "false") << std::endl;
      return detsign;
    }

    det1 = det(lhs, *rhs.p1);
    if (det1 == 0) {            // if lhs and rhs.p1 are collinear
      det1 = det(lhs, *rhs.p2);
      if (det1 == 0) {          // if lhs and rhs.p2 are collinear
        std::cerr << *lhs.p1 << " < " << *rhs.p1 << " : " << ((*lhs.p1 < *rhs.p1)? "true" : "false") << std::endl;
        return *lhs.p1 < *rhs.p1;
      }
      bool detsign = !signbit(det1);
      std::cerr << "lhs < rhs : " << ((detsign)? "true" : "false") << std::endl;
      return detsign;
    }

    bool detsign = !signbit(det1);
    std::cerr << "lhs < rhs : " << ((detsign)? "true" : "false") << std::endl;
    return detsign;

/*
    enum intersect_t retval = checkIntersection(lhs, rhs);
    if (retval == IS_VERTEX11) {
      if (o.isect == IS_FALSE) {o.lhs = lhs; o.rhs = rhs; o.isect = retval;}
      // the 2 edges have the same starting point, use a determinant test to check if left or right of lhs.
      double detval = det(lhs, *rhs.p2);
      if (detval == 0) {
        double relval = reldist(lhs, *rhs.p2);
        if (relval > 1) return true;
        else return false;
      }
      bool detsign = signbit(detval);
      return !detsign;
    }
    else if (retval == IS_VERTEX22) {
      if (o.isect == IS_FALSE) {o.lhs = lhs; o.rhs = rhs; o.isect = retval;}
      if (lhs.l_idx < rhs.l_idx) {
        if (lhs.l_idx < o.l_idx) o.l_idx = lhs.l_idx;
      } else {
        if (rhs.l_idx < o.l_idx) o.l_idx = rhs.l_idx;
      }
      // the 2 edges have the same end point, use a determinant test to check if left or right of lhs.
      double detval = det(lhs, *rhs.p1);
      if (detval == 0) {
        double relval = reldist(lhs, *rhs.p1);
        if (relval < 0) return true;
        else return false;
      }
      bool detsign = signbit(detval);
      return !detsign;
    }
    else if (retval == IS_SAME_EDGE) {
      if (o.isect == IS_FALSE) {o.lhs = lhs; o.rhs = rhs; o.isect = retval;}
      return false;
    }
    else if (retval >= IS_TRUE) {
      std::cerr << "IS: " << retval << ", found intersection between: " << lhs << ", and " << rhs << std::endl;
      if (o.isect < IS_TRUE) {o.lhs = lhs; o.rhs = rhs; o.isect = retval;}
      // grab the lowest lex index if more than one edge intersects the current edge.
      if (lhs.l_idx < rhs.l_idx) {
        if (lhs.l_idx < o.l_idx) o.l_idx = lhs.l_idx;
      } else {
        if (rhs.l_idx < o.l_idx) o.l_idx = rhs.l_idx;
      }

      Yval Ly, Ry;
      // calculate the y-axis order of the 2 edges at idx
      // use Yval in case of x1-x2 = 0
      Point L1 = *lhs.p1;
      Point L2 = *lhs.p2;

      if ((L2.x - L1.x) == 0) {
        Ly.set(L1.y, L2.y);
        Ly.setX(L1.x);
      } else {
        double slope = (L2.y-L1.y) / (L2.x-L1.x);
        double val = slope * (o.t - L1.x) + L1.y;
        if (abs(val) < EPSILON) Ly.set(0);
        else Ly.set(val);
        Ly.setX(o.t);
      }

      Point R1 = *rhs.p1;
      Point R2 = *rhs.p2;

      if ((R2.x - R1.x) == 0) {
        Ry.set(R1.y, R2.y);
        Ry.setX(R1.x);
      } else {
        double slope = (R2.y-R1.y) / (R2.x-R1.x);
        double val = slope * (o.t - R1.x) + R1.y;
        if (abs(val) < EPSILON) Ry.set(0);
        else Ry.set(val);
        Ry.setX(o.t);
      }

      std::cerr << Ly << " < " << Ry << " : " << ((Ly < Ry)? "true" : "false") << std::endl;
      return Ly < Ry;
    } else {
      // the 2 edges have the same end point, use a determinant test to check if left or right of lhs.
      double detl = det(lhs, *rhs.p1);
      double detr = det(lhs, *rhs.p2);

      bool detsign = signbit(detl) && signbit(detr);
      return !detsign;
    }
*/
  }
};

enum error getSP(std::vector<unsigned int>& polygon, std::vector<Point>& points, enum alg_t alg);
void printEnum(enum intersect_t val);
void test();

#endif
