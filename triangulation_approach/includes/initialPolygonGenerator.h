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
	added and the inner of the polygon is either just triangulated or filed with
	the requested number of holes, then the polygon is boxed
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

	@param	T 	The triangulation containing the already generated but not
				linked vertices of the start polygon

	Note:
		In principle this approach would work, but it gets very slow for larger
		initial polygons as each vertex shares an edge with the one center
		vertex so the edges list of the center vertex becomes very slow.
*/
void initialTriangulationPseudoStar(Triangulation * const T);


/*
	The function initialTriangulationZigZag adds the polygon edges to the
	vertices of a regular shaped polygon with no holes and triangulates the
	inner of the polygon in zig-zag style (for more information on the zig-zag
	style take a look at my master thesis).

	@param	T 	The triangulation containing the already generated but not
				linked vertices of the start polygon

	Note:
		The suitable startindex for the box polygon function is 0!
*/
void initialTriangulationZigZag(Triangulation * const T);


/*
	The function generateInitialHoleTriangle() generates a triangular shaped hole
	in the middle of the start polygon and triangulates the start polygon with the
	hole in it.

	@param	T 	The triangulation already containing the vertices of the outer polygon

	Note:
		The suitable startindex for the boxPolygon function is 0!
*/
void generateInitialHoleTriangle(Triangulation * const T);


/*
	The function splitHoleTriangle() splits the latest generated triangular hole into
	two triangular holes and places them as the latest holes in the vector of inner
	polygons. It can be applied successively.

	@param 	T 	The triangulation the polygons live in

	Note:
		For more information take a look at my Master Thesis
*/
void splitHoleTriangle(Triangulation * const T);


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
void boxPolygon(Triangulation * const T, const int startIndex);