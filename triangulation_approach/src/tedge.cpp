#include "tedge.h"

// Constructors
TEdge::TEdge(Vertex* V1, Vertex* V2, EdgeType tp) : T(NULL), v1(V1), v2(V2), t1(NULL), t2(NULL), type(tp), id(n) {
	
	if(type == EdgeType::POLYGON){
		(*v1).setToNext(this);
		(*v2).setToPrev(this);
	}

	(*v1).addEdge(this);
	(*v2).addEdge(this);

	n++;

	if(v1 == NULL || v2 == NULL || (*v1).getID() == (*v2).getID()){
		printf("error circle edge\n");
		exit(1);
	}
}

TEdge::TEdge(Vertex* V1, Vertex* V2) : T(NULL), v1(V1), v2(V2), t1(NULL), t2(NULL), type(EdgeType::TRIANGULATION), id(n) { 
	
	(*v1).addEdge(this);
	(*v2).addEdge(this);

	n++;

	if(v1 == NULL || v2 == NULL || (*v1).getID() == (*v2).getID()){
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

Vertex* TEdge::getV1(){
	return v1;
}

Vertex* TEdge::getV2(){
	return v2;
}

Triangle* TEdge::getT1(){
	return t1;
}

Triangle* TEdge::getT2(){
	return t2;
}

Triangle* TEdge::getTriangleNotContaining(Vertex* v){
	if((*t1).contains(v)) return t2;
	else return t1;
}

Triangle* TEdge::getTriangleContaining(Vertex* v){
	if((*t1).contains(v)) return t1;
	else return t2;
}

Triangle* TEdge::getOtherTriangle(Triangle* t){
	if((*t).getID() == (*t1).getID()) return t2;
	else return t1;
}

Vertex* TEdge::getOtherVertex(Vertex* v){
	if((*v).getID() == (*v1).getID())
		return v2;
	else
		return v1;
}

// Setter
void TEdge::setTriangulation(Triangulation* t){
	T = t;
}

void TEdge::setEdgeType(EdgeType tp){
	type = tp;

	if(tp == EdgeType::POLYGON){
		(*v1).setToNext(this);
		(*v2).setToPrev(this);
	}
}

void TEdge::setTriangle(Triangle* t){
	if(t1 == NULL) t1 = t;
	else if(t2 == NULL) t2 = t;
	else{
		printf("The edge from vertex %llu to vertex %llu already has two triangles!\n", (*v1).getID(), (*v2).getID());
		exit(1);
	}

	// absolutely important to compare by pointer here, because t is still in construction
	if(t1 == t2){
		printf("The edge from vertex %llu to vertex %llu has already registered the same triangle! \n", (*v1).getID(), (*v2).getID());
		exit(1);
	}	
}

Triangle* TEdge::setTriangle(Triangle* t, std::string context, bool &ok){
	if(t1 == NULL){
		t1 = t;
		return t2;
	}else if(t2 == NULL){
		t2 = t;
		return t1;
	}else{
		printf("The edge from vertex %llu to vertex %llu already has two triangles!\n", (*v1).getID(), (*v2).getID());
		printf("context: %s \n", context.c_str());
		ok = false;
	}
	return NULL;

	// absolutely important to compare by pointer here, because t is still in construction
	if(t1 == t2){
		printf("The edge from vertex %llu to vertex %llu has already registered the same triangle! \n", (*v1).getID(), (*v2).getID());
		printf("context: %s \n", context.c_str());
		print();
		printf("existing t1\n");
		(*t1).print();
		printf("existing t2\n");
		(*t2).print();
		printf("new t\n");
		(*t).print();
		printf("end\n");
		ok = false;
	}	
}

// Remover
void TEdge::removeTriangle(Triangle* t){

	if(t1 != NULL && (*t1).getID() == (*t).getID())
	 	t1 = NULL;
	else if(t2 != NULL && (*t2).getID() == (*t).getID()) 
		t2 = NULL;
	else 
		printf("Removed triangle was not adjacent to edge from vertex %llu to vertex %llu \n", (*v1).getID(), (*v2).getID());
}

// Printer
void TEdge::print(FILE* f){
	int w = 0;
	if(t1 != NULL) w++;
	if(t2 != NULL) w++;

	if(type == EdgeType::POLYGON) w = 5;
	if(type == EdgeType::FRAME) w = 10;
	fprintf(f, "<edge vertex1=\"%llu\" vertex2=\"%llu\" weight=\"%d\" useWeight=\"true\"></edge>\n", (*v1).getID(), (*v2).getID(), w);
}

void TEdge::print(){
	std::string tp;

	if(type == EdgeType::POLYGON) tp = "POLYGON";
	else if(type == EdgeType::FRAME) tp = "FRAME";
	else tp = "TRIANGULATION";

	printf("Edge %llu from vertex %llu to vertex %llu of type %s \n", id, (*v1).getID(), (*v2).getID(), tp.c_str());
}

// Others
double TEdge::length(){
	double x1, x2, y1, y2;

	x1 = (*v1).getX();
	y1 = (*v1).getY();

	x2 = (*v2).getX();
	y2 = (*v2).getY();

	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));	
}

bool TEdge::contains(Vertex* v){
	if((*v).getID() == (*v1).getID()) return true;
	if((*v).getID() == (*v2).getID()) return true;
	return false;
}

int TEdge::nrAssignedTriangles(){
	int n = 0;

	if(t1 != NULL) n++;
	if(t2 != NULL) n++;

	return n;
}

