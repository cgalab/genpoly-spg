#include <iostream> // for endl
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <math.h> // for signbit
#include <cmath> // for fabs
#include <assert.h>
#include "basicDefinitions.h"
#include "edge.h"
#include "point.h"
#include "predicates.h"

// function to remove edges from 'edgeS' up to and including value of 'index'
void decrementEdges(std::set<Edge>& edgeS) {
	edgeS.clear();
}

// this function should be used to guarantee removal of an edge from the 'edgeS' set.
bool eraseEdgeFromSet (Edge e, std::set<Edge>& edgeS) {
  std::set<Edge>::iterator it;

//  std::cerr << "edge being erased: " << e << std::endl;
  it = edgeS.find(e);

  if ((it == edgeS.end()) || (*it != e)) {
    // came to the end of the set without finding the edge, have to use the linear method of finding the edgeS
    for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) {
      if (*it != e) continue;
			else {
        edgeS.erase(it);
        return true;
      }
    }
  }
	else {
    edgeS.erase(it);
    return true;
  }
  return false;
}

// this function should be used to remove an edge from the 'edgeS' set if it was found.
void softEraseEdgeFromSet (Edge2 e, std::set<Edge2>& edgeS) {
  std::set<Edge2>::iterator it;

//  std::cerr << "edge being erased: " << e << std::endl;
  it = edgeS.find(e);
  if (it != edgeS.end()) {
    // if the vertex indices are not incidental, remove the edge in it' and find 'e' again
		if ((*it) == e) edgeS.erase(it);
    else if ((*it).getPHigh() - (*it).getPLow() != 1) {
      edgeS.erase(it);
    }
  }
}

// function to remove edges connected to a single vertex from 'edgeS' set.
// does not care whether it finds an edge or not, just attempts to remove it.
void eraseVertexFromSet(Point *p1, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  unsigned int before, after;
  Point *p2, *p3;
  Edge e1, e2;
	std::set<Edge>::iterator it1;

	before = ((*p1).v + polygon.size() -1) % polygon.size();
  after =  ((*p1).v + polygon.size() +1) % polygon.size();

  p2 = &points[polygon[before]];
  p3 = &points[polygon[after]];

  e1 = Edge(p1, p2);
  e2 = Edge(p1, p3);

//  std::cerr << "erasing vertexes: e1: " << e1 << ", e2: " << e2 << std::endl;
	edgeS.erase(e1);
	edgeS.erase(e2);
}

// function to remove Edge2 edges connected to a single vertex from 'edgeS' set.
// does not care whether it finds an edge or not, just attempts to remove it.
void eraseVertexFromSet(Point *p1, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  unsigned int before, after;
  Point *p2, *p3;
  Edge2 e1, e2;
	std::set<Edge2>::iterator it1;

  before = ((*p1).v + polygon.size() -1) % polygon.size();
  after =  ((*p1).v + polygon.size() +1) % polygon.size();

  p2 = &points[polygon[before]];
  p3 = &points[polygon[after]];

  e1 = Edge2(p1, p2);
  e2 = Edge2(p1, p3);

//  std::cerr << "erasing edges: e1: " << e1 << ", e2: " << e2 << std::endl;
	edgeS.erase(e1);
	edgeS.erase(e2);
}

double reldist(const Edge& e, const Point& p) {
	return reldist(*e.p1, *e.p2, p);
}
double reldist(const Edge2& e, const Point& p) {
	return reldist(*e.p1, *e.p2, p);
}

/*
// calculates the determinant of the vertices of 'e' and the point p (similar function in point.cpp for 3 points)
double det(const Edge e, const Point p) {
	const Point& pa = *e.p1;
	const Point& pb = *e.p2;
	double ans = (p.x * (pa.y - pb.y) - p.y * (pa.x-pb.x) + (pa.x*pb.y - pb.x*pa.y));
//	std::cerr << "det: " << ans << std::endl;
	//return (fabs(ans) < EPSILON) ? 0 : ans;
	return ans;
}
*/

// simpler calculates of the determinant where pb is moved to (0.0) and thus it is removed from the other 2 points.
double det(const Edge e, const Point p) {
	Point pa = Point(*e.p1);
	Point pb = Point(*e.p2);
	Point pc = Point(p);
	pa.x = pa.x - pb.x;pa.y = pa.y - pb.y;
	pc.x = pc.x - pb.x;pc.y = pc.y - pb.y;
	double ans = pc.x * pa.y - pc.y * pa.x;
	//std::cerr << "det: " << ans << std::endl;
	//return (fabs(ans) < EPSILON) ? 0 : ans;
	return ans;
}

// using shewchuks determinant.
double cdet(const Point p1, const Point p2, const Point p) {
	point pa, pb, pc;
	pa.x = p1.x; pa.y = p1.y;
	pb.x = p2.x; pb.y = p2.y;
	pc.x = p.x; pc.y = p.y;

	return orient2d(pa,pb,pc);
}

