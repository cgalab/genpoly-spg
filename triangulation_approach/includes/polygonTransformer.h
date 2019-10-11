/*
	Include standard libraries
*/
#include <stdio.h>
#include <list>
#include <vector>
#include <map>
#include <math.h>

/*
	Include my headers	
*/
#include "triangulation.h"
#include "tedge.h"
#include "vertex.h"
#include "triangle.h"
#include "translation.h"
#include "insertion.h"
#include "settings.h"


/*
	The function transformPolygonByMoves() transforms a polygon by randomly chosing a
	vertex and a direction to move and then randomly computing a distance to shift in
	the choosen direction which with the constraint that the movement should be
	applicable with reasonable effort. This is done iterations times.

	@param 	T 			The triangulation the polygon lives in
	@param 	iterations	The number of moves which should be tried
	@return 			The number of moves which were executed at least partially

	Note:
		- The number of really performed moves is most likely less then the number of moves
			which should be tried
		- For polygons with holes the vertex also is reandomly choosen, i.e. it can be from
			the outer polygon as well as of one of the inner polygons
*/
int transformPolygonByMoves(Triangulation * const T, const int iterations);


/*
	The function growPolygonBy() grows a polygon by n insertions.

	@param 	T 		The triangulation the polygon lives in
	@param	pID 	The ID of the polygon
	@param 	n 		The number of vertices to insert

	Note:
		This function works just for polygons without holes!
*/
void growPolygonBy(Triangulation * const T, const unsigned int pID,  const int n);


/*

*/
void strategyNoHoles0(Triangulation * const T);


/*

*/
void strategyWithHoles0(Triangulation * const T);