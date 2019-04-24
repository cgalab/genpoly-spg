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
	int id; // the id is always assumed to be equal to the index in the vertex vector of the triangulation

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

		T = NULL;
	}

	Vertex(double X, double Y, bool RV){ 
		x = X,
		y = Y;
		rectangleVertex = RV;

		id = n;
		n++;

		T = NULL;
	}

	Vertex(double X, double Y, int ID){
		x = X;
		y = Y;

		id = ID;

		rectangleVertex = false;

		T = NULL;
	}

	void setTriangulation(Triangulation* t);

	double getX(){
		return x;
	}

	double getY(){
		return y;
	}

	std::list<Triangle*> getTriangles(){ return triangles;}

	void addEdge(TEdge* e);
	void removeEdge(TEdge* e);
	void addTriangle(Triangle* t);
	void removeTriangle(Triangle* t);
	Triangle* getTriangle(int index);

	void setPosition(double X, double Y){
		x = X;
		y = Y;
	}

	void print(FILE* f){
		fprintf(f, "<node positionX=\"%f\" positionY=\"%f\" id=\"%d\" mainText=\"%d\"></node>\n", x * 50, y * 50, id, id);
	}

	void print(){
		printf("Vertex %d at (%f, %f)\n", id, x, y);
	}

	int getID(){ return id;}
	bool isRectangleVertex(){ return rectangleVertex;}

	Vertex* getTranslated(double dx, double dy){
		// TODO: keep id
		return new Vertex(x + dx, y + dy, id);
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