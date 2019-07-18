#include "tedge.h"

// Constructors
TEdge::TEdge(Vertex* V0, Vertex* V1, EdgeType tp) : T(NULL), v0(V0), v1(V1), 
t0(NULL), t1(NULL), type(tp), id(n) {
	
	if(type == EdgeType::POLYGON){
		(*v0).setToNext(this);
		(*v1).setToPrev(this);
	}

	(*v0).addEdge(this);
	(*v1).addEdge(this);

	n++;

	if(v0 == NULL || v1 == NULL || (*v0).getID() == (*v1).getID()){
		printf("error circle edge\n");
		exit(1);
	}
}

TEdge::TEdge(Vertex* V0, Vertex* V1) : T(NULL), v0(V0), v1(V1),
t0(NULL), t1(NULL), type(EdgeType::TRIANGULATION), id(n) { 
	
	(*v0).addEdge(this);
	(*v1).addEdge(this);

	n++;

	if(v0 == NULL || v1 == NULL || (*v0).getID() == (*v1).getID()){
		printf("error circle edge\n");
		exit(1);
	}
}

// Getter
unsigned long long TEdge::getID(){
	return id;
}

EdgeType TEdge::getEdgeType(){
	return type;
}

Vertex* TEdge::getV0(){
	return v0;
}

Vertex* TEdge::getV1(){
	return v1;
}

Triangle* TEdge::getT0(){
	return t0;
}

Triangle* TEdge::getT1(){
	return t1;
}

Triangle* TEdge::getTriangleNotContaining(Vertex* v){
	if((*t0).contains(v)) return t1;
	else return t0;
}

Triangle* TEdge::getTriangleContaining(Vertex* v){
	if((*t0).contains(v)) return t0;
	else return t1;
}

Triangle* TEdge::getOtherTriangle(Triangle* t){
	if((*t).getID() == (*t0).getID()) return t1;
	else return t0;
}

Vertex* TEdge::getOtherVertex(Vertex* v){
	if((*v).getID() == (*v0).getID())
		return v1;
	else
		return v0;
}

// Setter
void TEdge::setTriangulation(Triangulation* t){
	T = t;
}

void TEdge::setEdgeType(EdgeType tp){
	type = tp;

	if(tp == EdgeType::POLYGON){
		(*v0).setToNext(this);
		(*v1).setToPrev(this);
	}
}

void TEdge::setTriangle(Triangle* t){
	if(t0 == NULL) t0 = t;
	else if(t1 == NULL) t1 = t;
	else{
		printf("The edge from vertex %llu to vertex %llu already has two triangles!\n", (*v0).getID(), (*v1).getID());
		exit(4);
	}

	if(t0 != NULL && t1 != NULL && (*t0).getID() == (*t1).getID()){
		printf("The edge from vertex %llu to vertex %llu has already registered the same triangle! \n", (*v0).getID(), (*v1).getID());
		exit(5);
	}	
}

Triangle* TEdge::setTriangle(Triangle* t, std::string context, bool &ok){
	if(t0 == NULL){
		t0 = t;
		return t1;
	}else if(t1 == NULL){
		t1 = t;
		return t0;
	}else{
		printf("The edge from vertex %llu to vertex %llu already has two triangles!\n", (*v0).getID(), (*v1).getID());
		printf("context: %s \n", context.c_str());
		ok = false;
	}
	return NULL;

	if((*t0).getID() == (*t1).getID()){
		printf("The edge from vertex %llu to vertex %llu has already registered the same triangle! \n", (*v0).getID(), (*v1).getID());
		printf("context: %s \n", context.c_str());
		print();
		printf("existing t0\n");
		(*t0).print();
		printf("existing t1\n");
		(*t1).print();
		printf("new t\n");
		(*t).print();
		printf("end\n");
		ok = false;
	}	
}

// Remover
void TEdge::removeTriangle(Triangle* t){

	if(t0 != NULL && (*t0).getID() == (*t).getID())
	 	t0 = NULL;
	else if(t1 != NULL && (*t1).getID() == (*t).getID()) 
		t1 = NULL;
	else 
		printf("Removed triangle was not adjacent to edge from vertex %llu to vertex %llu \n", (*v0).getID(), (*v1).getID());
}

// Printer
void TEdge::print(FILE* f){
	int w = 0;
	if(t0 != NULL) w++;
	if(t1 != NULL) w++;

	if(type == EdgeType::POLYGON) w = 5;
	if(type == EdgeType::FRAME) w = 10;
	fprintf(f, "<edge vertex1=\"%llu\" vertex2=\"%llu\" weight=\"%d\" useWeight=\"true\"></edge>\n", (*v0).getID(), (*v1).getID(), w);
}

void TEdge::print(){
	std::string tp;

	if(type == EdgeType::POLYGON) tp = "POLYGON";
	else if(type == EdgeType::FRAME) tp = "FRAME";
	else tp = "TRIANGULATION";

	printf("Edge %llu from vertex %llu to vertex %llu of type %s \n", id, (*v0).getID(), (*v1).getID(), tp.c_str());
}

