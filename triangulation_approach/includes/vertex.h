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

class Vertex{

private:
	Triangulation* T;

	double x;
	double y;

	TEdge* toPrev;
	TEdge* toNext;

	std::list<TEdge*> edges;
	std::list<Triangle*> triangles;

	bool rectangleVertex; // determines whether the vertex is part of the rectangle surrounding the polygon

	static int n;
	int id; // the id is always assumed to be equal to the index in the vertex vector of the triangulation

public:

	// Constructors
	Vertex(double X, double Y);
	Vertex(double X, double Y, bool RV);
	Vertex(double X, double Y, int ID);
	Vertex* getTranslated(double dx, double dy);

	// Getters
	double getX();
	double getY();
	std::list<Triangle*> getTriangles();
	int getID();
	TEdge* getEdgeTo(Vertex* v);
	std::vector<TEdge*> getSurroundingEdges();
	std::list<TEdge*> getPolygonEdges();
	bool isRectangleVertex();
	double getMediumEdgeLength();
	double getDirectedEdgeLength(double alpha);
	TEdge* getToPrev();
	TEdge* getToNext();
	Vertex* getPrev();
	Vertex* getNext();

	// Setters
	void setTriangulation(Triangulation* t);
	void setPosition(double X, double Y);
	void addEdge(TEdge* e);	
	void addTriangle(Triangle* t);
	void setToPrev(TEdge* e);
	void setToNext(TEdge* e);

	// Remover
	void removeEdge(TEdge* e);
	void removeTriangle(Triangle* t);
	
	// Printer
	void print(FILE* f);
	void print();

	// Others
	void check();
	void stretch(double factor);
	
	// Destructor
	~Vertex();
};

#endif