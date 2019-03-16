#include "initialPolygonGenerator.h"

Polygon* generateRectangularPolygon(int n, enum RPShapes type){
	int i;
	double min, max;
	Point* v;
	Polygon* p = new Polygon;

	if(type == RPShapes::RPS_FLAT){
		// number of points must be even to generate two parallel lines
		if(n % 2 == 1) n++;
		// ensure symmetry around origin
		if(n % 4 == 0){
			min = - n / 4 + 0.5;
		}else{
			min = - n / 4;
		}
		max = - min;

		for(i = 0; i < n / 2; i++){
			v = new Point(min + i, -1, i);
			(*p).addVertex(*v);
		}

		for(i = 0; i < n / 2; i++){
			v = new Point(max - i, 1, i);
			(*p).addVertex(*v);
		}

	}else if(type == RPShapes::RPS_QUADRATIC){
		i = n % 4;
		n = n + (i + 2) - 4;
	}

	return p;
}