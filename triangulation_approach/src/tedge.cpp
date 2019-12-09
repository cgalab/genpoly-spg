#include "tedge.h"

/*
	S ~ T ~ A ~ T ~ I ~ C 	V ~ A ~ R ~ I ~ A ~ B ~ L ~ E ~ S
*/

/*
	The number of already generated edges
*/
unsigned long long TEdge::n = 0;


/*
	C ~ O ~ N ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R ~ S
*/

/*
	Constructor:
	Generates an edge defined by V0 and V1 of type TRIANGULATION and registers it
	at V0 and V1. Will error with exit code 1 if V0 and V1 are identically.

	@param	V0 	First vertex defining the edge
	@param 	V1 	Second vertex defining the edge
*/
TEdge::TEdge(Vertex * const V0, Vertex * const V1) :
	T(NULL), v0(V0), v1(V1), t0(NULL), t1(NULL), type(EdgeType::TRIANGULATION), id(n) { 
	
	// Register the new edge at its vertices
	(*v0).addEdge(this);
	(*v1).addEdge(this);

	n++;

	if(v0 == NULL || v1 == NULL || (*v0).getID() == (*v1).getID()){
		printf("error circle edge\n");
		exit(1);
	}
}

/*
	Constructor:
	Generates an edge defined by V0 and V1 of variable type and registers it
	at V0 and V1. Will error with exit code 1 if V0 and V1 are identically.
	In case the type is POLYGON, it sets V0 as predecessor of V1 in the polygon
	and V1 as successor of V0.

	@param	V0 	First vertex defining the edge
	@param 	V1 	Second vertex defining the edge
	@param 	tp 	Type of the new edge

	Note:
		For edges oft type POLYGON the ordering of the vertices as parameters is
		important!
*/
TEdge::TEdge(Vertex * const V0, Vertex * const V1, const EdgeType tp) :
	T(NULL), v0(V0), v1(V1), t0(NULL), t1(NULL), type(tp), id(n) {
	
	// For polygon edges set the ordering in the polygon
	if(type == EdgeType::POLYGON){
		(*v0).setToNext(this);
		(*v1).setToPrev(this);
	}

	// Register the new edge at its vertices
	(*v0).addEdge(this);
	(*v1).addEdge(this);

	n++;

	if(v0 == NULL || v1 == NULL || (*v0).getID() == (*v1).getID()){
		printf("error circle edge\n");
		exit(1);
	}
}


/*
	S ~ E ~ T ~ T ~ E ~ R ~ S
*/

/*
	@param 	t 	The triangulation the edge belongs to
*/
void TEdge::setTriangulation(Triangulation * const t){
	T = t;
}

/*
	@param 	tp 	The new type of the edge
*/
void TEdge::setEdgeType(const EdgeType tp){
	type = tp;

	// For polygon edges set the ordering in the polygon
	if(tp == EdgeType::POLYGON){
		(*v0).setToNext(this);
		(*v1).setToPrev(this);
	}
}

