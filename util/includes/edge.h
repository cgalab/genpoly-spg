#ifndef __EDGE_H_
#define __EDGE_H_

#include <iostream> // for endl
#include <vector>
#include <set>
#include <iomanip>      // std::setprecision
#include <cmath> // for fabs()
#include <math.h>  // for signbit
#include "basicDefinitions.h"
#include "point.h"
#include "predicates.h"

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
  void setX(const double X) {x=X;}
  double getX() const {return x;}
  double getMin() const {return min;}
  double getMax() const {return max;}
  bool isIn(double s) {
    if ((min < s) && (s < max)) return true;
    else return false;
  }

  bool operator< (const double s) const {
    return (min < s);
  }
  bool operator< (const Yval s) const {
    if (x < s.getX()) {return true;}
    else if (s.getX() < x) {return false;}
    else {
      if (fabs(min - s.getMin()) == 0) return (max < s.getMax());
      else return (min < s.getMin());
    }
    std::cerr << "Error: fallthrough in Yval < comparison!" << std::endl;
    return false;
  }
  bool operator> (const double s) const {
    if (min > s) return true;
    else return false;
  }
  bool operator> (const Yval s) const {
    if (x > s.getX()) return true;
    else if (min > s.getMin()) return true;
    else if ((fabs(min - s.getMin()) == 0) && (max > s.getMax())) return true;
    else return false;
  }

  bool operator== (const Yval rhs) {
    //std::cerr << std::setprecision(15) << (abs(lhs.min - rhs.min)) << " < " << EPSILON << " should be true : " << ((abs(lhs.min - rhs.min) < EPSILON) ? "true" : "false") << std::endl;
    //std::cerr << std::setprecision(15) << (abs(lhs.max - rhs.max)) << " < " << EPSILON << " should be true : " << ((abs(lhs.max - rhs.max) < EPSILON) ? "true" : "false") << std::endl;
    //std::cerr << std::setprecision(15) << (abs(lhs.x - rhs.x)) << " < " << EPSILON << " should be true : " << ((abs(lhs.x - rhs.x) < EPSILON) ? "true" : "false") << std::endl;
    if ((fabs(min - rhs.min) == 0) &&
        (fabs(max - rhs.max) == 0) &&
        (fabs(x - rhs.x) == 0)) return true;
		else return false;
	}

  bool operator!= (const Yval rhs) {
		if ((fabs(min - rhs.min) > 0) ||
        (fabs(max - rhs.max) > 0) ||
        (fabs(x - rhs.x) > 0)) return true;
		else return false;
	}

  friend std::ostream& operator<<(std::ostream& os, const Yval& y) {
    if (fabs(y.getMin() - y.getMax()) == 0)
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

  bool isNull() {
    return ((p1 == NULL) || (p2 == NULL));
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

  unsigned int getVLow() const {
    if ((*p1).v == 0) {
      if ((*p2).v != 1) return (*p2).v;
      else return (*p1).v;
    }
    if ((*p2).v == 0) {
      if ((*p1).v != 1) return (*p1).v;
      else return (*p2).v;
    }
    if ((*p1).v < (*p2).v)
      return (*p1).v;
    else
      return (*p2).v;
  }
  unsigned int getVHigh() const {
    if ((*p1).v == 0) {
      if ((*p2).v != 1) return (*p1).v;
      else return (*p2).v;
    }
    if ((*p2).v == 0) {
      if ((*p1).v != 1) return (*p2).v;
      else return (*p1).v;
    }
    if ((*p1).v < (*p2).v)
      return (*p2).v;
    else
      return (*p1).v;
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
  double getLowerLexIdx() const { // not necessary as p1 is always the lower lex. index.
    if ((*p1).l < (*p2).l) return (*p1).l;
    else return (*p2).l;
  }
  Yval getYatX(const double s) const {
  	Yval y;
  	// calculate the y-axis order of the 2 edges at idx
  	// use Yval in case of x1-x2 = 0
  	Point P1 = *p1;
  	Point P2 = *p2;

  	//assert((x <= P2.x) && (P1.x <= x));

  	if (fabs(P2.x - P1.x) == 0) {
  		y.set(P1.y, P2.y);
  		y.setX(P1.x);
  	}
    else if (s == P1.x) {y.set(P1.y); y.setX(s);}
    else if (s == P2.x) {y.set(P2.y); y.setX(s);}
  	else {
      // there's a problem with how I create the 'bias' and thus the 'value' (solved with the 'else if' statements)
      // when the slope is really steep, the bias is very sensitive.
      y.set(P1.y, P2.y);
  		double slope = (P2.y-P1.y) / (P2.x-P1.x);
      double bias, val;
      bias = P1.y - slope*P1.x;
      val = slope * s + bias;

      if (!y.isIn(val)) {
        bias = P2.y - slope*P2.x;
        val = slope * s + bias;
      }
      y.set(val);
  		y.setX(s);
  	}
  	return y;
  }

  double det(const Point p) const {
  	//const Point& pa = *p1;
  	//const Point& pb = *p2;
  	//double ans = (p.x * (pa.y - pb.y) - p.y * (pa.x-pb.x) + (pa.x*pb.y - pb.x*pa.y));
  	//return (fabs(ans) == 0) ? 0 : ans;
    Point pa = Point(*p1);
  	Point pb = Point(*p2);
  	Point pc = Point(p);
  	pa.x = pa.x - pb.x;pa.y = pa.y - pb.y;
  	pc.x = pc.x - pb.x;pc.y = pc.y - pb.y;
  	double ans = pc.x * pa.y - pc.y * pa.x;
    return ans;
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

  // check if the edge crosses an value on the x-axis 'x'
  // An edge already has p1 and p2 in lex. order.
  enum intersect_t checkCrossing(double x) {
    if (fabs((*p1).x - x) == 0) return IS_VERTEX11;
    if (fabs((*p2).x - x) == 0) return IS_VERTEX22;
    if ((*p1).x < x) {
      if (x < (*p2).x) return IS_TRUE;
    }
    else return IS_FALSE;
  }

  bool operator < (const Edge& e) const {
    // compares 2 edges at x-coordinate 's'
    // have to catch 5 cases, general case, t is in same beginning point, same end point, 3P collinear and 4P collinear
    // all 4 specific cases happen when y value is the same for lhs and rhs.
    // *Problem 1: if the comparison was true at p1, it might be false at p2 if there is
    //            an intersection between the first comparison at p1 and at p2 when the edge will be removed.
    // possible solution: always compare at higher p1 value(?)
//    std::cerr << "lhs: " << lhs << " < rhs: " << rhs << std::endl;
    Yval yl, yr;
    double s;
    // comparison always starts at lex. higher of the P1 points.
    if ((*p1 == *e.p1) && (*p2 == *e.p2)) {return false;} // e is same edge as 'this'
    if (*p1 == *e.p2) {return false;} // 'this' edge starts where 'e' ends: 'this' < 'e' is false
    if (*p2 == *e.p1) {return true;} // 'this' edge ends where 'e' starts: 'this' < 'e' is true
    if (*p1 == *e.p1) { // if both edges start in the same point
      if (*p2 < *e.p2) {s = (*p2).x;}
      else {s = (*e.p2).x;}
    }
    else if (*p1 < *e.p1) {s = (*e.p1).x;} // covers *e.p2 = *p2, i.e. when both edges end in the same point
    else {s = (*p1).x;}
    yl = (*this).getYatX(s);
    yr = e.getYatX(s);
//    std::cerr << "this: " << *this << ", e: " << e << std::endl;
//    std::cerr << "s: " << s << ", yl: " << yl << ", yr: " << yr << std::endl;

    if (yl == yr) {
//      std::cerr << "yl == yr" << std::endl;
      double det1, det2, det3, det4;
      det1 = (*this).det(*e.p1);
      det2 = (*this).det(*e.p2);
      det3 = e.det(*p1);
      det4 = e.det(*p2);
//      std::cerr << "det1: " << det1 << ", det2: " << det2 << ", det3: " << det3 << ", det4: " << det4 << std::endl;
/*
      // Shewchuks predicates
      point pa, pb, pc, pd;
      pa.x = (*p1).x; pa.y = (*p1).y;
      pb.x = (*p2).x; pb.y = (*p2).y;
      pc.x = (*e.p1).x; pc.y = (*e.p1).y;
      pd.x = (*e.p2).x; pd.y = (*e.p2).y;
      double or1 = orient2d(pa,pb,pc);
      double or2 = orient2d(pa,pb,pd);
      std::cerr << "or1: " << or1 << ", or2: " << or2 << ", or3: " << orient2d(pc,pd,pa) << ", or4: " << orient2d(pc,pd,pb) << std::endl;
*/

      // 4P coll.
      unsigned int count = 0;
      if (det1==0) {++count;} if (det2==0) {++count;} if (det3==0) {++count;} if (det4==0) {++count;}
      if (count > 1) {
//        std::cerr << "4Pc: " << std::endl;
        if (*p1 == *e.p1) return *p2 < *e.p2;
//        std::cerr << *p1 << " < " << *e.p1 << " : " << ((*p1 < *e.p1) ? "true" : "false") << std::endl;
        return *p1 < *e.p1;
      }
      // 3P coll.  Some determinant wasn't 0
      else {
        // when one determinant is 0, the other (so to speak) completely describes the relationship between the edges
        if (fabs(det1) + fabs(det2) == 0) { // possibly not needed as if count > 1, it's 4pc.
          if (fabs(det3) == 0) return  std::signbit(det4);
          if (fabs(det4) == 0) return  std::signbit(det3);
        }
        if (fabs(det3) + fabs(det4) == 0) { // possibly not needed as if count > 1, it's 4pc.
          if (fabs(det1) == 0) return !std::signbit(det2);
          if (fabs(det2) == 0) return !std::signbit(det1);
        }
        if (fabs(det1) == 0) return !std::signbit(det2);
        if (fabs(det2) == 0) return !std::signbit(det1);
        if (fabs(det3) == 0) return  std::signbit(det4);
        if (fabs(det4) == 0) return  std::signbit(det3);
        // if yl == yr, but no determinants are 0, but a pair of determinants for one edge has the same sign.
        if (!(std::signbit(det1) ^ std::signbit(det2))) return !(std::signbit(det1));
        if (!(std::signbit(det3) ^ std::signbit(det4))) return  (std::signbit(det3));
        // if yl == yr, but the edges intersect
        if (*p1 < *e.p1) return !std::signbit(det1);
        else return std::signbit(det3);

        std::cerr << "ERROR: Unexpected fallthrough in comparison!  yl == yr, this: " << *this << ", e: " << e << std::endl;
        std::cerr << "det1: " << det1 << ", det2: " << det2 << ", det3: " << det3 << ", det4: " << det4 << std::endl;

        return false;
      }
    }
    else {
//      std::cerr << "gen: " << yl << " < " << yr << " : " << ((yl < yr) ? "true" : "false") << std::endl;
      return yl < yr; // general case:
    }
    std::cerr << "ERROR: unexpected fallthrough in comparison! this: " << *this << ", e: " << e << std::endl;
    return false;
  }

  bool operator == (const Edge e) const {
//    std::cerr << "lhs: " << *this << ", rhs: " << e << std::endl;
		if ((*p1 == *e.p1) && (*p2 == *e.p2)) return true;
		else return false;
  }
/*
	friend bool operator==(const Edge lhs, const Edge rhs) {
    std::cerr << "lhs: " << lhs << ", rhs: " << rhs << std::endl;
		if ((*lhs.p1 == *rhs.p1) && (*lhs.p2 == *rhs.p2)) return true;
		else return false;
	}
*/
  friend bool operator!=(const Edge lhs, const Edge rhs) {
		if ((*lhs.p1 != *rhs.p1) || (*lhs.p2 != *rhs.p2)) return true;
		else return false;
	}

  // to print out an edge, gives the format:
  // (x-coord, y-coord),[original_index, polygonal_index, _lexicographical_index]
	friend std::ostream& operator<<(std::ostream& os, const Edge& e) {
		os << "(" << (*e.p1).x << "," << (*e.p1).y << "),[" << (*e.p1).i << "," << (*e.p1).v << "," << (*e.p1).l << "] , (" << (*e.p2).x << "," << (*e.p2).y << "),[" << (*e.p2).i << "," << (*e.p2).v << "," << (*e.p2).l << "]";
		return os;
	}
};

class I_Edge: public Edge {
private:
protected:
public:
  // l2ch : "left point (p1) is the direction towards the convex hull"
  bool l2ch;

  I_Edge() {p1=NULL; p2=NULL;l_idx=0; l2ch=false;}
  I_Edge(Point *P1, Point *P2) {
		if ((*P1) < (*P2)) {p1=P1; p2=P2;}
		else {p1=P2; p2=P1;}
    l_idx=0;l2ch=false;
	}
  I_Edge(Point *P1, Point *P2, bool b) {
		if ((*P1) < (*P2)) {p1=P1; p2=P2;}
		else {p1=P2; p2=P1;}
    l_idx=0;l2ch=b;
	}

  // the '*' is to see better which direction the c.h. is.
  friend std::ostream& operator<<(std::ostream& os, const I_Edge& e) {
    os << (e.l2ch ? "*" : "") << "(" << (*e.p1).x << "," << (*e.p1).y << "),[" << (*e.p1).i
    << "," << (*e.p1).v << "," << (*e.p1).l << "] , " << (e.l2ch ? "" : "*") << "(" << (*e.p2).x << "," << (*e.p2).y
    << "),[" << (*e.p2).i << "," << (*e.p2).v << "," << (*e.p2).l << "]";
    return os;
  }
};

class C_Edge: public Edge {
public:
  unsigned int sc;  // index into a vector of s_curves
  unsigned int par;  // index into a vector of < std::pair<uint, uint> > of curve ends.
  bool lower;       // whether the edge is an upper curve end or lower curve end.

  C_Edge() {p1=NULL; p2=NULL;l_idx=0; sc=0; par=0;}
  C_Edge(Point *P1, Point *P2) {
		if ((*P1) < (*P2)) {p1=P1; p2=P2;}
		else {p1=P2; p2=P1;}
    l_idx=0;sc=0;par=0;
	}

  friend bool operator==(const C_Edge lhs, const C_Edge rhs) {
		if ((*lhs.p1 == *rhs.p1) && (*lhs.p2 == *rhs.p2)) return true;
		else return false;
	}

  // to print out an edge, gives the format:
  // (x-coord, y-coord),[original_index, polygonal_index, _lexicographical_index]
	friend std::ostream& operator<<(std::ostream& os, const C_Edge& e) {
		os << "(" << (*e.p1).x << "," << (*e.p1).y << "),[" << (*e.p1).i
    << "," << (*e.p1).v << "," << (*e.p1).l << "] , (" << (*e.p2).x << "," << (*e.p2).y
    << "),[" << (*e.p2).i << "," << (*e.p2).v << "," << (*e.p2).l << "] : [" << e.sc << "," << e.par << "]";
		return os;
	}
};

class D_Edge: public Edge {
private:
protected:
public:
  unsigned int curve_id; // should be a valid index into a vector of 'Curve's
  bool bin; // variable to tell whether inside the polygon is on the right/bottom side of the edge (going from p1 to p2)

  D_Edge() {p1=NULL; p2=NULL;l_idx=0; curve_id=0;bin=false;}
  D_Edge(Point *P1, Point *P2) {
		if ((*P1) < (*P2)) {p1=P1; p2=P2;}
		else {p1=P2; p2=P1;}
    l_idx=0;curve_id=0;bin=false;
	}
  D_Edge(Edge e) {
    p1 = e.p1; p2 = e.p2;
    l_idx = e.l_idx;curve_id=0;bin=false;
  }

  // returns true if the point is on the side of the edge that is 'inside' the polygon.
  bool check_side (Point p) {
    double side = (*this).det(p);
    if (side < 0 &&  (*this).bin) return true;
    if (side > 0 && !(*this).bin) return true;
    return false;
  }

  friend bool operator==(const D_Edge lhs, const D_Edge rhs) {
		if ((*lhs.p1 == *rhs.p1) && (*lhs.p2 == *rhs.p2)) return true;
		else return false;
	}

  // to print out an edge, gives the format:
  // (x-coord, y-coord),[original_index, polygonal_index, _lexicographical_index]
	friend std::ostream& operator<<(std::ostream& os, const D_Edge& e) {
		os << "(" << (*e.p1).x << "," << (*e.p1).y << "),[" << (*e.p1).i
    << "," << (*e.p1).v << "," << (*e.p1).l << "] , (" << (*e.p2).x << "," << (*e.p2).y
    << "),[" << (*e.p2).i << "," << (*e.p2).v << "," << (*e.p2).l << "] : b : " << e.bin << "";
		return os;
	}
};

class E_Edge: public D_Edge {
private:
protected:
public:
  std::vector<D_Edge> closest; // of all the incidental edges, these were the closest that weren't intersecting others.

  E_Edge() {p1=NULL; p2=NULL;l_idx=0; curve_id=0; bin=false;}
  E_Edge(Point *P1, Point *P2) {
		if ((*P1) < (*P2)) {p1=P1; p2=P2;}
		else {p1=P2; p2=P1;}
    l_idx=0;curve_id=0;bin=false;
	}
  E_Edge(D_Edge e) {
    p1 = e.p1; p2 = e.p2;
    l_idx = e.l_idx; curve_id = e.curve_id;bin=e.bin;
  }
  E_Edge(Edge e) {
    p1 = e.p1; p2 = e.p2;
    l_idx = e.l_idx;curve_id=0;bin=false;
  }
/*
  // this shouldn't be here... better as a standalone function.
  bool check_visibility(Point p, bool check_lex_low) {
    // first check if the point is on the right side of the edge
    if ((*this).check_side(p)) {
      // then make the slope from this edge to 'p' to compare to 'smallest_slope':
      // first make the slope for the edge, given lex. lower point of edge is the origin and the line goes through the origin.
      double e_x, e_y, p_x, p_y, e_slope, p_slope;
      Point e1, e2;
      if (check_lex_low) {e1=(*this.p1);e2=(*this.p2);}
      else {e1=(*this.p2);e2=(*this.p1);}

      e_x = e2.x-e1.x;
      e_y = e2.x-e1.y;
      p_x = p.x-(*this.p1).x;
      p_y = p.y-(*this.p1).y;
      e_slope = e_y/e_x;
      p_slope = p_y/p_x;
      p_slope = p_slope - e_slope;
      if (fabs(p_slope) < fabs(smallest_slope)) return true;
      else return false;

      // then make the line from the origin to 'p'
      // deduct the edge line from the p line and check if it's small enough
    }
    else return false;
  }

  // same as 'check_visibility', but updates the 'closest' edge and the 'smallest_slope' variable.
  bool update_closest(E_Edge e, bool check_lex_low) {
    // first check if the edges are facing each other.
    bool face;
    if ((*this) < e) {
      if (!(*this).bin && e.bin) face = true;
      else face = false;
    }
    else {
      if((*this).bin && !e.bin) face = true;
      else face = false;
    }

    if (face) {
      Point p;
      if (check_lex_low) p = *e.p1;
      else p = *e.p2;
      // then make the slope from this edge to 'p' to compare to 'smallest_slope':
      // first make the slope for the edge, given lex. lower point of edge is the origin and the line goes through the origin.
      double e_x, e_y, p_x, p_y, e_slope, p_slope;
      Point p1, p2;
      if (check_lex_low) {p1=*(this->p1);p2=*(this->p2);}
      else {p1=*(this->p2);p2=*(this->p1);}

      e_x = p2.x-p1.x;
      e_y = p2.y-p1.y;
      p_x = p.x-(*this->p1).x;
      p_y = p.y-(*this->p1).y;
      e_slope = e_y/e_x;
      p_slope = p_y/p_x;
      p_slope = p_slope - e_slope;
      if (fabs(p_slope) < fabs(smallest_slope)) {
        this.smallest_slope = p_slope;
        this.closest = e;
        return true;
      }
      else return false;

      // then make the line from the origin to 'p'
      // deduct the edge line from the p line and check if it's small enough
      }
      else return false;

  }
*/
  // to print out an edge, gives the format:
  // (x-coord, y-coord),[original_index, polygonal_index, _lexicographical_index]
	friend std::ostream& operator<<(std::ostream& os, const E_Edge& e) {
		os << "(" << (*e.p1).x << "," << (*e.p1).y << "),[" << (*e.p1).i
    << "," << (*e.p1).v << "," << (*e.p1).l << "] , (" << (*e.p2).x << "," << (*e.p2).y
    << "),[" << (*e.p2).i << "," << (*e.p2).v << "," << (*e.p2).l << "] : [c" << e.curve_id
    << "], cl: " << e.closest.size() << ", b : " << e.bin;
		return os;
	}
};

void decrementEdges(std::set<Edge>& edgeS);
bool eraseEdgeFromSet (Edge e, std::set<Edge>& edgeS);
void eraseVertexFromSet(Point *p1, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
//void createRandPol(std::vector<unsigned int>& polygon,std::vector<Point>& points, unsigned int randseed);
double reldist(const Point& pa, const Point& pb, const Point& p);
double reldist(const Edge& e, const Point& p);
double det(const Edge e, const Point p);
double dety(const Edge e, const Point p);
Yval getYatX(const Edge& e, const double x);
unsigned int getLowestLexIdx(const Edge e1, const Edge e2);
enum intersect_t checkIntersection(const Edge e1, const Edge e2);
enum intersect_t checkIntersection2(const Edge e1, const Edge e2);
void flip(Edge& e1, Edge& e2, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void doFlip(unsigned int i1, unsigned int i2, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void doFlip(unsigned int i1, unsigned int i2, std::vector<Point>& points);
void flip2(Edge& e1, Edge& e2, std::vector<unsigned int>& polygon);
double get_smaller_angle(E_Edge& e1, E_Edge& e2, bool use_p1);
double get_larger_angle(E_Edge& e1, E_Edge& e2, bool use_p1);
void poldisplay (std::vector<unsigned int>& p);

#endif
