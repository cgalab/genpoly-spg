#include "initialPolygonGenerator.h"

Triangulation* generateRegularPolygon(int n){
	double r, alpha;
	int i;
	Vertex* v;
	Vertex* v0 = NULL, *v1 = NULL, *v2 = NULL;
	TEdge* e0 = NULL, *e1 = NULL, *e2 = NULL;
	Triangulation* T = new Triangulation(n); // 4 additional vertices for the rectangle
	Triangle* t;

	alpha = 2 * M_PI / n;
	r = n * 10 / (2 * M_PI); // maybe a good choice for the radius = n * sigma / (2 * pi)

	for(i = 0; i < n; i++){
		v = new Vertex(r * cos(i * alpha), r * sin(i * alpha));
		(*T).addVertex(v);
	}

	// the inital triangulation contains n-2 triangles
	v0 = (*T).getVertex(0);
	v1 = (*T).getVertex(n - 1);
	e0 = new TEdge(v0, v1, EdgeType::POLYGON);
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

			(*e0).setEdgeType(EdgeType::POLYGON);

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
			
			(*e1).setEdgeType(EdgeType::POLYGON);

			t = new Triangle(e0, e1, e2, v0, v1, v2);
		}

		//(*t).print();
	}

	if(n % 2 == 0){
		(*e0).setEdgeType(EdgeType::POLYGON);
	}else{
		(*e1).setEdgeType(EdgeType::POLYGON);
	}

	boxPolygon(T, r, n);

	return T; 
	
}

void boxPolygon(Triangulation* T, double r, int n){
	// vertices and edges of the rectangle
	Vertex *rv0, *rv1, *rv2, *rv3;
	TEdge *re0, *re1, *re2, *re3;
	int limit0, limit1, limit2, i;
	TEdge *start, *prev, *next;
	Vertex *v0, *v1;
	Triangle *t;
	double a = 5 * r; // lenght of a side

	// generate the rectangle
	/*
		1 - 0
		|   |
		2 - 3
	*/
	rv0 = new Vertex(a / 2, a / 2, true);
	rv1 = new Vertex(-a / 2, a / 2, true);
	rv2 = new Vertex(- a / 2, - a / 2, true);
	rv3 = new Vertex(a / 2, - a / 2, true);
	(*T).addVertex(rv0);
	(*T).addVertex(rv1);
	(*T).addVertex(rv2);
	(*T).addVertex(rv3);
	re0 = new TEdge(rv0, rv1, EdgeType::FRAME);
	re1 = new TEdge(rv1, rv2, EdgeType::FRAME);
	re2 = new TEdge(rv2, rv3, EdgeType::FRAME);
	re3 = new TEdge(rv3, rv0, EdgeType::FRAME);
	(*T).addEdge(re0);
	(*T).addEdge(re1);
	(*T).addEdge(re2);
	(*T).addEdge(re3);

	// studieren geht über probieren :D
	limit0 = (n + 1) / 4;
	limit1 = n / 2;
	limit2 = 3 * n / 4;

	// first quadrant
	v0 = (*T).getVertex(0);
	start = new TEdge(v0, rv0);
	(*T).addEdge(start);
	prev = start;

	for(i = 1; i <= limit0; i++){
		v1 = (*T).getVertex(i);
		next = new TEdge(v1, rv0);
		(*T).addEdge(next);

		t = new Triangle(prev, (*v0).getEdgeTo(v1), next, v0, v1, rv0);

		v0 = v1;
		prev = next;
	}

	// second quadrant
	next = new TEdge(v0, rv1);
	(*T).addEdge(next);
	t = new Triangle(prev, next, re0, v0, rv0, rv1);
	prev = next;

	for(; i <= limit1; i++){
		v1 = (*T).getVertex(i);
		next = new TEdge(v1, rv1);
		(*T).addEdge(next);

		t = new Triangle(prev, (*v0).getEdgeTo(v1), next, v0, v1, rv1);

		v0 = v1;
		prev = next;
	}

	// third quadrant
	next = new TEdge(v0, rv2);
	(*T).addEdge(next);
	t = new Triangle(prev, next, re1, v0, rv1, rv2);
	prev = next;

	for(; i <= limit2; i++){
		v1 = (*T).getVertex(i);
		next = new TEdge(v1, rv2);
		(*T).addEdge(next);

		t = new Triangle(prev, (*v0).getEdgeTo(v1), next, v0, v1, rv2);

		v0 = v1;
		prev = next;
	}

	// fourth quadrant
	next = new TEdge(v0, rv3);
	(*T).addEdge(next);
	t = new Triangle(prev, next, re2, v0, rv2, rv3);
	prev = next;

	for(; i < n; i++){
		v1 = (*T).getVertex(i);
		next = new TEdge(v1, rv3);
		(*T).addEdge(next);

		t = new Triangle(prev, (*v0).getEdgeTo(v1), next, v0, v1, rv3);

		v0 = v1;
		prev = next;
	}

	// close the triangulation
	v1 = (*T).getVertex(0);
	next = new TEdge(v1, rv3);
	(*T).addEdge(next);

	t = new Triangle(prev, (*v0).getEdgeTo(v1), next, v0, v1, rv3);
	t = new Triangle(next, start, re3, v1, rv0, rv3);
}