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
  bool operator() (const Edge& lhs, const Edge& rhs) const {
    std::cout << "lhs: " << lhs << std::endl;
    std::cout << "rhs: " << rhs << std::endl;
    // I think the lhs is always the one being compared to all the others
    double idx = lhs.l_idx;
    Yval Ly, Ry;


    // calculate the y-axis order of the 2 edges at idx
    // use Yval in case of x1-x2 = 0, hopefully this will be a better comparison function..
    // Line for lhs:
    Point L1 = *lhs.p1;
    Point L2 = *lhs.p2;

    if ((L2.x - L1.x) == 0) {
      Ly.set(L1.y, L2.y);
    } else {
      double slope = (L2.y-L1.y) / (L2.x-L1.x);
      Ly.set(slope * (idx - L1.x) + L1.y);
    }

    Point R1 = *rhs.p1;
    Point R2 = *rhs.p2;

    if ((R2.x - R1.x) == 0) {
      Ry.set(R1.y, R2.y);
    } else {
      double slope = (R2.y-R1.y) / (R2.x-R1.x);
      Ry.set(slope * (idx - R1.x) + R1.y);
    }

    return Ly < Ry;
  }
};


struct setComp2 {
  bool operator() (const Edge& lhs, const Edge& rhs) const {
    std::cout << "lhs: " << lhs << std::endl;
    std::cout << "rhs: " << rhs << std::endl;
    Point L1 = lhs.getLexLow();
    Point L2 = lhs.getLexHigh();
    Point R1 = rhs.getLexLow();
    Point R2 = rhs.getLexHigh();

    if (L1 < R1) return true;
    else if (L2 < R2) return true;
    else return false;
  }
};


// comparison class for the 'edgeS' set in "opt2.cpp"
struct setComp1 {
  bool operator() (const Edge& lhs, const Edge& rhs) const {
    std::cout << "lhs: " << lhs << std::endl;
    std::cout << "rhs: " << rhs << std::endl;
    Point L1 = lhs.getLexLow();
    L1.v = 0;
    Point L2 = lhs.getLexHigh();
    L2.v = 1;
    Edge L = Edge(&L1, &L2);
    
    Point R1 = rhs.getLexLow();
    R1.v = 0;
    Point R2 = rhs.getLexHigh();
    R2.v = 1;
    Edge R = Edge(&R1, &R2);
    
    double det_a = det(L,*R.p1);
    double det_b = det(L,*R.p2);
    double det_c = det(R,*L.p1);
    double det_d = det(R,*L.p2);
    std::cout << "s(a): " << signbit(det_a) << ", s(b): " << signbit(det_b) << ", s(c): " << signbit(det_c) << ", s(d): " << signbit(det_d) << std::endl;

    if ( (!signbit(det_a) || !signbit(det_b)) && (signbit(det_c) || signbit(det_d)) ) {
      std::cout << "return rhs" << std::endl;
      return false;
    }
    else {
      std::cout << "return lhs" << std::endl;
      return true;
    }
  }
};

enum error getSP(std::vector<unsigned int>& polygon, std::vector<Point>& points, enum alg_t alg);
void test();

#endif