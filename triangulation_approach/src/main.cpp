#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "triangulation.h"
#include "polygonTransformer.h"
#include "timer.h"
#include "statistics.h"


int main(){
	Triangulation* T;
	int n = 10;
	int translations = 50 * n;
	Timer t;

	t.start();
	T = generateRegularPolygon(n);
	(*T).check();
	//p = generateRectangularPolygon(n, RPShapes::RPS_FLAT);
	printf("Initial polygon with regular shape computed after %f seconds\n", t.elapsedTime());
	

	//transformPolygon(T, translations, t);
	//printf("Transformed polygon with %d translations in %f seconds\n", translations, t.elapsedTime());
	(*T).print("polygon.graphml");
	//printStats(p);

	//printPolygonToDat(*p, "test.dat");
}