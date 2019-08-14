#include <list>
#include <vector>
#include <stdio.h>
#include <map>
#include <queue>

#define _USE_MATH_DEFINES
#include <math.h>


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

	static unsigned long long n;
	static unsigned long long deleted;
	unsigned long long id;

	// is used for computing signed areas as by copying a vertex the id can not be copied
	// for uncopied vertices the reserveID is 2 * id, for copied vertices it can be 2 * id or 2 * id + 1
	unsigned long long reserveID;

	void setRID(unsigned long long rid);

	void getEnvironment(std::map<int, TEdge*> &es, std::map<int, Vertex*> &vs, int depth);
public:

	// Constructors
	Vertex(double X, double Y);
	Vertex(double X, double Y, bool RV);
	Vertex* getTranslated(double dx, double dy);

	// Getters
	double getX();
	double getY();
	std::list<Triangle*> getTriangles();
	unsigned long long getID();
	unsigned long long getRID();
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
	Triangulation* getTriangulation();
	Triangle* getTriangleWith(Vertex* v0, Vertex* v1);

	// Setters
	void setTriangulation(Triangulation* t);
	void setPosition(double X, double Y);
	void addEdge(TEdge* e);	
	void addTriangle(Triangle* t);
	void setToPrev(TEdge* e);
	void setToNext(TEdge* e);
	void setID(unsigned long long n);

	// Remover
	void removeEdge(TEdge* e);
	void removeTriangle(Triangle* t);
	
	// Printer
	void print(FILE* f, double factor);
	void print();
	void printEnvironment(int depth, const char* filename);
	void printSurroundingTriangulation(const char* filename);
	static void printStats();

	// Others
	bool check();
	void stretch(double factor);
	void checkSurroundingPolygonFast();
	bool checkSurroundingPolygonAdvanced();
	
	// Destructor
	~Vertex();
};

#endif