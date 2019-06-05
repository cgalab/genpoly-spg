#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "triangulation.h"
#include "polygonTransformer.h"
#include "timer.h"
#include "statistics.h"
#include "eventQueue.h"

int main(){
	Triangulation* T;
	int n = 1000000;
	int translations;
	Timer t;
	int performed;
	int initialSize = 100;
	int factor = 1000;

	t.start();
	if(n <= initialSize){
		T = generateRegularPolygon(n);
		translations = n * factor;
	}
	else{
		T = generateRegularPolygon(initialSize);
		translations = factor * initialSize;
	}
	(*T).check();
	printf("Initial polygon with %d vertices in regular shape computed after %f seconds\n", (*T).getActualNumberOfVertices(), t.elapsedTime());


	performed = transformPolygonByMoves(T, translations, t);
	printf("Transformed initial polygon with %d of %d translations in %f seconds\n\n", performed, translations, t.elapsedTime());

	(*T).check();

	//(*T).stretch(1000);

	(*T).print("triangulation_init.graphml");
	//printStats(p);

	(*T).printPolygon("polygon_int.graphml");

	growPolygon(T, n, t);
	printf("Grew initial polygon to %d vertices afters %f seconds \n\n", n, t.elapsedTime());
	
	(*T).check();

	performed = transformPolygonByMoves(T, 1000000, t);
	printf("Transformed initial polygon with %d of %d translations in %f seconds\n\n", performed, translations, t.elapsedTime());

	(*T).check();
	
	//(*T).print("triangulation.graphml");
	//printStats(p);

	(*T).printPolygon("polygon.graphml");
}