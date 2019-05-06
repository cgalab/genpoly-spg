#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "triangulation.h"
#include "polygonTransformer.h"
#include "timer.h"
#include "statistics.h"


int main(){
	Triangulation* T;
	int n = 15;
	int translations = 100 * n;
	Timer t;
	Vertex* v;
	TEdge* e;
	Triangle* dreieck;
	int performed;

	t.start();
	if(n <= 10)
		T = generateRegularPolygon(n);
	else
		T = generateRegularPolygon(10);
	(*T).check();
	printf("Initial polygon with %d vertices in regular shape computed after %f seconds\n", (*T).getActualNumberOfVertices(), t.elapsedTime());


	performed = transformPolygonByMoves(T, translations, t);
	printf("Transformed initial polygon with %d of %d translations in %f seconds\n", performed, translations, t.elapsedTime());

	(*T).check();

	(*T).print("triangulation_init.graphml");
	//printStats(p);

	(*T).printPolygon("polygon_int.graphml");

	growPolygon(T, n, t);
	printf("Grow initial polygon to %d vertices afters %f seconds \n", n, t.elapsedTime());

	(*T).check();
	
	(*T).print("triangulation.graphml");
	//printStats(p);

	(*T).printPolygon("polygon.graphml");
}