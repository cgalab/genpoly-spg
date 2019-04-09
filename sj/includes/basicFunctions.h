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
};

// comparison class for the set of edges in 'opt2.cpp'
struct setComp {
private:
public:
  compObject& o;

  setComp(compObject& O) : o(O) {}
  bool operator() (const Edge& lhs, const Edge& rhs) const {
    //if (o.isect >= IS_TRUE) return false;
    std::cout << "lhs: " << lhs << " < rhs: " << rhs << std::endl;
    //std::cout << "o.t: " << o.t << std::endl;
/*
    enum intersect_t retval = checkIntersection(lhs, rhs);
    if (retval == IS_SAME_EDGE) return false;
    if (retval >= IS_TRUE) {
      o.isect = retval;
      o.lhs = lhs;
      o.rhs = rhs;
      std::cerr << "inserting: Found an intersection!" << std::endl;
      return false;
    }
    else if (retval == IS_VERTEX11) {
      std::cerr << "IS_VERTEX11" << std::endl;
      double slopel, sloper;
      bool isLvertical=false, isRvertical=false;
      // can't be lexicographical ordering or y values of the other end points in case they are the same...
      // try to compare slopes and treat the special case when line is vertical or has same slope.
      if (((*lhs.p2).x - (*lhs.p1).x) != 0) {
        slopel = ((*lhs.p2).y - (*lhs.p1).y) / ((*lhs.p2).x - (*lhs.p1).x);
      } else {
        slopel = (*lhs.p2).y; // catching condition when one line is vertical and the other isn't.
        isLvertical = true;
      }
      if (((*rhs.p2).x - (*rhs.p1).x) != 0) {
        sloper = ((*rhs.p2).y - (*rhs.p1).y) / ((*rhs.p2).x - (*rhs.p1).x);
      } else {
        sloper = (*rhs.p2).y;
        isRvertical = true;
      }

      if(isLvertical && isRvertical) {
        std::cerr << "both: "<< slopel << " < " << sloper << " : " << ((slopel < sloper)? "true" : "false") << std::endl;
        return slopel < sloper;
      } else if (isLvertical || isRvertical) {
        std::cerr << "one: " << (*lhs.p2).x << " < " << (*rhs.p2).x << " : " << (((*lhs.p2).x < (*rhs.p2).x)? "true" : "false") << std::endl;
        return (*lhs.p2).x < (*rhs.p2).x;
      }

      if (slopel == sloper) {
        // if the 2 segments have the same slope, the lesser is the smaller line segment
        double lenl, lenr;
        lenl = ((*lhs.p2).y - (*lhs.p1).y) * ((*lhs.p2).y - (*lhs.p1).y) + ((*lhs.p2).x - (*lhs.p1).x) * ((*lhs.p2).x - (*lhs.p1).x);
        lenr = ((*rhs.p2).y - (*rhs.p1).y) * ((*rhs.p2).y - (*rhs.p1).y) + ((*rhs.p2).x - (*rhs.p1).x) * ((*rhs.p2).x - (*rhs.p1).x);
        std::cerr << "samelen: "<< lenl << " < " << lenr << " : " << ((lenl < lenr)? "true" : "false") << std::endl;
        return lenl < lenr;
      }

      std::cerr << "neither: "<< slopel << " < " << sloper << " : " << ((slopel < sloper)? "true" : "false") << std::endl;
      return slopel < sloper;
    }
    else if (retval == IS_VERTEX12) {
      std::cerr << "IS_VERTEX12" << std::endl;
      // I believe lexicographical comparison should be fine here.
      std::cerr << *lhs.p2 << " < " << *rhs.p1 << " : " << ((*lhs.p2 < *rhs.p1)? "true" : "false") << std::endl;
      return *lhs.p2 < *rhs.p1;
    }
    else if (retval == IS_VERTEX21) {
      std::cerr << "IS_VERTEX21" << std::endl;
      std::cerr << *lhs.p1 << " < " << *rhs.p2 << " : " << ((*lhs.p1 < *rhs.p2)? "true" : "false") << std::endl;
      return *lhs.p1 < *rhs.p2;
    }
    else if (retval == IS_VERTEX22) {
      std::cerr << "IS_VERTEX22" << std::endl;
      double slopel, sloper;
      bool isLvertical=false, isRvertical=false;
      // can't be lexicographical ordering or y values of the other end points in case they are the same...
      // try to compare slopes and treat the special case when line is vertical or has same slope.
      if (((*lhs.p2).x - (*lhs.p1).x) != 0) {
        slopel = ((*lhs.p2).y - (*lhs.p1).y) / ((*lhs.p2).x - (*lhs.p1).x);
      } else {
        slopel = (*lhs.p1).y; // catching condition when one line is vertical and the other isn't.
        isLvertical = true;
      }
      if (((*rhs.p2).x - (*rhs.p1).x) != 0) {
        sloper = ((*rhs.p2).y - (*rhs.p1).y) / ((*rhs.p2).x - (*rhs.p1).x);
      } else {
        sloper = (*rhs.p1).y;
        isRvertical = true;
      }

      if(isLvertical && isRvertical) {
        std::cerr << "both: "<< slopel << " < " << sloper << " : " << ((slopel < sloper)? "true" : "false") << std::endl;
        return slopel < sloper;
      } else if (isLvertical || isRvertical) {
        std::cerr << "one: " << (*lhs.p1).x << " < " << (*rhs.p1).x << " : " << (((*lhs.p1).x < (*rhs.p1).x)? "true" : "false") << std::endl;
        return (*lhs.p1).x < (*rhs.p1).x;
      }

      if (slopel == sloper) {
        // if the 2 segments have the same slope, the lesser is the larger line segment
        double lenl, lenr;
        lenl = ((*lhs.p2).y - (*lhs.p1).y) * ((*lhs.p2).y - (*lhs.p1).y) + ((*lhs.p2).x - (*lhs.p1).x) * ((*lhs.p2).x - (*lhs.p1).x);
        lenr = ((*rhs.p2).y - (*rhs.p1).y) * ((*rhs.p2).y - (*rhs.p1).y) + ((*rhs.p2).x - (*rhs.p1).x) * ((*rhs.p2).x - (*rhs.p1).x);
        std::cerr << "samelen: "<< lenl << " > " << lenr << " : " << ((lenl > lenr)? "true" : "false") << std::endl;
        return lenl < lenr;
      }

      std::cerr << "neither: "<< slopel << " < " << sloper << " : " << ((slopel < sloper)? "true" : "false") << std::endl;
      return slopel < sloper;
    }
    // Kind of sad and stupid that I can't break out of the comparison when the below 3 conditions apply
    //else if (retval == IS_TRUE) {}
    //else if (retval == IS_FALSE) {}
    else if (retval == IS_COLLINEAR) {
      double lenl, lenr;
      lenl = ((*lhs.p2).y - (*lhs.p1).y) * ((*lhs.p2).y - (*lhs.p1).y) + ((*lhs.p2).x - (*lhs.p1).x) * ((*lhs.p2).x - (*lhs.p1).x);
      lenr = ((*rhs.p2).y - (*rhs.p1).y) * ((*rhs.p2).y - (*rhs.p1).y) + ((*rhs.p2).x - (*rhs.p1).x) * ((*rhs.p2).x - (*rhs.p1).x);
      std::cerr << "coll: "<< lenl << " < " << lenr << " : " << ((lenl < lenr)? "true" : "false") << std::endl;
      return lenl < lenr;
    }
*/
    enum intersect_t retval = checkIntersection(lhs, rhs);
    if (retval == IS_VERTEX11) {
      // the 2 edges have the same starting point, use a determinant test to check if left or right of lhs.
      bool detsign = signbit(det(lhs, *rhs.p2));
      return !detsign;
    }
    else if (retval >= IS_TRUE) {
      std::cerr << "found intersection between: " << lhs << ", and " << rhs << std::endl;
      if (o.isect == IS_FALSE) {
        o.lhs = lhs;
        o.rhs = rhs;
        o.isect = retval;
      }
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
  }
};

enum error getSP(std::vector<unsigned int>& polygon, std::vector<Point>& points, enum alg_t alg);
void test();

#endif
