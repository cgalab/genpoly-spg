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
TPolygon::TPolygon(Triangulation const * const t, const int n) :
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
void TPolygon::addVertex(Vertex * const v){
	vertices.push_back(v);

	(*v).setPolygon(this);
}


/*
	G ~ E ~ T ~ T ~ E ~ R ~ S
*/

/*
	@return 	The id of the polygon
*/
unsigned int TPolygon::getID() const{
	return id;
}

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
Vertex *TPolygon::getVertex(const int i) const{ 	
	int n;

	n = vertices.size();

	/*if(i < 0){
		return vertices[n + i];
	}else if(i >= n){
		return vertices[i - n];
	}else{
		return vertices[i];
	}
	return NULL;*/

	return vertices[i % n];
}

/*
	@return 	The actual size of the polygon
*/
int TPolygon::getActualPolygonSize() const{
	return vertices.size();
}


/*
	R ~ E ~ M ~ O ~ V ~ E ~ R
*/

/*
	@param 	i 	The index of the vertex to be removed in the vertices list
	@return 	The vertex to be removed
*/
Vertex *TPolygon::removeVertex(const int i){
	Vertex *v;

	v = vertices[i];

	vertices.erase(vertices.begin() + i);

	return v;
}