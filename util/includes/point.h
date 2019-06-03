#ifndef __POINT_H_
#define __POINT_H_

#include <iostream> // for endl
#include <vector>
#include "basicDefinitions.h"

class Point {
public:
  double x;
  double y;
  unsigned int i; // index in input file
  unsigned int v; // index in polygon
  unsigned int l; // index in lexicographical order.

  Point() {x=0; y=0; i=0; v=0;l=0;}
  Point(const Point& p) {x=p.x;y=p.y;i=p.i;v=p.v;l=p.l;}
  Point(double X, double Y, unsigned int I) {x=X;y=Y;i=I;v=0;l=0;}
  Point(double X, double Y, unsigned int I, unsigned int V) {x=X;y=Y;i=I;v=V;l=0;}
  Point(double X, double Y, unsigned int I, unsigned int V, unsigned int L) {x=X;y=Y;i=I;v=V;l=L;}
  void set(double X, double Y) {x=X; y=Y;}
  void set(double X, double Y, unsigned int I) {x=X; y=Y; i=I;}
  void set(double X, double Y, unsigned int I, unsigned int V) {x=X; y=Y; i=I; v=V;}
  void set(double X, double Y, unsigned int I, unsigned int V, unsigned int L) {x=X; y=Y; i=I; v=V;l=L;}
  void setX(double X) {x=X;}
  void setY(double Y) {y=Y;}
  void setI(unsigned int I) {i=I;}
  void setV(unsigned int V) {v=V;}
  void setL(unsigned int L) {l=L;}
  void printCoord(){
    printf("x=%f y=%f \n", x, y);
  }

  bool operator < (const Point& s) const {
    if (x < s.x) return true;
    else if ((x == s.x) && (y < s.y)) return true;
    else return false;
  }

  bool operator > (const Point& s) const {
    if (x > s.x) return true;
    else if ((x == s.x) && (y > s.y)) return true;
    else return false;
  }

  friend bool operator==(const Point & lhs, const Point & rhs) {
    if ((abs(lhs.x - rhs.x) < EPSILON) && (abs(lhs.y - rhs.y)) < EPSILON) return true;
    else return false;
  }

  friend bool operator!=(const Point & lhs, const Point & rhs) {
    if ((abs(lhs.x - rhs.x) > EPSILON) || (abs(lhs.y - rhs.y)) > EPSILON) return true;
    else return false;
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
  os << "(" << p.x << "," << p.y << "), [" << p.i << "," << p.v << "," << p.l << "]";
  return os;
  }
};

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

void fill_lex(std::vector<unsigned int>& lex, std::vector<Point>& points);
void pdisplay (const std::vector< Point >& vy);
double getXmin(const std::vector<Point>& p);
double getXmax(const std::vector<Point>& p);
double getYmin(const std::vector<Point>& p);
double getYmax(const std::vector<Point>& p);


#endif