double cdet(const Edge2 e, const Point p) {
	point pa, pb, pc;
	pa.x = (*e.p1).x; pa.y = (*e.p1).y;
	pb.x = (*e.p2).x; pb.y = (*e.p2).y;
	pc.x = p.x; pc.y = p.y;

	return orient2d(pa,pb,pc);
}

double dety(const Edge e, const Point p) {
	Point pa = *e.p1;
	Point pb = *e.p2;
	if ((*e.p1).y > (*e.p2).y) {
		pa = *e.p2;
		pb = *e.p1;
	}

	double ans = (p.x * (pa.y - pb.y) - p.y * (pa.x-pb.x) + (pa.x*pb.y - pb.x*pa.y));
	return (fabs(ans) < EPSILON) ? 0 : ans;
}

Yval getYatX(const Edge& e, const double x) {
	Yval y;
	// calculate the y-axis order of the 2 edges at idx
	// use Yval in case of x1-x2 = 0
	Point P1 = *e.p1;
	Point P2 = *e.p2;

	//assert((x <= P2.x) && (P1.x <= x));

	if ((P2.x - P1.x) == 0) {
		y.set(P1.y, P2.y);
		y.setX(P1.x);
	}
	else {
		double slope = (P2.y-P1.y) / (P2.x-P1.x);
		double bias = P1.y - slope*P1.x;
		double val = slope * x + bias;
		if (fabs(val) < EPSILON) y.set(0);
		else y.set(val);
		y.setX(x);
	}
	return y;
}

unsigned int getLowestLexIdx(const Edge e1, const Edge e2) {
	unsigned int le1 = e1.getLowerLexIdx();
	unsigned int le2 = e2.getLowerLexIdx();
	if (le1 < le2) return le1;
	else return le2;
}

enum intersect_t checkIntersection(const Edge e1, const Edge e2) {
	// is e1 and e2 the same edge?
	if (e1 == e2) return IS_SAME_EDGE;

	double det_a, det_b, det_c, det_d;
	double dp_1, dp_2, dp_3, dp_4;
	bool same11 = false, same12 = false, same21 = false, same22 = false;

//	std::cout << "e1.p1 == e2.p1: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << ", e1.p1: " << *e1.p1 << std::endl;
//	std::cout << "e1.p1 == e2.p2: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << ", e1.p2: " << *e1.p2 << std::endl;
//	std::cout << "e1.p2 == e2.p1: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << ", e2.p1: " << *e2.p1 << std::endl;
//	std::cout << "e1.p2 == e2.p2: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << ", e2.p2: " << *e2.p2 << std::endl;

	// determinant between edge 1 and a point in edge 2
	det_a = det(e1, *e2.p1);
	det_b = det(e1, *e2.p2);
	// determinant between edge 2 and a point in edge 1
	det_c = det(e2, *e1.p1);
	det_d = det(e2, *e1.p2);

	//std::cerr.precision(24);
	//int sig,ex;
	//sig = frexp(det_d, &ex);
	//std::cerr << "sig: " << sig << ", exp: " << ex << std::endl;
	//std::cerr.precision(17);
//	std::cerr << "det_a: " << det_a << std::endl;
//	std::cerr << "det_b: " << det_b << std::endl;
//	std::cerr << "det_c: " << det_c << std::endl;
//	std::cerr << "det_d: " << det_d << std::endl;

	if (det_a*det_b*det_c*det_d == 0) {
		bool col = false;

		//quick check if the edges share a vertex
		if (*(e1.p1) == *(e2.p1)) same11 = true;
		if (*(e1.p1) == *(e2.p2)) same12 = true;
		if (*(e1.p2) == *(e2.p1)) same21 = true;
		if (*(e1.p2) == *(e2.p2)) same22 = true;

		// some determinant was 0, need to check if it's inside an edge or outside.
		dp_1 = reldist(e1, *e2.p1);
		dp_2 = reldist(e1, *e2.p2);
		dp_3 = reldist(e2, *e1.p1);
		dp_4 = reldist(e2, *e1.p2);

		//std::cerr.precision(17);
//		std::cerr << "det_a: " << det_a << ", dp1: " << dp_1 << ", same11: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << std::endl;
//		std::cerr << "det_b: " << det_b << ", dp2: " << dp_2 << ", same12: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << std::endl;
//		std::cerr << "det_c: " << det_c << ", dp3: " << dp_3 << ", same21: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << std::endl;
//		std::cerr << "det_d: " << det_d << ", dp4: " << dp_4 << ", same22: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << std::endl;

				 if ( (det_a == 0) && (dp_1 > 0) && (dp_1 < 1) ) col = true;
		else if ( (det_b == 0) && (dp_2 > 0) && (dp_2 < 1) ) col = true;
		else if ( (det_c == 0) && (dp_3 > 0) && (dp_3 < 1) ) col = true;
		else if ( (det_d == 0) && (dp_4 > 0) && (dp_4 < 1) ) col = true;

		// 2opt function only cares about collinearity when it's 4 point and the points intercept in some way.
		if ((fabs(det_a)+fabs(det_b)+fabs(det_c)+fabs(det_d) == 0) && col) return IS_4P_COLLINEAR;
		else if (same11) return IS_VERTEX11;
		else if (same12) return IS_VERTEX12;
		else if (same21) return IS_VERTEX21;
		else if (same22) return IS_VERTEX22;
		unsigned int count = 0;
		if (det_a==0) {++count;} if (det_b==0) {++count;} if (det_c==0) {++count;} if (det_d==0) {++count;}
		if (count > 1) return IS_4P_COLLINEAR;

		else if (col) return IS_3P_COLLINEAR;
		else return IS_FALSE;
	}
	else {
		// none of the determinants were 0, so just need to check the sign for intersection.
		if ( (std::signbit(det_a) ^ std::signbit(det_b)) && (std::signbit(det_c) ^ std::signbit(det_d)) ) {
 			return IS_TRUE;
		}
		else return IS_FALSE;
	}
}

