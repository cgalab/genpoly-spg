#include <iostream> // for endl
#include <vector>

#ifndef __POINT_H_
#define __POINT_H_

class Point {
public:
  double x;
  double y;
  unsigned int i; // index in input file
  unsigned int v; // index in polygon

  Point() {x=0; y=0; i=0; v=0;}
  Point(double X, double Y, unsigned int I) {x=X; y=Y; i=I; v=0;}
  Point(double X, double Y, unsigned int I, unsigned int V) {x=X; y=Y; i=I; v=V;}
  void set(double X, double Y) {x=X; y=Y;}
  void set(double X, double Y, unsigned int I) {x=X; y=Y; i=I;}
  void set(double X, double Y, unsigned int I, unsigned int V) {x=X; y=Y; i=I; v=V;}
  void setX(double X) {x=X;}
  void setY(double Y) {y=Y;}
  void setI(unsigned int I) {i=I;}
  void setV(unsigned int V) {v=V;}

  bool operator < (const Point& s) const {
    if (x < s.x) return true;
    else if ((x == s.x) && (y < s.y)) return true;
    else return false;
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
  os << " x: " << p.x << ", y: " << p.y << ", i:" << p.i << ", v: " << p.v;
  return os;
  };
};

void pdisplay (const std::vector< Point >& vy);
double getXmin(const std::vector<Point>& p);
double getXmax(const std::vector<Point>& p);
double getYmin(const std::vector<Point>& p);
double getYmax(const std::vector<Point>& p);


#endif