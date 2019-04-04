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

struct setComp {
private:
  double& t;
public:
  setComp(double& T) : t(T) {}
  bool operator() (const Edge& lhs, const Edge& rhs) const {
    //std::cout << "lhs: " << lhs << std::endl;
    //std::cout << "rhs: " << rhs << std::endl;
    //std::cout << "t: " << t << std::endl;

    enum intersect_t retval = checkIntersection(lhs, rhs);
    if (retval == IS_SAME_EDGE) return false;
    else if (retval == IS_VERTEX11) {
      return *lhs.p2 < *rhs.p2;
    }
    else if (retval == IS_VERTEX12) {
      return *lhs.p2 < *rhs.p1;
    }
    else if (retval == IS_VERTEX21) {
      return *lhs.p1 < *rhs.p2;
    }
    else if (retval == IS_VERTEX22) {
      return *lhs.p1 < *rhs.p1;
    }
    // Kind of sad and stupid that I can't break out of the comparison when the below 3 conditions apply
    //else if (retval == IS_TRUE) {}
    //else if (retval == IS_FALSE) {}
    //else if (retval == IS_COLLINEAR) {}

    Yval Ly, Ry;

    // calculate the y-axis order of the 2 edges at idx
    // use Yval in case of x1-x2 = 0, hopefully this will be a better comparison function..
    // Line for lhs:
    Point L1 = *lhs.p1;
    Point L2 = *lhs.p2;

    if ((L2.x - L1.x) == 0) {
      Ly.set(L1.y, L2.y);
      Ly.setX(L1.x);
    } else {
      double slope = (L2.y-L1.y) / (L2.x-L1.x);
      Ly.set(slope * (t - L1.x) + L1.y);
      Ly.setX(t);
    }

    Point R1 = *rhs.p1;
    Point R2 = *rhs.p2;

    if ((R2.x - R1.x) == 0) {
      Ry.set(R1.y, R2.y);
      Ry.setX(R1.x);
    } else {
      double slope = (R2.y-R1.y) / (R2.x-R1.x);
      Ry.set(slope * (t - R1.x) + R1.y);
      Ry.setX(t);
    }

    std::cerr << Ly << " < " << Ry << " : " << ((Ly < Ry)? "true" : "false") << std::endl;
    return Ly < Ry;
  }
};

enum error getSP(std::vector<unsigned int>& polygon, std::vector<Point>& points, enum alg_t alg);
void test();

#endif
