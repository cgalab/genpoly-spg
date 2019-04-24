#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "triangulation.h"
#include "polygonTransformer.h"
#include "timer.h"
#include "statistics.h"


int main(){
	Triangulation* T;
	int n = 20;
	int translations = 1000 * n;
	Timer t;
	Vertex* v;
	TEdge* e;
	Triangle* dreieck;
	int performed;

	t.start();
	T = generateRegularPolygon(n);
	(*T).check();
	//p = generateRectangularPolygon(n, RPShapes::RPS_FLAT);
	printf("Initial polygon with %d vertices in regular shape computed after %f seconds\n", (*T).getNumberOfVertices(), t.elapsedTime());


	performed = transformPolygon(T, translations, t);
	printf("Transformed polygon with %d of %d translations in %f seconds\n", performed, translations, t.elapsedTime());

	(*T).check();
	
	(*T).print("triangulation.graphml");
	//printStats(p);

	(*T).printPolygon("polygon.graphml");
}