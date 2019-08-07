#include "vertex.h"

// Constructors
Vertex::Vertex(double X, double Y) : T(NULL), x(X), y(Y), toPrev(NULL), toNext(NULL), rectangleVertex(false), id(n), reserveID(2 * n) {
	n++;
}

Vertex::Vertex(double X, double Y, bool RV) : T(NULL), x(X), y(Y), toPrev(NULL), toNext(NULL), rectangleVertex(RV), id(n), reserveID(2 * n) {
	n++;
}

Vertex* Vertex::getTranslated(double dx, double dy){
	Vertex *v = new Vertex(x + dx, y + dy);

	// a just copied version of the vertex has the same reserveID as the vertex, but a really translated one has an by
	// 1 incremented reserveID
	if(dx != 0 || dy != 0)
		(*v).setRID(2 * id + 1);
	
	return v;
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

unsigned long long Vertex::getID(){
	return id;
}

unsigned long long Vertex::getRID(){
	return reserveID;
}

TEdge* Vertex::getEdgeTo(Vertex* toV){
	unsigned long long toID = (*toV).getID();
	Vertex* v;

	for(auto const& i : edges){
		v = (*i).getV0();
		if(toID == (*v).getID()) return i;
		v = (*i).getV1();
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

	out.push_back(toPrev);
	out.push_back(toNext);

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
	double length, angle;

	for(auto const& i : triangles){
		length = (*i).getRange(this, alpha);

		if(length > 0){
			return length;
		}
	}

	printf("id: %llu alpha: %.16f \n", id, alpha / M_PI * 180);

	for(auto const& i : edges){
		angle = (*i).getAngle(this);

		(*i).print();
		(*(*i).getV0()).print();
		(*(*i).getV1()).print();
		printf("edge alpha: %.20f truely zero: %d \n", angle / M_PI * 180, alpha == 0);
	}
	
	printf("was not able to find the triangle for vertex %llu in direction %f, truely zero: \n", id, alpha);
	/*printSurroundingTriangulation("env.graphml");
	exit(7);*/
	
	return - getMediumEdgeLength();
}

TEdge* Vertex::getToPrev(){
	return toPrev;
}

TEdge* Vertex::getToNext(){
	return toNext;
}

Vertex* Vertex::getPrev(){
	return (*toPrev).getOtherVertex(this);
}

Vertex* Vertex::getNext(){
	return (*toNext).getOtherVertex(this);
}

Triangulation* Vertex::getTriangulation(){
	return T;
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

void Vertex::setToPrev(TEdge* e){
	toPrev = e;
}

void Vertex::setToNext(TEdge* e){
	toNext = e;
}

void Vertex::setID(unsigned long long n){
	id = n;
}

void Vertex::setRID(unsigned long long rid){
	reserveID = rid;
}

// Remover
void Vertex::removeEdge(TEdge* e){
	edges.remove(e);
}

void Vertex::removeTriangle(Triangle* t){
	triangles.remove(t);
}

//Printer
void Vertex::print(FILE* f, double factor){
	int n; 

	if(factor == 0){
		n = (*T).getActualNumberOfVertices();
		if(n < 100)
			factor = 100.0 / (double)n;
		else if(n < 5000)
			factor = 10;
		else
			factor = 20;
	}
	

	fprintf(f, "<node positionX=\"%f\" positionY=\"%f\" id=\"%llu\" mainText=\"%llu\"></node>\n", x * factor, y * factor, id, id);
}

void Vertex::print(){
	printf("Vertex %llu at (%.15f, %.15f)\n", id, x, y);
}

void Vertex::printEnvironment(int depth, const char* filename){
	FILE* f;
	std::map<int, TEdge*> es;
	std::map<int, Vertex*> vs;
	TEdge* e;
	Vertex* v;

	f = fopen(filename, "w");

	getEnvironment(es, vs, depth);

	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<graphml>\n");
	fprintf(f, "<graph id=\"Graph\" edgeDefault=\"undirected\">\n");

	fprintf(f, "<nodes>\n");

	for(auto const& i : vs){
		v = i.second;
		(*v).print(f, 2000);
	}
	fprintf(f, "</nodes>\n");

	fprintf(f, "<edges>\n");
	for(auto const& i : es){
		e = i.second;
		(*e).print(f);
	}
	fprintf(f, "</edges>\n");

	fprintf(f, "</graph>\n");
	fprintf(f, "</graphml>\n");

	fclose(f);
}

void Vertex::getEnvironment(std::map<int, TEdge*> &es, std::map<int, Vertex*> &vs, int depth){
	Vertex* v;

	vs.insert(std::pair<int, Vertex*>(id, this));

	if(depth > 0){
		for(auto const& i : edges){
			es.insert(std::pair<int, TEdge*>((*i).getID(), i));
			v = (*i).getOtherVertex(this);
			(*v).getEnvironment(es, vs, depth - 1);
		}
	}
	
}

void Vertex::printSurroundingTriangulation(const char* filename){
	FILE* f;
	std::map<int, TEdge*> es;
	std::map<int, Vertex*> vs;
	TEdge* e;
	Vertex* v;

	f = fopen(filename, "w");

	vs.insert(std::pair<int, Vertex*>(id, this));

	for(auto const& i : triangles){
		e = (*i).getEdge(0);
		es.insert(std::pair<int, TEdge*>((*e).getID(), e));

		e = (*i).getEdge(1);
		es.insert(std::pair<int, TEdge*>((*e).getID(), e));

		e = (*i).getEdge(2);
		es.insert(std::pair<int, TEdge*>((*e).getID(), e));

		v = (*i).getVertex(0);
		vs.insert(std::pair<int, Vertex*>((*v).getID(), v));

		v = (*i).getVertex(1);
		vs.insert(std::pair<int, Vertex*>((*v).getID(), v));

		v = (*i).getVertex(2);
		vs.insert(std::pair<int, Vertex*>((*v).getID(), v));
	}

	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<graphml>\n");
	fprintf(f, "<graph id=\"Graph\" edgeDefault=\"undirected\">\n");

	fprintf(f, "<nodes>\n");

	for(auto const& i : vs){
		v = i.second;
		(*v).print(f, 3000);
	}
	fprintf(f, "</nodes>\n");

	fprintf(f, "<edges>\n");
	for(auto const& i : es){
		e = i.second;
		(*e).print(f);
	}
	fprintf(f, "</edges>\n");

	fprintf(f, "</graph>\n");
	fprintf(f, "</graphml>\n");

	fclose(f);
}

void Vertex::printStats(){
	printf("created: %llu deleted: %llu still existing: %llu \n", n, deleted, n - deleted);
}

// Others
bool Vertex::check(){
	int n = 0;
	bool ok = true;

	if(!rectangleVertex){
		for(auto const& i : edges){
			if((*i).getEdgeType() == EdgeType::POLYGON) 
				n++;
		}

		if(n != 2){
			printf("Vertex %llu has %d polygon edges\n", id, n);
			ok = false;
		}

		if(toPrev == NULL){
			printf("Edge to previous vertex is missing for vertex %llu \n", id);
			ok = false;
		}

		if(toNext == NULL){
			printf("Edge to next vertex is missing for vertex %llu \n", id);
			ok = false;
		}
	}

	return ok;
}

void Vertex::stretch(double factor){
	x = factor * x;
	y = factor * y;
}

void Vertex::checkSurroundingPolygonFast(){
	bool lowerX = false, higherX = false, lowerY = false, higherY = false;
	Vertex *v;

	for(auto& i : edges){
		v = (*i).getOtherVertex(this);

		if((*v).getX() > x)
			higherX = true;
		else
			lowerX = true;

		if((*v).getY() > y)
			higherY = true;
		else
			lowerY = true;
	}

	if(!(lowerX && lowerY && higherX && higherY)){
		printf("Triangulation error: fromV is outside of its surrounding polygon\n");
		printSurroundingTriangulation("env.graphml");
		exit(6);
	}
}

// i got it:
// the calculation of the event time is that inaccurate that we get problems
// if there is an edge very close to the triangle of the actual event
// so it may appear that by shifting the vertex to the actual time we already
// move it across this edge by accident => the vertex doesn't stay inside its
// surrouding polygon 
bool Vertex::checkSurroundingPolygonAdvanced(){
	std::queue<Vertex*> Q;
	double area0, area;
	Vertex *first, *second;
	Triangle *t, *t0;
	TEdge* e;
    
    // insert all vertices into the Q in the order they form the surrouding polygon
    // the start vertex is is contained a second time at the end of the queue
	t = triangles.front();
	t0 = t;
	e = (*t).getEdgeContaining(this);
	first = (*e).getOtherVertex(this);
	Q.push(first);

	e = (*t).getOtherEdgeContaining(this, e);
	first = (*e).getOtherVertex(this);
	Q.push(first);

	t = (*e).getOtherTriangle(t);
	while((*t).getID() != (*t0).getID()){
		e = (*t).getOtherEdgeContaining(this, e);
		first = (*e).getOtherVertex(this);
		Q.push(first);

		t = (*e).getOtherTriangle(t);
	}

	// compute the area of the first triangle outside of the loop to get the right sign
	first = Q.front();
	Q.pop();
	second = Q.front();
	Q.pop();

	t = new Triangle(first, second, this);
	area0 = (*t).signedArea();
	delete t;

	if(area0 == 0)
		printf("surrouding polygon check: area is exactly 0!\n");

	while(!Q.empty()){
		first = second;
		second = Q.front();
		Q.pop();
        
		t = new Triangle(first, second, this);
		area = (*t).signedArea();
		delete t;

		if(area == 0){
			printf("surrouding polygon check: area is exactly 0!\n");
			continue;
		}

		// compare orientation with the oriantation of the first triangle
		if(signbit(area) != signbit(area0)){
			// in the case that the first area was exactly zero we can take the next area as first area
			if(area0 == 0){
				area0 = area;
				continue;
			}

			return false;
		}
	}

	return true;
}

// Destructor
Vertex::~Vertex(){
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

	deleted++;
}

// static member variables
unsigned long long Vertex::n = 0;
unsigned long long Vertex::deleted = 0;
