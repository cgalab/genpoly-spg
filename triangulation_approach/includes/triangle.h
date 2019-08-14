#include <list>
#include <stdio.h>
#include <vector>
#include <string>
#include "predicates.h"

#define _USE_MATH_DEFINES
#include <math.h>

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

	unsigned long long id;
	static unsigned long long n;

	//double det(Vertex *V0, Vertex *V1, Vertex *V2);

public:
	
	// Constructors
	Triangle(TEdge* E0, TEdge* E1, TEdge* E2, Vertex* V0, Vertex* V1, Vertex* V2);
	Triangle(TEdge* E0, TEdge* E1, TEdge* E2, Vertex* V0, Vertex* V1, Vertex* V2, std::string context, bool &ok);
	Triangle(Vertex* V0, Vertex* V1, Vertex* V2);

	// Getter
	unsigned long long getID();
	Vertex* getVertex(int index);
	TEdge* getEdge(int index);
	Vertex* getOtherVertex(TEdge* e);
	TEdge* getEdgeNotContaining(Vertex* v);
	TEdge* getEdgeContaining(Vertex* v);
	TEdge* getOtherEdgeContaining(Vertex* v, TEdge* e);
	std::vector<TEdge*> getOtherEdges(TEdge* e);
	TEdge* getLongestEdge(int epsilon);
	TEdge* getLongestEdgeAlt();
	double getRange(Vertex* v, double alpha);

	// Printer
	void print();

	// Others
	bool contains(Vertex* v);
	void enqueue();
	void dequeue();
	bool isEnqueued();
	double calculateCollapseTime(Vertex* moving, double dx, double dy);
	double signedArea();
	double det(Vertex *V0, Vertex *V1, Vertex *V2);
	
	// Destructor
	~Triangle();	
};
	
#endif