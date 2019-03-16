#include <iostream> // for endl
#include <vector>

#ifndef __POINTS_H_
#define __POINTS_H_

class Point {
public:
  double x;
  double y;
  unsigned int i;

  Point() {x = 0; y = 0; i = 0;}
  Point(double X, double Y, unsigned int I) { x = X; y = Y; i = I; }
  void set(double X, double Y, unsigned int I) { x = X; y = Y; i = I; }
  void set(double X, double Y) { x = X; y = Y; }

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "x: " << p.x << ", y: " << p.y << ", i:" << p.i;
    return os;
  };

};

void pdisplay (const std::vector< Point >& vy);


#endif