// uses shewchuks predicates for the determinant.
enum intersect_t checkIntersection2(const Edge e1, const Edge e2) {
	// is e1 and e2 the same edge?
	if (e1 == e2) return IS_SAME_EDGE;

	double det_a, det_b, det_c, det_d;
	double dp_1, dp_2, dp_3, dp_4;
	point pa, pb, pc, pd;
	bool same11 = false, same12 = false, same21 = false, same22 = false;

//	std::cout << "e1.p1 == e2.p1: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << ", e1.p1: " << *e1.p1 << std::endl;
//	std::cout << "e1.p1 == e2.p2: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << ", e1.p2: " << *e1.p2 << std::endl;
//	std::cout << "e1.p2 == e2.p1: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << ", e2.p1: " << *e2.p1 << std::endl;
//	std::cout << "e1.p2 == e2.p2: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << ", e2.p2: " << *e2.p2 << std::endl;

	// converting to 'point' class from 'predicates'
	pa.x = (*e1.p1).x; pa.y = (*e1.p1).y;
	pb.x = (*e1.p2).x; pb.y = (*e1.p2).y;
	pc.x = (*e2.p1).x; pc.y = (*e2.p1).y;
	pd.x = (*e2.p2).x; pd.y = (*e2.p2).y;

	// determinant between edge 1 and a point in edge 2
	det_a = orient2d(pa, pb, pc);
	det_b = orient2d(pa, pb, pd);
	// determinant between edge 2 and a point in edge 1
	det_c = orient2d(pc, pd, pa);
	det_d = orient2d(pc, pd, pb);

	//std::cerr.precision(24);
	//int sig,ex;
	//sig = frexp(det_d, &ex);
	//std::cerr << "sig: " << sig << ", exp: " << ex << std::endl;
//	std::cerr.precision(17);
//	std::cerr << "det_a: " << det_a << std::endl;
//	std::cerr << "det_b: " << det_b << std::endl;
//	std::cerr << "det_c: " << det_c << std::endl;
//	std::cerr << "det_d: " << det_d << std::endl;

	if (det_a*det_b*det_c*det_d == 0) {
		bool col = false;

		//quick check if the edges share a vertex
		if (*(e1.p1) == *(e2.p1)) same11 = true;
		if (*(e1.p1) == *(e2.p2)) same12 = true;
		if (*(e1.p2) == *(e2.p1)) same21 = true;
		if (*(e1.p2) == *(e2.p2)) same22 = true;

		// some determinant was 0, need to check if it's inside an edge or outside.
		dp_1 = reldist(e1, *e2.p1);
		dp_2 = reldist(e1, *e2.p2);
		dp_3 = reldist(e2, *e1.p1);
		dp_4 = reldist(e2, *e1.p2);

		//std::cerr.precision(17);
//		std::cerr << "det_a: " << det_a << ", dp1: " << dp_1 << ", same11: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << std::endl;
//		std::cerr << "det_b: " << det_b << ", dp2: " << dp_2 << ", same12: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << std::endl;
//		std::cerr << "det_c: " << det_c << ", dp3: " << dp_3 << ", same21: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << std::endl;
//		std::cerr << "det_d: " << det_d << ", dp4: " << dp_4 << ", same22: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << std::endl;

				 if ( (det_a == 0) && (dp_1 > 0) && (dp_1 < 1) ) col = true;
		else if ( (det_b == 0) && (dp_2 > 0) && (dp_2 < 1) ) col = true;
		else if ( (det_c == 0) && (dp_3 > 0) && (dp_3 < 1) ) col = true;
		else if ( (det_d == 0) && (dp_4 > 0) && (dp_4 < 1) ) col = true;

		// 2opt function only cares about collinearity when it's 4 point and the points intercept in some way.
		if ((fabs(det_a)+fabs(det_b)+fabs(det_c)+fabs(det_d) == 0) && col) return IS_4P_COLLINEAR;
		else if (same11) return IS_VERTEX11;
		else if (same12) return IS_VERTEX12;
		else if (same21) return IS_VERTEX21;
		else if (same22) return IS_VERTEX22;

		else if (col) return IS_3P_COLLINEAR;
		else return IS_FALSE;
	}
	else {
		// none of the determinants were 0, so just need to check the sign for intersection.
		if ( (std::signbit(det_a) ^ std::signbit(det_b)) && (std::signbit(det_c) ^ std::signbit(det_d)) ) {
 			return IS_TRUE;
		}
		else return IS_FALSE;
	}
}

