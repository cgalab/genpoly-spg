#include "statistics.h"

void printStats(Polygon* p){
	printf("Polygon stats:\n");
	printf("Size: %d \n", (*p).getNumberOfVertices());
	printf("Range ratio: %f \n", rangeRatio(p));
}

double rangeRatio(Polygon *p){
	double xRange, yRange;

	xRange = findMax(p, 0) - findMin(p, 0);
	yRange = findMax(p, 1) - findMin(p, 1);

	// lower range divided by higher range => 0 < ratio <= 1
	if(xRange > yRange)
		return yRange/xRange;
	else
		return xRange/yRange;
}

double findMin(Polygon* p, int dim){
	double min;
	int n = (*p).getNumberOfVertices();
	int i;
	Point* v;

	// init min with first point
	v = (*p).getVertex(0);
	if(dim == 0){
		min = (*v).x;
	}else{
		min = (*v).y;
	}

	for(i = 1; i < n; i++){
		v = (*p).getVertex(i);
		if(dim == 0){
			if(min > (*v).x) min = (*v).x;
		}else{
			if(min > (*v).y) min = (*v).y;
		}
	}

	return min;
}

double findMax(Polygon* p, int dim){
	double max;
	int n = (*p).getNumberOfVertices();
	int i;
	Point* v;

	// init min with first point
	v = (*p).getVertex(0);
	if(dim == 0){
		max = (*v).x;
	}else{
		max = (*v).y;
	}

	for(i = 1; i < n; i++){
		v = (*p).getVertex(i);
		if(dim == 0){
			if(max < (*v).x) max = (*v).x;
		}else{
			if(max < (*v).y) max = (*v).y;
		}
	}

	return max;
}