// Others
double TEdge::length(){
	double x0, x1, y0, y1;

	x0 = (*v0).getX();
	y0 = (*v0).getY();

	x1 = (*v1).getX();
	y1 = (*v1).getY();

	return sqrt(pow((x0 - x1), 2) + pow((y0 - y1), 2));	
}

bool TEdge::contains(Vertex* v){
	if((*v).getID() == (*v0).getID()) return true;
	if((*v).getID() == (*v1).getID()) return true;
	return false;
}

int TEdge::nrAssignedTriangles(){
	int n = 0;

	if(t0 != NULL) n++;
	if(t1 != NULL) n++;

	return n;
}

// computes angle between edge and x-axis in range between -pi and pi
// starting point is v
double TEdge::getAngle(Vertex* v){
	double x0, y0, x1, y1, dx, dy;
	double cosa, alpha;

	if((*v).getID() == (*v0).getID()){
		x0 = (*v0).getX();
		y0 = (*v0).getY();
		x1 = (*v1).getX();
		y1 = (*v1).getY();
	}else{
		x0 = (*v1).getX();
		y0 = (*v1).getY();
		x1 = (*v0).getX();
		y1 = (*v0).getY();
	}

	dx = x1 - x0;
	dy = y1 - y0;

	cosa = dx / sqrt(pow((dx), 2) + pow((dy), 2));

	alpha = acos(cosa);

	if(dy < 0)
		alpha = - alpha;

	if(alpha == 0 && dx < 0)
		alpha = M_PI;


	return alpha;
}

bool TEdge::isBetween(Vertex* v){
	double v0x, v1x, v0y, v1y, dx, dy, z;

	v0x = (*v0).getX();
	v0y = (*v0).getY();
	v1x = (*v1).getX();
	v1y = (*v1).getY();

	dx = fabs(v0x - v1x);
	dy = fabs(v0y - v1y);

	if(dx >= dy){
		z = (*v).getX();

		if((z >= v0x && z <= v1x) || (z <= v0x && z >= v1x))
			return true;
	}else{
		z = (*v).getY();

		if((z >= v0y && z <= v1y) || (z <= v0y && z >= v1y))
			return true;
	}

	return false;
}

// Destructor
// Attention: don't remove edges before there triangles are removed
TEdge::~TEdge(){
	(*v0).removeEdge(this);
	(*v1).removeEdge(this);

	if(T != NULL) (*T).removeEdge(this);

	if(t0 != NULL) delete t0;
	if(t1 != NULL) delete t1;
}

// Static member variables
unsigned long long TEdge::n = 0;

// Other non-member stuff
// from steinthors Edge class
/*double reldist(TEdge* e, Vertex* p){
	Vertex* pa = (*e).getV0();
	Vertex* pb = (*e).getV1();
	double px = (*p).getX();
	double py = (*p).getY();
	double pax = (*pa).getX();
	double pay = (*pa).getY();
	double pbx = (*pb).getX();
	double pby = (*pb).getY();

	return ((px - pax) * (pbx - pax) + (py - pay) * (pby - pay) / ((pbx - pax) * (pbx - pax) + (pby - pay) * (pby - pay)));
}

double det(TEdge* e, Vertex* p){
	Vertex* pa = (*e).getV0();
	Vertex* pb = (*e).getV1();
	double px = (*p).getX();
	double py = (*p).getY();
	double pax = (*pa).getX();
	double pay = (*pa).getY();
	double pbx = (*pb).getX();
	double pby = (*pb).getY();

	return (px * (pay - pby) - py * (pax - pbx) + (pax * pby - pbx * pay));
}

enum IntersectionType checkIntersection(TEdge* e0, TEdge* e1){
	double det_a, det_b, det_c, det_d;
	double dp_1, dp_2, dp_3, dp_4;
	bool same00 = false, same01 = false, same10 = false, same11 = false;

	// determinant between edge 0 and a point in edge 1
	det_a = det(e0, (*e1).getV0());
	det_b = det(e0, (*e1).getV1());
	// determinant between edge 1 and a point in edge 0
	det_c = det(e1, (*e0).getV0());
	det_d = det(e1, (*e0).getV1());

	if(fabs(det_a * det_b * det_c * det_d) < 0.00000000000000000001){
		bool col = false; // if true, check for collinearity

		//quick check if the edges share a vertex
		if((*(*e0).getV0()).getID() == (*(*e1).getV0()).getID())
			same00 = true;
		if((*(*e0).getV0()).getID() == (*(*e1).getV1()).getID())
			same01 = true;
		if((*(*e0).getV1()).getID() == (*(*e1).getV0()).getID())
			same10 = true;
		if((*(*e0).getV1()).getID() == (*(*e1).getV1()).getID())
			same11 = true;

		// TODO: this makes no sense if the vertices in the edges are unordered
		// is e1 and e2 the same edge? then return IS_TRUE
		if(same00 && same11)
			return IntersectionType::VERTEX;

		// some determinant was 0, need to check if it's inside an edge or outside.
		dp_1 = reldist(e0, (*e1).getV0());
		dp_2 = reldist(e0, (*e1).getV1());
		dp_3 = reldist(e1, (*e0).getV0());
		dp_4 = reldist(e1, (*e0).getV1());

		//std::cout << "dp1: " << dp_1 << std::endl;
		//std::cout << "dp2: " << dp_2 << std::endl;
		//std::cout << "dp3: " << dp_3 << std::endl;
		//std::cout << "dp4: " << dp_4 << std::endl;

		if((det_a == 0) && (dp_1 > 0) && (dp_1 < 1))
			col = true;
		else if((det_b == 0) && (dp_2 > 0) && (dp_2 < 1))
			col = true;
		else if((det_c == 0) && (dp_3 > 0) && (dp_3 < 1))
			col = true;
		else if((det_d == 0) && (dp_4 > 0) && (dp_4 < 1))
			col = true;

		if(col)
			return IntersectionType::VERTEX;
		else if(same00)
			return IntersectionType::VERTEX;
		else if(same01)
			return IntersectionType::VERTEX;
		else if(same10)
			return IntersectionType::VERTEX;
		else if(same11)
			return IntersectionType::VERTEX;
		else
			return IntersectionType::VERTEX;

	}else{
		// none of the determinants were 0, so just need to check the sign for intersection.
		if((signbit(det_a) ^ signbit(det_b)) && (signbit(det_c) ^ signbit(det_d)))
 			return IntersectionType::EDGE;
		else 
			return IntersectionType::NONE;
	}
}*/

