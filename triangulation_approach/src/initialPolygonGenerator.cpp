#include "initialPolygonGenerator.h"

Triangulation* generateRegularPolygon(int n){
	double r, alpha;
	int i;
	Vertex* v;
	Triangulation* T = new Triangulation(n);
	
	alpha = 2 * M_PI / n;
	r = 100; //n * 10 / (2 * M_PI); // maybe a good choice for the radius = n * sigma / (2 * pi)

	for(i = 0; i < n; i++){
		v = new Vertex(r * cos(i * alpha), r * sin(i * alpha));
		(*T).addVertex(v);
	}

	initialTriangulationZigZag(T, n);

	boxPolygon(T, r, n, 0);

	return T; 
	
}

// boxPolygon startindex 1
// works in principle, but is slow as shit
void initialTriangulationPseudoStar(Triangulation* T, int n){
	int i;
	Vertex *center, *v0, *v1;
	TEdge *e0, *e1, *e2, *start;
	Triangle *t;
	bool ok = true;

	// move vertex 0 to the origin
	center = (*T).getVertex(0);
	(*center).setPosition(0, 0);

	v0 = (*T).getVertex(1);
	v1 = (*T).getVertex(2);

	e0 = new TEdge(v0, v1, EdgeType::POLYGON); // e0 is the polygon edge
	e1 = new TEdge(v1, center); // e1 is the edge of the higher vertex to the center
	start = new TEdge(center, v0, EdgeType::POLYGON);
	(*T).addEdge(e0);
	(*T).addEdge(e1);
	(*T).addEdge(start);

	t = new Triangle(e0, e1, start, v0, v1, center, "initialGeneratorStart", ok);

	for(i = 3; i < n; i++){
		v0 = v1;
		v1 = (*T).getVertex(i);

		e0 = new TEdge(v0, v1, EdgeType::POLYGON);
		e2 = e1;
		e1 = new TEdge(v1, center);

		(*T).addEdge(e0);
		(*T).addEdge(e1);

		t = new Triangle(e0, e1, e2, v0, v1, center, "initialGenerator1", ok);
	}

	v0 = (*T).getVertex(1);
	(*e1).setEdgeType(EdgeType::POLYGON);
	e2 = new TEdge(v0, v1);
	(*T).addEdge(e2);
	t = new Triangle(e1, e2, start, v0, v1, center, "initialGeneratorEnd", ok);
}

// boxPolygon startindex 0
// doesn't work at all
void initialTriangulationStar(Triangulation* T, int n){
	int i;
	Vertex *center, *v0, *v1;
	TEdge *e0, *e1, *e2, *start;
	Triangle *t;

	center = new Vertex(0, 0, true);
	(*T).addVertex(center);

	v0 = (*T).getVertex(0);
	v1 = (*T).getVertex(1);

	e0 = new TEdge(v0, v1, EdgeType::POLYGON); // e0 is the polygon edge
	e1 = new TEdge(v1, center); // e1 is the edge of the higher vertex to the center
	start = new TEdge(center, v0);
	(*T).addEdge(e0);
	(*T).addEdge(e1);
	(*T).addEdge(start);

	t = new Triangle(e0, e1, start, v0, v1, center);

	for(i = 2; i < n; i++){
		v0 = v1;
		v1 = (*T).getVertex(i);

		e0 = new TEdge(v0, v1, EdgeType::POLYGON);
		e2 = e1;
		e1 = new TEdge(v1, center);

		(*T).addEdge(e0);
		(*T).addEdge(e1);

		t = new Triangle(e0, e1, e2, v0, v1, center);
	}

	v0 = (*T).getVertex(n - 1);
	v1 = (*T).getVertex(0);
	e0 = new TEdge(v0, v1, EdgeType::POLYGON);
	(*T).addEdge(e0);
	t = new Triangle(e0, e1, start, v0, v1, center);
}

// boxPolygon startindex 0
void initialTriangulationZigZag(Triangulation* T, int n){
	int i;
	Vertex* v0 = NULL, *v1 = NULL, *v2 = NULL;
	TEdge* e0 = NULL, *e1 = NULL, *e2 = NULL;
	Triangle* t;
	bool ok;

	// the inital triangulation contains n-2 triangles
	v0 = (*T).getVertex(0);
	v1 = (*T).getVertex(n - 1);
	e0 = new TEdge(v1, v0, EdgeType::POLYGON);
	(*T).addEdge(e0);
	for(i = 0; i < n - 2; i++){
		ok = true;
		if(i % 2 == 0){
			// v0 stays as it is and v1 becomes v2
			v2 = v1;
			v1 = (*T).getVertex(i / 2 + 1);

			// e0 becomes e2
			e2 = e0;
			e0 = new TEdge(v0, v1, EdgeType::POLYGON);
			e1 = new TEdge(v1, v2);

			(*T).addEdge(e0);
			(*T).addEdge(e1);

			t = new Triangle(e0, e1, e2, v0, v1, v2, "initialGeneratorZigZag1", ok);
		}else{
			// v1 becomes v0 and v2 stays as it is
			v0 = v1;
			v1 = (*T).getVertex(n - i / 2 - 2);

			// e1 becomes e2
			e2 = e1;
			e0 = new TEdge(v0, v1);
			e1 = new TEdge(v1, v2, EdgeType::POLYGON);

			(*T).addEdge(e0);
			(*T).addEdge(e1);

			t = new Triangle(e0, e1, e2, v0, v1, v2, "initialGeneratorZigZag2", ok);
		}

		//(*t).print();
	}

	if(n % 2 == 0){
		(*e0).setEdgeType(EdgeType::POLYGON);
	}else{
		(*e1).setEdgeType(EdgeType::POLYGON);
	}
}

void boxPolygon(Triangulation* T, double r, int n, int startIndex){
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
	(*T).setRectangle(rv0, rv1, rv2, rv3);
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
	v0 = (*T).getVertex(startIndex);
	start = new TEdge(v0, rv0);
	(*T).addEdge(start);
	prev = start;

	for(i = startIndex + 1; i <= limit0; i++){
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
	v1 = (*T).getVertex(startIndex);
	next = new TEdge(v1, rv3);
	(*T).addEdge(next);

	t = new Triangle(prev, (*v0).getEdgeTo(v1), next, v0, v1, rv3);
	t = new Triangle(next, start, re3, v1, rv0, rv3);
}