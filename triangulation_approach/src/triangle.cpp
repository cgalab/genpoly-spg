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

	id = n;
	n++;

	isEnqueued = false;
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

TEdge* Triangle::getEdge(int i){
	if(i == 0)
		return e0;
	else if(i == 1)
		return e1;
	else if(i == 2)
		return e2;
	else
		printf("error: index must be 0, 1 or 2\n");
	return NULL;
}

TEdge* Triangle::getEdgeNotContaining(Vertex* v){
	if(!(*e0).contains(v)) return e0;
	if(!(*e1).contains(v)) return e1;
	if(!(*e2).contains(v)) return e2;

	printf("error: all edges are containing the vertex %d \n", (*v).getID());
	return NULL;
}

std::vector<TEdge*> Triangle::getOtherEdges(TEdge* e){
	std::vector<TEdge*> out(2);
	int index = 0;

	if(e0 != e){
		out[index] = e0;
		index++;
	}
	if(e1 != e){
		out[index] = e1;
		index++;
	}
	if(e2 != e){
		out[index] = e2;
		index++;
	}

	return out;
}

double Triangle::calculateCollapseTime(Vertex* moving, double dx, double dy){
	double ax, ay, bx, by, cx, cy;
	double numerator, denominator;

	cx = (*moving).getX();
	cy = (*moving).getY();

	if(moving == v0){
		ax = (*v1).getX();
		ay = (*v1).getY();
		bx = (*v2).getX();
		by = (*v2).getY();
	}else if(moving == v1){
		ax = (*v0).getX();
		ay = (*v0).getY();
		bx = (*v2).getX();
		by = (*v2).getY();
	}else{
		ax = (*v0).getX();
		ay = (*v0).getY();
		bx = (*v1).getX();
		by = (*v1).getY();
	}

	numerator = bx * ay - ax * by - cx * (ay - by) - cy * (bx - ax);
	denominator = dx * (ay - by) + dy * (bx - ax);

	return numerator / denominator;
}

TEdge* Triangle::getLongestEdge(){
	double l0 = (*e0).length();
	double l1 = (*e1).length();
	double l2 = (*e2).length();

	if(l0 >= l1 && l0 >= l2)
		return e0;
	else if(l1 >= l2)
		return e1;
	else
		return e2;

	return 0;
}

Triangle::~Triangle(){
	(*v0).removeTriangle(this);
	(*v1).removeTriangle(this);
	(*v2).removeTriangle(this);
	
	(*e0).removeTriangle(this);
	(*e1).removeTriangle(this);
	(*e2).removeTriangle(this);
}

int Triangle::n = 0;