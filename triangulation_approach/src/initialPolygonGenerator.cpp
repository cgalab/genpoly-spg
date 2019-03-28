#include "initialPolygonGenerator.h"

Triangulation* generateRegularPolygon(int n){
	double r, alpha;
	int i;
	Vertex* v;
	Vertex* v0 = NULL, *v1 = NULL, *v2 = NULL;
	TEdge* e0 = NULL, *e1 = NULL, *e2 = NULL;
	Triangulation* T = new Triangulation(n + 4); // 4 additional vertices for the rectangle
	Triangle* t;

	alpha = 2 * M_PI / n;
	r = 30.0;

	for(i = 0; i < n; i++){
		v = new Vertex(r * cos(i * alpha), r * sin(i * alpha));
		(*T).addVertex(v);
	}

	// the inital triangulation contains n-2 triangles
	v0 = (*T).getVertex(0);
	v1 = (*T).getVertex(n - 1);
	e0 = new TEdge(v0, v1, true, false);
	(*T).addEdge(e0);
	for(i = 0; i < n - 2; i++){
		if(i % 2 == 0){
			// v0 stays as it is and v1 becomes v2
			v2 = v1;
			v1 = (*T).getVertex(i / 2 + 1);

			// e0 becomes e2
			e2 = e0;
			e0 = new TEdge(v0, v1);
			e1 = new TEdge(v1, v2);

			(*T).addEdge(e0);
			(*T).addEdge(e1);

			(*e0).setPEdge(true);

			t = new Triangle(e0, e1, e2, v0, v1, v2);		
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
			
			(*e1).setPEdge(true);

			t = new Triangle(e0, e1, e2, v0, v1, v2);
		}

		//(*t).print();
	}

	if(n % 2 == 0){
		(*e0).setPEdge(true);
	}else{
		(*e1).setPEdge(true);
	}

	boxPolygon(T, r);

	return T; 
	
}

void boxPolygon(Triangulation* T, double r){
	// vertices and edges of the rectangle
	Vertex *rv0, *rv1, *rv2, *rv3;
	TEdge *re0, *re1, *re2, *re3;

	// generate the rectangle
	/*
		1 - 0
		|   |
		2 - 3
	*/
	rv0 = new Vertex(2 * r, 2 * r, true);
	rv1 = new Vertex(-2 * r, 2 * r, true);
	rv2 = new Vertex(-2 * r, -2 * r, true);
	rv3 = new Vertex(2 * r, -2 * r, true);
	(*T).addVertex(rv0);
	(*T).addVertex(rv1);
	(*T).addVertex(rv2);
	(*T).addVertex(rv3);
	re0 = new TEdge(rv0, rv1, false, true);
	re1 = new TEdge(rv1, rv2, false, true);
	re2 = new TEdge(rv2, rv3, false, true);
	re3 = new TEdge(rv3, rv0, false, true);
	(*T).addEdge(re0);
	(*T).addEdge(re1);
	(*T).addEdge(re2);
	(*T).addEdge(re3);
}