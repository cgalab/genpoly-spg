#include "triangle.h"

// Constructors
Triangle::Triangle(TEdge* E0, TEdge* E1, TEdge* E2, Vertex* V0, Vertex* V1, Vertex* V2) :
e0(E0), e1(E1), e2(E2), v0(V0), v1(V1), v2(V2), enqueued(false), id(n) {

	(*e0).setTriangle(this);
	(*e1).setTriangle(this);
	(*e2).setTriangle(this);

	(*v0).addTriangle(this);
	(*v1).addTriangle(this);
	(*v2).addTriangle(this);

	n++;
}

Triangle::Triangle(TEdge* E0, TEdge* E1, TEdge* E2, Vertex* V0, Vertex* V1, Vertex* V2, std::string context, bool &ok) :
e0(E0), e1(E1), e2(E2), v0(V0), v1(V1), v2(V2), enqueued(false), id(n) {
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

// Attention: this triangles are just for testing
Triangle::Triangle(Vertex* V0, Vertex* V1, Vertex* V2) :
e0(NULL), e1(NULL), e2(NULL), v0(V0), v1(V1), v2(V2), enqueued(false), id(n) {

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

// gets one of the two edges containing vertex v
// which one depends just on the ordering in the triangle
TEdge* Triangle::getEdgeContaining(Vertex* v){
	if((*e0).contains(v)) return e0;
	if((*e1).contains(v)) return e1;
	if((*e2).contains(v)) return e2;

	printf("error: none of the edges is containing the vertex %llu \n", (*v).getID());
	return NULL;
}


// gets the edge of the triangle which contains v, but is not e
TEdge* Triangle::getOtherEdgeContaining(Vertex* v, TEdge* e){
	if((*e).getID() != (*e0).getID() && (*e0).contains(v)) return e0;
	if((*e).getID() != (*e1).getID() && (*e1).contains(v)) return e1;
	if((*e).getID() != (*e2).getID() && (*e2).contains(v)) return e2;

	printf("error: no other edge is containing the vertex %llu \n", (*v).getID());
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

	// questionable whether this is really helpful
	return getLongestEdge(0.0001);
}

// assumption: - pi <= alpha <= pi
// returns -1 if the direction is not inside the triangle
double Triangle::getRange(Vertex* v, double alpha){
	TEdge *e, *f, *g;
	double alpha1, alpha2, l;

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
	(*v0).print();
	(*v1).print();
	(*v2).print();
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

void Triangle::dequeue(){
	enqueued = false;
}

bool Triangle::isEnqueued(){
	return enqueued;
}

double Triangle::calculateCollapseTime(Vertex* moving, double dx, double dy){
	double ax, ay, bx, by, cx, cy;
	double areaOld, areaNew, portion;

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

	// shift a to zero
	bx = bx - ax;
	by = by - ay;

	cx = cx - ax;
	cy = cy - ay;

	dx = cx + dx;
	dy = cy + dy;

	areaOld = cx * by - cy * bx;
	areaNew = bx * dy - by * dx;

	portion = areaNew / areaOld;

	return 1 / (portion + 1);
}

/*double Triangle::signedArea(){
	point p0, p1, p2;

	p0.x = (*v0).getX();
	p0.y = (*v0).getY();

	p1.x = (*v1).getX();
	p1.y = (*v1).getY();

	p2.x = (*v2).getX();
	p2.y = (*v2).getY();

	return orient2d(p0, p1, p2);
}*/

double Triangle::signedArea(){
	unsigned long long id0, id1, id2;
	double area;

	id0 = (*v0).getRID();
	id1 = (*v1).getRID();
	id2 = (*v2).getRID();

	if(id0 < id1 && id0 < id2){
		if(id1 < id2)
			area = det(v0, v1, v2);
		else
			area = - det(v0, v2, v1);
	}else if(id1 < id0 && id1 < id2){
		if(id0 < id2)
			area = - det(v1, v0, v2);
		else
			area = det(v1, v2, v0);
	}else{
		if(id0 < id1)
			area = det(v2, v0, v1);
		else
			area = - det(v2, v1, v0);
	}

	return area;
}

// assert: V0.id < V1.id < V2.id
double Triangle::det(Vertex *V0, Vertex *V1, Vertex *V2){
	double area;
	double ax, ay, bx, by, cx, cy;

	ax = (*V0).getX();
	ay = (*V0).getY();

	bx = (*V1).getX();
	by = (*V1).getY();

	cx = (*V2).getX();
	cy = (*V2).getY();

	area = ay * (cx - bx) + by * (ax - cx) + cy * (bx - ax);

	return area;
}

// Destructor
Triangle::~Triangle(){

	(*v0).removeTriangle(this);
	(*v1).removeTriangle(this);
	(*v2).removeTriangle(this);

	// some test triangles may not have assigned edges
	if(e0 != NULL)
		(*e0).removeTriangle(this);
	if(e0 != NULL)
		(*e1).removeTriangle(this);
	if(e0 != NULL)
		(*e2).removeTriangle(this);
}

// static member variables
unsigned long long Triangle::n = 0;
