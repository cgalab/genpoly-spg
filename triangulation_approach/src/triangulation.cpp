#include "triangulation.h"

/*
	C ~ O ~ N ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R ~ S
*/

/*
	Constructor:
	Already allocates memory for the vector of vertices and generates the TPolygon
	instances for the outer polygon.
*/
Triangulation::Triangulation() :
	Rectangle0(NULL), Rectangle1(NULL), Rectangle2(NULL), Rectangle3(NULL), N(0) { 

	// Calculate the total number of vertices
	N = Settings::outerSize;
	for(auto const& i : Settings::innerSizes)
		N = N + i;

	vertices.reserve(N);

	// Generate outer polygon instance
	outerPolygon = new TPolygon(this, Settings::outerSize);

	innerPolygons.reserve(Settings::nrInnerPolygons);
}



/*
	S ~ E ~ T ~ T ~ E ~ R ~ S
*/

/*
	@param 	p 	The new inner polygon
*/
void Triangulation::addInnerPolygon(TPolygon *p){
	innerPolygons.push_back(p);
}

/*
	The function addVertex() inserts a vertex into the vertices list of the triangulation
	and also calls the addVertex() function of the polygon the vertex belongs to. It errors
	with exit code 12 if pID is greater then the number of inner polygons.

	@param	v 		Vertex to be added to the vertices vector
	@param 	pID 	The polygon the vertex belongs to (pID = 0 outer polygon, else inner
					polygon)
*/
void Triangulation::addVertex(Vertex *v, unsigned int pID){
	vertices.push_back(v);

	if(pID == 0)
		(*outerPolygon).addVertex(v);
	else if(pID > 0 && pID <= getActualNrInnerPolygons())
		(*innerPolygons[pID - 1]).addVertex(v);
	else
		exit(12);

	// Do not forget to register the triangulation at the vertex
	(*v).setTriangulation(this);
}

/*
	The function changeVertex() removes the vertex at index i from the polygon with ID
	fromP and adds it to the polygon with ID toP.

	@param	i 		The index of the vertex to be moved in the polygon with ID fromP
	@param 	fromP 	The ID of the polygon the vertex lives in originally
	@param 	toP 	The ID of the polygon the vertex should be moved to
*/
void Triangulation::changeVertex(int i, unsigned int fromP, unsigned int toP){
	Vertex *v;

	if(fromP == 0)
		v = (*outerPolygon).removeVertex(i);
	else if(fromP > 0 && fromP <= getActualNrInnerPolygons())
		v = (*innerPolygons[fromP - 1]).removeVertex(i);
	else
		exit(12);

	if(toP == 0)
		(*outerPolygon).addVertex(v);
	else if(toP > 0 && toP <= getActualNrInnerPolygons())
		(*innerPolygons[toP - 1]).addVertex(v);
	else
		exit(12);
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
	@return 	The actual number of inner polygons
*/
int Triangulation::getActualNrInnerPolygons(){
	return innerPolygons.size();
}

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
	@param 	pID The polygon of interest
	@return		The number of vertices the polygon with pID does contain now
*/
int Triangulation::getActualNumberOfVertices(unsigned int pID){
	
	if(pID == 0)
		return (*outerPolygon).getActualPolygonSize();
	else if(pID > 0 && pID <= Settings::nrInnerPolygons)
		return (*innerPolygons[pID - 1]).getActualPolygonSize();
	else
		return -1;
}

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
Vertex *Triangulation::getVertex(int i, unsigned int pID){ 	
	
	if(pID == 0)
		return (*outerPolygon).getVertex(i);
	else if(pID > 0 && pID <= Settings::nrInnerPolygons)
		return (*innerPolygons[pID - 1]).getVertex(i);
	else
		return NULL;
}

/*
	@param 	i 	The index of the vertex in the vertices vector
	@return 	The vertex at index i in the vertices vector
*/
Vertex *Triangulation::getVertex(int i){
	return vertices[i];
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
	int scale = 2000;

	f = fopen(filename, "w");

	// Print the graphml header
	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<graphml>\n");
	fprintf(f, "<graph id=\"Graph\" edgeDefault=\"undirected\">\n");

	// Print all nodes
	fprintf(f, "<nodes>\n");

	// Start with the bounding box
	if(Rectangle0 != NULL){
		(*Rectangle0).print(f, scale);
		(*Rectangle1).print(f, scale);
		(*Rectangle2).print(f, scale);
		(*Rectangle3).print(f, scale);
	}

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
	int scale = 1000;

	f = fopen(filename, "w");

	// Print the graphml header
	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<graphml>\n");
	fprintf(f, "<graph id=\"Graph\" edgeDefault=\"undirected\">\n");

	// Print all polygon nodes
	fprintf(f, "<nodes>\n");
	for(auto const& i : vertices){
		if(i != NULL && !(*i).isRectangleVertex())
			(*i).print(f, scale);
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
	The function printPolygonToDat() prints all polygons to a .dat file which can be
	interpreted by gnuplot.

	@param 	filename 	The name of the .dat file
*/
void Triangulation::printPolygonToDat(const char *filename){
	Vertex *start, *other;
	FILE *f;
	int id = 0;

	f = fopen(filename, "w");

	// Start with the outer polygon
	fprintf(f, "\"outer polygon\"\n");

	start = (*outerPolygon).getVertex(0);
	(*start).printToDat(f);

	other = (*start).getNext();
	while((*start).getID() != (*other).getID()){
		(*other).printToDat(f);

		other = (*other).getNext();
	}
	(*start).printToDat(f);

	for(auto const& i : innerPolygons){
		// Add all inner polygons
		fprintf(f, "\n\n\"inner polygon %d\"\n", id);

		start = (*i).getVertex(0);
		(*start).printToDat(f);

		other = (*start).getNext();
		while((*start).getID() != (*other).getID()){
			(*other).printToDat(f);

			other = (*other).getNext();
		}
		(*start).printToDat(f);

		id++;
	}
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
	if(Settings::simplicityCheck)
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

		// Get the next edge
		v = (*v).getNext();
		toCheck = (*v).getToNext();
	}
}