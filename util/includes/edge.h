#include <iostream> // for endl
#include <vector>
#include <iomanip>      // std::setprecision
#include "basicDefinitions.h"
#include "point.h"

#ifndef __EDGE_H_
#define __EDGE_H_

// class for when an edge is vertical on the x axis
class Yval {
private:
  double max, min;
  double x;
public:

  Yval() {max=0;min=0;x=0;}
  Yval(double val) {max=val;min=val;x=0;}
  Yval(double a, double b) {
    if (a<b) {min=a;max=b;}
    else {min=b;max=a;}
    x=0;
  }
  Yval(double a, double b, double X) {
    if (a<b) {min=a;max=b;}
    else {min=b;max=a;}
    x=X;
  }

  void set(double val) {max=val;min=val;}
  void set(double a, double b) {
    if (a<b) {min=a;max=b;}
    else {min=b;max=a;}
  }
  void setX(double X) {x=X;}
  double getX() const {return x;}
  double getMin() const {return min;}
  double getMax() const {return max;}

  bool operator < (const double s) const {
    if (min < s) return true;
    else return false;
  }
  bool operator < (const Yval s) const {
    if (x < s.getX()) {return true;}
    else if (s.getX() < x) {return false;}
    else {
      if (min == s.getMin()) return (max < s.getMax());
      else return (min < s.getMin());
    }
    std::cerr << "Error: fallthrough in Yval < comparison!" << std::endl;
    return false;
  }
  bool operator > (const double s) const {
    if (min > s) return true;
    else return false;
  }
  bool operator > (const Yval s) const {
    if (x > s.getX()) return true;
    else if (min > s.getMin()) return true;
    else if ((min == s.getMin()) && (max > s.getMax())) return true;
    else return false;
  }

  friend bool operator==(const Yval lhs, const Yval rhs) {
    //std::cerr << std::setprecision(15) << (abs(lhs.min - rhs.min)) << " < " << EPSILON << " should be true : " << ((abs(lhs.min - rhs.min) < EPSILON) ? "true" : "false") << std::endl;
    //std::cerr << std::setprecision(15) << (abs(lhs.max - rhs.max)) << " < " << EPSILON << " should be true : " << ((abs(lhs.max - rhs.max) < EPSILON) ? "true" : "false") << std::endl;
    //std::cerr << std::setprecision(15) << (abs(lhs.x - rhs.x)) << " < " << EPSILON << " should be true : " << ((abs(lhs.x - rhs.x) < EPSILON) ? "true" : "false") << std::endl;
    if ((abs(lhs.min - rhs.min) < EPSILON) &&
        (abs(lhs.max - rhs.max) < EPSILON) &&
        (abs(lhs.x - rhs.x) < EPSILON)) return true;
		else return false;
	}

  friend bool operator!=(const Yval lhs, const Yval rhs) {
		if ((abs(lhs.min - rhs.min) > EPSILON) ||
        (abs(lhs.max - rhs.max) > EPSILON) ||
        (abs(lhs.x - rhs.x) > EPSILON)) return true;
		else return false;
	}

  friend std::ostream& operator<<(std::ostream& os, const Yval& y) {
    if (y.getMin() == y.getMax())
      os << std::setprecision(15) << "x:" << y.getX() << ", (" << y.min << "," << y.max << ")";
    else
      os << std::setprecision(15) << "x:" << y.getX() << ", [" << y.min << "," << y.max << "]";
    return os;
  }
};

class Edge {
public:
	Point *p1;
	Point *p2;
  unsigned int l_idx;

	Edge() {p1=NULL; p2=NULL;l_idx=0;}
	Edge(const Edge& e) {p1=e.p1; p2=e.p2;l_idx=e.l_idx;}
	Edge(Point *P1, Point *P2) {
		if ((*P1) < (*P2)) {p1=P1; p2=P2;}
		else {p1=P2; p2=P1;}
    l_idx=0;
	}
  Edge(Point *P1, Point *P2, double idx) {
    if ((*P1) < (*P2)) {p1=P1; p2=P2;}
    else {p1=P2; p2=P1;}
    l_idx=idx;
  }

	void set(Point* v1, Point* v2) {
    if ((*v1) < (*v2)) {p1=v1; p2=v2;}
		else {p1=v2; p2=v1;}
	}
  void set(Point& v1, Point& v2) {
    if ((v1) < (v2)) {p1=&v1; p2=&v2;}
		else {p1=&v2; p2=&v1;}
	}

  void set(Edge val) {
    p1 = val.p1;
    p2 = val.p2;
    l_idx = val.l_idx;
  }

  Point getPLow() const {
    if ((*p1).v < (*p2).v)
      return *p1;
    else
      return *p2;
  }
  Point getPHigh() const {
    if ((*p1).v < (*p2).v)
      return *p2;
    else
      return *p1;
  }
  double getxMin() const {
    if ((*p1).x < (*p2).x) return (*p1).x;
    else return (*p2).x;
  }
  double getxMax() const {
    if ((*p1).x < (*p2).x) return (*p2).x;
    else return (*p1).x;
  }
  double getyMin() const {
    if ((*p1).y < (*p2).y) return (*p1).y;
    else return (*p2).y;
  }
  double getyMax() const {
    if ((*p1).y < (*p2).y) return (*p2).y;
    else return (*p1).y;
  }
  double getLowerLexIdx() const {
    if ((*p1).l < (*p2).l) return (*p1).l;
    else return (*p2).l;
  }

  // check for if p1 is a 'left' vertex compared to p2
  bool checkPolLoHi() {
    if ((*p1).v == 0) {
      if ((*p2).v != 1) return false;
      else return true;
    }
    if ((*p2).v == 0) {
      if ((*p1).v == 1) return false;
      else return true;
    }
    else
      return (*p1).v < (*p2).v;
  }

	friend bool operator==(const Edge lhs, const Edge rhs) {
		if ((*lhs.p1 == *rhs.p1) && (*lhs.p2 == *rhs.p2)) return true;
		else return false;
	}

  // to print out an edge, gives the format:
  // (x-coord, y-coord),[original_index, polygonal_index, _lexicographical_index]
	friend std::ostream& operator<<(std::ostream& os, const Edge& e) {
		os << "(" << (*e.p1).x << "," << (*e.p1).y << "),[" << (*e.p1).i << "," << (*e.p1).v << "," << (*e.p1).l << "] , (" << (*e.p2).x << "," << (*e.p2).y << "),[" << (*e.p2).i << "," << (*e.p2).v << "," << (*e.p2).l << "]";
		return os;
	}
};

void createRandPol(std::vector<unsigned int>& polygon,std::vector<Point>& points);
double reldist(const Point& pa, const Point& pb, const Point& p);
double reldist(const Edge& e, const Point& p);
double det(const Edge e, const Point p);
double dety(const Edge e, const Point p);
Yval getYatX(const Edge& e, const double x);
unsigned int getLowestLexIdx(const Edge e1, const Edge e2);
enum intersect_t checkIntersection(const Edge e1, const Edge e2);
void flip(Edge& e1, Edge& e2, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void doFlip(unsigned int i1, unsigned int i2, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void poldisplay (std::vector<unsigned int>& p);

#endif
