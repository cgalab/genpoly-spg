#include "polygonTransformer.h"

/*
	The function transformPolygonByMoves() transforms a polygon by randomly chosing a
	vertex and a direction to move and then randomly computing a distance to shift in
	the choosen direction which with the constraint that the movement should be
	applicable with reasonable effort. This is done iterations times.

	@param 	T 			The triangulation the polygon lives in
	@param 	iterations	The number of moves which should be tried
	@return 			The number of moves which were executed at least partially

	Note:
		- The number of really performed moves is most likely less then the number of moves
			which should be tried
		- For polygons with holes the vertex also is reandomly choosen, i.e. it can be from
			the outer polygon as well as of one of the inner polygons
*/
int transformPolygonByMoves(Triangulation *T, int iterations){
	int index = 0;
	double dx = 0, dy = 0, stddev, alpha, r;
	bool simple, overroll;
	Translation *trans;
	int n = (*T).getActualNumberOfVertices();
	int performedTranslations = 0;
	Vertex *v;
	enum Executed ex;
	int div;

	printf("n: %d\n", n);

	div = 0.01 * iterations;

	// Try the given number of moves
	for(int i = 0; i < iterations; i++){

		// Chose a vertex randomly
		index = (*Settings::generator).getRandomIndex(n);

		v = (*T).getVertex(index);

		// Chose a direction randomly and get a suitable stddev for the distance
		alpha = (*Settings::generator).getDoubleUniform(- M_PI, M_PI);
		stddev = (*v).getDirectedEdgeLength(alpha);

		// Generate a random distance
		r = (*Settings::generator).getDoubleNormal(stddev / 2, stddev / 6);

		// Split the translation into x- and y-components
		dx = r * cos(alpha);
		dy = r * sin(alpha);

		trans = new Translation(T, index, dx, dy);

		// Check for an overroll
		overroll = (*trans).checkOverroll();

		if(!overroll){
			// Check whether the translation leads to a simple polygon
			simple = (*trans).checkSimplicityOfTranslation();

			if(simple){
				// Check whether the translation can be performed directly or must be split
				(*trans).checkSplit();

				// Try to execute the translation
				ex = (*trans).execute();

				// Count executed translations
				if(ex != Executed::REJECTED)
					performedTranslations++;
			}
		}

		delete trans;

		if(i % div == 0 && Settings::feedback != FeedbackMode::LACONIC)
			printf("%f%% of %d translations performed after %f seconds \n", (double)i / (double)iterations * 100, iterations, (*Settings::timer).elapsedTime());
	}

	return performedTranslations;
}

/*
	The function growPolygon() grows a polygon by n insertions.

	@param 	T 		The triangulation the polygon lives in
	@param	pID 	The ID of the polygon
	@param 	n 		The number of vertices to insert

	Note:
		This function works just for polygons without holes!
*/
void growPolygonBy(Triangulation *T, unsigned int pID, int n){
	int index, actualN, i;
	Insertion *in;
	bool ok;
	int div;
	int counter = 0;

	div = 0.01 * n;
	if(div == 0)
		div = 1;

	for(i = 0; i < n;){

		actualN = (*T).getActualNumberOfVertices(pID);

		// Chose randomly an edge to insert in
		index = (*Settings::generator).getRandomIndex(actualN);

		in = new Insertion(T, pID, index);

		// Check whether the choosen edge fulfills the stability criteria for insertions
		ok = (*in).checkStability();
;
		// Recognized when it is hard to find a suitable edge to insert in
		if(!ok){
			delete in;

			counter++;
			if(counter > 10000)
				printf("%d tries to find a suitable edge to insert in\n", counter);
			
			continue;
		}

		counter = 0;

		// Execute the insertion and try to move the new vertex away from the edge
		(*in).execute();

		(*in).translate();

		delete in;

		// Just increase the iteration count if a vertex has really been inserted
		i++;

		if(i % div == 0 && Settings::feedback != FeedbackMode::LACONIC)
			printf("%f%% of %d insertions performed after %f seconds \n", (double)i / (double)n * 100, n, (*Settings::timer).elapsedTime());
	}
}