// uses shewchuks predicates for the determinant.
enum intersect_t checkIntersection(const Edge2 e1, const Edge2 e2) {
	// is e1 and e2 the same edge?
	if (e1 == e2) return IS_SAME_EDGE;

	double det_a, det_b, det_c, det_d;
	double dp_1, dp_2, dp_3, dp_4;
	point pa, pb, pc, pd;
	bool same11 = false, same12 = false, same21 = false, same22 = false;

//	std::cout << "e1.p1 == e2.p1: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << ", e1.p1: " << *e1.p1 << std::endl;
//	std::cout << "e1.p1 == e2.p2: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << ", e1.p2: " << *e1.p2 << std::endl;
//	std::cout << "e1.p2 == e2.p1: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << ", e2.p1: " << *e2.p1 << std::endl;
//	std::cout << "e1.p2 == e2.p2: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << ", e2.p2: " << *e2.p2 << std::endl;

	// converting to 'point' class from 'predicates'
	pa.x = (*e1.p1).x; pa.y = (*e1.p1).y;
	pb.x = (*e1.p2).x; pb.y = (*e1.p2).y;
	pc.x = (*e2.p1).x; pc.y = (*e2.p1).y;
	pd.x = (*e2.p2).x; pd.y = (*e2.p2).y;

	// determinant between edge 1 and a point in edge 2
	det_a = orient2d(pa, pb, pc);
	det_b = orient2d(pa, pb, pd);
	// determinant between edge 2 and a point in edge 1
	det_c = orient2d(pc, pd, pa);
	det_d = orient2d(pc, pd, pb);

//	std::cerr.precision(24);
//	int sig,ex;
//	sig = frexp(det_d, &ex);
//	std::cerr << "sig: " << sig << ", exp: " << ex << std::endl;
//	std::cerr.precision(17);
//	std::cerr << "det_a: " << det_a << std::endl;
//	std::cerr << "det_b: " << det_b << std::endl;
//	std::cerr << "det_c: " << det_c << std::endl;
//	std::cerr << "det_d: " << det_d << std::endl;

	if (det_a*det_b*det_c*det_d == 0) {
		bool col = false;

		//quick check if the edges share a vertex
		if (*(e1.p1) == *(e2.p1)) same11 = true;
		if (*(e1.p1) == *(e2.p2)) same12 = true;
		if (*(e1.p2) == *(e2.p1)) same21 = true;
		if (*(e1.p2) == *(e2.p2)) same22 = true;

		// some determinant was 0, need to check if it's inside an edge or outside.
		dp_1 = reldist(e1, *e2.p1);
		dp_2 = reldist(e1, *e2.p2);
		dp_3 = reldist(e2, *e1.p1);
		dp_4 = reldist(e2, *e1.p2);

		//std::cerr.precision(17);
//		std::cerr << "det_a: " << det_a << ", dp1: " << dp_1 << ", same11: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << std::endl;
//		std::cerr << "det_b: " << det_b << ", dp2: " << dp_2 << ", same12: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << std::endl;
//		std::cerr << "det_c: " << det_c << ", dp3: " << dp_3 << ", same21: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << std::endl;
//		std::cerr << "det_d: " << det_d << ", dp4: " << dp_4 << ", same22: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << std::endl;

		if ( (det_a == 0) && (dp_1 > 0) && (dp_1 < 1) ) col = true;
		if ( (det_b == 0) && (dp_2 > 0) && (dp_2 < 1) ) col = true;
		if ( (det_c == 0) && (dp_3 > 0) && (dp_3 < 1) ) col = true;
		if ( (det_d == 0) && (dp_4 > 0) && (dp_4 < 1) ) col = true;

		// 2opt function only cares about collinearity when it's 4 point and the points intercept in some way.
		if ((fabs(det_a)+fabs(det_b)+fabs(det_c)+fabs(det_d) == 0) && col) return IS_4P_COLLINEAR;
		// 3P collinearity assumes the 3 vertices are incidental in polygon and the 3 points are collinear.
		if ((same11 || same12 || same21 || same22) && col) return IS_3P_COLLINEAR;
		if (col) return IS_TRUE; // 3 points being collinear (and not inc. vert.) is best solved with a 2opt flip.
		if (same11) return IS_VERTEX11; // not collinear, but are incidental vertices.
		if (same12) return IS_VERTEX12;
		if (same21) return IS_VERTEX21;
		if (same22) return IS_VERTEX22;

		return IS_FALSE;
	}
	else {
		// none of the determinants were 0, so just need to check the sign for intersection.
		if ( (std::signbit(det_a) ^ std::signbit(det_b)) && (std::signbit(det_c) ^ std::signbit(det_d)) ) {
 			return IS_TRUE;
		}
		else return IS_FALSE;
	}
}


