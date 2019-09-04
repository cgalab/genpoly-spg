#include "initialPolygonGenerator.h"

/*
	The function generateRegularPolygon at first generates a new triangulation
	with the vertices of the start polygon in regular shape, i.e. all vertices 
	lay on a circle.
	Afterwards the the start polygon is triangulated, i.e. the polygon edges are
	added and the inner of the polygon is triangulated, then the polygon is boxed
	by a square with a triangulation between the polygon and the square.

	The radius of the start polygon, the number of vertices and the size of the
	box are globally defined in settings.

	@return		The generated triangulation
*/
Triangulation *generateRegularPolygon(){
	double alpha; // The angular distance between the vertices
	int i;
	Vertex *v;
	Triangulation* T = new Triangulation();

	alpha = 2 * M_PI / Settings::initialSize;

	// Distribute the vertices equally on a circle
	for(i = 0; i < Settings::initialSize; i++){
		v = new Vertex(Settings::radiusPolygon * cos(i * alpha), Settings::radiusPolygon * sin(i * alpha));
		(*T).addVertex(v, 0);
	}

	// Triangulate the inner of the polygon in zig-zag style
	if(Settings::nrInnerPolygons == 0)
		initialTriangulationZigZag(T);
	else
		generateInitialHoleTriangle(T);

	printf("reached end of inner triangulation\n");

	// Box the polygon by a square and triangulate the area between the square and the polygon
	boxPolygon(T, 0);

	return T; 	
}


/*
	The function initialTriangulationPseudoStar adds the polygon edges to the
	vertices of a regular shaped polygon with no holes. Then it moves the
	vertex 0 to the center of the circle and triangulates the inner of the
	polygon by connecting each other vertex to the vertex in the center.

	Note:
	In principle this approach would work, but it gets very slow for larger
	initial polygons as each vertex shares an edge with the one center vertex
	so the edges list of the center vertex becomes very slow.

	@param	T 	The triangulation containing the already generated but not
				linked vertices of the start polygon
*/
void initialTriangulationPseudoStar(Triangulation* T){
	int i;
	Vertex *center, *v0, *v1;
	TEdge *e0, *e1, *e2, *start;

	// Move vertex 0 to the origin
	center = (*T).getVertex(0, 0);
	(*center).setPosition(0, 0);

	v0 = (*T).getVertex(1, 0);
	v1 = (*T).getVertex(2, 0);

	e0 = new TEdge(v0, v1, EdgeType::POLYGON); // e0 is the polygon edge
	e1 = new TEdge(v1, center); // e1 is the edge of the higher vertex to the center
	start = new TEdge(center, v0, EdgeType::POLYGON);
	(*T).addEdge(e0);
	(*T).addEdge(e1);
	(*T).addEdge(start);

	new Triangle(e0, e1, start, v0, v1, center);

	for(i = 3; i < Settings::initialSize; i++){
		v0 = v1;
		v1 = (*T).getVertex(i, 0);

		e0 = new TEdge(v0, v1, EdgeType::POLYGON);
		e2 = e1;
		e1 = new TEdge(v1, center);

		(*T).addEdge(e0);
		(*T).addEdge(e1);

		new Triangle(e0, e1, e2, v0, v1, center);
	}

	// At the end we have to insert a triangle outside of the polygon such that
	// the triangulation forms the convex hull of the vertices
	v0 = (*T).getVertex(1, 0);
	(*e1).setEdgeType(EdgeType::POLYGON);
	e2 = new TEdge(v0, v1);
	(*T).addEdge(e2);
	new Triangle(e1, e2, start, v0, v1, center);
}


/*
	The function initialTriangulationZigZag adds the polygon edges to the
	vertices of a regular shaped polygon with no holes and triangulates the
	inner of the polygon in zig-zag style (for more information on the zig-zag
	style take a look at my master thesis).

	Note:
	The suitable startindex for the box polygon function is 0!
*/
void initialTriangulationZigZag(Triangulation *T){
	int i;
	Vertex *v0 = NULL, *v1 = NULL, *v2 = NULL;
	TEdge *e0 = NULL, *e1 = NULL, *e2 = NULL;

	// The first edge connects vertex 0 and vertex n-1 to close the circle
	v0 = (*T).getVertex(0, 0);
	v1 = (*T).getVertex(Settings::initialSize - 1, 0);
	e0 = new TEdge(v1, v0, EdgeType::POLYGON);
	(*T).addEdge(e0);

	// The inital triangulation contains n-2 triangles
	for(i = 0; i < Settings::initialSize - 2; i++){

		if(i % 2 == 0){
			// v0 stays as it is and v1 becomes v2
			v2 = v1;
			v1 = (*T).getVertex(i / 2 + 1, 0);

			// e0 becomes e2
			e2 = e0;
			e0 = new TEdge(v0, v1, EdgeType::POLYGON);
			e1 = new TEdge(v1, v2);

			(*T).addEdge(e0);
			(*T).addEdge(e1);

			new Triangle(e0, e1, e2, v0, v1, v2);

		}else{
			// v1 becomes v0 and v2 stays as it is
			v0 = v1;
			v1 = (*T).getVertex(Settings::initialSize - i / 2 - 2, 0);

			// e1 becomes e2
			e2 = e1;
			e0 = new TEdge(v0, v1);
			e1 = new TEdge(v1, v2, EdgeType::POLYGON);

			(*T).addEdge(e0);
			(*T).addEdge(e1);

			new Triangle(e0, e1, e2, v0, v1, v2);
		}
	}

	// To close the polygon it makes a difference whether the number of veritces
	// is even or odd
	if(Settings::initialSize % 2 == 0){
		(*e0).setEdgeType(EdgeType::POLYGON);
	}else{
		(*e1).setEdgeType(EdgeType::POLYGON);
	}
}


