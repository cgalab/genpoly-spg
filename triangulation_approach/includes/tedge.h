#include <list>
#include <stdio.h>

#ifndef __TEDGE_H_
#define __TEDGE_H_

class Triangle;
class Vertex;

#include "triangle.h"
#include "vertex.h"

class TEdge {

private:
	Vertex* v1;
	Vertex* v2;
	Triangle* t1;
	Triangle* t2;
	bool isPEdge; // polygon edge?
	bool isCHEdge; // convex hull edge?

public:
	TEdge(Vertex* V1, Vertex* V2);
	TEdge(Vertex* V1, Vertex* V2, bool PEdge, bool CHEdge);

	void makeEdgePEdge(){
		isPEdge = true;
	}

	void makeEdgeCHEdge(){
		isCHEdge = true;
	}

	void setTriangle(Triangle* t);

	void print(FILE* f);
};

#endif