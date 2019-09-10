#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "triangulation.h"
#include "polygonTransformer.h"
#include "settings.h"
#include "randomGenerator.h"
#include "predicates.h"

/*
list of exit codes:
code 	name						meaning
1		Circle edge					An edge starts at the same vertex as it ends
2		Vertex at PE end			A vertex lies exactly at a polygon edge after a translation
3		PE flip 					The edge to be fliped at an event is a polygon edge
4		Triangle overflow			A new triangle is assigned to an edge which already has two triangles
5		Triangle redundance			A new triangle is assigned to an edge which already has the same 
									triangle registered
6		Surrounding polygon fail	The moving vertex is not anymore inside of its surrouding polygon
									at the end of a translation
7		Vertex at PE start 			The moving vertex lies at a polygon edge at the begining of a translation
8		temporarly empty
9		Triangulation error			Some vertex has left its surrouding polygon but it hasn't been recognized
									during the translation
10		Vertex at PE end 			The surrounding polygon check recognizes a vertex exactly laying on a PE
11 		Not simple					The check for simplicity found an intersection
12 		Vertex insertion error 		The pID passed to Triangulation::addVertex() exceeds the number of inner
									polygons
*/

int main(){
	Triangulation* T;
	int performed;

	Settings::initSettings();

	T = generateRegularPolygon();

	(*T).check();
	printf("Initial polygon with %d vertices in regular shape computed after %f seconds\n", Settings::initialSize, (*Settings::timer).elapsedTime());

	(*T).print("triangulation_init.graphml");
	growPolygonBy(T, 1, Settings::innerSizes[0] - 3);
	growPolygonBy(T, 2, Settings::innerSizes[1] - 3);
	performed = transformPolygonByMoves(T, Settings::initialTranslationNumber);
	printf("Transformed initial polygon with %d of %d translations in %f seconds\n\n", performed, Settings::initialTranslationNumber, (*Settings::timer).elapsedTime());

	if(!(*T).check()){
		printf("Triangulation error: something is wrong in the triangulation at the end\n");
		exit(9);
	}

	(*T).print("triangulation_init.graphml");
	(*T).printPolygonToDat("polygon_init.dat");

	
	growPolygonBy(T, 0, Settings::outerSize - Settings::initialSize);
	printf("Grew initial polygon to %d vertices afters %f seconds \n\n", Settings::outerSize, (*Settings::timer).elapsedTime());

	if(!(*T).check()){
		printf("Triangulation error: something is wrong in the triangulation at the end\n");
		exit(9);
	}

	performed = transformPolygonByMoves(T, 1000);
	printf("Transformed polygon with %d of %d translations in %f seconds\n\n", performed, 1000000, (*Settings::timer).elapsedTime());
	printf("number of vertices: %d \n", (*T).getActualNumberOfVertices());
	

	if(!(*T).check()){
		printf("Triangulation error: something is wrong in the triangulation at the end\n");
		exit(9);
	}

	Vertex::printStats();
	
	(*T).print("triangulation.graphml");

	(*T).printPolygonToDat("polygon.dat");
	
	exit(0);
}
