#ifndef __POINT_H_
#define __POINT_H_

#include <iostream> // for endl
#include <vector>
#include <cmath>
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
  Point(double X, double Y) {x=X;y=Y;i=0;v=0;l=0;}
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

  // function to compare 2 points based on y axis first, then x.
  bool lessY (const Point& s) const {
    if (y < s.y) return true;
    else if ((fabs(y - s.y) < EPSILON) && (x < s.x)) return true;
    else return false;
  }

  // first priority is the x axis comparison, then y.
  bool operator < (const Point& s) const {
    if (x < s.x) return true;
    else if ((fabs(x - s.x) < EPSILON) && (y < s.y)) return true;
    else return false;
  }

  bool operator > (const Point& s) const {
    if (x > s.x) return true;
    else if ((fabs(x - s.x) < EPSILON) && (y > s.y)) return true;
    else return false;
  }

  bool operator == (const Point& s) const {
//    std::cerr << "x comp: abs(" << x << " - " << s.x << ") = " <<  fabs(x - s.x) << std::endl;
//    std::cerr << "y comp: abs(" << y << " - " << s.y << ") = " <<  fabs(y - s.y) << std::endl;
    if ((fabs(x - s.x) < EPSILON) && (fabs(y - s.y)) < EPSILON) return true;
    else return false;
  }
/*
  friend bool operator==(const Point & lhs, const Point & rhs) {
    if ((abs(lhs.x - rhs.x) < EPSILON) && (abs(lhs.y - rhs.y)) < EPSILON) return true;
    else return false;
  }
*/
  friend bool operator!=(const Point & lhs, const Point & rhs) {
    if ((fabs(lhs.x - rhs.x) > EPSILON) || (fabs(lhs.y - rhs.y)) > EPSILON) return true;
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
      else if ((fabs(value_vector[i].x - value_vector[j].x) < EPSILON) && (value_vector[i].y < value_vector[j].y)) return true;
      else return false;
    }
};

double det(const Point& pa, const Point& pb, const Point& p);
void get_convex_hull(std::vector<unsigned int>& ch, std::vector<Point>& points, bool enforceCCWOrder=false);
void get_inner_points(std::vector<unsigned int>& ip, std::vector<unsigned int>& ch, std::vector<Point>& points);
bool isPolLeft(Point *p1, Point *p2, unsigned int cycle);
bool isPol1Left(Point *p1, Point *p2, unsigned int cycle);
void fill_lex(std::vector<unsigned int>& lex, std::vector<Point>& points);
void fill_lex(std::vector<unsigned int>& lex, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void pdisplay (const std::vector< Point >& vy);
void pdisplay (const std::vector<unsigned int>& ind, const std::vector<Point>& p);
double getXmin(const std::vector<Point>& p);
double getXmax(const std::vector<Point>& p);
double getYmin(const std::vector<Point>& p);
double getYmax(const std::vector<Point>& p);
bool angleComparator (Point i,Point j);

#endif
