#include "triangulation.h"

/*
	C ~ O ~ N ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R ~ S
*/

/*
	Constructor:
	Already allocates memory for the vector of vertices

	@param 	n 	Final number of vertices (including the vertices of inner polygons)
*/
Triangulation::Triangulation(int n) :
	Rectangle0(NULL), Rectangle1(NULL), Rectangle2(NULL), Rectangle3(NULL), N(n) { 
	
	vertices.reserve(N);
}



/*
	S ~ E ~ T ~ T ~ E ~ R ~ S
*/

/*
	@param	v 	Vertex to be added to the vertices vector
*/
void Triangulation::addVertex(Vertex *v){
	vertices.push_back(v);

	// Do not forget the register the triangulation at the vertex
	(*v).setTriangulation(this);
}

/*
	@param	e 	Edge to be added to the edge map
*/
void Triangulation::addEdge(TEdge *e){
	edges.insert(std::pair<int, TEdge*>((*e).getID(), e));

	// Do not forget the register the triangulation at the edge
	(*e).setTriangulation(this);
}

/*
	The function setRectangle() sets the vertices of the Rectangle0, ..., Rectangle3 of
	bounding box for the polygons

	@param	v0, v1, v2, v3 	Vertices building the bounding box for the polygon (ordering
							doesn't matter as the vertices are connected by their edges)
*/
void Triangulation::setRectangle(Vertex *v0, Vertex *v1, Vertex *v2, Vertex *v3){
	Rectangle0 = v0;
	Rectangle1 = v1;
	Rectangle2 = v2;
	Rectangle3 = v3;

	// Do not forget the register the triangulation at the vertices
	(*v0).setTriangulation(this);
	(*v1).setTriangulation(this);
	(*v2).setTriangulation(this);
	(*v3).setTriangulation(this);
}



/*
	G ~ E ~ T ~ T ~ E ~ R ~ S
*/

/*
	@return		Final number of vertices the polygon will contain (including the vertices
				of inner polygons)
*/
int Triangulation::getTargetNumberOfVertices(){
	return N;
}

