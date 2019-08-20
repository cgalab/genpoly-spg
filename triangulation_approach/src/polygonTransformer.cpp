#include "polygonTransformer.h"
#include <string>
#include <math.h>

int transformPolygonByMoves(Triangulation* T, int iterations){
	int index = 0;
	double dx = 0, dy = 0, stddev, alpha, r; // radius of the initial polygon was 30
	bool simple, overroll;
	Translation* trans;
	int n = (*T).getActualNumberOfVertices();
	int performedTranslations = 0;
	Vertex* v;
	enum Executed ex;
	int div;

	div = 0.01 * iterations;

	for(int i = 0; i < iterations; i++){

		index = (*Settings::generator).getRandomIndex(n);

		v = (*T).getVertex(index);

		alpha = (*Settings::generator).getTranslationUniform(- M_PI, M_PI);
		stddev = (*v).getDirectedEdgeLength(alpha);

		r = (*Settings::generator).getTranslationNormal(stddev / 2, stddev / 6);

		dx = r * cos(alpha);
		dy = r * sin(alpha);

		trans = new Translation(T, index, dx, dy);

		overroll = (*trans).checkOverroll();

		if(!overroll){
			simple = (*trans).checkSimplicityOfTranslation();

			if(simple){
				(*trans).checkSplit();

				ex = (*trans).execute();
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

void growPolygon(Triangulation* T){
	int n, index, actualN, i;
	Insertion *in;
	bool ok;
	int div;
	int counter = 0;

	n = Settings::targetSize - (*T).getActualNumberOfVertices();
	div = 0.01 * n;

	for(i = 0; i < n;){
		
		actualN = (*T).getActualNumberOfVertices();

		index = (*Settings::generator).getRandomIndex(actualN);

		in = new Insertion(T, index);

		ok = (*in).checkStability();
		if(!ok){
			delete in;

			counter++;
			if(counter > 10000)
				printf("%d tries to find a suitable edge to insert in\n", counter);
			
			continue;
		}

		counter = 0;

		(*in).execute();		
		(*in).translate();

		delete in;

		i++;

		if(i % div == 0 && Settings::feedback != FeedbackMode::LACONIC)
			printf("%f%% of %d insertions performed after %f seconds \n", (double)i / (double)n * 100, n, (*Settings::timer).elapsedTime());
	}
}