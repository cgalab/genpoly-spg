#include "triangle.h"

Triangle::Triangle(TEdge* E0, TEdge* E1, TEdge* E2, Vertex* V0, Vertex* V1, Vertex* V2){ 
	e0 = E0;
	e1 = E1;
	e2 = E2;

	v0 = V0;
	v1 = V1;
	v2 = V2;

	(*e0).setTriangle(this);
	(*e1).setTriangle(this);
	(*e2).setTriangle(this);

	(*v0).addTriangle(this);
	(*v1).addTriangle(this);
	(*v2).addTriangle(this);
}

bool Triangle::contains(Vertex* v){
	int id = (*v).getID();

	if((*v0).getID() == id) return true;
	if((*v1).getID() == id) return true;
	if((*v2).getID() == id) return true;
	return false;
}


void Triangle::print(){
	printf("Triangle:\n");
	(*e0).print();
	(*e1).print();
	(*e2).print();
}