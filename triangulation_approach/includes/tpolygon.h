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
	Triangulation *T;

	/*
		A vector of all vertices belonging to the polygon
	*/
	std::vector<Vertex*> vertices;

	/*
		The ID of the polygon
		It is always assumed that the outer polygon has ID 0 (so it was generated first) and the
		inner polygons have IDs from 1 to n
	*/
	unsigned int id;

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
	TPolygon(Triangulation *t, int n);


	/*
		S ~ E ~ T ~ T ~ E ~ R ~ S
	*/

	/*
		@param	v 	Vertex to be added to the vertices vector
	*/
	void addVertex(Vertex *v);


	/*
		G ~ E ~ T ~ T ~ E ~ R ~ S
	*/

	/*
		@return 	The id of the polygon
	*/
	unsigned int getID();
};

#endif