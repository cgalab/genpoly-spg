#include <list>
#include <stdio.h>
#include <vector>

#ifndef __TRIANGLE_H_
#define __TRIANGLE_H_

class TEdge;
class Vertex;

#include "tedge.h"
#include "vertex.h"

class Triangle {

private:
	TEdge* e0;
	TEdge* e1;
	TEdge* e2;

	Vertex* v0;
	Vertex* v1;
	Vertex* v2;

	int id;
	static int n;

public:
	Triangle(TEdge* E0, TEdge* E1, TEdge* E2, Vertex* V0, Vertex* V1, Vertex* V2);
	void print();
	bool contains(Vertex* v);
	int getID(){ return id;}
	~Triangle();
	TEdge* getEdge(int i);
	TEdge* getEdgeNotContaining(Vertex* v);
	std::vector<TEdge*> getOtherEdges(TEdge* e);

};

#endif