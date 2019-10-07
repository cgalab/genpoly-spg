/*
	Include standard libraries
*/
#include <vector>


/*
	Include my headers	
*/
#include "settings.h"


#ifndef __TPOLYGON_H_
#define __TPOLYGON_H_

/*
	Define and include headers to the entites used in TPolygon
*/
class Triangulation;
class Vertex;

#include "triangulation.h"
#include "vertex.h"

class TPolygon{

private:

	/*
		The triangulation the polygon lives in
	*/
	Triangulation const * const T;

	/*
		A vector of all vertices belonging to the polygon
	*/
	std::vector<Vertex*> vertices;

	/*
		The ID of the polygon
		It is always assumed that the outer polygon has ID 0 (so it was generated first) and the
		inner polygons have IDs from 1 to n
	*/
	const unsigned int id;

	/*
		The number of already generated polygons
	*/
	static unsigned int N;

public:

	/*
		C ~ O ~ N ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R ~ S
	*/

	/*
		Contructor:
		Already allocates memory for all vertices which will be inserted in the future.

		@param 	t 	The triangulation the new polygon lives in
		@param 	n 	The target number of vertices of the new polygon
	*/
	TPolygon(Triangulation const * const t, const int n);


	/*
		S ~ E ~ T ~ T ~ E ~ R ~ S
	*/

	/*
		@param	v 	Vertex to be added to the vertices vector
	*/
	void addVertex(Vertex * const v);


	/*
		G ~ E ~ T ~ T ~ E ~ R ~ S
	*/

	/*
		@return 	The id of the polygon
	*/
	unsigned int getID() const;

	/*
		@param	i 	Index of the vertex in the vertices vector
		@return 	The vertex at index i in the vertices vector

		Note:
			- Be n the actual number of vertices in the vertex vector, then i < 0 
				returns the vertex with index n + i and i >= n returns the vertex at index
				i - n. This is helpful to get the previous and next vertex while generating
				the initial polygon.
			- This will not work after inserting additional vertices, as the vertices won't be 
				in the same order in the vertices vector as they are in the polygon
	*/
	Vertex *getVertex(const int i) const;

	/*
		@return 	The actual size of the polygon
	*/
	int getActualPolygonSize() const;


	/*
		R ~ E ~ M ~ O ~ V ~ E ~ R
	*/

	/*
		@param 	i 	The index of the vertex to be removed in the vertices list
		@return 	The vertex to be removed
	*/
	Vertex *removeVertex(const int i);
};

#endif