/*
	Include standard libraries
*/
#include <stdio.h>
#include <string>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#include <math.h>
#endif


/*
	Include my headers	
*/
#include "settings.h"


/*
	Define the different intersection types:
		NONE: 	No intersection between two edges
		EDGE: 	One edge is crossing the other edge
		VERTEX: One vertex lays exactly on the other edge
*/
#ifndef __INTERSECTTYPE_
#define __INTERSECTTYPE_
enum class IntersectionType {NONE, EDGE, VERTEX};
#endif


#ifndef __TEDGE_H_
#define __TEDGE_H_

/*
	Define and include headers to the entites used in TEdge
*/
class Triangulation;
class Triangle;
class Vertex;

#include "triangulation.h"
#include "triangle.h"
#include "vertex.h"

/*
	Define the different edge types:
		POLYGON: 		An edge belonging to a polygon
		FRAME: 			An edge belonging to the bounding box
		TRIANGULATION: 	All other edges of the triangulation
*/
enum class EdgeType {POLYGON, FRAME, TRIANGULATION};

/*
	Define the class TEdge
*/
class TEdge{

private:
	
	/*
		The Triangulation the edge lives in
	*/
	Triangulation *T;

	/*
		The two vertices defining the edge
	*/
	Vertex * const v0;
	Vertex * const v1;

	/*
		The two triangles which contain the edge
	*/
	Triangle *t0;
	Triangle *t1;

	/*
		The type of the edge
	*/
	EdgeType type;

	/*
		The unique ID of the edge
	*/
	const unsigned long long id;

	/*
		The number of already generated edges
	*/
	static unsigned long long n;

public:

	/*
		LIST OF PUBLIC MEMBER FUNCTIONS

		CONSTRUCTORS:
		
							TEdge(Vertex * const V0, Vertex * const V1)
							TEdge(Vertex * const V0, Vertex * const V1, const EdgeType tp)

		SETTER:

		void 				setTriangulation(Triangulation * const t)
		void 				setEdgeType(const EdgeType tp)
		void 				setTriangle(Triangle * const t)

		GETTER:

		unsigned long long 	getID() const
		EdgeType 			getEdgeType() const
		Vertex* 			getV0() const
		Vertex* 			getV1() const
		Triangle* 			getT0() const
		Triangle* 			getT1() const
		Triangle* 			getTriangleNotContaining(Vertex const * const v) const
		Triangle* 			getTriangleContaining(Vertex const * const v) const
		Triangle* 			getOtherTriangle(Triangle const * const t) const
		Vertex* 			getOtherVertex(Vertex const * const v) const

		REMOVER:

		void 				removeTriangle(Triangle * const t)

		PRINTER:

		void 				write(FILE * const f) const
		void 				print() const

		OTHERS:

		double 				length() const
		bool 				contains(Vertex const * const v) const
		int 				nrAssignedTriangles() const
		double 				getAngle(Vertex const * const v) const
		bool 				isBetween(Vertex const * const v) const
	*/


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
	TEdge(Vertex * const V0, Vertex * const V1);

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
	TEdge(Vertex * const V0, Vertex * const V1, const EdgeType tp);


	/*
		S ~ E ~ T ~ T ~ E ~ R ~ S
	*/

	/*
		@param 	t 	The triangulation the edge belongs to
	*/
	void setTriangulation(Triangulation * const t);

	/*
		@param 	tp 	The new type of the edge
	*/
	void setEdgeType(const EdgeType tp);

	/*
		The function setTriangle() registers a new triangle, which contains the edge, at the
		edge. The new triangle is set at t0 if t0 is NULL, otherwwise it is set at t1 if
		t1 is NULL, otherwise the function errors with exit code 4 if the edge has already
		registered two triangles. The function errors with exit code 5 if the edge has already
		registered the same triangle.

		@param 	t 	The new triangle
	*/
	void setTriangle(Triangle * const t);

	
	/*
		G ~ E ~ T ~ T ~ E ~ R ~ S
	*/

