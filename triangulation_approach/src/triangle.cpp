#include "triangle.h"

Triangle::Triangle(TEdge* E0, TEdge* E1, TEdge* E2){ 
	e0 = E0;
	e1 = E1;
	e2 = E2;

	(*e0).setTriangle(this); // questionable whether this works the way it should
	(*e1).setTriangle(this);
	(*e2).setTriangle(this);
}