// flips the smaller range of indexes in a polygon and updates the references in 'points' vector
void flip(Edge& e1, Edge& e2, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
	//std::cout << "inside flip" << std::endl;

	// edge case, one edge goes from index 0 to last index
	if ( ((e1.getPLow().v == 0) && (e1.getPHigh().v == points.size()-1)) || ((e2.getPLow().v == 0) && (e2.getPHigh().v == points.size()-1)) ) {
		//std::cout << "one edge is on the boundary" << std::endl;
		Edge middle, boundary;
		middle = (e1.getPLow().v == 0) ? e2 : e1;
		boundary = (middle == e1) ? e2 : e1;
		//std::cout << "middle: " << middle << ", boundary: " << boundary << std::endl;

		unsigned int lower, higher;
		lower = middle.getPLow().v +1;
		higher = points.size() - middle.getPHigh().v;
		//std::cout << "lower: " << lower << std::endl;
		//std::cout << "higher: " << higher << std::endl;

		if (lower < higher) doFlip(0, middle.getPLow().v, polygon, points);
		else doFlip(middle.getPHigh().v, points.size()-1, polygon, points);
	}
	else {
		//std::cout << "both edges are inside boundary" << std::endl;
		Edge lower, higher;
		lower = (e1.getPHigh().v < e2.getPLow().v) ? e1 : e2;
		higher = (lower == e1) ? e2 : e1;
//		std::cout << "lower: " << lower << std::endl;
//		std::cout << "higher: " << higher << std::endl;

		unsigned int inner, outer;
		inner = higher.getPLow().v - lower.getPHigh().v +1;
		outer = (points.size() - higher.getPHigh().v) + lower.getPLow().v +1;
//		std::cout << "inner: " << inner << ", outer: " << outer << std::endl;

		if (inner < outer) doFlip(lower.getPHigh().v, higher.getPLow().v, polygon, points);
		else doFlip(higher.getPHigh().v, lower.getPLow().v, polygon, points);
	}
}

// wrapper around flip for Edge2 class
void flip(Edge2& e1, Edge2& e2, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
	Edge te1 = Edge(e1.p1, e1.p2);
	Edge te2 = Edge(e2.p1, e2.p2);
	flip(te1, te2, polygon, points);
}