/*
	The function generateInitialHoleTriangle() generates a triangular shaped hole
	in the middle of the start polygon and triangulates the start polygon with the
	hole in it.

	@param	T 	The triangulation already containing the vertices of the outer polygon

	Note:
		The suitable startindex for the boxPolygon function is 0!
*/
void generateInitialHoleTriangle(Triangulation *T){
	double alpha;
	int i;
	Vertex *triangleV0, *triangleV1, *triangleV2;
	Vertex *v0, *v1;
	TEdge *e0, *e1, *e2;
	TEdge *start, *triangleE0, *triangleE1, *triangleE2;

	alpha = 2 * M_PI / 3;

	// Generate the vertices of the inner triangle
	triangleV0 = new Vertex(Settings::radiusHole, 0);
	triangleV1 = new Vertex(Settings::radiusHole * cos(alpha), Settings::radiusHole * sin(alpha));
	triangleV2 = new Vertex(Settings::radiusHole * cos(2 * alpha), Settings::radiusHole * sin(2 * alpha));
	(*T).addVertex(triangleV0, 1);
	(*T).addVertex(triangleV1, 1);
	(*T).addVertex(triangleV2, 1);
		
	v0 = (*T).getVertex(0, 0);
	v1 = (*T).getVertex(1, 0);

	e0 = new TEdge(v0, v1, EdgeType::POLYGON);
	e1 = new TEdge(v0, triangleV0);
	e2 = new TEdge(v1, triangleV0);
	(*T).addEdge(e0);
	(*T).addEdge(e1);
	(*T).addEdge(e2);

	start = e1;

	new Triangle(e0, e1, e2, v0, v1, triangleV0);

	for(i = 2; i <= Settings::initialSize / 3; i++){
		v0 = v1;
		v1 = (*T).getVertex(i, 0);

		e0 = new TEdge(v0, v1, EdgeType::POLYGON);
		e1 = e2;
		e2 = new TEdge(v1, triangleV0);
		(*T).addEdge(e0);
		(*T).addEdge(e2);

		new Triangle(e0, e1, e2, v0, v1, triangleV0);
	}

	e0 = new TEdge(triangleV0, triangleV1, EdgeType::POLYGON);
	e1 = e2;
	e2 = new TEdge(v1, triangleV1);
	(*T).addEdge(e0);
	(*T).addEdge(e2);

	new Triangle(e0, e1, e2, v1, triangleV0, triangleV1);

	triangleE0 = e0;

	for(; i <= 2 * Settings::initialSize / 3; i++){
		v0 = v1;
		v1 = (*T).getVertex(i, 0);

		e0 = new TEdge(v0, v1, EdgeType::POLYGON);
		e1 = e2;
		e2 = new TEdge(v1, triangleV1);
		(*T).addEdge(e0);
		(*T).addEdge(e2);

		new Triangle(e0, e1, e2, v0, v1, triangleV1);
	}

	e0 = new TEdge(triangleV1, triangleV2, EdgeType::POLYGON);
	e1 = e2;
	e2 = new TEdge(v1, triangleV2);
	(*T).addEdge(e0);
	(*T).addEdge(e2);

	new Triangle(e0, e1, e2, v1, triangleV1, triangleV2);

	triangleE1 = e0;

	for(; i <= Settings::initialSize - 1; i++){
		v0 = v1;
		v1 = (*T).getVertex(i, 0);

		e0 = new TEdge(v0, v1, EdgeType::POLYGON);
		e1 = e2;
		e2 = new TEdge(v1, triangleV2);
		(*T).addEdge(e0);
		(*T).addEdge(e2);

		new Triangle(e0, e1, e2, v0, v1, triangleV2);
	}

	e0 = new TEdge(triangleV2, triangleV0, EdgeType::POLYGON);
	e1 = e2;
	e2 = new TEdge(v1, triangleV0);
	(*T).addEdge(e0);
	(*T).addEdge(e2);

	new Triangle(e0, e1, e2, v1, triangleV2, triangleV0);

	triangleE2 = e0;

	new Triangle(triangleE0, triangleE1, triangleE2, triangleV0, triangleV1, triangleV2);

	e0 = new TEdge(v1, (*T).getVertex(0, 0), EdgeType::POLYGON);
	(*T).addEdge(e0);

	new Triangle(e0, e2, start, v1, (*T).getVertex(0, 0), triangleV0);
}


