#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "IOtools.h"
#include "polygonTransformer.h"


int main(){
	Polygon* p;
	int n = 100;

	p = generateRectangularPolygon(n, RPShapes::RPS_FLAT);
	transformPolygon(p, 1000);
	printPolygonToDat(*p, "test.dat");

	printf("Size of polygon: %d \n", (*p).getNumberOfVertices());
}