/*
	@return		The number of vertices the polygon does contain now (including the vertices
				of inner polygons)
*/
int Triangulation::getActualNumberOfVertices(){
	return vertices.size();
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
Vertex* Triangulation::getVertex(int i){ 	
	int n;

	n = vertices.size();

	if(i < 0){
		return vertices[n + i];
	}else if(i >= n){
		return vertices[i - n];
	}else{
		return vertices[i];
	}
	 return NULL;
}



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
void Triangulation::removeVertex(int index){
	vertices[index] = NULL;
}

/*
	The function removeEdge() searches one edge by its ID in the edges map and removes it

	@param	e 	The edge to be removed
*/
void Triangulation::removeEdge(TEdge *e){
	edges.erase((*e).getID());
}



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
void Triangulation::print(const char *filename){
	FILE *f;
	TEdge *e;
	int scale = 5000;

	f = fopen(filename, "w");

	// Print the graphml header
	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<graphml>\n");
	fprintf(f, "<graph id=\"Graph\" edgeDefault=\"undirected\">\n");

	// Print all nodes
	fprintf(f, "<nodes>\n");

	// Start with the bounding box
	(*Rectangle0).print(f, scale);
	(*Rectangle1).print(f, scale);
	(*Rectangle2).print(f, scale);
	(*Rectangle3).print(f, scale);

	// Then all polygon vertices
	for(auto const& i : vertices){
		if(i != NULL) (*i).print(f, scale);
	}
	fprintf(f, "</nodes>\n");

	// Print all edges from the edge map
	fprintf(f, "<edges>\n");
	for(auto const& i : edges){
		e = i.second;
		(*e).print(f);
	}
	fprintf(f, "</edges>\n");

	fprintf(f, "</graph>\n");
	fprintf(f, "</graphml>\n");

	fclose(f);
}

/*
	The function printPolygon() prints just the polygon in .graphml style into a file

	@param	filename	The name of the .graphml file

	Note:
		- Graphml: https://de.wikipedia.org/wiki/GraphML
		- Works here: http://graphonline.ru/en/
*/
void Triangulation::printPolygon(const char *filename){
	FILE *f;
	TEdge *e;

	f = fopen(filename, "w");

	// Print the graphml header
	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<graphml>\n");
	fprintf(f, "<graph id=\"Graph\" edgeDefault=\"undirected\">\n");

	// Print all polygon nodes
	fprintf(f, "<nodes>\n");
	for(auto const& i : vertices){
		if(i != NULL && !(*i).isRectangleVertex()) (*i).print(f, 0);
	}
	fprintf(f, "</nodes>\n");

	// Print all polygon edges
	// TODO:
	// Make this more efficient by using the function Vertex::getToNext()
	fprintf(f, "<edges>\n");
	for(auto const& i : edges){
		e = i.second;
		if((*e).getEdgeType() == EdgeType::POLYGON)(*e).print(f);
	}
	fprintf(f, "</edges>\n");

	fprintf(f, "</graph>\n");
	fprintf(f, "</graphml>\n");

	fclose(f);
}



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
bool Triangulation::check(){
	EdgeType type;
	int n;
	TEdge *e;
	bool ok = true;
	bool part;

	for(auto const& i : edges){
		e = i.second;
		type = (*e).getEdgeType();
		n = (*e).nrAssignedTriangles();

		// Check the number of triangles for each edge
		if(type == EdgeType::FRAME){
			if(n != 1){
				printf("Edge of type FRAME with %d triangles:\n \t", n);
				(*e).print();
				ok = false;
			}
		}else{
			if(n != 2){
				printf("Edge of type not FRAME with %d triangles:\n \t", n);
				(*e).print();
				ok = false;
			}
		}

		// Check whether there is a circle edge
		if((*e).getV0() == (*e).getV1()){
			printf("Edge %llu has two identical vertices with id %llu \n", (*e).getID(), (*(*e).getV1()).getID());
			ok = false;
		}		
	}

	for(auto const& i : vertices){
		// Check whether each vertex as a next and a previous vertex
		ok = ok && (*i).check();
		// Check whether each vertex lives inside its surrounding polygon
		part = (*i).checkSurroundingPolygon();
		ok = ok && part;

		if(!part){
			printf("Triangulation error: vertex %llu is outside of its surrounding polygon\n", (*i).getID());
		}
	}

	// Check the simplicity of the polygon
	checkSimplicity();

	return ok;
}

/*
	The function stretch() stretches the whole polygon by a constant factor, i.e. the
	x- and y-coordinates of all vertices get multiplied by the factor.

	@param	factor	The factor by which all vertex coordinates get multiplied

	Note:
		- It is not checked, whether this operations is numerically stable!
		- It is not used anywhere at the moment
*/
void Triangulation::stretch(double factor){
	(*Rectangle0).stretch(factor);
	(*Rectangle1).stretch(factor);
	(*Rectangle2).stretch(factor);
	(*Rectangle3).stretch(factor);

	for(auto const& i : vertices){
		(*i).stretch(factor);
	}
}

/*
	The function checkSimplicity() checks whether a polygon is simple by checking for self-
	intersections. So it basically compares each edge with each other edge. To do so it uses
	two nested for-loops: The outer one loops over all edges, whereas the inner one just loops
	over the edges which have not been compared yet with the edge of the outer one. If it finds
	any intersection it errors with exit code 11.

	Note:
		This function does not consider edges of other polygons!
*/
void Triangulation::checkSimplicity(){
	Vertex *v, *u, *w;
	TEdge *toCheck; // The edge which is compared to all others at the moment
	TEdge *otherEdge;
	int n; // Number of following edges toCheck has to be compared with
	int i;
	IntersectionType type;

	// Get the first edge to be compared with all others
	v = vertices[0];
	toCheck = (*v).getToNext();

	// Get the next vertex
	u = (*v).getNext();

	// Get the number of following edges the first edge has to be compared with,
	// which is the actual number of vertices in the polygon minus 3 (the edge itself and
	// its two neighbors)
	n = getActualNumberOfVertices() - 3;

	// Iterate over all edges
	// Take into account that the first and the second edge have to be compared with the same
	// number of edges
	n++;
	i = 1;
	while(n > 0){
		// The first edge to compare with is the next but one
		u = (*u).getNext();
		otherEdge = (*u).getToNext();
		w = u;

		// Compare against the other edges
		while(i < n){
			
			type = checkIntersection(toCheck, otherEdge, true);

			if(type != IntersectionType::NONE)
				exit(11);

			// Get the next edge to check with
			w = (*w).getNext();
			otherEdge = (*w).getToNext();

			i++;
		}
		i = 0;

		// The next edge has to be checked against one other edge less
		n--;
		printf("n: %d\n", n);

		// Get the next edge
		v = (*v).getNext();
		toCheck = (*v).getToNext();
	}
}