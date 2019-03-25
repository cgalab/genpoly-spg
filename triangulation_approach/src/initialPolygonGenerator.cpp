#include "initialPolygonGenerator.h"

/*Polygon* generateRectangularPolygon(int n, enum RPShapes type){
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
}*/

Triangulation* generateRegularPolygon(int n){
	double r, alpha;
	int i;
	Vertex* v;
	Vertex* v0 = NULL;
	Vertex* v1 = NULL;
	Vertex* v2 = NULL;
	TEdge* e0 = NULL;
	TEdge* e1 = NULL;
	TEdge* e2 = NULL;
	Triangulation* T = new Triangulation(n);

	alpha = 2 * M_PI / n;
	r = 30.0;

	for(i = 0; i < n; i++){
		v = new Vertex(r * cos(i * alpha), r * sin(i * alpha), i);
		(*T).addVertex(v);
	}

	// the inital triangulation contains n-2 triangles
	v0 = (*T).getVertex(0);
	v1 = (*T).getVertex(n - 1);
	e0 = new TEdge(v0, v1);
	(*T).addEdge(e0);
	for(i = 0; i < n - 1; i++){
		if(i % 2 == 0){
			// v0 stays as it is and v1 becomes v2
			v2 = v1;
			v1 = (*T).getVertex(i / 2 +1);

			// e0 becomes e2
			e2 = e0;
			e0 = new TEdge(v0, v1);
			e1 = new TEdge(v1, v2);

			(*T).addEdge(e0);
			(*T).addEdge(e1);

			(*e0).makeEdgePEdge();
			(*e0).makeEdgeCHEdge();

			new Triangle(e0, e1, e2);		
		}else{
			// v1 becomes v0 and v2 stays as it is
			v0 = v1;
			v1 = (*T).getVertex(n - i / 2 - 2);

			// e1 becomes e2
			e2 = e1;
			e0 = new TEdge(v0, v1);
			e1 = new TEdge(v1, v2);

			(*T).addEdge(e0);
			(*T).addEdge(e1);
			
			(*e1).makeEdgePEdge();
			(*e1).makeEdgeCHEdge();

			new Triangle(e0, e1, e2);
		}
	}
	// TODO: check why last vertex has a circle and if all triangles are right

	return T; 
}