// flip a polygon from index: i1 to index: i2.
void doFlip(unsigned int i1, unsigned int i2, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
	//std::cout << "inside doFlip" << std::endl;
//	if (i1 == 15675) {
//		std::cerr << "flipping: i1: " << i1 << ", i2: " << i2 << std::endl;
//		std::cerr << "points[23845]: " << points[23845] << std::endl;
//		std::cerr << "poi[pol[poi[23845]]]: " << points[polygon[points[23845].v]] << std::endl;
//		std::cerr << "points[27925]: " << points[27925] << std::endl;
//		std::cerr << "poi[pol[poi[27925].v]]: " << points[polygon[points[27925].v]] << std::endl;
//	}


	unsigned int t;
	if (i1 < i2) {
		while (i1 < i2) {
//			std::cout << "i1: " << i1 << ", i2: " << i2 << std::endl;
//			std::cout << "p[i1]: " << polygon[i1] << ", p[i2]: " << polygon[i2] << std::endl;
//			std::cout << "p[p[i1]].v: " << points[polygon[i1]].v << ", p[p[i2]].v: " << points[polygon[i2]].v << std::endl;
			t = polygon[i1];
			polygon[i1] = polygon[i2];
			polygon[i2] = t;
//			std::cout << "p[i1]:" << polygon[i1] << ", p[i2]:" << polygon[i2] << std::endl;
			points[polygon[i1]].v = i1;
			points[polygon[i2]].v = i2;
//			std::cout << "p[p[i1]].v:" << points[polygon[i1]].v << ", p[p[i2]].v:" << points[polygon[i2]].v << std::endl;
			++i1;
			--i2;
//			std::cout << "new i1:" << i1 << std::endl;
//			std::cout << "new i2:" << i2 << std::endl;
		}
	}
	else { // if i1 is higher than i2 we flip the outer polygonal chain
		i2 += polygon.size();
		//std::cout << "new i2: " << i2 << std::endl;

		while(i1 < i2) {
			//std::cout << "i1: " << i1 << ", i2: " << i2 << std::endl;;
			//std::cout << "mod of i1: " << (i1 % points.size()) << std::endl;
			//std::cout << "mod of i2: " << (i2 % points.size()) << std::endl;
			//std::cout << "p[i1]: " << polygon[(i1 % points.size())] << ", p[i2]: " << polygon[(i2 % points.size())] << std::endl;
			//std::cout << "p[p[i1]]: " << points[polygon[(i1 % points.size())]].v << ", p[p[i2]]: " << points[polygon[(i2 % points.size())]].v << std::endl;
			t = polygon[(i1 % polygon.size())];
			polygon[(i1 % polygon.size())] = polygon[(i2 % polygon.size())];
			polygon[(i2 % polygon.size())] = t;
			//std::cout << "p[i1]:" << polygon[(i1 % points.size())] << ", p[i2]:" << polygon[(i2 % points.size())] << std::endl;
			points[polygon[(i1 % polygon.size())]].v = i1 % polygon.size();
			points[polygon[(i2 % polygon.size())]].v = i2 % polygon.size();
			//std::cout << "p[p[i1]].v:" << points[polygon[(i1 % points.size())]].v << ", p[p[i2]].v:" << points[polygon[(i2 % points.size())]].v << std::endl;
			++i1;
			--i2;
			//std::cout << "new i1:" << i1 << std::endl;
			//std::cout << "new i2:" << i2 << std::endl;
		}
	}
//	if (i1 == 15675) {
//		std::cerr << "flipping: i1: " << i1 << ", i2: " << i2 << std::endl;
//		std::cerr << "points[23845]: " << points[23845] << std::endl;
//		std::cerr << "poi[pol[poi[23845]]]: " << points[polygon[points[23845].v]] << std::endl;
//		std::cerr << "points[27925]: " << points[27925] << std::endl;
//		std::cerr << "poi[pol[poi[27925].v]]: " << points[polygon[points[27925].v]] << std::endl;
//	}
}

// flip a part of point set 'points' from index: i1 to index: i2 .
void doFlip(unsigned int i1, unsigned int i2, std::vector<Point>& points) {
	//std::cout << "inside doFlip" << std::endl;
	//std::cout << "flipping: i1: " << i1 << ", i2: " << i2 << std::endl;

	Point t;
	if (i1 < i2) {
		while (i1 < i2) {
			//std::cout << "i1: " << i1 << ", i2: " << i2 << std::endl;
			//std::cout << "p[i1]: " << polygon[i1] << ", p[i2]: " << polygon[i2] << std::endl;
			//std::cout << "p[p[i1]].v: " << points[polygon[i1]].v << ", p[p[i2]].v: " << points[polygon[i2]].v << std::endl;
			t = points[i1];
			points[i1] = points[i2];
			points[i2] = t;
			//std::cout << "p[i1]:" << polygon[i1] << ", p[i2]:" << polygon[i2] << std::endl;
			++i1;
			--i2;
			//std::cout << "new i1:" << i1 << std::endl;
			//std::cout << "new i2:" << i2 << std::endl;
		}
	}
	else { // if i1 is higher than i2 we flip the outer polygonal chain
		i2 += points.size();
		//std::cout << "new i2: " << i2 << std::endl;

		while(i1 < i2) {
			//std::cout << "i1: " << i1 << ", i2: " << i2 << std::endl;;
			//std::cout << "mod of i1: " << (i1 % points.size()) << std::endl;
			//std::cout << "mod of i2: " << (i2 % points.size()) << std::endl;
			//std::cout << "p[i1]: " << polygon[(i1 % points.size())] << ", p[i2]: " << polygon[(i2 % points.size())] << std::endl;
			//std::cout << "p[p[i1]]: " << points[polygon[(i1 % points.size())]].v << ", p[p[i2]]: " << points[polygon[(i2 % points.size())]].v << std::endl;
			t = points[(i1 % points.size())];
			points[(i1 % points.size())] = points[(i2 % points.size())];
			points[(i2 % points.size())] = t;
			//std::cout << "p[i1]:" << polygon[(i1 % points.size())] << ", p[i2]:" << polygon[(i2 % points.size())] << std::endl;
			++i1;
			--i2;
			//std::cout << "new i1:" << i1 << std::endl;
			//std::cout << "new i2:" << i2 << std::endl;
		}
	}
}

