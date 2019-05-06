#include <stdio.h>
#include <math.h>
#include <string>
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

class TEdge{

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
	
	// Constructors
	TEdge(Vertex* V1, Vertex* V2);
	TEdge(Vertex* V1, Vertex* V2, EdgeType tp);

	// Getter
	int getID();
	EdgeType getEdgeType();
	Vertex* getV1();
	Vertex* getV2();
	Triangle* getT1();
	Triangle* getT2();
	Triangle* getTriangleNotContaining(Vertex* v);
	Triangle* getTriangleContaining(Vertex* v);
	Triangle* getOtherTriangle(Triangle* t);
	Vertex* getOtherVertex(Vertex* v);

	// Setter
	void setTriangulation(Triangulation* t);
	void setEdgeType(EdgeType tp);
	void setTriangle(Triangle* t);

	// Remover
	void removeTriangle(Triangle* t);
	
	// Printer
	void print(FILE* f);
	void print();

	// Others
	double length();	
	bool contains(Vertex* v);
	int nrAssignedTriangles();
	double getAngle(Vertex* v);

	// Destructor
	~TEdge();
};

#endif

enum intersect_t checkIntersection(TEdge* e1, TEdge* e2);
double det(TEdge* e, Vertex* p);
double reldist(TEdge* e, Vertex* p);
Vertex* getIntersectionPoint(Vertex* s0, Vertex* e0, Vertex* s1, Vertex* e1);
double crossProduct2D(double x0, double y0, double x1, double y1);