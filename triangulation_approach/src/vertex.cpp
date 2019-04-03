#include "vertex.h"

void Vertex::setTriangulation(Triangulation* t){
	T = t;
}

void Vertex::addEdge(TEdge* e){
	edges.push_back(e);
}

void Vertex::removeEdge(TEdge* e){
	edges.remove(e);
}

void Vertex::addTriangle(Triangle* t){
	triangles.push_back(t);
}

void Vertex::removeTriangle(Triangle* t){
	triangles.remove(t);
}

Triangle* Vertex::getTriangle(int index){
	std::list<Triangle*>::iterator it = triangles.begin();
    std::advance(it, index);

	return *it;
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
		if((*i).getEdgeType() == EdgeType::POLYGON) out.push_back(i);
	}

	return out;
}

Vertex::~Vertex(){
	TEdge *e0, *e1, *e2;
	int nEdges = edges.size();
	int nTriangles = triangles.size();
	int i;
	// make array copy of lists, otherwise we can not iterate over the entries while disconnecting
	TEdge* edgeArray[nEdges];
	std::copy(edges.begin(), edges.end(), edgeArray);
	Triangle* triangleArray[nTriangles];
	std::copy(triangles.begin(), triangles.end(), triangleArray);

	for(i = 0; i < nTriangles; i++){
		delete triangleArray[i];
	}

	for(i = 0; i < nEdges; i++){
		delete edgeArray[i];
	}
}

int Vertex::n = 0;