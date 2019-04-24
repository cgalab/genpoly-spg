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

	int N; // number of polygon vertices in the vertex vector

public:
	
	// Constructors
	Triangulation(int n);

	// Getter
	int getNumberOfVertices();
	Vertex* getVertex(int i);
	Vertex* getPVertex(int i);
	TEdge* getEdge(int index);

	// Setter
	void addVertex(Vertex* v);
	void addEdge(TEdge* e);

	// Remover
	void removeVertex(int i);
	void removeEdge(TEdge* e);

	// Printer	
	void print(const char* filename);
	void printPolygon(const char* filename);

	// Others
	void check();
};

#endif