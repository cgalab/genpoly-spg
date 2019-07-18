#include <stdio.h>
#include <math.h>
#include <string>
#include "basicDefinitions.h"

#ifndef __INTERSECTTYPE_
#define __INTERSECTTYPE_

enum class IntersectionType {NONE, EDGE, VERTEX};

#endif

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

	Vertex* v0;
	Vertex* v1;

	Triangle* t0;
	Triangle* t1;

	EdgeType type;

	unsigned long long id;
	static unsigned long long n;

public:
	
	// Constructors
	TEdge(Vertex* V0, Vertex* V1);
	TEdge(Vertex* V0, Vertex* V1, EdgeType tp);

	// Getter
	unsigned long long getID();
	EdgeType getEdgeType();
	Vertex* getV0();
	Vertex* getV1();
	Triangle* getT0();
	Triangle* getT1();
	Triangle* getTriangleNotContaining(Vertex* v);
	Triangle* getTriangleContaining(Vertex* v);
	Triangle* getOtherTriangle(Triangle* t);
	Vertex* getOtherVertex(Vertex* v);

	// Setter
	void setTriangulation(Triangulation* t);
	void setEdgeType(EdgeType tp);
	void setTriangle(Triangle* t);
	Triangle* setTriangle(Triangle* t, std::string context, bool &ok);

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
	bool isBetween(Vertex* v);

	// Destructor
	~TEdge();
};

#endif

enum IntersectionType checkIntersection(TEdge* e0, TEdge* e1, const double epsilon);
Vertex* getIntersectionPoint(Vertex* s0, Vertex* e0, Vertex* s1, Vertex* e1);
double crossProduct2D(double x0, double y0, double x1, double y1);
