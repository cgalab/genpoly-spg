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
  void set(Point p) {x=p.x; y=p.y; i=p.i; v=p.v;l=p.l;}
  void set(double X, double Y) {x=X; y=Y;}
  void set(double X, double Y, unsigned int I) {x=X; y=Y; i=I;}
  void set(double X, double Y, unsigned int I, unsigned int V) {x=X; y=Y; i=I; v=V;}
  void set(double X, double Y, unsigned int I, unsigned int V, unsigned int L) {x=X; y=Y; i=I; v=V;l=L;}
  void setX(double X) {x=X;}
  void setY(double Y) {y=Y;}
  void setI(unsigned int I) {i=I;}
  void setV(unsigned int V) {v=V;}
  void setL(unsigned int L) {l=L;}
  //void printCoord(){
  //  printf("x=%f y=%f \n", x, y);
  //}

  // function to compare 2 points based on y axis first, then x.
  bool lessY (const Point& s) const {
/*
    if (fabs(y - s.y) < EPSILON) {
      if (fabs(x - s.x) < EPSILON) return false;
      else return (x < s.x);
    }
    else return (y < s.y);
*/
    if (y == s.y) return (x < s.x);
    return (y < s.y);
  }

  // first priority is the x axis comparison, then y.
  bool operator < (const Point& s) const {
//    std::cerr << "in < comparison" << std::endl;
//    if (fabs(x - s.x) < EPSILON) {
//      if (fabs(y - s.y) < EPSILON) return false;
//      else return (y < s.y);
//    }
//    else return (x < s.x);
    if (x == s.x) return (y < s.y);
    return (x < s.x);
  }

  bool operator > (const Point& s) const {
/*
    if (fabs(x - s.x) < EPSILON) {
      if (fabs(y - s.y) < EPSILON) return false;
      else return (y > s.y);
    }
    else return (x > s.x);
*/
    if (x == s.x) return (y > s.y);
    return (x > s.x);
  }

  bool operator == (const Point& s) const {
//    std::cerr << "x comp: abs(" << x << " - " << s.x << ") = " <<  fabs(x - s.x) << std::endl;
//    std::cerr << "y comp: abs(" << y << " - " << s.y << ") = " <<  fabs(y - s.y) << std::endl;

/*
    if ((fabs(x - s.x) < EPSILON) && (fabs(y - s.y)) < EPSILON) return true;
    else return false;
*/
    if ((x == s.x) && (y == s.y)) return true;
    return false;
  }

/*
  friend bool operator==(const Point & lhs, const Point & rhs) {
    if ((abs(lhs.x - rhs.x) < EPSILON) && (abs(lhs.y - rhs.y)) < EPSILON) return true;
    else return false;
  }
*/
  friend bool operator!=(const Point & lhs, const Point & rhs) {
/*
    if ((fabs(lhs.x - rhs.x) > EPSILON) || (fabs(lhs.y - rhs.y)) > EPSILON) return true;
    else return false;
*/
    if ((lhs.x == rhs.x) && (lhs.y == rhs.y)) return false;
    return true;
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
      if (fabs(value_vector[i].x - value_vector[j].x) == 0) {
        if (fabs(value_vector[i].y - value_vector[j].y) == 0) return false;
        else return (value_vector[i].y < value_vector[j].y);
      }
      else return (value_vector[i].x < value_vector[j].x);
    }
};

enum error verify_point_set(std::vector<Point>& points);
double det(Point& pa, Point& pb, Point& p);
void get_convex_hull(std::vector<unsigned int>& ch, std::vector<Point>& points);
void get_convex_hull(std::vector<Point>& ch, std::vector<Point>& points);
void get_inner_points(std::vector<unsigned int>& ip, std::vector<unsigned int>& ch, std::vector<Point>& points);
void get_inner_points(std::vector<Point>& ip, std::vector<Point>& ch, std::vector<Point>& points);
bool isPolLeft(Point *p1, Point *p2, unsigned int cycle);
bool isPol1Left(Point *p1, Point *p2, unsigned int cycle);
bool isPol1Left(unsigned int p1, unsigned int p2, unsigned int cycle);
void fill_lex(std::vector<unsigned int>& lex, std::vector<Point>& points);
void fill_lex(std::vector<unsigned int>& lex, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void pdisplay (const std::vector< Point >& vy);
void pdisplay (const std::vector<unsigned int>& ind, const std::vector<Point>& p);
double getXmin(const std::vector<Point>& p);
double getXmax(const std::vector<Point>& p);
double getYmin(const std::vector<Point>& p);
double getYmax(const std::vector<Point>& p);
unsigned int get_lowest_v(Point *a, Point *b, Point *c);
unsigned int get_highest_v(Point *a, Point *b, Point *c);
double get_length(const Point& p1, const Point& p2);
bool angleComparator (Point i,Point j);

#endif
