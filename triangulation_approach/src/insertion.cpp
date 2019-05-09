#include "insertion.h"

// Constructors

Insertion::Insertion(Triangulation* t, int index){
	T = t;

	i = index;
	v0 = (*T).getVertex(i);
	v1 = (*v0).getNext();
	e = (*v0).getToNext();
}

// Others
void Insertion::execute(){
	Triangle *t0, *t1;
	double x, y;
	Vertex *other0, *other1;
	TEdge *fromV0ToOther0, *fromV0ToOther1, *fromV1ToOther0, *fromV1ToOther1;
	TEdge *fromV0ToNew, *fromV1ToNew;
	TEdge *fromNewToOther0, *fromNewToOther1;

	x = (*v0).getX() + ((*v1).getX() - (*v0).getX()) / 2;
	y = (*v0).getY() + ((*v1).getY() - (*v0).getY()) / 2;

	newV = new Vertex(x, y);
	(*T).addVertex(newV);

	t0 = (*e).getT1();
	t1 = (*e).getT2();

	other0 = (*t0).getOtherVertex(e);
	other1 = (*t1).getOtherVertex(e);

	delete e;

	fromV0ToOther0 = (*v0).getEdgeTo(other0);
	fromV0ToOther1 = (*v0).getEdgeTo(other1);
	fromV1ToOther0 = (*v1).getEdgeTo(other0);
	fromV1ToOther1 = (*v1).getEdgeTo(other1);

	fromV0ToNew = new TEdge(v0, newV, EdgeType::POLYGON);
	fromV1ToNew = new TEdge(newV, v1, EdgeType::POLYGON);
	(*T).addEdge(fromV0ToNew);
	(*T).addEdge(fromV1ToNew);

	fromNewToOther0 = new TEdge(newV, other0);
	fromNewToOther1 = new TEdge(newV, other1);
	(*T).addEdge(fromNewToOther0);
	(*T).addEdge(fromNewToOther1);

	new Triangle(fromV0ToNew, fromV0ToOther0, fromNewToOther0, v0, newV, other0);
	new Triangle(fromV0ToNew, fromV0ToOther1, fromNewToOther1, v0, newV, other1);
	new Triangle(fromV1ToNew, fromV1ToOther0, fromNewToOther0, v1, newV, other0);
	new Triangle(fromV1ToNew, fromV1ToOther1, fromNewToOther1, v1, newV, other1);
}

void Insertion::translate(RandomGenerator* generator){
	int index;
	bool overroll, simple = false;
	double alpha, stddev, r, dx, dy;
	Translation* trans;
	int count = 0;

	index = (*T).getActualNumberOfVertices() - 1;

	while(!simple && count < 100){
		alpha = (*generator).getTranslationUniform(- M_PI, M_PI);
		stddev = (*newV).getDirectedEdgeLength(alpha);

		r = (*generator).getTranslationNormal(stddev / 2, stddev / 6);

		dx = r * cos(alpha);
		dy = r * sin(alpha);

		trans = new Translation(T, index, dx, dy);

		overroll = (*trans).checkOverroll();

		if(!overroll){
			simple = (*trans).checkSimplicityOfTranslation();

			if(simple){
				(*trans).checkSplit();

				(*trans).execute();
			}
		}

		delete trans;

		count++;
	}
}