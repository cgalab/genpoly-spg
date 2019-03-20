#include "initialPolygonGenerator.h"

Polygon* generateRectangularPolygon(int n, enum RPShapes type){
	int i;
	double min, max;
	Point* v;
	Polygon* p = new Polygon(n);

	if(type == RPShapes::RPS_FLAT){

		// ensure symmetry around origin
		if(n % 4 == 0){
			min = - n / 4 + 0.5;
		}else{
			min = - n / 4;
		}
		max = - min;

		for(i = 0; i < n / 2; i++){
			v = new Point(min + i, -1, i);
			(*p).addVertex(v);
		}

		if(n % 2 == 1) n++;

		for(i = 0; i < n / 2; i++){
			v = new Point(max - i, 1, i);
			(*p).addVertex(v);
		}

	}else if(type == RPShapes::RPS_QUADRATIC){
		i = n % 4;
		n = n + (i + 2) - 4;
	}

	return p;
}

Polygon* generateRegularPolygon(int n){

}