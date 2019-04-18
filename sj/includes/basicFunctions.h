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
    std::cout << "lhs: " << lhs << " < rhs: " << rhs << std::endl;
    Yval yl, yr;
    yl = getYatX(lhs, o.t);
    yr = getYatX(rhs, o.t);
    if (yl == yr) {
      bool pickP1;

      // check for case 1: same beginning point in 't'
      if ((*lhs.p1 == *rhs.p1) && ((*lhs.p1).x == o.t)) {
        // pick the lower of the p2s as the 't' point
        pickP1 = false;
      }
      // check for case 2: same endpoint in 't'
      else if ((*lhs.p2 == *rhs.p2) && ((*lhs.p2).x == o.t)) {
        // pick the higher of the p1s as the 't' point
        pickP1 = true;
      }
      // check for case 3 and 4:
      // same y value because at t lhs and rhs intersect;
      // case 3: 3P collinear
      // case 4: 4P collinear
      else {
        double det1, det2;
        det1 = det(lhs, *rhs.p1);
        det2 = det(lhs, *rhs.p2);

        // 4P
        if ((abs(det1) < EPSILON) && (abs(det2) < EPSILON)) {
          std::cerr << "4P collinearity: " << *lhs.p1 << " < " << *rhs.p1 << " : " <<
          ((*lhs.p1 < *rhs.p1) ? "true" : "false") << std::endl;
          return *lhs.p1 < *rhs.p1;
        }
        // 3P and technically any other situation where yl == yr
        else {
          std::cerr << "3P collinearity" << std::endl;
          // find what point is at 't'
          if ((*lhs.p1).x == o.t) {
            // pick the lower of the p2s as the x point
            pickP1 = false;
          }
          else if ((*lhs.p2).x == o.t) {
            // pick the higher of the p1s as the x point
            pickP1 = true;
          }
          else if ((*rhs.p1).x == o.t) {
            // pick the lower of the p2s
            pickP1 = false;
          }
          else if ((*rhs.p2).x == o.t) {
            // pick the higher of the p1s
            pickP1 = true;
          }
          else {
            std::cerr << "ERROR: Unexpected fallthrough in comparison!" << std::endl;
          }
        }
      }

      if (pickP1) {
        // pick the higher of the p1s as the 't' point
        if (*lhs.p1 < *rhs.p1) {
          std::cerr << "higher of P1, at rhs.p1: " <<
          getYatX(lhs, (*rhs.p1).x) << " < " << getYatX(rhs, (*rhs.p1).x) << " : "<<
          ((getYatX(lhs, (*rhs.p1).x) < getYatX(rhs, (*rhs.p1).x)) ? "true" : "false") << std::endl;
          return getYatX(lhs, (*rhs.p1).x) < getYatX(rhs, (*rhs.p1).x);
        }
        else {
          std::cerr << "higher of P1, at lhs.p1: " <<
          getYatX(lhs, (*lhs.p1).x) << " < " << getYatX(rhs, (*lhs.p1).x) << " : "<<
          ((getYatX(lhs, (*lhs.p1).x) < getYatX(rhs, (*lhs.p1).x)) ? "true" : "false") << std::endl;
          return getYatX(lhs, (*lhs.p1).x) < getYatX(rhs, (*lhs.p1).x);
        }
      } else {
        // pick the lower of the p2s as the 't' point
        if (*lhs.p2 < *rhs.p2) {
          std::cerr << "lower of p2, at lhs.p2: " <<
          getYatX(lhs, (*lhs.p2).x) << " < " << getYatX(rhs, (*lhs.p2).x) << " : "<<
          ((getYatX(lhs, (*lhs.p2).x) < getYatX(rhs, (*lhs.p2).x)) ? "true" : "false") << std::endl;
          return getYatX(lhs, (*lhs.p2).x) < getYatX(rhs, (*lhs.p2).x);
        }
        else {
          std::cerr << "lower of p2, at rhs.p2: " <<
          getYatX(lhs, (*rhs.p2).x) << " < " << getYatX(rhs, (*rhs.p2).x) << " : "<<
          ((getYatX(lhs, (*rhs.p2).x) < getYatX(rhs, (*rhs.p2).x)) ? "true" : "false") << std::endl;
          return getYatX(lhs, (*rhs.p2).x) < getYatX(rhs, (*rhs.p2).x);
        }
      }
    }
    else {
      std::cerr << "general: " << yl << " < " << yr << " : " << ((yl < yr) ? "true" : "false") << std::endl;
      return yl < yr; // general case:
    }
  }




/*  Old version that used determinants, wasn't consistent enough.
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
      double reld1 = reldist(lhs, *rhs.p1);
      if (reld1 < 0) {          // if lhs and rhs.p2 are collinear
        std::cerr << "coll: lhs < rhs: false" << std::endl;
        return false;
      } else {
        std::cerr << "coll: lhs < rhs: true" << std::endl;
        return true;
      }
      bool detsign = !signbit(det1);
      std::cerr << "lhs < rhs : " << ((detsign)? "true" : "false") << std::endl;
      return detsign;
    }

    bool detsign = !signbit(det1);
    std::cerr << "lhs < rhs : " << ((detsign)? "true" : "false") << std::endl;
    return detsign;
*/

/*  //Old version that used intersection checks and yVals
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
  }
*/
};

enum error getSP(std::vector<unsigned int>& polygon, std::vector<Point>& points, enum alg_t alg);
void printEnum(enum intersect_t val);
void test();

#endif
