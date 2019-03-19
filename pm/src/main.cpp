#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "IOtools.h"
#include "polygonTransformer.h"
#include "timer.h"


int main(){
	Polygon* p;
	int n = 10000;
	int translations = 10 * n;
	Timer t;

	t.start();
	p = generateRectangularPolygon(n, RPShapes::RPS_FLAT);
	printf("Initial polygon with rectangular flat shape computed after %f seconds\n", t.elapsedTime());

	transformPolygon(p, translations, t);
	printf("Transformed polygon with %d translations in %f seconds\n", translations, t.elapsedTime());

	printPolygonToDat(*p, "test.dat");
}