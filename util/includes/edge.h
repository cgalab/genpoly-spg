#include <iostream> // for endl
#include <vector>
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
    if (max < s) return true;
    else return false;
  }
  bool operator < (const Yval s) const {
    if (x < s.getX()) return true;
    else if (max == s.getMin() && (min < s.getMin())) return true;
    else if (max < s.getMin()) return true;
    //else if ((min == s.getMin()) && (max < s.getMax())) return true;
    else return false;
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

  friend std::ostream& operator<<(std::ostream& os, const Yval& y) {
    if (y.getMin() == y.getMax())
      os << "x:" << y.getX() << ", (" << y.min << "," << y.max << ")";
    else
      os << "x:" << y.getX() << ", [" << y.min << "," << y.max << "]";
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

/*
  IMPORTANT:
  1) Given that the 2 edges both exist at xcoord: lhs.l_idx, the comparison of the 2 edges is made at lhs.l_idx
*/
/*
  friend bool operator<(const Edge& lhs, const Edge& rhs) {
    //std::cout << "lhs: " << lhs << std::endl;
    //std::cout << "rhs: " << rhs << std::endl;
    // The lhs is always the one being compared to all the others
    double idx = lhs.l_idx;
    //std::cout << "idx: " << idx << std::endl;

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
*/
/*
  friend bool operator>(const Edge& lhs, const Edge& rhs) {
    //std::cout << "lhs: " << lhs << std::endl;
    //std::cout << "rhs: " << rhs << std::endl;
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
*/
	friend bool operator==(const Edge& lhs, const Edge& rhs) {
		if ((lhs.p1 == rhs.p1) && (lhs.p2 == rhs.p2)) return true;
		else return false;
	};

	friend std::ostream& operator<<(std::ostream& os, const Edge& e) {
		os << "(" << (*e.p1).x << "," << (*e.p1).y << "),[" << (*e.p1).i << "," << (*e.p1).v << "] , (" << (*e.p2).x << "," << (*e.p2).y << "),[" << (*e.p2).i << "," << (*e.p2).v << "]";
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
