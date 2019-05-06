#include "polygonTransformer.h"
#include <string>
#include <math.h>

int transformPolygon(Triangulation* T, int iterations, Timer t){
	int index = 1;
	RandomGenerator* generator = new RandomGenerator(); // could maybe be an object instead of a pointer
	double dx = 0, dy = 0, stddev, alpha, r; // radius of the initial polygon was 30
	bool simple, split, overroll;
	Translation* trans;
	int n = (*T).getActualNumberOfVertices();
	std::string filename;
	int performedTranslations = 0;
	Vertex* v;

	for(int i = 0; i < iterations; i++){
		//filename = "output/triangulation" + std::to_string(i) + ".graphml";
		//(*T).print(filename.c_str());

		index = (*generator).getRandomIndex(n);

		v = (*T).getVertex(index);

		alpha = (*generator).getTranslationUniform(- M_PI, M_PI);
		stddev = (*v).getDirectedEdgeLength(alpha);

		r = (*generator).getTranslationUniform(stddev / 2, stddev / 4);

		dx = r * cos(alpha);
		dy = r * sin(alpha);

		trans = new Translation(T, index, dx, dy);

		overroll = (*trans).checkOverroll();

		if(!overroll){
			simple = (*trans).checkSimplicityOfTranslation();

			if(simple){
				(*trans).checkSplit();

				(*trans).execute();
				performedTranslations++;
			}

			delete trans;
		}

		if(i % 1000 == 0)
			printf("%f%% of %d translations performed after %f seconds \n", (double)i / (double)iterations * 100, iterations, t.elapsedTime());
	}

	return performedTranslations;
}