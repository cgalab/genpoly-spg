/*
	Include standard libraries
*/
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/*
	Include my headers
*/
#include "vertex.h"
#include "triangulation.h"
#include "tedge.h"
#include "triangle.h"
#include "settings.h"

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
Triangulation *generateRegularPolygon();

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
void initialTriangulationPseudoStar(Triangulation *T);

/*
	The function initialTriangulationZigZag adds the polygon edges to the
	vertices of a regular shaped polygon with no holes and triangulates the
	inner of the polygon in zig-zag style (for more information on the zig-zag
	style take a look at my master thesis).

	Note:
	The suitable startindex for the box polygon function is 0!

	@param	T 	The triangulation containing the already generated but not
				linked vertices of the start polygon
*/
void initialTriangulationZigZag(Triangulation *T);

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
void boxPolygon(Triangulation *T, int startIndex);