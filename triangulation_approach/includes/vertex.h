/*
	Include standard libraries
*/
#include <list>
#include <vector>
#include <stdio.h>
#include <map>
#include <queue>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#include <math.h>
#endif

/*
	Include my headers	
*/
#include "settings.h"

#ifndef __VERTEX_H_
#define __VERTEX_H_

/*
	Define and include headers to the entites used in Vertex
*/
class Triangulation;
class TEdge;
class Triangle;
class TPolygon;

#include "triangulation.h"
#include "tedge.h"
#include "triangle.h"
#include "tpolygon.h"

class Vertex{

private:
	
	/*
		The triangulation the vertex lives in
	*/
	Triangulation *T;

	/*
		The polygon the vertex belongs to
	*/
	TPolygon *P;

	/*
		The coordinates of the vertex
	*/
	double x;
	double y;

	/*
		Pointers to the polygon edges which connect the vertex with its predecessor and
		its successor in the polygon
	*/
	TEdge *toPrev;
	TEdge *toNext;

	/*
		List of all edges containing the vertex
	*/
	std::list<TEdge*> edges;

	/*
		List of all triangles containing the vertex
	*/
	std::list<Triangle*> triangles;

	/*
		Is true if the vertex is one of the four vertices of the bounding box
	*/
	// TODO:
	// Maybe rename this to bounding box or something
	const bool rectangleVertex;

	/*
		The unique ID of the vertex
	*/
	const unsigned long long id;

	/*
		The reserveID is used for computing determinants with double arithmetics, because
		for copied vertices the ID can not be copied (otherwise it would violate the
		uniqueness of IDs), but computing determinants with copied vertices should be done
		in the same ordering as computing it with the original one. So the reserveID is
		2 * ID for normal vertices and 2 * ID + 1 for copied vertices (here the ID of the
		original vertex is used to compute the reserveID).
	*/
	unsigned long long reserveID;

	/*
		The number of already generated vertices
	*/
	static unsigned long long n;

	/*
		The number of deleted vertices
	*/
	static unsigned long long deleted;


	/*
		P ~ R ~ I ~ V ~ A ~ T ~ E 	M ~ E ~ M ~ B ~ E ~ R 	F ~ U ~ N ~ C ~ T ~ I ~ O ~ N ~ S
	*/

	/*
		Sets the reserveID of the vertex

		@param 	rid 	The new value for the reserveID

		Note:
			By default the reserveID is set to 2 * ID
	*/
	void setRID(const unsigned long long rid);

	/*
		The function getEnvironment() recursivelly inserts all adjacent edges and vertices
		into maps. As key the IDs are used. This function is used for debug outputs.

		@param 	es 		The map of edges
		@param 	vs 		The map of vertices
		@param 	depth 	The number of recursive steps to be done, basically a depth of n
						means that all vertices with distance less then n to this vertex
						are included
	*/
	void getEnvironment(std::map<const unsigned long long, const TEdge*> &es,
		std::map<const unsigned long long, const Vertex*> &vs, const int depth) const;
	

public:

	/*
		LIST OF PUBLIC MEMBER FUNCTIONS

		CONSTRUCTORS:

								Vertex(const double X, const double Y)
								Vertex(const double X, const double Y, const bool RV)
		Vertex* 				getTranslated(const double dx, const double dy) const
		
		SETTER:

		void 					setTriangulation(Triangulation * const t)
		void 					setPolygon(TPolygon * const p)
		void 					setPosition(const double X, const double Y)
		void 					addEdge(TEdge * const e)
		void 					addTriangle(Triangle * const t)
		void 					setToPrev(TEdge * const e)
		void 					setToNext(TEdge * const e)

		GETTER:

		double 					getX() const
		double 					getY() const
		std::list<Triangle*> 	getTriangles() const
		unsigned long long 		getID() const
		unsigned long long 		getRID() const
		TEdge* 					getEdgeTo(Vertex * const toV) const
		std::vector<TEdge*> 	getSurroundingEdges() const
		std::list<TEdge*> 		getPolygonEdges() const
		bool 					isRectangleVertex() const
		double 					getMediumEdgeLength() const
		double 					getDirectedEdgeLength(const double alpha) const
		TEdge* 					getToPrev() const
		TEdge* 					getToNext() const
		Vertex* 				getPrev() const
		Vertex* 				getNext() const
		Triangulation* 			getTriangulation() const
		Triangle* 				getTriangleWith(Vertex const * const v0, Vertex const * const v1)
		unsigned int 			getPID() const
		int 					getActualPolygonSize() const
		double					getInsideAngle() const;

		REMOVER:

		void 					removeEdge(TEdge * const e)
		void 					removeTriangle(Triangle * const t)

		PRINTER:

		void 					write(FILE * const f, double factor) const
		void 					writeToDat(FILE * const f)
		void 					print() const
		void 					writeEnvironment(const int depth, const char *filename) const
		void 					writeSurroundingTriangulation(const char *filename) const
		static void				printStats()

		OTHERS:

		bool 					check()
		void 					stretch(const double factor)
		bool 					checkSurroundingPolygon()
		double 					getDistanceToOrigin() const
	*/


