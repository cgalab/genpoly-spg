#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "triangulation.h"
//#include "polygonTransformer.h"
#include "timer.h"
#include "statistics.h"


int main(){
	Triangulation* T;
	int n = 1000;
	int translations = 50 * n;
	Timer t;

	t.start();
	T = generateRegularPolygon(n);
	//p = generateRectangularPolygon(n, RPShapes::RPS_FLAT);
	printf("Initial polygon with regular shape computed after %f seconds\n", t.elapsedTime());

	//transformPolygon(p, translations, t);
	//printf("Transformed polygon with %d translations in %f seconds\n", translations, t.elapsedTime());
	//printStats(p);

	//printPolygonToDat(*p, "test.dat");
}