/*
	The function setTriangle() registers a new triangle, which contains the edge, at the
	edge. The new triangle is set at t0 if t0 is NULL, otherwwise it is set at t1 if
	t1 is NULL, otherwise the function errors with exit code 4 if the edge has already
	registered two triangles. The function errors with exit code 5 if the edge has already
	registered the same triangle.

	@param 	t 	The new triangle
*/
void TEdge::setTriangle(Triangle * const t){
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


/*
	G ~ E ~ T ~ T ~ E ~ R ~ S
*/

/*
	@return 	The ID of the edge
*/
unsigned long long TEdge::getID() const{
	return id;
}

/*
	@return 	The type of the edge
*/
EdgeType TEdge::getEdgeType() const{
	return type;
}

/*
	@return 	The vertex at v0
*/
Vertex *TEdge::getV0() const{
	return v0;
}

/*
	@return 	The vertex at v1
*/
Vertex *TEdge::getV1() const{
	return v1;
}

/*
	@return 	The triangle at t0
*/
Triangle *TEdge::getT0() const{
	return t0;
}

/*
	@return 	The triangle at t1
*/
Triangle *TEdge::getT1() const{
	return t1;
}

/*
	The function getTriangleNotContaining() returns t0 if it does not contain the vertex v,
	otherwise it returns t0 without checking whether it contains v.
		
	@param	v 	The vertex which should not be contained by the triangle
	@return 	The triangle not containing v
*/
Triangle *TEdge::getTriangleNotContaining(Vertex const * const v) const{
	if((*t0).contains(v)) return t1;
	else return t0;
}

/*
	The function getTriangleContaining() returns t0 if it does contain the vertex v,
	otherwise it returns t0 without checking whether it contains v.
		
	@param	v 	The vertex which should be contained by the triangle
	@return 	The triangle containing v
*/
Triangle *TEdge::getTriangleContaining(Vertex const * const v) const{
	if((*t0).contains(v)) return t0;
	else return t1;
}

/*
	The function getOtherTriangle() returns t1 if t equals t0, otherwise it returns t0.

	@param 	t 	The triangle which should not be returned
	@return 	Another triangle
*/
Triangle *TEdge::getOtherTriangle(Triangle const * const t) const{
	if((*t).getID() == (*t0).getID()) return t1;
	else return t0;
}

/*
	The function getOtherVertex() returns v1 if v is equal to v0, otherwise it returns
	v0.

	@param 	v 	The vertex which should not be returned
	@return 	Another vertex
*/
Vertex *TEdge::getOtherVertex(Vertex const * const v) const{
	if((*v).getID() == (*v0).getID())
		return v1;
	else
		return v0;
}


/*
	R ~ E ~ M ~ O ~ V ~ E ~ R
*/

/*
	The function removeTriangle() cancels the assignment of the triangle t at the edge.

	@param 	t 	Triangle to be removed

	Note:
		This function does not remove the edge from the triangle
*/
void TEdge::removeTriangle(Triangle * const t){
	if(t0 != NULL && (*t0).getID() == (*t).getID())
	 	t0 = NULL;
	else if(t1 != NULL && (*t1).getID() == (*t).getID()) 
		t1 = NULL;
	else 
		printf("Removed triangle was not adjacent to edge from vertex %llu to vertex %llu \n", (*v0).getID(), (*v1).getID());
}


/*
	P ~ R ~ I ~ N ~ T ~ E ~ R
*/

/*
	The function write() writes the edge to a .graphml file.
	Edges of type POLYGON get the weight 5 plus the number of assigned triangles, of type FRAME gets the weight 10 plus
	the number of assigned triangles and edges oftype triangulation get as weight the number of assigned triangles (2
	for a correct triangulation).

	@param 	f 	Pointer to the file to print in
*/
void TEdge::write(FILE * const f) const{
	int w;

	w = nrAssignedTriangles();

	if(type == EdgeType::POLYGON) w = 5 + w;
	if(type == EdgeType::FRAME) w = 10 + w;

	fprintf(f, "<edge vertex1=\"%llu\" vertex2=\"%llu\" weight=\"%d\" useWeight=\"true\"></edge>\n", (*v0).getID(), (*v1).getID(), w);
}

/*
	The function print() prints the the edge ID, the IDs of its vertices and its edge type
	to stdout.
*/
void TEdge::print() const{
	std::string tp;

	if(type == EdgeType::POLYGON) tp = "POLYGON";
	else if(type == EdgeType::FRAME) tp = "FRAME";
	else tp = "TRIANGULATION";

	printf("Edge %llu from vertex %llu to vertex %llu of type %s \n", id, (*v0).getID(), (*v1).getID(), tp.c_str());
	(*v0).print();
	(*v1).print();
}


/*
	O ~ T ~ H ~ E ~ R ~ S
*/

/*
	@return 	The euclidean length of the edge
*/
double TEdge::length() const{
	double x0, x1, y0, y1;

	x0 = (*v0).getX();
	y0 = (*v0).getY();

	x1 = (*v1).getX();
	y1 = (*v1).getY();

	return sqrt(pow((x0 - x1), 2) + pow((y0 - y1), 2));	
}

/*
	The function contains() checks whether the edge contains the vertex v.

	@param 	v 	The vertex of interest
	@return 	True if the edge contains v, otherwise false
*/
bool TEdge::contains(Vertex const * const v) const{
	if((*v).getID() == (*v0).getID()) return true;
	if((*v).getID() == (*v1).getID()) return true;
	return false;
}

/*
	@return 	The number of triangles which are assigned at the edge
*/
int TEdge::nrAssignedTriangles() const{
	int n = 0;

	if(t0 != NULL) n++;
	if(t1 != NULL) n++;

	return n;
}

/*
	The function getAngle() computes the angle between the edge and the x-axis with the vertex
	v virtually shifted to the origin.

	@param 	v 	The vertex where the edge and the x-axis meet
	@return 	The angle between the edge and the x-axis with -pi < angle <= pi
*/
double TEdge::getAngle(Vertex const * const v) const{
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

	// Compute the right direction vector for the edge
	dx = x1 - x0;
	dy = y1 - y0;

	cosa = dx / sqrt(pow((dx), 2) + pow((dy), 2));

	alpha = acos(cosa);

	// If the direction vector goes into the third or fourth quadrant, we get a negative angle
	if(dy < 0)
		alpha = - alpha;

	// If the angle is zero but the x-component of the direction vector is less than zero
	// we get pi
	if(alpha == 0 && dx < 0)
		alpha = M_PI;

	return alpha;
}

/*
	The function isBetween() checks whether the vertex v is between the two vertices of the
	edge, i.e. is v inside the rectangle with edges parallel to the coordinate axes and spaned
	by the two vertices of the edge.

	@param 	v 	The vertex to be checked
	@return 	True if v is inside the ractangle, otherwise false

	Note:
		It is assumed that v lays pretty close to the supporting line of the edge, so it is only
		necessary to check whether v lays between the longer edge of the ractangle.
*/
bool TEdge::isBetween(Vertex const * const v) const{
	double v0x, v1x, v0y, v1y, dx, dy, z;

	v0x = (*v0).getX();
	v0y = (*v0).getY();
	v1x = (*v1).getX();
	v1y = (*v1).getY();

	dx = fabs(v0x - v1x);
	dy = fabs(v0y - v1y);

	// Take the direction with larger intervall for comparison
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

/*
	D ~ E ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R
*/

/*
	Destructor:
	While deletion of an edge it gets automatically deregistered of its vertices and removed from
	the edge list of the triangulation to make sure that no dangling pointers to the edge remain
	anywhere. It also deletes the triangles containing the edge.

	Note:
		It is the more clean way to delete the assigned triangles before deleting an edge!
*/
TEdge::~TEdge(){
	(*v0).removeEdge(this);
	(*v1).removeEdge(this);

	if(T != NULL) (*T).removeEdge(this);

	if(t0 != NULL) delete t0;
	if(t1 != NULL) delete t1;
}


/*
	N ~ O ~ N ~ M ~ E ~ M ~ B ~ E ~ R 	F ~ U ~ N ~ C ~ T ~ I ~ O ~ N ~ S
*/

/*
	The function checkIntersection() checks whether to edge e0 and e1 intersect.
	Therefore it computes the determinants of all possible combination of one edge with one vertex of
	the other edge and classify into the following intersection types:
		- VERTEX: One vertex lays at the other edge, if one of the determinantes is zero and the used
				vertex lays between the vertices of the used edge
		- EDGE: The two edges are intersecting, if the pairs of determinantes using the same edge have
				different signs
		- NONE: Otherwise, the two edges do not intersect

	@param 	e0 		First edge
	@param 	e1 		Second edge
	@param 	precise If false the function uses Settings::epsInt instead of zero for the VERTEX
					intersections
	@return 		The tpye of intersection

	Note:
		For numerical stability a vertex is considered to lay exactly at an edge if the corresponding
		determinantent's absolute value is less than Settings::EpsInt. This also keeps vertices a bit
		away from edges.
*/
enum IntersectionType checkIntersection(TEdge const * const e0, TEdge const * const e1,
	const bool precise){
	
	Vertex *v00, *v01, *v10, *v11;
	double area00, area01, area10, area11;
	Triangle *t;
	double epsilon = 0;

	if(!precise)
		epsilon = Settings::epsInt;

	// Get vertices
	v00 = (*e0).getV0();
	v01 = (*e0).getV1();
	v10 = (*e1).getV0();
	v11 = (*e1).getV1();

	// Triangle areas containing edge e0
	t = new Triangle(v00, v01, v10);
	area00 = (*t).signedArea();
	delete t;
	t = new Triangle(v00, v01, v11);
	area01 = (*t).signedArea();
	delete t;
	// Triangle areas containing edge e1
	t = new Triangle(v10, v11, v00);
	area10 = (*t).signedArea();
	delete t;
	t = new Triangle(v10, v11, v01);
	area11 = (*t).signedArea();
	delete t;

	// Check whether v10 lays on e0
	if(fabs(area00) <= epsilon){
		// Check whether v10 lays between the vertices of e0
		if((*e0).isBetween(v10))
			return IntersectionType::VERTEX;
	}

	// Check whether v11 lays on e0
	if(fabs(area01) <= epsilon){
		// Check whether v10 lays between the vertices of e0
		if((*e0).isBetween(v11))
			return IntersectionType::VERTEX;
	}

	// Check whether v00 lays on e1
	if(fabs(area10) <= epsilon){
		// Check whether v10 lays between the vertices of e0
		if((*e1).isBetween(v00))
			return IntersectionType::VERTEX;
	}

	// Check whether v01 lays on e1
	if(fabs(area11) <= epsilon){
		// Check whether v10 lays between the vertices of e0
		if((*e1).isBetween(v01))
			return IntersectionType::VERTEX;
	}

	if((signbit(area00) != signbit(area01)) && (signbit(area10) != signbit(area11)))
		return IntersectionType::EDGE;

	return IntersectionType::NONE;
}

/*
	The function getIntersectionPoint() computes the intersection point between the edges e0 and e1.
	This algorithm is taken from the article "Intersection of two lines in three-space" by Ronald
	Goldman, published in Graphics Gems, page 304.

	@param 	e0 	The first edge
	@param 	e1 	The second edge
	@return 	The intersection point if both edges do intersect, otherwise NULL

	Note:
		https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
*/
Vertex *getIntersectionPoint(TEdge const * const e0, TEdge const * const e1){
	double s0x, s0y, e0x, e0y, s1x, s1y, e1x, e1y;
	double d0x, d0y, d1x, d1y; // Compenents of the displacement vectors
	double t, s; // Intersection times
	double crossD;

	// Get the coordinates of the vertices forming the edges
	s0x = (*(*e0).getV0()).getX();
	s0y = (*(*e0).getV0()).getY();
	e0x = (*(*e0).getV1()).getX();
	e0y = (*(*e0).getV1()).getY();
	s1x = (*(*e1).getV0()).getX();
	s1y = (*(*e1).getV0()).getY();
	e1x = (*(*e1).getV1()).getX();
	e1y = (*(*e1).getV1()).getY();

	d0x = e0x - s0x;
	d0y = e0y - s0y;
	d1x = e1x - s1x;
	d1y = e1y - s1y;

	// Compute cross product of the translation vectors, if d0 x d1 = 0 then there is no single
	// intersection point
	crossD = crossProduct2D(d0x, d0y, d1x, d1y);

	if(crossD == 0)
		return NULL;

	// Compute time of intersection
	t = crossProduct2D(s1x - s0x, s1y - s0y, d1x, d1y) / crossD;
	s = - crossProduct2D(s0x - s1x, s0y - s1y, d0x, d0y) / crossD; // Don't understand why we need a minus here :O

	if(0 <= t && t <= 1 && 0 <= s && s <= 1)
		return new Vertex(s0x + t * d0x, s0y + t * d0y);

	return NULL;
}

/*
	The function crossProduct2D() computes a 2D-version of the cross product of two vectors which is
	basically just the 2D determinant.

	@param 	x0 	x-component of the first vector
	@param 	y0 	y-component of the first vector
	@param 	x1 	x-component of the second vector
	@param 	y1 	y-component of the second vector
	@returm 	The 2D cross product of the two vectors
*/
double crossProduct2D(const double x0, const double y0, const double x1, const double y1){
	return x0 * y1 - y0 * x1;
}