	/*
		C ~ O ~ N ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R ~ S
	*/

	/*
		Constructor:
		Sets the coordinates and the ID of the new vertex. Also it sets the reserveID to
		2 * ID and the property rectangleVertex to false, i.e. the new vertex is not part
		of the bounding box.

		@param 	X 	The x coordinate of the vertex
		@param 	Y 	The y coordinate of the vertex
	*/
	Vertex(const double X, const double Y);

	/*
		Constructor:
		Sets the coordinates and the ID of the new vertex. Also it sets the reserveID to
		2 * ID.
		The property rectangleVertex can be directly set here with the parameter RV.

		@param 	X 	The x coordinate of the vertex
		@param 	Y 	The y coordinate of the vertex
		@param 	RV 	Determines whether the new vertex is part of the bounding box
	*/
	Vertex(const double X, const double Y, const bool RV);

	/*
		The function getTranslated() is an implicit constructor which generates a new vertex
		out of a already existing vertex and a translation vector. The position of the new
		vertex is the position of the old vertex plus the translation vector.
		The reserveID is set to 2 * ID of the original vertex if the translation vector is
		zero, otherwise it is set to 2 * ID + 1 of the original vertex.
		The property rectangleVertex is set to false.

		@param 	dx 	X-component of the translation vector
		@param 	dy 	Y-component of the translation vector
		@return		The translated vertex
	*/
	Vertex *getTranslated(const double dx, const double dy) const;

	
	/*
		S ~ E ~ T ~ T ~ E ~ R ~ S
	*/

	/*
		@param 	t 	The triangulation the vertex lives in
	*/
	void setTriangulation(Triangulation * const t);

	/*
		@param 	p 	The polygon the vertex belongs to
	*/
	void setPolygon(TPolygon *p);

	/*
		@param 	X 	The new x-coordinate of the vertex
		@param 	Y 	The new y-coordinate of the vertex
	*/
	void setPosition(const double X, const double Y);

	/*
		Adds the edge e to the edges list of the vertex.

		@param 	e 	The edge to be added to the edges list

		Note:
			- Each edge contained in the vertex's edges list should contain the vertex!
			- While constructing an edge this function is automatically called to add the
				edge to the edges lists of its vertices
	*/
	void addEdge(TEdge * const e);

	/*
		Adds the triangle t to the triangles list of the vertex.

		@param 	t 	The triangle to be added to the triangles list

		Note:
			- Each triangle contained in the vertex's triangles list should contain the vertex!
			- While constructing a triangle this function is automatically called to add the
				triangle to the triangles lists of its vertices
	*/
	void addTriangle(Triangle * const t);

	/*
		The function setToPrev() sets the pointer for the polygon edge which connects the vertex
		to its predecessor in the polygon.

		@param 	e 	The edge to the predecessor of the vertex in the polygon

		Note:
			This function is automatically called when a new polygon edge is constructed.
	*/
	void setToPrev(TEdge * const e);

