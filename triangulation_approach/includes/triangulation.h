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
	int N;

public:
	Triangulation(int n){ 
		vertices.reserve(n + 4);
		N = n;
	}
	void addVertex(Vertex* v);
	void addEdge(TEdge* e);
	TEdge* getEdge(int index);
	void removeEdge(TEdge* e);
	Vertex* getVertex(int i);
	Vertex* getPVertex(int i);
	void removeVertex(int i);
	void print(const char* filename);
	void printPolygon(const char* filename);
	void check();
	void deleteVertex(int index);
	int getNumberOfVertices(){ return N;}
	void setVertex(int index, Vertex* v){
		vertices[index] = v;
	}
};

#endif