enum IntersectionType checkIntersection(TEdge* e0, TEdge* e1, const double epsilon){
	Vertex *v00, *v01, *v10, *v11;
	double area00, area01, area10, area11;
	double x00, x01, x10, x11, y00, y01, y10, y11;
	double l0, l1, d0x, d0y, d1x, d1y;
	Triangle *t;

	// get vertices
	v00 = (*e0).getV0();
	v01 = (*e0).getV1();
	v10 = (*e1).getV0();
	v11 = (*e1).getV1();

	// compute edge lengths
	l0 = (*e0).length();
	l1 = (*e1).length();

	// triangle areas containing edge e0
	t = new Triangle(v00, v01, v10);
	area00 = (*t).signedArea();
	delete t;
	t = new Triangle(v00, v01, v11);
	area01 = (*t).signedArea();
	delete t;
	// triangle areas containing edge e1
	t = new Triangle(v10, v11, v00);
	area10 = (*t).signedArea();
	delete t;
	t = new Triangle(v10, v11, v01);
	area11 = (*t).signedArea();
	delete t;

	// check whether v10 lays on e0
	if(fabs(area00) <= epsilon){
		// check whether v10 lays between the vertices of e0
		if((*e0).isBetween(v10))
			return IntersectionType::VERTEX;
	}

	// check whether v11 lays on e0
	if(fabs(area01) <= epsilon){
		// check whether v10 lays between the vertices of e0
		if((*e0).isBetween(v11))
			return IntersectionType::VERTEX;
	}

	// check whether v00 lays on e1
	if(fabs(area10) <= epsilon){
		// check whether v10 lays between the vertices of e0
		if((*e1).isBetween(v00))
			return IntersectionType::VERTEX;
	}

	// check whether v01 lays on e1
	if(fabs(area11) <= epsilon){
		// check whether v10 lays between the vertices of e0
		if((*e1).isBetween(v01))
			return IntersectionType::VERTEX;
	}

	if((signbit(area00) != signbit(area01)) && (signbit(area10) != signbit(area11)))
		return IntersectionType::EDGE;

	return IntersectionType::NONE;
}

// From: "Intersection of two lines in three-space" by Ronald Goldman
// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
// edges from s0 to e0 and s1 to e1
Vertex* getIntersectionPoint(Vertex* s0, Vertex* e0, Vertex* s1, Vertex* e1){
	double s0x, s0y, e0x, e0y, s1x, s1y, e1x, e1y;
	double d0x, d0y, d1x, d1y; // compenents of the displacement vectors
	double t, s; // intersection times
	double crossD;

	s0x = (*s0).getX();
	s0y = (*s0).getY();
	e0x = (*e0).getX();
	e0y = (*e0).getY();
	s1x = (*s1).getX();
	s1y = (*s1).getY();
	e1x = (*e1).getX();
	e1y = (*e1).getY();

	d0x = e0x - s0x;
	d0y = e0y - s0y;
	d1x = e1x - s1x;
	d1y = e1y - s1y;

	// compute cross product of the translation vectors, if d0 x d1 = 0 then there is no single intersection point
	crossD = crossProduct2D(d0x, d0y, d1x, d1y);

	if(crossD == 0)
		return NULL;

	// compute time of intersection
	t = crossProduct2D(s1x - s0x, s1y - s0y, d1x, d1y) / crossD;
	s = - crossProduct2D(s0x - s1x, s0y - s1y, d0x, d0y) / crossD; // don't understand why we need a minus here :O

	if(0 <= t && t <= 1 && 0 <= s && s <= 1)
		return new Vertex(s0x + t * d0x, s0y + t * d0y);

	return NULL;
}

double crossProduct2D(double x0, double y0, double x1, double y1){
	return x0 * y1 - y0 * x1;
}
