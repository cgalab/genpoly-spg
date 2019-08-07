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
*/


int main(){
	Settings settings;
	Triangulation* T;
	int performed;
	RandomGenerator generator;

	exactinit();

	readSettings(settings);

	T = generateRegularPolygon(settings);

	(*T).check();
	printf("Initial polygon with %d vertices in regular shape computed after %f seconds\n", settings.getInitialSize(), settings.elapsedTime());


	performed = transformPolygonByMoves(settings, generator, T, settings.getInitialTN(), false);
	printf("Transformed initial polygon with %d of %d translations in %f seconds\n\n", performed, settings.getInitialTN(), settings.elapsedTime());

	if(!(*T).check()){
		printf("Triangulation error: something is wrong in the triangulation at the end\n");
		exit(9);
	}

	(*T).print("triangulation_init.graphml");
	(*T).printPolygon("polygon_int.graphml");

	growPolygon(settings, generator, T);
	printf("Grew initial polygon to %d vertices afters %f seconds \n\n", settings.getTargetSize(), settings.elapsedTime());

	if(!(*T).check()){
		printf("Triangulation error: something is wrong in the triangulation at the end\n");
		exit(9);
	}

	performed = transformPolygonByMoves(settings, generator, T, 1000000, false);
	printf("Transformed polygon with %d of %d translations in %f seconds\n\n", performed, 1000000, settings.elapsedTime());
	printf("number of vertices: %d \n", (*T).getActualNumberOfVertices());
	

	if(!(*T).check()){
		printf("Triangulation error: something is wrong in the triangulation at the end\n");
		exit(9);
	}

	Vertex::printStats();
	
	//(*T).print("triangulation.graphml");

	(*T).printPolygon("polygon.graphml");

	exit(0);
}
