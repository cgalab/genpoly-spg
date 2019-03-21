#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "IOtools.h"
#include "polygonTransformer.h"
#include "timer.h"
#include "statistics.h"


int main(){
	Polygon* p;
	int n = 1000;
	int translations = 5 * n;
	Timer t;

	t.start();
	p = generateRegularPolygon(n);
	//p = generateRectangularPolygon(n, RPShapes::RPS_FLAT);
	printf("Initial polygon with rectangular flat shape computed after %f seconds\n", t.elapsedTime());

	transformPolygon(p, translations, t);
	printf("Transformed polygon with %d translations in %f seconds\n", translations, t.elapsedTime());
	printStats(p);

	printPolygonToDat(*p, "test.dat");
}