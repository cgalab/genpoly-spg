#include <list>
#include <vector>
#include <stdio.h>


#ifndef __VERTEX_H_
#define __VERTEX_H_

class Triangulation;
class TEdge;
class Triangle;

#include "triangulation.h"
#include "tedge.h"
#include "triangle.h"

class Vertex {

private:
	Triangulation* T;

	double x;
	double y;

	static int n;
	int id;

	std::list<TEdge*> edges;
	std::list<Triangle*> triangles;

	bool rectangleVertex;

public:
	Vertex(double X, double Y){ 
		x = X,
		y = Y;
		rectangleVertex = false;

		id = n;
		n++;
	}

	Vertex(double X, double Y, bool RV){ 
		x = X,
		y = Y;
		rectangleVertex = RV;

		id = n;
		n++;
	}

	void setTriangulation(Triangulation* t);

	double getX(){
		return x;
	}

	double getY(){
		return y;
	}

	void addEdge(TEdge* e);
	void removeEdge(TEdge* e);
	void addTriangle(Triangle* t);
	void removeTriangle(Triangle* t);
	Triangle* getTriangle(int index);

	void print(FILE* f){
		fprintf(f, "<node positionX=\"%f\" positionY=\"%f\" id=\"%d\" mainText=\"%d\"></node>\n", x * 10, y * 10, id, id);
	}

	void print(){
		printf("Vertex %d at (%f, %f)\n", id, x, y);
	}

	int getID(){ return id;}
	bool isRectangleVertex(){ return rectangleVertex;}

	Vertex* getTranslated(double dx, double dy){
		return new Vertex(x + dx, y + dy);
	}

	TEdge* getEdgeTo(Vertex* v);

	std::list<TEdge*> getAdjacentEdges(){
		return edges;
	}

	std::vector<TEdge*> getSurroundingEdges();

	std::list<TEdge*> getPolygonEdges();

	~Vertex();
};

#endif