// Only switches the 2 edges, does not reverse the whole polygonal chain.
void flip2(Edge& e1, Edge& e2, std::vector<unsigned int>& polygon) {
	//std::cout << "inside flip" << std::endl;

	// edges being lex. sorted points, e.p1 is always lower than e.p2, so only the 2 latter points have to be switched.
	Point *temp = e1.p2;
	e1.p2 = e2.p2;
	e2.p2 = temp;

	unsigned int tmp = (*e1.p2).v;
	(*e1.p2).v = (*e2.p2).v;
	(*e2.p2).v = tmp;

	polygon[(*e1.p2).v] = (*e1.p2).i;
	polygon[(*e2.p2).v] = (*e2.p2).i;
}

// simple function to print out a polygon
void poldisplay (std::vector<unsigned int>& p) {
	for (unsigned int i = 0; i < p.size();++i) {
		std::cout << p[i] << " ";
	}
	std::cout  << std::endl;
}

// function to get the angle between an edge and a point, where the origin is one of the points of the edge,
// controlled by the boolean 'use_p1' and the x-axis is along the edge.
double get_angle(Edge e, Point p, bool use_p1) {
	double a1, a2;
	Point p1 = *e.p1, p2 = *e.p2;
	if (use_p1) {
		a1 = atan2(p2.y - p1.y, p2.x - p1.x);
		a2 = atan2(p.y - p1.y, p.x - p1.x);
	}
	else {
		a1 = atan2(p1.y - p2.y, p1.x - p2.x);
		if (a1 < 0) a1 = -PI - a1;
		else a1 = PI - a1;
		a2 = atan2(p.y - p2.y, p.x - p2.x);
		if (a2 < 0) a2 = -PI - a2;
		else a2 = PI - a2;
	}
	// angles must always be [-180°, 180°]
	double sum = a2 - a1;
	if (sum < -PI) sum = sum + 2*PI;
	if (sum > PI) sum = sum - 2*PI;
	return sum;
}

// function to get the angle between an edge and a point, where the origin is one of the points of the edge,
// controlled by the boolean 'use_p1' and the x-axis is along the edge.
double get_angle(E_Edge e, Point p, bool l2r) {
//	std::cerr << "inside edge & point get_angle()" << std::endl;
	double a1, a2;
	Point p1, p2;

	if (l2r) {p1 = *e.p1; p2 = *e.p2;}
	else {p1 = *e.p2; p2 = *e.p1;}

	a1 = atan2(p2.y - p1.y, p2.x - p1.x);
	a2 = atan2(p.y - p1.y, p.x - p1.x);
//	std::cerr << "angle of edge:  " << a1 << ", e: " << e << std::endl;
//	std::cerr << "angle of point: " << a2 << ", p: " << p << std::endl;

	// angles must always be [-180°, 180°]
	double sum = a2 - a1;
//	std::cerr << "a2 - a1: " << sum << std::endl;
	sum = fabs(sum);
	if (l2r) {
		if (signbit(a1) == signbit(a2)) {
			if (a2 > a1 &&  e.bin) sum = 2*PI - sum;
			if (a2 < a1 && !e.bin) sum = 2*PI - sum;
		}
		else {
			if ((a2 > 0) && ( e.bin)) sum = 2*PI - sum;
			if ((a2 < 0) && (!e.bin)) sum = 2*PI - sum;
		}
	}
	else {
		if (signbit(a1) == signbit(a2)) {
			if ((a2 < a1) &&  e.bin) sum = 2*PI - sum;
			if ((a1 < a2) && !e.bin) sum = 2*PI - sum;
		}
		else {
			if ((a2 < 0) && ( e.bin)) sum = 2*PI - sum;
			if ((a2 > 0) && (!e.bin)) sum = 2*PI - sum;
		}
	}

//	std::cerr << "return: " << sum << std::endl;
	return sum;
}

