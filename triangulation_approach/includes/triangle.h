#include <list>
#include <stdio.h>
#include <vector>

#ifndef __TRIANGLE_H_
#define __TRIANGLE_H_

class TEdge;
class Vertex;

#include "tedge.h"
#include "vertex.h"

class Triangle{

private:
	TEdge* e0;
	TEdge* e1;
	TEdge* e2;

	Vertex* v0;
	Vertex* v1;
	Vertex* v2;

	bool enqueued; // indicates whether the triangle is in the actual event queue

	int id;
	static int n;

public:
	
	// Constructors
	Triangle(TEdge* E0, TEdge* E1, TEdge* E2, Vertex* V0, Vertex* V1, Vertex* V2);

	// Getter
	int getID();
	Vertex* getOtherVertex(TEdge* e);
	TEdge* getEdgeNotContaining(Vertex* v);
	std::vector<TEdge*> getOtherEdges(TEdge* e);
	TEdge* getLongestEdge();
	double getRange(Vertex* v, double alpha);

	// Printer
	void print();

	// Others
	bool contains(Vertex* v);
	void enqueue();
	bool isEnqueued();
	double calculateCollapseTime(Vertex* moving, double dx, double dy);
	bool isInside(Vertex* v);
	
	// Destructor
	~Triangle();	
};
	
#endif