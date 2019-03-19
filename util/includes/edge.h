#include <iostream> // for endl
#include <vector>
#include "basicDefinitions.h"
#include "point.h"

#ifndef __EDGE_H_
#define __EDGE_H_

class Edge {
public:
	Point *p1;
	Point *p2;
	unsigned int l_idx; // the lower lexicographical index of the edge

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

	void setVertices(Point* v1, Point* v2){
		p1 = v1;
		p2 = v2;  
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

class lexComp
{
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


void createRandPol(std::vector<unsigned int>& polygon,std::vector<Point>& points);
void lexSort(std::vector<unsigned int>& lex, std::vector<Point>& points);
enum intersect_t checkIntersection(const Edge e1, const Edge e2);
void flip(Edge& e1, Edge& e2, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void doFlip(unsigned int i1, unsigned int i2, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void poldisplay (std::vector<unsigned int>& p);

#endif