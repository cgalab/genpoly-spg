#include "tedge.h"
#include <string>

TEdge::TEdge(Vertex* V1, Vertex* V2, EdgeType tp){ 
	v1 = V1;
	v2 = V2;

	t1 = NULL;
	t2 = NULL;

	type = tp;

	(*V1).addEdge(this);
	(*V2).addEdge(this);

	id = n;
	n++;
}

TEdge::TEdge(Vertex* V1, Vertex* V2){ 
	v1 = V1;
	v2 = V2;

	t1 = NULL;
	t2 = NULL;

	type = EdgeType::TRIANGULATION;

	(*V1).addEdge(this);
	(*V2).addEdge(this);

	id = n;
	n++;
}

void TEdge::setTriangulation(Triangulation* t){
	T = t;
}

void TEdge::setTriangle(Triangle* t){
	if(t1 == NULL) t1 = t;
	else if(t2 == NULL) t2 = t;
	else printf("This edge already has two triangles\n");
}

void TEdge::removeTriangle(Triangle* t){

	if(t1 == t)
	 	t1 = NULL;
	else if(t2 == t) 
		t2 = NULL;
	else 
		printf("Removed triangle was not adjacent to edge from vertex %d to vertex %d \n", (*v1).getID(), (*v2).getID());
}

int TEdge::nrAssignedTriangles(){
	int n = 0;

	if(t1 != NULL) n++;
	if(t2 != NULL) n++;

	return n;
}

void TEdge::print(FILE* f){
	int w = 0;
	if(t1 != NULL) w++;
	if(t2 != NULL) w++;

	if(type == EdgeType::POLYGON) w = 5;
	if(type == EdgeType::FRAME) w = 10;
	fprintf(f, "<edge vertex1=\"%d\" vertex2=\"%d\" weight=\"%d\" useWeight=\"true\"></edge>\n", (*v1).getID(), (*v2).getID(), w);
}

void TEdge::print(){
	std::string tp;

	if(type == EdgeType::POLYGON) tp = "POLYGON";
	else if(type == EdgeType::FRAME) tp = "FRAME";
	else tp = "TRIANGULATION";

	printf("Edge %d from point %d to point %d of type %s \n", id, (*v1).getID(), (*v2).getID(), tp.c_str());
}

Vertex* TEdge::getV1(){
	return v1;
}

Vertex* TEdge::getV2(){
	return v2;
}

bool TEdge::contains(Vertex* v){
	if((*v).getID() == (*v1).getID()) return true;
	if((*v).getID() == (*v2).getID()) return true;
	return false;
}

// Attention: don't remove edges before there triangles are removed
TEdge::~TEdge(){
	(*v1).removeEdge(this);
	(*v2).removeEdge(this);

	(*T).removeEdge(this);
}

int TEdge::n = 0;