// function to return an angle:
// if l2r is true: return angle (e.p1, f.p1)
// if l2r is false: return angle (e.p2, f.p2)
// INPUT:
//			e: the edge with the origin point of the angle
//			f: the point to get an angle, (l2r) ? f.p1 : f.p2
//			l2r: true if linesweep is sweeping left2right
double get_angle(E_Edge e, E_Edge f, bool l2r, bool dbl_rem) {
//	std::cerr << "inside 2 edge get_angle()" << std::endl;
	double a1, a2;
	Point p1, p2, p;
	if (l2r) {
		p1 = *e.p1; p2 = *e.p2;
		if (dbl_rem) {
			if (*f.p2 == *e.p2) p = *f.p1;
			else p = *f.p2;
		}
		else {
			if (*f.p1 == *e.p1) p = *f.p2;
			else p = *f.p1;
		}
	}
	else {
		p1 = *e.p2; p2 = *e.p1;
		if (dbl_rem) {
			if (*f.p1 == *e.p1) p = *f.p2;
			else p = *f.p1;
		}
		else {
			if (*f.p2 == *e.p2) p = *f.p1;
			else p = *f.p2;
		}
	}

	a1 = atan2(p2.y - p1.y, p2.x - p1.x); // value of the angle from p1 point to p2 point.
	a2 = atan2(p.y - p1.y, p.x - p1.x); // value of angle from p1 point to p point
//	std::cerr << "angle of edge:  " << a1 << ", e: " << e << std::endl;
//	std::cerr << "angle of point: " << a2 << ", p: " << p << std::endl;

	// angles must always be [-180°, 180°]
	double sum = a2 - a1;
//	std::cerr << "a2 - a1: " << sum << std::endl;
	sum = fabs(sum);
	if (l2r) {
		if (a2 > 0 &&  e.bin) sum = 2*PI - sum;
		if (a2 < 0 && !e.bin) sum = 2*PI - sum;
	}
	else {
		if (signbit(a1) == signbit(a2)) {
			if ((a2 < a1) &&  e.bin) sum = 2*PI - sum;
			if ((a1 < a2) && !e.bin) sum = 2*PI - sum;
		}
		else {
			if ((a2 < 0) && ( e.bin)) sum = 2*PI - sum;
			if ((a2 > 0) && (!e.bin)) sum = 2*PI - sum;
		}
	}

//	std::cerr << "return: " << sum << std::endl;
	return sum;
}

// function that returns the smaller angle value between ang('e1',e2.p1) and ang('e1',e2.p2)
// use_p1 : boolean that defines whether the origin is e1.p1 or e1.p2
double get_smaller_angle(E_Edge& e1, E_Edge& e2, bool use_p1) {
//	std::cerr << "=== get_smaller_angle function ===" << std::endl;
  double a1 = get_angle(e1, *e2.p1, use_p1);
  double a2 = get_angle(e1, *e2.p2, use_p1);
//	std::cerr << "a1: " << a1 << ", a2: " << a2 << std::endl;
//	std::cerr << "e1.bin: " << ((e1.bin) ? "true" : "false") << ", use_p1: " << ((use_p1) ? "true" : "false") << std::endl;
//	std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;

	if (e1.bin) {
		if (a1 < 0) {
			if (a2 < 0) return (fabs(a1) < fabs(a2) ? a1 : a2);
			return a1;
		}
		if (a2 < 0) return a2;
//		std::cerr << "Possible error, bin was true, and neither angle was negative." << std::endl;
		return (a1 < a2 ? a1 : a2);
	}
	else {
		if (a1 > 0) {
			if (a2 > 0) return (a1 < a2 ? a1 : a2);
			return a1;
		}
		if (a2 > 0) return a2;
		std::cerr << "Possible error, bin was false, and neither angle was positive." << std::endl;
		return (fabs(a1) < fabs(a2) ? a1 : a2);
	}
}

// function that returns the larger angle value between ang('e1',e2.p1) and ang('e1',e2.p2)
// there is a restriction that the returned angle is of the point on the side of 'e1'
// which 'e1.bin' designates.
// use_p1 : boolean that defines whether the origin is e1.p1 or e1.p2
double get_larger_angle(E_Edge& e1, E_Edge& e2, bool use_p1) {
//	std::cerr << "=== get_larger_angle function ===" << std::endl;
  double a1 = get_angle(e1, *e2.p1, use_p1);
  double a2 = get_angle(e1, *e2.p2, use_p1);
//	std::cerr << "a1: " << a1 << ", a2: " << a2 << std::endl;
//	std::cerr << "e1.bin: " << ((e1.bin) ? "true" : "false") << ", use_p1: " << ((use_p1) ? "true" : "false") << std::endl;
//	std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;

	if (e1.bin) {
		if (a1 < 0) {
			if (a2 < 0) return (fabs(a1) < fabs(a2) ? a2 : a1);
			return a1;
		}
		if (a2 < 0) return a2;
		std::cerr << "Possible error, bin was true, and neither angle was negative." << std::endl;
		return (a1 < a2 ? a2 : a1);
	}
	else {
		if (a1 > 0) {
			if (a2 > 0) return (a1 < a2 ? a2 : a1);
			return a1;
		}
		if (a2 > 0) return a2;
		std::cerr << "Possible error, bin was false, and neither angle was positive." << std::endl;
		return (fabs(a1) < fabs(a2) ? a2 : a1);
	}
}

// Function to modify the 'bin' value of 'e2'
// input: 2 E_Edges that must share exactly one point
// assumption: e1 has a valid 'bin' value
// output: the 'bin' value of 'e2' is modified
bool get_sidedness(E_Edge e1, E_Edge e2) {
  if ((*e1.p1 == *e2.p1) || (*e1.p2 == *e2.p2)) return !e1.bin;
  if ((*e1.p1 == *e2.p2) || (*e1.p2 == *e2.p1)) return e1.bin;
	std::cerr << "Error!  Fallthrough in get_sidedness check!" << std::endl;
	return false;
}
