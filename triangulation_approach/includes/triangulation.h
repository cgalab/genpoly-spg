/*
	Include standard libraries
*/
#include <vector>
#include <stdio.h>
#include <map>

/*
	Include my headers	
*/
#include "settings.h"

/*
	Define the class Triangulation
*/
#ifndef __TRIANGULATION_H_
#define __TRIANGULATION_H_

/*
	Define and include headers to the entites contained by triangulations
*/
class Vertex;
class TEdge;
class Triangle;
class TPolygon;

#include "vertex.h"
#include "tedge.h"
#include "triangle.h"
#include "tpolygon.h"

class Triangulation{

private:

	/*
		The polygons living in the triangulation
	*/
	TPolygon *outerPolygon;
	std::vector<TPolygon*> innerPolygons;
	
	/*
		A vector of all polygon vertices contained by the triangulation
	*/
	std::vector<Vertex*> vertices;

	/*
		A map of all edges contained by the triangulation
		Note:
			- Keeping this is not necessary for running the polygon generator, but it's pretty 
				useful for debuging, because otherwise it's hard to print the whole triangulation
			- An unordered map with a hashtable could be faster, but I do not know how efficient 
				the deletion of elements is implemented their
	*/
	// TODO:
	// Create something to disable this
	std::map<int, TEdge*> edges;

	/*
		Vertices of the bounding box
	*/
	Vertex *Rectangle0;
	Vertex *Rectangle1;
	Vertex *Rectangle2;
	Vertex *Rectangle3;

	/*
		The target number of polygon vertices (including the vertices of inner polygons)
	*/
	int N;

public:
	
	/*
		LIST OF PUBLIC MEMBER FUNCTIONS

		CONSTRUCTORS:

					Triangulation()

		SETTER:

		void 		addVertex(Vertex *v, unsigned int pID)
		void 		addEdge(TEdge *e)
		void 		setRectangle(Vertex *v0, Vertex *v1, Vertex *v2, Vertex *v3)

		GETTER:

		int 		getTargetNumberOfVertices()
		int 		getActualNumberOfVertices()
		Vertex*		getVertex(int i, unsigned int pID)
		Vertex* 	getVertex(int i)

		REMOVER:

		void 		removeVertex(int index)
		void 		removeEdge(TEdge *e)

		PRINTER:

		void 		print(const char *filename)
		void 		printPolygon(const char *filename)

		OTHERS:

		bool 		check()
		void 		stretch(double factor)
		void		checkSimplicity()
	*/


	/*
		C ~ O ~ N ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R ~ S
	*/

	/*
		Constructor:
		Already allocates memory for the vector of vertices and generates the TPolygon
		instances.
	*/
	Triangulation();


	/*
		S ~ E ~ T ~ T ~ E ~ R ~ S
	*/

	/*
		The function addVertex() inserts a vertex into the vertices list of the triangulation
		and also calls the addVertex() function of the polygon the vertex belongs to. It errors
		with exit code 12 if pID is greater then the number of inner polygons.

		@param	v 		Vertex to be added to the vertices vector
		@param 	pID 	The polygon the vertex belongs to (pID = 0 outer polygon, else inner
						polygon)
	*/
	void addVertex(Vertex *v, unsigned int pID);

	/*
		@param	e 	Edge to be added to the edge map
	*/
	void addEdge(TEdge *e);

	/*
		The function setRectangle() sets the vertices of the Rectangle0, ..., Rectangle3 of
		bounding box for the polygons

		@param	v0, v1, v2, v3 	Vertices building the bounding box for the polygon (ordering
								doesn't matter as the vertices are connected by their edges)
	*/
	void setRectangle(Vertex *v0, Vertex *v1, Vertex *v2, Vertex *v3);


	/*
		G ~ E ~ T ~ T ~ E ~ R ~ S
	*/

	/*
		@return		Final number of vertices the polygon will contain (including the vertices
					of inner polygons)
	*/
	int getTargetNumberOfVertices();

	/*
		@return		The number of vertices the polygon does contain now (including the vertices
					of inner polygons)
	*/
	int getActualNumberOfVertices();

	/*
		@param 	pID The polygon of interest
		@return		The number of vertices the polygon with pID does contain now
	*/
	int getActualNumberOfVertices(unsigned int pID);

	/*
		@param	i 	Index of the vertex in the vertices vector of the polygon with pID
		@param 	pID	The ID of the polygon of interest
		@return 	The vertex at index i in the vertices vector, NULL if no polygon with pID
					exists

		Note:
			- Be n the actual number of vertices in the vertex vector, then i < 0 
				returns the vertex with index n + i and i >= n returns the vertex at index
				i - n. This is helpful to get the previous and next vertex while generating
				the initial polygon.
			- This will not work after inserting additional vertices, as the vertices won't be 
				in the same order in the vertices vector as they are in the polygon
	*/
	Vertex *getVertex(int i, unsigned int pID);

	/*
		@param 	i 	The index of the vertex in the vertices vector
		@return 	The vertex at index i in the vertices vector
	*/
	Vertex *getVertex(int i);


	/*
		R ~ E ~ M ~ O ~ V ~ E ~ R
	*/

	/*
		The function removeVertex() removes one vertex from the vector of vertices and sets the
		entry NULL

		@param	i 	Index of the vertex to be removed

		Note:
			- This function is just for debugging purposes and should normally not be used
				anywhere in the code
	*/
	void removeVertex(int i);

	/*
		The function removeEdge() searches one edge by its ID in the edges map and removes it

		@param	e 	The edge to be removed
	*/
	void removeEdge(TEdge *e);

	
	/*
		P ~ R ~ I ~ N ~ T ~ E ~ R
	*/

	/*
		The function print() prints the whole triangulation in .graphml style into a file

		@param	filename	The name of the .graphml file

		Note:
			- This function just works, if the triangulation stores all edges
			- Graphml: https://de.wikipedia.org/wiki/GraphML
			- Works here: http://graphonline.ru/en/
			- This crappy website is the reason why we need the scaling factor here
	*/
	void print(const char *filename);

	/*
		The function printPolygon() prints just the polygon in .graphml style into a file

		@param	filename	The name of the .graphml file

		Note:
			- Graphml: https://de.wikipedia.org/wiki/GraphML
			- Works here: http://graphonline.ru/en/
	*/
	void printPolygon(const char *filename);


	/*
		O ~ T ~ H ~ E ~ R ~ S
	*/

	/*
		The function check() checks for errors in the triangulation. It checks the following
		criterions:
			- Has each edge of the bounding box exactly one triangle assigned
			- Has each other edge exactly two triangles assigned
			- Has each edge to different vertices assigned
			- Has each vertex a previous and a next vertex connected by a polygon edge
			- Stays each vertex inside of its surrounding polygon

		@return 	true if everything is alright, otherwise false
	*/
	bool check();

	/*
		The function stretch() stretches the whole polygon by a constant factor, i.e. the
		x- and y-coordinates of all vertices get multiplied by the factor.

		@param	factor	The factor by which all vertex coordinates get multiplied

		Note:
			- It is not checked, whether this operations is numerically stable!
			- It is not used anywhere at the moment
	*/
	void stretch(double factor);

	/*
	The function checkSimplicity() checks whether a polygon is simple by checking for self-
	intersections. So it basically compares each edge with each other edge. To do so it uses
	two nested for-loops: The outer one loops over all edges, whereas the inner one just loops
	over the edges which have not been compared yet with the edge of the outer one. If it finds
	any intersection it errors with exit code 11.

	Note:
		This function does not consider edges of other polygons!
*/
	void checkSimplicity();
};

#endif