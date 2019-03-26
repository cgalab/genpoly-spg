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
	bool isPE; // polygon edge?
	bool isCHE; // convex hull edge?

public:
	TEdge(Vertex* V1, Vertex* V2);
	TEdge(Vertex* V1, Vertex* V2, bool PEdge, bool CHEdge);

	void makeEdgePEdge(){
		isPE = true;
	}

	void makeEdgeCHEdge(){
		isCHE = true;
	}

	bool isPEdge(){ return isPE;}
	bool isCHEdge(){ return isCHE;}

	void setTriangle(Triangle* t);

	void print(FILE* f);

	void print();

	Vertex* getV1();
	Vertex* getV2();
};

#endif