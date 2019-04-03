#include <vector>
#include <stdio.h>

#ifndef __TRIANGULATION_H_
#define __TRIANGULATION_H_

class Vertex;
class TEdge;
class Triangle;

#include "vertex.h"
#include "tedge.h"
#include "triangle.h"

class Triangulation {

private:
	std::vector<Vertex*> vertices;
	std::list<TEdge*> edges;

public:
	Triangulation(int n){ vertices.reserve(n); }
	void addVertex(Vertex* v);
	void addEdge(TEdge* e);
	TEdge* getEdge(int index);
	void removeEdge(TEdge* e);
	Vertex* getVertex(int i);
	void removeVertex(int i);
	void print(char* filename);
	void check();
};

#endif