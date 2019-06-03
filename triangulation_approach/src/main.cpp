#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "triangulation.h"
#include "polygonTransformer.h"
#include "timer.h"
#include "statistics.h"
#include "eventQueue.h"

int main(){
	Triangulation* T;
	int n = 1000000;
	int translations;
	Timer t;
	int performed;
	int initialSize = 100;
	int factor = 1000;

	t.start();
	if(n <= initialSize){
		T = generateRegularPolygon(n);
		translations = n * factor;
	}
	else{
		T = generateRegularPolygon(initialSize);
		translations = factor * initialSize;
	}
	(*T).check();
	printf("Initial polygon with %d vertices in regular shape computed after %f seconds\n", (*T).getActualNumberOfVertices(), t.elapsedTime());


	performed = transformPolygonByMoves(T, translations, t);
	printf("Transformed initial polygon with %d of %d translations in %f seconds\n\n", performed, translations, t.elapsedTime());

	(*T).check();

	//(*T).stretch(1000);

	(*T).print("triangulation_init.graphml");
	//printStats(p);

	(*T).printPolygon("polygon_int.graphml");

	growPolygon(T, n, t);
	printf("Grew initial polygon to %d vertices afters %f seconds \n", n, t.elapsedTime());
	
	//(*T).check();
	
	//(*T).print("triangulation.graphml");
	//printStats(p);

	//(*T).printPolygon("polygon.graphml");
}

/*int main(){
	EventQueue Q(0.01);
	Triangle* t = NULL;
	int i = 1;

	printf("1. insertion t = 0.21, result %d \n", Q.insert(0.21, t));
	Q.print();
	printf("2. insertion t = 0.24, result %d \n", Q.insert(0.24, t));
	Q.print();
	printf("3. insertion t = 0.21001, result %d \n", Q.insert(0.21001, t));
	Q.print();
	printf("4. insertion t = 0.23999, result %d \n", Q.insert(0.23999, t));
	Q.print();
	printf("5. insertion t = 0.11, result %d \n", Q.insert(0.11, t));
	Q.print();
	printf("6. insertion t = 0.81, result %d \n", Q.insert(0.81, t));
	Q.print();

	while(Q.size() > 0){
		printf("%d. pop: time = %f size after: %d \n", i, Q.pop().first, Q.size());
		printf("size after: %d \n", Q.size());
		Q.print();

		i++;
	}
	

}*/