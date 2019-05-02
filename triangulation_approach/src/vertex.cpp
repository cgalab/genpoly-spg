#include "vertex.h"

// Constructors
Vertex::Vertex(double X, double Y){ 
	x = X,
	y = Y;
	rectangleVertex = false;

	id = n;
	n++;

	T = NULL;
}

Vertex::Vertex(double X, double Y, bool RV){ 
	x = X,
	y = Y;
	rectangleVertex = RV;

	id = n;
	n++;

	T = NULL;
}

Vertex::Vertex(double X, double Y, int ID){
	x = X;
	y = Y;

	id = ID;

	rectangleVertex = false;

	T = NULL;
}

Vertex* Vertex::getTranslated(double dx, double dy){
	return new Vertex(x + dx, y + dy, id);
}

// Getter
double Vertex::getX(){
	return x;
}

double Vertex::getY(){
	return y;
}

std::list<Triangle*> Vertex::getTriangles(){ 
	return triangles;
}

int Vertex::getID(){
	return id;
}

TEdge* Vertex::getEdgeTo(Vertex* toV){
	int toID = (*toV).getID();
	Vertex* v;

	for(auto const& i : edges){
		v = (*i).getV1();
		if(toID == (*v).getID()) return i;
		v = (*i).getV2();
		if(toID == (*v).getID()) return i;
	}
	//printf("no connection from vertex %d to vertex %d found\n", id, toID);
	return NULL;
}

std::vector<TEdge*> Vertex::getSurroundingEdges(){
	std::vector<TEdge*> out(triangles.size());
	int n = 0;

	for(auto const& i : triangles){
		out[n] = (*i).getEdgeNotContaining(this);
		n++;
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

bool Vertex::isRectangleVertex(){
	return rectangleVertex;
}

double Vertex::getMediumEdgeLength(){
	int n = edges.size();
	double sum = 0;

	for(auto const& i : edges){
		sum = sum + (*i).length();
	}

	return sum / n;
}

double Vertex::getDirectedEdgeLength(double alpha){
	double length;
	int count = 0;

	for(auto const& i : triangles){
		length = (*i).getRange(this, alpha);

		if(length > 0){
			return length;
		}
	}
	
	printf("was not able to find the right triangle\n");
	exit(1);
}

// Setter
void Vertex::setTriangulation(Triangulation* t){
	T = t;
}

void Vertex::setPosition(double X, double Y){
	x = X;
	y = Y;
}

void Vertex::addEdge(TEdge* e){
	edges.push_back(e);
}

void Vertex::addTriangle(Triangle* t){
	triangles.push_back(t);
}

// Remover
void Vertex::removeEdge(TEdge* e){
	edges.remove(e);
}

void Vertex::removeTriangle(Triangle* t){
	triangles.remove(t);
}

//Printer
void Vertex::print(FILE* f){
	double factor = 100.0 / (*T).getNumberOfVertices();
	if(factor < 1)
		factor = 1;

	fprintf(f, "<node positionX=\"%f\" positionY=\"%f\" id=\"%d\" mainText=\"%d\"></node>\n", x * factor, y * factor, id, id);
}

void Vertex::print(){
	printf("Vertex %d at (%f, %f)\n", id, x, y);
}

// Others
void Vertex::check(){
	int n = 0;

	if(!rectangleVertex){
		for(auto const& i : edges){
			if((*i).getEdgeType() == EdgeType::POLYGON) 
				n++;
		}

		if(n != 2){
			printf("Vertex %d has %d polygon edges\n", id, n);
		}
	}
}

// Destructor
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

	if(T != NULL) (*T).removeVertex(id);
}

// static member variables
int Vertex::n = 0;