/*
	The function boxPolygon generates a square around the polygon and
	triangulates the area between the polygon and the square.

	@param	T 			The triangulation containing the polygon and the 
						triangulation inside the polygon
	@param	startIndex 	The index of the first vertex being placed at the circle
						of vertices (normally each vertex is placed at the circle
						so the startindex can be zero, but for some special inner
						triangulations, e.g. pseudoStar, the vertex 0 is not at 
						the	circle)
*/
void boxPolygon(Triangulation* T, int startIndex){
	// Vertices and edges of the rectangle
	Vertex *rv0, *rv1, *rv2, *rv3;
	TEdge *re0, *re1, *re2, *re3;
	// End indices of one quadrant
	int limit0, limit1, limit2;

	int i;
	TEdge *start, *prev, *next;
	Vertex *v0, *v1;

	/* 
		Generate the rectangle int the following orientation:

		1 - 0
		|   |
		2 - 3
	*/
	// Vertices
	rv0 = new Vertex(Settings::boxSize / 2, Settings::boxSize / 2, true);
	rv1 = new Vertex(- Settings::boxSize / 2, Settings::boxSize / 2, true);
	rv2 = new Vertex(- Settings::boxSize / 2, - Settings::boxSize / 2, true);
	rv3 = new Vertex(Settings::boxSize / 2, - Settings::boxSize / 2, true);
	(*T).setRectangle(rv0, rv1, rv2, rv3);

	// Edges
	re0 = new TEdge(rv0, rv1, EdgeType::FRAME);
	re1 = new TEdge(rv1, rv2, EdgeType::FRAME);
	re2 = new TEdge(rv2, rv3, EdgeType::FRAME);
	re3 = new TEdge(rv3, rv0, EdgeType::FRAME);
	(*T).addEdge(re0);
	(*T).addEdge(re1);
	(*T).addEdge(re2);
	(*T).addEdge(re3);

	// Calculate the last vertex for each quadrant
	limit0 = (Settings::initialSize + 1) / 4;
	limit1 = Settings::initialSize / 2;
	limit2 = 3 * Settings::initialSize / 4;

	// First quadrant
	v0 = (*T).getVertex(startIndex, 0);
	start = new TEdge(v0, rv0);
	(*T).addEdge(start);
	prev = start;

	for(i = startIndex + 1; i <= limit0; i++){
		v1 = (*T).getVertex(i, 0);
		next = new TEdge(v1, rv0);
		(*T).addEdge(next);

		new Triangle(prev, (*v0).getEdgeTo(v1), next, v0, v1, rv0);

		v0 = v1;
		prev = next;
	}

	// Second quadrant

	// Close the gap between first and second quadrant
	next = new TEdge(v0, rv1);
	(*T).addEdge(next);
	new Triangle(prev, next, re0, v0, rv0, rv1);
	prev = next;

	for(; i <= limit1; i++){
		v1 = (*T).getVertex(i, 0);
		next = new TEdge(v1, rv1);
		(*T).addEdge(next);

		new Triangle(prev, (*v0).getEdgeTo(v1), next, v0, v1, rv1);

		v0 = v1;
		prev = next;
	}

	// Third quadrant

	// Close the gap between second and third quadrant
	next = new TEdge(v0, rv2);
	(*T).addEdge(next);
	new Triangle(prev, next, re1, v0, rv1, rv2);
	prev = next;

	for(; i <= limit2; i++){
		v1 = (*T).getVertex(i, 0);
		next = new TEdge(v1, rv2);
		(*T).addEdge(next);

		new Triangle(prev, (*v0).getEdgeTo(v1), next, v0, v1, rv2);

		v0 = v1;
		prev = next;
	}

	// Fourth quadrant

	// Close the gap between third and fourth quadrant
	next = new TEdge(v0, rv3);
	(*T).addEdge(next);
	new Triangle(prev, next, re2, v0, rv2, rv3);
	prev = next;

	for(; i < Settings::initialSize; i++){
		v1 = (*T).getVertex(i, 0);
		next = new TEdge(v1, rv3);
		(*T).addEdge(next);

		new Triangle(prev, (*v0).getEdgeTo(v1), next, v0, v1, rv3);

		v0 = v1;
		prev = next;
	}

	// Close the gap between the first and the fourth quadrant
	v1 = (*T).getVertex(startIndex, 0);
	next = new TEdge(v1, rv3);
	(*T).addEdge(next);

	new Triangle(prev, (*v0).getEdgeTo(v1), next, v0, v1, rv3);
	new Triangle(next, start, re3, v1, rv0, rv3);
}