#include "vertex.h"

void Vertex::addEdge(TEdge* e){
	edges.push_back(e);
}

void Vertex::addTriangle(Triangle* t){
	triangles.push_back(t);
}

TEdge* Vertex::getEdgeTo(Vertex* toV){
	int id = (*toV).getID();
	Vertex* v;

	for(auto const& i : edges){
		v = (*i).getV1();
		if(id == (*v).getID()) return i;
		v = (*i).getV2();
		if(id == (*v).getID()) return i;
	}

	return NULL;
}

std::list<Triangle*> Vertex::getAllAdjacentTrianglesNotContaining(Vertex* v){
	std::list<Triangle*> out;

	for(auto const& i : triangles){
		if(!(*i).contains(v)) out.push_back(i);
	}

	return out;
}

std::list<TEdge*> Vertex::getPolygonEdges(){
	std::list<TEdge*> out;

	for(auto const& i : edges){
		if((*i).isPEdge()) out.push_back(i);
	}

	return out;
}