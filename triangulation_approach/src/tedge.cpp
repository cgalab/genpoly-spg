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

	//quick check if the edges share a vertex
	if ((*e1).getV1() == (*e2).getV1() || (*e1).getV1() == (*e2).getV2() || (*e1).getV2() == (*e2).getV1() || (*e1).getV2() == (*e2).getV2())
		return IS_VERTEX;

	// determinant between edge 1 and a point in edge 2
	det_a = det(e1, (*e2).getV1());
	det_b = det(e1, (*e2).getV2());
	// determinant between edge 2 and a point in edge 1
	det_c = det(e2, (*e1).getV1());
	det_d = det(e2, (*e1).getV2());

	if(det_a * det_b * det_c * det_b == 0){
		bool col = false; // if true, check for collinearity

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

		if(col && (det_a == 0) && (det_b == 0) && (det_c == 0) && (det_d == 0))
			return IS_COLLINEAR;
		else if(col)
			return IS_TRUE;
		else
			return IS_FALSE;
	}else{
		// none of the determinants were 0, so just need to check the sign for intersection.
		if((signbit(det_a) ^ signbit(det_b)) && (signbit(det_c) ^ signbit(det_d)))
 			return IS_TRUE;
		else 
			return IS_FALSE;
	}
}
