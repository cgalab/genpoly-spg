#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>


int main(){
	Polygon* p;
	int n = 100;

	p = generateRectangularPolygon(n, RPShapes::RPS_FLAT);

	printf("Size of polygon: %d \n", (*p).getNumberOfVertices());
}