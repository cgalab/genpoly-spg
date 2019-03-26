#include "polygonTransformer.h"

/*void transformPolygon(Polygon* p, int iterations, Timer t){
	int n = (*p).getNumberOfVertices();
	int i, index;
	bool accepted;
	RandomGenerator *generator = new RandomGenerator();
	Point *original, *translated;
	int tries;
	double stddev = 5.0;
 	
	for(i = 0; i < iterations; i++){
		index = (*generator).getRandomIndex(n);
		original = (*p).getVertex(index);
		
		tries = 0;
		accepted = false;
		while(!accepted){
			translated = (*generator).translatePointNormal(original, 0.0, stddev);
			(*p).replaceVertex(translated, index);
			accepted = checkSimplicityOfTranslation(p, index);
			tries++;
		}
		printf("Translation %d took %d tries and finished after %f seconds \n", i, tries, t.elapsedTime());
		
		free(original);
	}
	
}*/

void transformPolygon(Triangulation* T, int iterations, Timer t){
	int index = 1;
	RandomGenerator* generator = new RandomGenerator();
	double dx = 3, dy = 0;
	Vertex *newV, *oldV;

	oldV = (*T).getVertex(index);
	newV = (*oldV).getTranslated(dx, dy);

	checkSimplicityAndExecute(T, newV, index);
}

void checkSimplicityAndExecute(Triangulation* T, Vertex* newV, int index){
	Vertex *prev, *next, *oldV;
	TEdge *toPrev, *toNext;
	std::list<Triangle*> prevTriangles;
	std::list<Triangle*> nextTriangles;

	// get old vertex
	oldV = (*T).getVertex(index);

	// get neighboring vertices
	prev = (*T).getVertex(index - 1);
	next = (*T).getVertex(index + 1);

	// compute edges from neighbors to new vertex
	toPrev = new TEdge(prev, newV);
	toNext = new TEdge(newV, next);

	// get all triangles of the neighboring vertices which do not contain the old vertex
	prevTriangles = (*prev).getAllAdjacentTrianglesNotContaining(oldV);
	nextTriangles = (*next).getAllAdjacentTrianglesNotContaining(oldV);

	// print the triangle lists
	printf("Triangles of the previous vertex:\n");
	for(auto const& i : prevTriangles){
		(*i).print();
	}
	printf("Triangles of the next vertex:\n");
	for(auto const& i : nextTriangles){
		(*i).print();
	}


}

/*bool checkSimplicityOfTranslation(Polygon* p, int index){
	Point* newPoint = (*p).getVertex(index);
	Point* prevPoint = (*p).getVertex(index - 1);
	Point* nextPoint = (*p).getVertex(index + 1);
	int n = (*p).getNumberOfVertices();
	int i;
	Edge prev(prevPoint, newPoint);
	Edge next(newPoint, nextPoint);
	Edge e;
	int intersectionLevel = 0; // should be exactly 8 after the whole check...why not 6???
	
	for(i = 0; i <= n && intersectionLevel < 9; i++){
		e.setVertices((*p).getVertex(i - 1), (*p).getVertex(i));
		intersectionLevel += checkIntersection(prev, e);
		intersectionLevel += checkIntersection(e, next);		
	}

	if(intersectionLevel < 9) return true;
	else return false;
}*/