	/*
		The function setToNext() sets the pointer for the polygon edge which connects the vertex
		to its successor in the polygon.

		@param 	e 	The edge to the succesor of the vertex in the polygon

		Note:
			This function is automatically called when a new polygon edge is constructed.
	*/
	void setToNext(TEdge * const e);

	
	/*
		G ~ E ~ T ~ T ~ E ~ R ~ S
	*/

	/*
		@return 	The x-coordinate of the vertex's position
	*/
	double getX() const;

	/*
		@return 	The y-coordinate of the vertex's position
	*/
	double getY() const;

	/*
		@return 	The triangles list of the vertex
	*/
	std::list<Triangle*> getTriangles() const;

	/*
		@return 	The ID of the vertex
	*/
	unsigned long long getID() const;

	/*
		@return 	The reserve ID of the vertex
	*/
	unsigned long long getRID() const;

	/*
		The function getEdgeTo() searches the edges list for an edge to the vertex toV
		and returns it if existing. Otherwise it returns NULL.

		@param 	toV 	The target vertex of the searched edge
		@return 		The edge from the vertex to the vertex toV if existing, otherwise
						NULL
	*/
	TEdge *getEdgeTo(Vertex * const v) const;

	/*
		The function getSurroundingEdges() returns a list of all surrounding edges of the
		vertex which form the surrounding polygon of the vertex. I.e. basically a list of
		all edges which are contained by a triangle containing the vertex, but do not
		contain the vertex itself.

		@return 	A list of all surrounding edges
	*/
	std::vector<TEdge*> getSurroundingEdges() const;

	/*
		@return 	A list of all (2) polygon edges incident to the vertex
	*/
	std::list<TEdge*> getPolygonEdges() const;

	/*
		@return 	True if the vertex is part of the bounding box, otherwise false
	*/
	bool isRectangleVertex() const;

	/*
		The function getMediumEdgeLength() computes the mean of the lengths of all
		incident edges to the vertex. This can be used as estimate for appropriate 
		distribution parameters.

		@return 	The mean of the lengths of all incident edges
	*/
	double getMediumEdgeLength() const;

	/*
		The function getDirectedEdgeLength() finds the triangle incident to the vertex
		in direction alpha (alpha = 0 corresponds to the positive x-driection) and
		computes the mean edge length of the edges of the triangle containing the
		vertex. This value can be used as estimate for appropriate distribution
		parameters.
		If the function is not able to find the right triangle it uses getMediumEdgeLength()
		instead the get an estimate.

		@param 	alpha 	Direction of the planed translation
		@return 		Mean length of the triangles edge in direction alpha

		Note:
			This gives a better estimate then getMediumEdgeLength(), but is more
			expensive to compute
	*/
	double getDirectedEdgeLength(const double alpha) const;

	/*
		@return 	The edge to the predecessor of the vertex in the polygon
	*/
	TEdge *getToPrev() const;

	/*
		@return 	The edge to the successor of the vertex in the polygon
	*/
	TEdge *getToNext() const;

	/*
		@return 	 The predecessor of the vertex in the polygon
	*/
	Vertex *getPrev() const;

	/*
		@return 	The successor of the vertex in the polygon
	*/
	Vertex *getNext() const;

	/*
		@return 	The triangulation the vertex lives in
	*/
	Triangulation *getTriangulation() const;

	/*
		The function getTriangleWith() searches for a triangle in the triangles list
		which is formed by the vertex and the vertices v0 and v1. If such a triangle
		does not exist, it returns NULL

		@param 	v0 	Second vertex contained by the searched triangle
		@param 	v1 	Third vertex vontained by the searched triangle
		@return 	The searched triangle if it exists, otherwise NULL
	*/
	Triangle *getTriangleWith(Vertex const * const v0, Vertex const * const v1) const;

	/*
		@return 	The id of the polygon the vertex belongs to
	*/
	unsigned int getPID() const;

	/*
		@return 	The actual size of the polygon the vertex belongs to
	*/
	int getActualPolygonSize() const;
	
	/*
		The function getInsideAngle() computes the angle in the interior of the polygon
		(in counter-clockwise orientation) between the to polygon edges incident to the
		vertex.

		@return 	The interior angle of the polygon at this vertex
	*/
	double getInsideAngle() const;


