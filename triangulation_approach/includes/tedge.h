#include <list>
#include <stdio.h>
#include <math.h> 
#include "basicDefinitions.h"

#ifndef __TEDGE_H_
#define __TEDGE_H_

class Triangulation;
class Triangle;
class Vertex;

#include "triangulation.h"
#include "triangle.h"
#include "vertex.h"

enum class EdgeType {POLYGON, FRAME, TRIANGULATION};

class TEdge {

private:
	Triangulation* T;

	Vertex* v1;
	Vertex* v2;

	Triangle* t1;
	Triangle* t2;

	EdgeType type;

	int id;
	static int n;

public:
	TEdge(Vertex* V1, Vertex* V2);
	TEdge(Vertex* V1, Vertex* V2, EdgeType tp);

	void setTriangulation(Triangulation* t);

	void setEdgeType(EdgeType tp){
		type = tp;
	}

	EdgeType getEdgeType(){ return type;}

	void setTriangle(Triangle* t);
	void removeTriangle(Triangle* t);
	int nrAssignedTriangles();

	void print(FILE* f);

	void print();

	Vertex* getV1();
	Vertex* getV2();
	bool contains(Vertex* v);

	

	~TEdge();
};

#endif

enum intersect_t checkIntersection(TEdge* e1, TEdge* e2);
double det(TEdge* e, Vertex* p);
double reldist(TEdge* e, Vertex* p); 