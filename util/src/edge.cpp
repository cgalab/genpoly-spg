#include <iostream> // for endl
#include <vector>
#include <list>
#include <algorithm>
#include <math.h> // for signbit
#include <assert.h>
#include "basicDefinitions.h"
#include "edge.h"
#include "point.h"
#include "rand.h"

void createRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
	if (randseed) mt.seed(randseed);

	unsigned int i, j, k;
	polygon.resize(points.size());

	for(unsigned int j = 0; j < polygon.size();++j) {
		polygon[j] = points[j].i;
	}

	for (i = polygon.size()-1; i > 0; --i) {
		UniformRandomI(j, 0, i);
		k = polygon[i];
		polygon[i] = polygon[j];
		polygon[j] = k;
		points[polygon[i]].v = i;
	}
	points[polygon[i]].v = i;
}

// returns relative distance of a point to an edge.
double reldist(const Point& pa, const Point& pb, const Point& p) {
	double ans = ((p.x-pa.x)*(pb.x-pa.x) + (p.y-pa.y)*(pb.y-pa.y)) / ((pb.x-pa.x)*(pb.x-pa.x) + (pb.y-pa.y)*(pb.y-pa.y));
	return (abs(ans) < EPSILON) ? 0 : ans;
}
double reldist(const Edge& e, const Point& p) {
	return reldist(*e.p1, *e.p2, p);
}

// calculates the determinant of the vertices of 'e' and the point p
double det(const Edge e, const Point p) {
	const Point& pa = *e.p1;
	const Point& pb = *e.p2;
	double ans = (p.x * (pa.y - pb.y) - p.y * (pa.x-pb.x) + (pa.x*pb.y - pb.x*pa.y));
	return (abs(ans) < EPSILON) ? 0 : ans;
}

