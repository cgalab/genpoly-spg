#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "triangulation.h"
#include "polygonTransformer.h"
#include "settings.h"

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
	printf("Initial polygon with %d vertices in regular shape computed after %f seconds\n",
		Settings::initialSize, (*Settings::timer).elapsedTime());

	if(Settings::nrInnerPolygons == 0)
		strategyNoHoles0(T);
	else if(Settings::nrInnerPolygons == 1)
		strategyWithHoles0(T);
	else
		strategyWithHoles0(T);
	
	exit(0);
}