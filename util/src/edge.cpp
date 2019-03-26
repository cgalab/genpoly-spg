#include <iostream> // for endl
#include <vector>
#include <list>
#include <algorithm>
#include <math.h>
#include "basicDefinitions.h"
#include "edge.h"
#include "point.h"
#include "rand.h"

void createRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
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
double reldist(const Edge& e, const Point& p) {
	const Point& pa = *e.p1;
	const Point& pb = *e.p2;
	return ( (p.x-pa.x)*(pb.x-pa.x) + (p.y-pa.y)*(pb.y-pa.y) / ((pb.x-pa.x)*(pb.x-pa.x) + (pb.y-pa.y)*(pb.y-pa.y)) );
}

double det(const Edge& e, const Point& p) {
	const Point& pa = *e.p1;
	const Point& pb = *e.p2;
	return (p.x * (pa.y - pb.y) - p.y * (pa.x-pb.x) + (pa.x*pb.y - pb.x*pa.y));
}

enum intersect_t checkIntersection(const Edge e1, const Edge e2) {
	double det_a, det_b, det_c, det_d;
	double dp_1, dp_2, dp_3, dp_4;

	//std::cout << "e1.p1 == e2.p1: " << ((*e1.p1 == *e2.p1) ? "true" : "false") << ", e1.p1: " << *e1.p1 << std::endl;
	//std::cout << "e1.p1 == e2.p2: " << ((*e1.p1 == *e2.p2) ? "true" : "false") << ", e1.p2: " << *e1.p2 << std::endl;
	//std::cout << "e1.p2 == e2.p1: " << ((*e1.p2 == *e2.p1) ? "true" : "false") << ", e2.p1: " << *e2.p1 << std::endl;
	//std::cout << "e1.p2 == e2.p2: " << ((*e1.p2 == *e2.p2) ? "true" : "false") << ", e2.p2: " << *e2.p2 << std::endl;

	//quick check if the edges share a vertex
	if ( (*(e1.p1) == *(e2.p1)) || (*(e1.p1) == *(e2.p2)) || (*(e1.p2) == *(e2.p1)) || (*(e1.p2) == *(e2.p2)) )
		return IS_VERTEX;

	// determinant between edge 1 and a point in edge 2
	det_a = det(e1, *e2.p1);
	det_b = det(e1, *e2.p2);
	// determinant between edge 2 and a point in edge 1
	det_c = det(e2, *e1.p1);
	det_d = det(e2, *e1.p2);

	if (det_a*det_b*det_c*det_b == 0) {
		bool col = false; // if true, check for collinearity

		// some determinant was 0, need to check if it's inside an edge or outside.
		dp_1 = reldist(e1, *e2.p1);
		dp_2 = reldist(e1, *e2.p2);
		dp_3 = reldist(e2, *e1.p1);
		dp_4 = reldist(e2, *e1.p2);

		//std::cout << "dp1: " << dp_1 << std::endl;
		//std::cout << "dp2: " << dp_2 << std::endl;
		//std::cout << "dp3: " << dp_3 << std::endl;
		//std::cout << "dp4: " << dp_4 << std::endl;

		if ( (det_a == 0) && (dp_1 > 0) && (dp_1 < 1) )
			col = true;
		else if ( (det_b == 0) && (dp_2 > 0) && (dp_2 < 1) )
			col = true;
		else if ( (det_c == 0) && (dp_3 > 0) && (dp_3 < 1) )
			col = true;
		else if ( (det_d == 0) && (dp_4 > 0) && (dp_4 < 1) )
			col = true;

		if (col && (det_a == 0) && (det_b == 0) && (det_c == 0) && (det_d == 0))
			return IS_COLLINEAR;
		else if (col)
			return IS_TRUE;
		else
			return IS_FALSE;
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
	if ( (((*e1.p1).v ==0) && ((*e1.p2).v == points.size()-1)) || (((*e2.p1).v == 0) && ((*e2.p2).v == points.size()-1)) ) {
		//std::cout << "one edge is on the boundary" << std::endl;
		Edge middle, boundary;
		middle = ((*e1.p1).v == 0) ? e2 : e1;
		boundary = (middle == e1) ? e2 : e1;
		//std::cout << "middle: " << middle << std::endl;
		//std::cout << "boundary: " << boundary << std::endl;

		unsigned int lower, higher;
		lower = (*middle.p1).v +1;
		higher = points.size() - (*middle.p2).v;
		//std::cout << "lower: " << lower << std::endl;
		//std::cout << "higher: " << higher << std::endl;

		if (lower < higher) doFlip(0, (*middle.p1).v, polygon, points);
		else doFlip((*middle.p2).v, points.size()-1, polygon, points);

	}
	else {
		//std::cout << "both edges are inside boundary" << std::endl;
		Edge lower, higher;
		lower = ((*e1.p2).v < (*e2.p1).v) ? e1 : e2;
		higher = (lower == e1) ? e2 : e1;
		//std::cout << "lower: " << lower << std::endl;
		//std::cout << "higher: " << higher << std::endl;

		unsigned int inner, outer;
		inner = (*higher.p1).v - (*lower.p2).v +1;
		outer = (points.size() - (*higher.p2).v) + (*lower.p1).v +1;
		//std::cout << "inner: " << inner << ", outer: " << outer << std::endl;

		if (inner < outer) doFlip((*lower.p2).v, (*higher.p1).v, polygon, points);
		else doFlip((*higher.p2).v, (*lower.p1).v, polygon, points);
	}
}

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