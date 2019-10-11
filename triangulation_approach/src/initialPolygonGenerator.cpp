#include "initialPolygonGenerator.h"

/*
	The function generateRegularPolygon at first generates a new triangulation
	with the vertices of the start polygon in regular shape, i.e. all vertices 
	lay on a circle.
	Afterwards the the start polygon is triangulated, i.e. the polygon edges are
	added and the inner of the polygon is either just triangulated or filed with
	the requested number of holes, then the polygon is boxed
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
	else{
		generateInitialHoleTriangle(T);

		for(i = 1; i < (int)Settings::nrInnerPolygons; i++)
			splitHoleTriangle(T);
	}

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
void initialTriangulationPseudoStar(Triangulation * const T){
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
void initialTriangulationZigZag(Triangulation * const T){
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
void generateInitialHoleTriangle(Triangulation * const T){
	double alpha;
	int i;
	Vertex *triangleV0, *triangleV1, *triangleV2;
	Vertex *v0, *v1;
	TEdge *e0, *e1, *e2;
	TEdge *start, *triangleE0, *triangleE1, *triangleE2;

	// Add an inner polygon to the triangulation
	(*T).addInnerPolygon(new TPolygon(T, Settings::innerSizes[0]));

	alpha = 2 * M_PI / 3;

	// Generate the vertices of the inner triangle
	triangleV0 = new Vertex(Settings::radiusHole, 0);
	triangleV1 = new Vertex(Settings::radiusHole * cos(alpha), Settings::radiusHole * sin(alpha));
	triangleV2 = new Vertex(Settings::radiusHole * cos(2 * alpha), Settings::radiusHole * sin(2 * alpha));
	(*T).addVertex(triangleV0, 1);
	(*T).addVertex(triangleV1, 1);
	(*T).addVertex(triangleV2, 1);
	
	// Start with the range from 0 to 2*pi/3	
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

	// Connect the first third with the second one
	e0 = new TEdge(triangleV1, triangleV0, EdgeType::POLYGON);
	e1 = e2;
	e2 = new TEdge(v1, triangleV1);
	(*T).addEdge(e0);
	(*T).addEdge(e2);

	new Triangle(e0, e1, e2, v1, triangleV0, triangleV1);

	triangleE0 = e0;

	// Range 2*pi/3 to 4*pi/3 
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

	// Connect the second third with the third one
	e0 = new TEdge(triangleV2, triangleV1, EdgeType::POLYGON);
	e1 = e2;
	e2 = new TEdge(v1, triangleV2);
	(*T).addEdge(e0);
	(*T).addEdge(e2);

	new Triangle(e0, e1, e2, v1, triangleV1, triangleV2);

	triangleE1 = e0;

	// Range 4*pi/3 to 2*pi 
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

	// Connect the third third with the first one and close the outer polygon
	e0 = new TEdge(triangleV0, triangleV2, EdgeType::POLYGON);
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
	The function splitHoleTriangle() splits the latest generated triangular hole into
	two triangular holes and places them as the latest holes in the vector of inner
	polygons. It can be applied successively.

	@param 	T 	The triangulation the polygons live in

	Note:
		For more information take a look at my Master Thesis
*/
void splitHoleTriangle(Triangulation * const T){
	int n;
	double x0, x1, x2, y1, y2;
	Vertex *v0, *v1, *v2;
	TEdge *e0, *e1, *e2;
	Vertex *n0, *n1, *n2;
	Triangle *t, *t0, *t1;
	Vertex *store0, *store1;
	TEdge *p1e0, *p1e1;
	TEdge *p2e0, *p2e1, *p2e2;
	TEdge *h0, *h1, *h2;

	// Get the actual number of inner polygons; we want to split the last one
	n = (*T).getActualNrInnerPolygons();

	// Get all vertices and edges of the choosen polygon
	v0 = (*T).getVertex(0, n);
	v1 = (*T).getVertex(1, n);
	v2 = (*T).getVertex(2, n);
	e0 = (*v0).getEdgeTo(v1);
	e1 = (*v1).getEdgeTo(v2);
	e2 = (*v2).getEdgeTo(v0);

	// Get the inner triangle of the polygon
	t = (*v0).getTriangleWith(v1, v2);

	// Get the other triangles incident to the edges which get deleted
	t0 = (*e0).getOtherTriangle(t);
	t1 = (*e2).getOtherTriangle(t);

	// Get the vertices we will need later of triangles
	store0 = (*t0).getOtherVertex(e0);
	store1 = (*t1).getOtherVertex(e2);

	// Delete the edges containing v0
	delete e0;
	delete e2;

	// Get the coordinates of the triangle's vertices
	x0 = (*v0).getX();
	x1 = (*v1).getX();
	y1 = (*v1).getY();
	x2 = (*v2).getX();
	y2 = (*v2).getY();

	// Add a new inner polygon
	(*T).addInnerPolygon(new TPolygon(T, Settings::innerSizes[n]));


	/*
		REBUILD THE OLD POLYGON
	*/

	// Generate the third vertex for the first polygon
	n2 = new Vertex(fabs(x0 - x1) / 4 + x1, (y1 + y2) / 2);
	(*T).addVertex(n2, n);

	// Generate the edges and the triangle of the first polygon
	p1e0 = new TEdge(v1, n2, EdgeType::POLYGON);
	p1e1 = new TEdge(n2, v2, EdgeType::POLYGON);
	(*T).addEdge(p1e0);
	(*T).addEdge(p1e1);
	new Triangle(p1e0, p1e1, e1, v1, v2, n2);


	/*
		GENERATE THE NEW POLYGON
	*/

	// Change the first vertex to the second polygon
	(*T).changeVertex(0, n, n + 1);

	// Generate the new vertices for the second polygon
	n0 = new Vertex(2 * fabs(x0 - x1) / 3 + x1, y1 / 3);
	n1 = new Vertex(2 * fabs(x0 - x2) / 3 + x2, y2 / 3);
	(*T).addVertex(n0, n + 1);
	(*T).addVertex(n1, n + 1);

	// Generate the edges and the triangle of the second polygon
	p2e0 = new TEdge(n0, v0, EdgeType::POLYGON);
	p2e1 = new TEdge(n1, n0, EdgeType::POLYGON);
	p2e2 = new TEdge(v0, n1, EdgeType::POLYGON);
	(*T).addEdge(p2e0);
	(*T).addEdge(p2e1);
	(*T).addEdge(p2e2);
	new Triangle(p2e0, p2e1, p2e2, v0, n0, n1);

	// Add triangles to the outside
	h0 = new TEdge(store1, n1);
	h1 = (*v0).getEdgeTo(store1);
	(*T).addEdge(h0);
	new Triangle(h0, h1, p2e2, v0, store1, n1);

	h1 = new TEdge(n1, v2);
	h2 = (*store1).getEdgeTo(v2);
	(*T).addEdge(h1);
	new Triangle(h0, h1, h2, store1, n1, v2);

	h0 = new TEdge(store0, n0);
	h1 = (*v0).getEdgeTo(store0);
	(*T).addEdge(h0);
	new Triangle(h0, h1, p2e0, v0, store0, n0);

	h1 = new TEdge(n0, v1);
	h2 = (*store0).getEdgeTo(v1);
	(*T).addEdge(h1);
	new Triangle(h0, h1, h2, store0, n0, v1);

	// Add triangles between the polygons
	h0 = new TEdge(n1, n2);
	h1 = new TEdge(n0, n2);
	(*T).addEdge(h0);
	(*T).addEdge(h1);
	new Triangle(h0, h1, p2e1, n0, n1, n2);

	new Triangle(h0, (*n1).getEdgeTo(v2), (*n2).getEdgeTo(v2), n1, n2, v2);
	new Triangle(h1, (*n0).getEdgeTo(v1), (*n2).getEdgeTo(v1), n0, n2, v1);
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
void boxPolygon(Triangulation * const T, const int startIndex){
	// Vertices and edges of the rectangle
	Vertex *rv0, *rv1, *rv2, *rv3;
	TEdge *re0, *re1, *re2, *re3;
	// End indices of one quadrant
	int limit0, limit1, limit2;

	int i;
	TEdge *start, *prev, *next;
	Vertex *v0, *v1;

	/* 
		Generate the rectangle in the following orientation:

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