double dety(const Edge e, const Point p) {
	Point pa = *e.p1;
	Point pb = *e.p2;
	if ((*e.p1).y > (*e.p2).y) {
		pa = *e.p2;
		pb = *e.p1;
	}

	double ans = (p.x * (pa.y - pb.y) - p.y * (pa.x-pb.x) + (pa.x*pb.y - pb.x*pa.y));
	return (abs(ans) < EPSILON) ? 0 : ans;
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
		if (abs(val) < EPSILON) y.set(0);
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
	double det_a, det_b, det_c, det_d;
	double dp_1, dp_2, dp_3, dp_4;
	bool same11 = false, same12 = false, same21 = false, same22 = false;

	//std::cout << "e1.p1 == e2.p1: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << ", e1.p1: " << *e1.p1 << std::endl;
	//std::cout << "e1.p1 == e2.p2: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << ", e1.p2: " << *e1.p2 << std::endl;
	//std::cout << "e1.p2 == e2.p1: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << ", e2.p1: " << *e2.p1 << std::endl;
	//std::cout << "e1.p2 == e2.p2: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << ", e2.p2: " << *e2.p2 << std::endl;

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
	//std::cerr << "det_a: " << det_a << std::endl;
	//std::cerr << "det_b: " << det_b << std::endl;
	//std::cerr << "det_c: " << det_c << std::endl;
	//std::cerr << "det_d: " << det_d << std::endl;

	if (det_a*det_b*det_c*det_d == 0) {
		bool col = false;

		//quick check if the edges share a vertex
		if (*(e1.p1) == *(e2.p1)) same11 = true;
		if (*(e1.p1) == *(e2.p2)) same12 = true;
		if (*(e1.p2) == *(e2.p1)) same21 = true;
		if (*(e1.p2) == *(e2.p2)) same22 = true;

		// is e1 and e2 the same edge?
		if (e1 == e2) return IS_SAME_EDGE;

		// some determinant was 0, need to check if it's inside an edge or outside.
		dp_1 = reldist(e1, *e2.p1);
		dp_2 = reldist(e1, *e2.p2);
		dp_3 = reldist(e2, *e1.p1);
		dp_4 = reldist(e2, *e1.p2);

		//std::cerr.precision(17);
		//std::cerr << "det_a: " << det_a << ", dp1: " << dp_1 << ", same11: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << std::endl;
		//std::cerr << "det_b: " << det_b << ", dp2: " << dp_2 << ", same12: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << std::endl;
		//std::cerr << "det_c: " << det_c << ", dp3: " << dp_3 << ", same21: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << std::endl;
		//std::cerr << "det_d: " << det_d << ", dp4: " << dp_4 << ", same22: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << std::endl;

				 if ( (det_a == 0) && (dp_1 > 0) && (dp_1 < 1) ) col = true;
		else if ( (det_b == 0) && (dp_2 > 0) && (dp_2 < 1) ) col = true;
		else if ( (det_c == 0) && (dp_3 > 0) && (dp_3 < 1) ) col = true;
		else if ( (det_d == 0) && (dp_4 > 0) && (dp_4 < 1) ) col = true;

		// 2opt function only cares about collinearity when it's 4 point and the points intercept in some way.
		if ((abs(det_a)+abs(det_b)+abs(det_c)+abs(det_d) == 0) && col) return IS_4P_COLLINEAR;
		else if (same11) return IS_VERTEX11;
		else if (same12) return IS_VERTEX12;
		else if (same21) return IS_VERTEX21;
		else if (same22) return IS_VERTEX22;

		else if (col) return IS_3P_COLLINEAR;
		else return IS_FALSE;
	}
	else {
		// none of the determinants were 0, so just need to check the sign for intersection.
		if ( (signbit(det_a) ^ signbit(det_b)) && (signbit(det_c) ^ signbit(det_d)) ) {
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
		//std::cout << "lower: " << lower << std::endl;
		//std::cout << "higher: " << higher << std::endl;

		unsigned int inner, outer;
		inner = higher.getPLow().v - lower.getPHigh().v +1;
		outer = (points.size() - higher.getPHigh().v) + lower.getPLow().v +1;
		//std::cout << "inner: " << inner << ", outer: " << outer << std::endl;

		if (inner < outer) doFlip(lower.getPHigh().v, higher.getPLow().v, polygon, points);
		else doFlip(higher.getPHigh().v, lower.getPLow().v, polygon, points);
	}
}

// flip a polygon from index: i1 to index: i2.
void doFlip(unsigned int i1, unsigned int i2, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
	//std::cout << "inside doFlip" << std::endl;
	//std::cout << "flipping: i1: " << i1 << ", i2: " << i2 << std::endl;

	unsigned int t;
	if (i1 < i2) {
		while (i1 < i2) {
			//std::cout << "i1: " << i1 << ", i2: " << i2 << std::endl;
			//std::cout << "p[i1]: " << polygon[i1] << ", p[i2]: " << polygon[i2] << std::endl;
			//std::cout << "p[p[i1]].v: " << points[polygon[i1]].v << ", p[p[i2]].v: " << points[polygon[i2]].v << std::endl;
			t = polygon[i1];
			polygon[i1] = polygon[i2];
			polygon[i2] = t;
			//std::cout << "p[i1]:" << polygon[i1] << ", p[i2]:" << polygon[i2] << std::endl;
			points[polygon[i1]].v = i1;
			points[polygon[i2]].v = i2;
			//std::cout << "p[p[i1]].v:" << points[polygon[i1]].v << ", p[p[i2]].v:" << points[polygon[i2]].v << std::endl;
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
			t = polygon[(i1 % points.size())];
			polygon[(i1 % points.size())] = polygon[(i2 % points.size())];
			polygon[(i2 % points.size())] = t;
			//std::cout << "p[i1]:" << polygon[(i1 % points.size())] << ", p[i2]:" << polygon[(i2 % points.size())] << std::endl;
			points[polygon[(i1 % points.size())]].v = i1 % points.size();
			points[polygon[(i2 % points.size())]].v = i2 % points.size();
			//std::cout << "p[p[i1]].v:" << points[polygon[(i1 % points.size())]].v << ", p[p[i2]].v:" << points[polygon[(i2 % points.size())]].v << std::endl;
			++i1;
			--i2;
			//std::cout << "new i1:" << i1 << std::endl;
			//std::cout << "new i2:" << i2 << std::endl;
		}
	}
}

// simple function to print out a polygon
void poldisplay (std::vector<unsigned int>& p) {
	for (unsigned int i = 0; i < p.size();++i) {
		std::cout << p[i] << " ";
	}
	std::cout  << std::endl;
}
