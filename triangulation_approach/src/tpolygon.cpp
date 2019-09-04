#include "tpolygon.h"

/*
	S ~ T ~ A ~ T ~ I ~ C 	V ~ A ~ R ~ I ~ A ~ B ~ L ~ E ~ S
*/

/*
	The number of already generated polygons
*/
unsigned int TPolygon::N = 0;


/*
	C ~ O ~ N ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R ~ S
*/

/*
	Contructor:
	Already allocates memory for all vertices which will be inserted in the future.

	@param 	t 	The triangulation the new polygon lives in
	@param 	n 	The target number of vertices of the new polygon
*/
TPolygon::TPolygon(Triangulation *t, int n) :
	T(t), id(N) {

	vertices.reserve(n);

	N++;
}


/*
	S ~ E ~ T ~ T ~ E ~ R ~ S
*/

/*
	@param	v 	Vertex to be added to the vertices vector
*/
void TPolygon::addVertex(Vertex *v){
	vertices.push_back(v);

	(*v).setPolygon(this);
}


/*
	G ~ E ~ T ~ T ~ E ~ R ~ S
*/

/*
	@return 	The id of the polygon
*/
unsigned int TPolygon::getID(){
	return id;
}