// computes angle between edge and x-axis in range between -pi and pi
// starting point is v
double TEdge::getAngle(Vertex* v){
	double x0, y0, x1, y1, dx, dy;
	double cosa, alpha;

	if((*v).getID() == (*v1).getID()){
		x0 = (*v1).getX();
		y0 = (*v1).getY();
		x1 = (*v2).getX();
		y1 = (*v2).getY();
	}else{
		x0 = (*v2).getX();
		y0 = (*v2).getY();
		x1 = (*v1).getX();
		y1 = (*v1).getY();
	}

	dx = x1 - x0;
	dy = y1 - y0;

	cosa = dx / sqrt(pow((x1 - x0), 2) + pow((y1 - y0), 2));

	alpha = acos(cosa);

	if(dy < 0)
		alpha = - alpha;

	return alpha;
}

bool TEdge::isBetween(Vertex* v){
	double v1x, v2x, v1y, v2y, dx, dy, z;

	v1x = (*v1).getX();
	v1y = (*v1).getY();
	v2x = (*v2).getX();
	v2y = (*v2).getY();

	dx = fabs(v1x - v2x);
	dy = fabs(v1y - v2y);

	if(dx >= dy){
		z = (*v).getX();

		if((z >= v1x && z <= v2x) || (z <= v1x && z >= v2x))
			return true;
	}else{
		z = (*v).getY();

		if((z >= v1y && z <= v2y) || (z <= v1y && z >= v2y))
			return true;
	}

	return false;
}

// Destructor
// Attention: don't remove edges before there triangles are removed
TEdge::~TEdge(){
	(*v1).removeEdge(this);
	(*v2).removeEdge(this);

	if(T != NULL) (*T).removeEdge(this);

	if(t1 != NULL) delete t1;
	if(t2 != NULL) delete t2;
}

// Static member variables
unsigned long long TEdge::n = 0;

// Other non-member stuff
// from steinthors Edge class
double reldist(TEdge* e, Vertex* p){
	Vertex* pa = (*e).getV1();
	Vertex* pb = (*e).getV2();
	double px = (*p).getX();
	double py = (*p).getY();
	double pax = (*pa).getX();
	double pay = (*pa).getY();
	double pbx = (*pb).getX();
	double pby = (*pb).getY();

	return ((px - pax) * (pbx - pax) + (py - pay) * (pby - pay) / ((pbx - pax) * (pbx - pax) + (pby - pay) * (pby - pay)));
}

double det(TEdge* e, Vertex* p){
	Vertex* pa = (*e).getV1();
	Vertex* pb = (*e).getV2();
	double px = (*p).getX();
	double py = (*p).getY();
	double pax = (*pa).getX();
	double pay = (*pa).getY();
	double pbx = (*pb).getX();
	double pby = (*pb).getY();

	return (px * (pay - pby) - py * (pax - pbx) + (pax * pby - pbx * pay));
}

enum intersect_t checkIntersection(TEdge* e1, TEdge* e2){
	double det_a, det_b, det_c, det_d;
	double dp_1, dp_2, dp_3, dp_4;
	bool same11 = false, same12 = false, same21 = false, same22 = false;

	// determinant between edge 1 and a point in edge 2
	det_a = det(e1, (*e2).getV1());
	det_b = det(e1, (*e2).getV2());
	// determinant between edge 2 and a point in edge 1
	det_c = det(e2, (*e1).getV1());
	det_d = det(e2, (*e1).getV2());

	if(det_a * det_b * det_c * det_d == 0){
		bool col = false; // if true, check for collinearity

		//quick check if the edges share a vertex
		if((*(*e1).getV1()).getID() == (*(*e2).getV2()).getID()) same11 = true;
		if((*(*e1).getV1()).getID() == (*(*e2).getV2()).getID()) same12 = true;
		if((*(*e1).getV2()).getID() == (*(*e2).getV1()).getID()) same21 = true;
		if((*(*e1).getV2()).getID() == (*(*e2).getV2()).getID()) same22 = true;

		// is e1 and e2 the same edge? then return IS_TRUE
		if (same11 && same22) return IS_SAME_EDGE;

		// some determinant was 0, need to check if it's inside an edge or outside.
		dp_1 = reldist(e1, (*e2).getV1());
		dp_2 = reldist(e1, (*e2).getV2());
		dp_3 = reldist(e2, (*e1).getV1());
		dp_4 = reldist(e2, (*e1).getV2());

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

		if(col) return IS_4P_COLLINEAR;
		else if(same11) return IS_VERTEX11;
		else if(same12) return IS_VERTEX12;
		else if(same21) return IS_VERTEX21;
		else if(same22) return IS_VERTEX22;
		else return IS_FALSE;

	}else{
		// none of the determinants were 0, so just need to check the sign for intersection.
		if((signbit(det_a) ^ signbit(det_b)) && (signbit(det_c) ^ signbit(det_d)))
 			return IS_TRUE;
		else 
			return IS_FALSE;
	}
}

// From: "Intersection of two lines in three-space" by Ronald Goldman
// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
// edges from s0 to e0 and s1 to e1
Vertex* getIntersectionPoint(Vertex* s0, Vertex* e0, Vertex* s1, Vertex* e1){
	double s0x, s0y, e0x, e0y, s1x, s1y, e1x, e1y;
	double d0x, d0y, d1x, d1y; // compenents of the displacement vectors
	double t, s; // intersection time
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

	if(crossD == 0) return NULL;

	// compute time of intersection
	t = crossProduct2D(s1x - s0x, s1y - s0y, d1x, d1y) / crossD;
	s = - crossProduct2D(s0x - s1x, s0y - s1y, d0x, d0y) / crossD; // don't understand why we need a minus here :O

	if(0 <= t && t <= 1 && 0 <= s && s <= 1) return new Vertex(s0x + t * d0x, s0y + t * d0y);

	return NULL;
}

double crossProduct2D(double x0, double y0, double x1, double y1){
	return x0 * y1 - y0 * x1;
}
