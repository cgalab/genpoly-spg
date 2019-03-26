#include <list>
#include <stdio.h>


#ifndef __VERTEX_H_
#define __VERTEX_H_

class TEdge;
class Triangle;

#include "tedge.h"
#include "triangle.h"

class Vertex {

private:
	double x;
	double y;
	int id;
	std::list<TEdge*> edges;
	std::list<Triangle*> triangles;

public:
	Vertex(double X, double Y, int ID){ 
		x = X,
		y = Y;
		id = ID;
	}

	void addEdge(TEdge* e);
	void addTriangle(Triangle* t);

	void print(FILE* f){
		fprintf(f, "<node positionX=\"%f\" positionY=\"%f\" id=\"%d\" mainText=\"%d\"></node>\n", x * 20, y * 20, id, id);
	}

	int getID(){ return id;}

	Vertex* getTranslated(double dx, double dy){
		return new Vertex(x + dx, y + dy, id);
	}

	TEdge* getEdgeTo(Vertex* v);

	std::list<TEdge*> getAdjacentEdges(){
		return edges;
	}

	std::list<Triangle*> getAllAdjacentTrianglesNotContaining(Vertex* v);

	std::list<TEdge*> getPolygonEdges();
};

#endif