	/*
		@return 	The ID of the edge
	*/
	unsigned long long getID() const;

	/*
		@return 	The type of the edge
	*/
	EdgeType getEdgeType() const;

	/*
		@return 	The vertex at v0
	*/
	Vertex *getV0() const;

	/*
		@return 	The vertex at v1
	*/
	Vertex *getV1() const;

	/*
		@return 	The triangle at t0
	*/
	Triangle *getT0() const;
	
	/*
		@return 	The triangle at t1
	*/
	Triangle *getT1() const;

	/*
		The function getTriangleNotContaining() returns t0 if it does not contain the vertex v,
		otherwise it returns t0 without checking whether it contains v.
		
		@param	v 	The vertex which should not be contained by the triangle
		@return 	The triangle not containing v
	*/
	Triangle *getTriangleNotContaining(Vertex const * const v) const;

	/*
		The function getTriangleContaining() returns t0 if it does contain the vertex v,
		otherwise it returns t0 without checking whether it contains v.
		
		@param	v 	The vertex which should be contained by the triangle
		@return 	The triangle containing v
	*/
	Triangle *getTriangleContaining(Vertex const * const v) const;

	/*
		The function getOtherTriangle() returns t1 if t equals t0, otherwise it returns t0.

		@param 	t 	The triangle which should not be returned
		@return 	Another triangle
	*/
	Triangle *getOtherTriangle(Triangle const * const t) const;

	/*
		The function getOtherVertex() returns v1 if v is equal to v0, otherwise it returns
		v0.

		@param 	v 	The vertex which should not be returned
		@return 	Another vertex
	*/
	Vertex *getOtherVertex(Vertex const *  const v) const;



	/*
		R ~ E ~ M ~ O ~ V ~ E ~ R
	*/

	/*
		The function removeTriangle() cancels the assignment of the triangle t at the edge.

		@param 	t 	Triangle to be removed

		Note:
			This function does not remove the edge from the triangle
	*/
	void removeTriangle(Triangle * const t);

	
	/*
		P ~ R ~ I ~ N ~ T ~ E ~ R
	*/

	/*
		The function write() writes the edge to a .graphml file.
		Edges of type POLYGON get the weight 5 plus the number of assigned triangles, of type
		FRAME gets the weight 10 plus the number of assigned triangles and edges oftype
		triangulation get as weight the number of assigned triangles (2 for a correct
		triangulation).

		@param 	f 	Pointer to the file to print in
	*/
	void write(FILE * const f) const;

	/*
		The function print() prints the the edge ID, the IDs of its vertices and its edge type
		to stdout.
	*/
	void print() const;


	/*
		O ~ T ~ H ~ E ~ R ~ S
	*/

	/*
		@return 	The euclidean length of the edge
	*/
	double length() const;

	/*
		The function contains() checks whether the edge contains the vertex v.

		@param 	v 	The vertex of interest
		@return 	True if the edge contains v, otherwise false
	*/
	bool contains(Vertex const * const v) const;

	/*
		@return 	The number of triangles which are assigned at the edge
	*/
	int nrAssignedTriangles() const;

	/*
		The function getAngle() computes the angle between the edge and the x-axis with the vertex
		v virtually shifted to the origin.

		@param 	v 	The vertex where the edge and the x-axis meet
		@return 	The angle between the edge and the x-axis with -pi < angle <= pi
	*/
	double getAngle(Vertex const *  const v) const;

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
	bool isBetween(Vertex const * const v) const;


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
	~TEdge();
};

#endif


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
	const bool precise);


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
Vertex *getIntersectionPoint(TEdge const * const e0, TEdge const * const e1);

/*
	The function crossProduct2D() computes a 2D-version of the cross product of two vectors which is
	basically just the 2D determinant.

	@param 	x0 	x-component of the first vector
	@param 	y0 	y-component of the first vector
	@param 	x1 	x-component of the second vector
	@param 	y1 	y-component of the second vector
	@returm 	The 2D cross product of the two vectors
*/
double crossProduct2D(const double x0, const double y0, const double x1, const double y1);
