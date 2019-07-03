#include "triangle.h"

// Constructors
Triangle::Triangle(TEdge* E0, TEdge* E1, TEdge* E2, Vertex* V0, Vertex* V1, Vertex* V2) :
e0(E0), e1(E1), e2(E2), v0(V0), v1(V1), v2(V2), id(n), enqueued(false) { 

	(*e0).setTriangle(this);
	(*e1).setTriangle(this);
	(*e2).setTriangle(this);

	(*v0).addTriangle(this);
	(*v1).addTriangle(this);
	(*v2).addTriangle(this);

	n++;
}

Triangle::Triangle(TEdge* E0, TEdge* E1, TEdge* E2, Vertex* V0, Vertex* V1, Vertex* V2, std::string context, bool &ok) :
e0(E0), e1(E1), e2(E2), v0(V0), v1(V1), v2(V2), id(n), enqueued(false) { 
	Triangle* t;
	bool cont;

	t = (*e0).setTriangle(this, context, ok);
	if(t != NULL){
		cont = (*t).contains(V0);
		cont = cont && (*t).contains(V1);
		cont = cont && (*t).contains(V2);
		if(cont){
			printf("0An equal triangle already exists, context: %s\n", context.c_str());
			printf("vertex IDs: %llu %llu %llu \n", (*V0).getID(), (*V1).getID(), (*V2).getID());
			exit(5);
			ok = false;
		}
	}
	t = (*e1).setTriangle(this, context, ok);
	if(t != NULL){
		cont = (*t).contains(V0);
		cont = cont && (*t).contains(V1);
		cont = cont && (*t).contains(V2);
		if(cont){
			printf("1An equal triangle already exists, context: %s\n", context.c_str());
			printf("vertex IDs: %llu %llu %llu \n", (*V0).getID(), (*V1).getID(), (*V2).getID());
			exit(5);
			ok = false;
		}
	}
	t = (*e2).setTriangle(this, context, ok);
	if(t != NULL){
		cont = (*t).contains(V0);
		cont = cont && (*t).contains(V1);
		cont = cont && (*t).contains(V2);
		if(cont){
			printf("2An equal triangle already exists, context: %s\n", context.c_str());
			printf("vertex IDs: %llu %llu %llu \n", (*V0).getID(), (*V1).getID(), (*V2).getID());
			exit(5);
			ok = false;
		}
	}

	(*v0).addTriangle(this);
	(*v1).addTriangle(this);
	(*v2).addTriangle(this);

	n++;
}

// Getter
unsigned long long Triangle::getID(){
	return id;
}

Vertex* Triangle::getVertex(int index){
	if(index == 0)
		return v0;
	if(index == 1)
		return v1;
	if(index == 2)
		return v2;

	return NULL;	
}

TEdge* Triangle::getEdge(int index){
	if(index == 0)
		return e0;
	if(index == 1)
		return e1;
	if(index == 2)
		return e2;

	return NULL;
}

Vertex* Triangle::getOtherVertex(TEdge* e){
	if(!(*e).contains(v0)) 
		return v0;
	else if(!(*e).contains(v1))
		return v1;
	else
		return v2; 
}

TEdge* Triangle::getEdgeNotContaining(Vertex* v){
	if(!(*e0).contains(v)) return e0;
	if(!(*e1).contains(v)) return e1;
	if(!(*e2).contains(v)) return e2;

	printf("error: all edges are containing the vertex %llu \n", (*v).getID());
	return NULL;
}

std::vector<TEdge*> Triangle::getOtherEdges(TEdge* e){
	std::vector<TEdge*> out(2);
	int index = 0;

	if((*e0).getID() != (*e).getID()){
		out[index] = e0;
		index++;
	}
	if((*e1).getID() != (*e).getID()){
		out[index] = e1;
		index++;
	}
	if((*e2).getID() != (*e).getID()){
		out[index] = e2;
		index++;
	}

	return out;
}

