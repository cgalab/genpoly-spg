#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "IOtools.h"


int main(){
	Polygon* p;
	int n = 100;

	p = generateRectangularPolygon(n, RPShapes::RPS_FLAT);
	printPolygonToDat(*p, "test.dat");

	printf("Size of polygon: %d \n", (*p).getNumberOfVertices());
}