#include <list>
#include <stdio.h>

#ifndef __TEDGE_H_
#define __TEDGE_H_

class Triangle;
class Vertex;

#include "triangle.h"
#include "vertex.h"

enum class EdgeType {POLYGON, FRAME, TRIANGULATION};

class TEdge {

private:
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

	void setEdgeType(EdgeType tp){
		type = tp;
	}

	EdgeType getEdgeType(){ return type;}

	void setTriangle(Triangle* t);
	int nrAssignedTriangles();

	void print(FILE* f);

	void print();

	Vertex* getV1();
	Vertex* getV2();
};

#endif