TEdge* Triangle::getLongestEdge(int epsilon){
	double l0 = (*e0).length();
	double l1 = (*e1).length();
	double l2 = (*e2).length();
	double longest, second;
	TEdge *longestE, *secondE;

	if(l0 >= l1 && l0 >= l2){
		longest = l0;
		longestE = e0;

		if(l1 >= l2){
			second = l1;
			secondE = e1;
		}else{
			second = l2;
			secondE = e2;
		}
	}else if(l1 >= l2){
		longest = l1;
		longestE = e1;

		if(l0 >= l2){
			second = l0;
			secondE = e0;
		}else{
			second = l2;
			secondE = e2;
		}
	}else{
		longest = l2;
		longestE = e2;

		if(l0 >= l1){
			second = l0;
			secondE = e0;
		}else{
			second = l1;
			secondE = e1;
		}
	}

	if((*longestE).getEdgeType() == EdgeType::POLYGON){
		if(longest - epsilon <= second)
			longestE = secondE;
	}

	return longestE;
}

// seems to work right and the problem of getting a PE here is caused by an error in the triangulation
TEdge* Triangle::getLongestEdgeAlt(){
	TEdge* e;
	Vertex* v;

	v = getOtherVertex(e0);
	if((*e0).isBetween(v))
		return e0;

	v = getOtherVertex(e1);
	if((*e1).isBetween(v))
		return e1;

	v = getOtherVertex(e2);
	if((*e2).isBetween(v))
		return e2;

	printf("was not able to detect a longest edge by comparison\n");
	printf("triangle area: %.16f \n", signedArea());
	print();
	(*v0).print();
	(*v1).print();
	(*v2).print();
	//(*(*v0).getTriangulation()).print("strang.graphml");
	//exit(1);

	// questionable whether this is really helpful
	return getLongestEdge(0.0001);
}

// assumption: - pi <= alpha <= pi
// returns -1 if the direction is not inside the triangle
double Triangle::getRange(Vertex* v, double alpha){
	TEdge *e, *f, *g;
	double alpha1, alpha2, l;
	Vertex *test;
	bool inside;

	if(!(*e0).contains(v)){
		e = e1;
		f = e2;
	}else if(!(*e1).contains(v)){
		e = e0;
		f = e2;
	}else{
		e = e0;
		f = e1;
	}

	alpha1 = (*e).getAngle(v);
	alpha2 = (*f).getAngle(v);

	if(alpha1 < alpha2){
		l = alpha1;
		alpha1 = alpha2;
		alpha2 = l;

		g = e;
		e = f;
		f = g;
	}

	l = ((*e).length() + (*f).length()) / 2;
	
	// assume: alpha1 >= alpha2
	if(alpha1 - alpha2 <= M_PI){
		if(alpha <= alpha1 && alpha >= alpha2)
			return l;
	}else{
		if(alpha >= alpha1 || alpha <= alpha2)
			return l;
	}

	// alpha is not inside this triangle
	return -1;
}

// Printer
void Triangle::print(){
	printf("Triangle %llu:\n", id);
	(*e0).print();
	(*e1).print();
	(*e2).print();
}

// Others
bool Triangle::contains(Vertex* v){
	unsigned long long id = (*v).getID();

	if((*v0).getID() == id) return true;
	if((*v1).getID() == id) return true;
	if((*v2).getID() == id) return true;
	return false;
}

void Triangle::enqueue(){
	enqueued = true;
}

bool Triangle::isEnqueued(){
	return enqueued;
}

double Triangle::calculateCollapseTime(Vertex* moving, double dx, double dy){
	double ax, ay, bx, by, cx, cy;
	double numerator, denominator;

	if(!contains(moving))
		return -1;

	cx = (*moving).getX();
	cy = (*moving).getY();

	if((*moving).getID() == (*v0).getID()){
		ax = (*v1).getX();
		ay = (*v1).getY();
		bx = (*v2).getX();
		by = (*v2).getY();
	}else if((*moving).getID() == (*v1).getID()){
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

double Triangle::signedArea(){
	double area;
	double ax, ay, bx, by, cx, cy;

	ax = (*v0).getX();
	ay = (*v0).getY();

	bx = (*v1).getX();
	by = (*v1).getY();

	cx = (*v2).getX();
	cy = (*v2).getY();

	area = 0.5 * (- ay * bx + ax * by + ay * cx - by * cx - ax * cy + bx * cy);

	return area;
}

// Destructor
Triangle::~Triangle(){
	(*v0).removeTriangle(this);
	(*v1).removeTriangle(this);
	(*v2).removeTriangle(this);
	
	(*e0).removeTriangle(this);
	(*e1).removeTriangle(this);
	(*e2).removeTriangle(this);
}

// static member variables
unsigned long long Triangle::n = 0;