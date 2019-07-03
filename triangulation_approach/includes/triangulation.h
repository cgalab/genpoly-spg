#include <vector>
#include <stdio.h>
#include <map>

#ifndef __TRIANGULATION_H_
#define __TRIANGULATION_H_

class Vertex;
class TEdge;
class Triangle;

#include "vertex.h"
#include "tedge.h"
#include "triangle.h"

class Triangulation{

private:
	std::vector<Vertex*> vertices;
	std::map<int, TEdge*> edges; // maybe an unordered_amp would be even better, but I'm not sure because of deleting elements from hashtables

	// Vertices of the boxing rectangle
	Vertex* Rectangle0;
	Vertex* Rectangle1;
	Vertex* Rectangle2;
	Vertex* Rectangle3;

	int N; // target number of polygon vertices

public:
	
	// Constructors
	Triangulation(int n);

	// Getter
	int getTargetNumberOfVertices();
	int getActualNumberOfVertices();
	Vertex* getVertex(int i);
	TEdge* getEdge(int index);

	// Setter
	void addVertex(Vertex* v);
	void addEdge(TEdge* e);
	void setRectangle(Vertex* v0, Vertex* v1, Vertex* v2, Vertex* v3);

	// Remover
	void removeVertex(int i);
	void removeEdge(TEdge* e);

	// Printer	
	void print(const char* filename);
	void printPolygon(const char* filename);

	// Others
	bool check();
	void stretch(double factor);
	void renumberVertices();
};

#endif