	/*
		R ~ E ~ M ~ O ~ V ~ E ~ R
	*/

	/*
		@param 	e 	Edge to be removed from the edges list
	*/
	void removeEdge(TEdge * const e);

	/*
		@param 	t 	Triangle to be removed from the triangles list
	*/
	void removeTriangle(Triangle * const t);
	

	/*
		P ~ R ~ I ~ N ~ T ~ E ~ R
	*/

	/*
		The function write() writes the information of a vertex as node into a .graphml file.
		The node information contains the ID of the vertex, its coordinates and its ID as
		mainText. This functions also provides a scaling factor for the coordinates as some
		graphml-viewers are not capable of scaling. The setting of the scaling factor is
		trial-and-error.

		@param 	f 		The .graphml file to print in
		@param 	factor 	The used scaling factor

		Note:
			This function just prints one node into a .graphml file, to print the hole
			triangulation use the print functions of the Triangulation class
	*/
	void write(FILE * const f, double factor) const;

	/*
		The function writeToDat() writes the coordinates of a vertex to a .dat file such that it
		can be interpreted by gnuplot.

		@param 	f 	A handle for the .dat file
	*/
	void writeToDat(FILE * const f) const;

	/*
		The function print() prints the basic information of a vertex to standard out.
		It prints the ID and both coordinates in a precision of 15 decimal digits.
	*/
	void print() const;

	/*
		The function writeEnvironment() writes a local part of the triangulation around
		the vertex to a -graphml file. It collects the data recursivelly by the function
		getEnvironment().

		@param 	depth 		The number of recursive steps to be done, basically a depth of n
							means that all vertices with distance less then n to this vertex
							are included
		@param 	filename 	The name of the file to write to
	*/
	void writeEnvironment(const int depth, const char *filename) const;

	/*
		The function writeSurroundingTriangulation() writes a vertex and all vertices and
		edges of its adjacent triangles into a .graphml file.

		@param 	filename 	The name of the .graphml file
	*/
	void writeSurroundingTriangulation(const char *filename) const;

	/*
		The function printStats() prints an overview of created, deleted and still existing
		vertices to stdout.
	*/
	static void printStats();

	
	/*
		O ~ T ~ H ~ E ~ R ~ S
	*/

	/*
		The function check() checks whether the vertex has exactly two polygon edges (except
		the vertex is a vertex of the bounding box) and whether toPrev and toNext are set
		correctly.

		@return 	True if everything is alright, otherwise false
	*/
	bool check();

	/*
		The function stretch() scales the coordinates of a vertex by a fixed factor.

		@param 	factor 	The scaling factor

		Note:
			- The function should just be called from Triangulation::stretch() to stretch
				the whole triangulation, but not stretch a single vertex and expect the
				triangulation to stay correct
			- It is not checked that a simple polygon after stretching is still simple.
				In theory it has to, but for numerical reasons this might fail
	*/
	void stretch(const double factor);

	/*
		The function checkSurroundingPolygon() examines whether the vertex still is inside
		of its surrounding polygon. It errors with exit code 10 if a vertex lays exactly at
		a polygon edge.

		Defintion surrounding polygon:
			Be T the set of triangles containing the vertex v. Be E the set of edges
			contained by the triangles of T. Be E' the subset of E which only contains
			those elements of E which do not contain v.
			Then all elements of E' together form a polygon around v.

			For a correct triangulation it holds that each vertex must be inside of its
			surrounding polygon.

		@return 	True if the vertex is inside of its surrounding polygon, otherwise false
	*/
	bool checkSurroundingPolygon();

	/*
		@return 	The Euclidean distance to the origin
	*/
	double getDistanceToOrigin() const;
	
	
	/*
		D ~ E ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R
	*/

	/*
		Destructor:
		The destructor also delete all triangles and edges which still contain the vertex by
		calling their destructors. Therefore we need copies of the lists in form of arrays
		because iterating over a list while deleting its elements is pretyy dangerous.

		Note:
			It is the best way to delete all edges and triangles containing the vertex
			before deleting the vertex itself such that the edges and triangles lists are
			already empty.
	*/
	~Vertex();
};

#endif