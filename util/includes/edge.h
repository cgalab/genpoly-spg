#include <iostream> // for endl
#include <vector>
#include "basicDefinitions.h"
#include "point.h"

#ifndef __EDGE_H_
#define __EDGE_H_

// class for when an edge is vertical on the x axis
class Yval {
private:
public:
  double max, min;

  Yval() {max=0;min=0;}
  Yval(double val) {max=val;min=val;}
  Yval(double a, double b) {
    if (a<b) {min=a;max=b;}
    else {min=b;max=a;}
  }

  void set(double val) {max=val;min=val;}
  void set(double a, double b) {
    if (a<b) {min=a;max=b;}
    else {min=b;max=a;}
  }

  bool operator < (const double s) const {
    if (min < s) return true;
    else return false;
  }
  bool operator < (const Yval s) const {
         if (min < s.min) return true;
    else if ((min == s.min) && (max < s.max)) return true;
    else return false;
  }
  bool operator > (const double s) const {
    if (min > s) return true;
    else return false;
  }
  bool operator > (const Yval s) const {
         if (min > s.min) return true;
    else if ((min == s.min) && (max > s.max)) return true;
    else return false;
  }

  friend std::ostream& operator<<(std::ostream& os, const Yval& y) {
    os << "(" << y.min << "," << y.max << ")";
    return os;
  };
};

class Edge {
public:
	Point *p1;
	Point *p2;
	double l_idx; // the lower lexicographical index of the edge

	Edge() {p1=NULL; p2=NULL; l_idx=0;}
	Edge(const Edge& e) {p1=e.p1; p2=e.p2; l_idx = e.l_idx;}
	Edge(Point *P1, Point *P2) {
		if ((*P1).v < (*P2).v) {p1=P1; p2=P2;}
		else {p1=P2; p2=P1;}
		l_idx=0;
	}
	Edge(Point *P1, Point *P2, unsigned int L_I) {
		if ((*P1).v < (*P2).v) {p1=P1; p2=P2;}
		else {p1=P2; p2=P1;}
		l_idx=L_I;
	}

	void set(Point* v1, Point* v2) {
		p1 = v1;
		p2 = v2;  
	}

  void set(Edge val) {
    p1 = val.p1;
    p2 = val.p2;
    l_idx = val.l_idx;
  }

  Point getLexLow() const {
    if ((*p1).x < (*p2).x)
      return *p1;
    else {
      if (((*p1).x == (*p2).x) && ((*p1).y < (*p2).y))
        return *p1;
      else
        return *p2;
    }
  }

  Point getLexHigh() const {
    if ((*p1).x < (*p2).x)
      return *p2;
    else {
      if (((*p1).x == (*p2).x) && ((*p1).y < (*p2).y))
        return *p2;
      else
        return *p1;
    }
  }

  friend bool operator<(const Edge& lhs, const Edge& rhs) {
    std::cout << "lhs: " << lhs << std::endl;
    std::cout << "rhs: " << rhs << std::endl;
    // The lhs is always the one being compared to all the others
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

  friend bool operator>(const Edge& lhs, const Edge& rhs) {
    std::cout << "lhs: " << lhs << std::endl;
    std::cout << "rhs: " << rhs << std::endl;
    // The lhs is always the one being compared to all the others
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

    return Ly > Ry;
  }

	friend bool operator==(const Edge& lhs, const Edge& rhs) {
		if ((lhs.p1 == rhs.p1) && (lhs.p2 == rhs.p2)) return true;
		else return false;
	};

	friend std::ostream& operator<<(std::ostream& os, const Edge& e) {
		os << " p1: x:" << (*e.p1).x << ", y:" << (*e.p1).y << ", i:" << (*e.p1).i << ", v:" << (*e.p1).v << ", p2: x:" << (*e.p2).x << ", y:" << (*e.p2).y << ", i:" << (*e.p2).i << ", v:" << (*e.p2).v << ": l_idx:" << e.l_idx;
		return os;
	};
};

void createRandPol(std::vector<unsigned int>& polygon,std::vector<Point>& points);
double reldist(const Edge& e, const Point& p);
double det(const Edge& e, const Point& p);
enum intersect_t checkIntersection(const Edge e1, const Edge e2);
void flip(Edge& e1, Edge& e2, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void doFlip(unsigned int i1, unsigned int i2, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void poldisplay (std::vector<unsigned int>& p);

#endif