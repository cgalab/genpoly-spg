#include "polygonTransformer.h"
#include <string>

int transformPolygon(Triangulation* T, int iterations, Timer t){
	int index = 1;
	RandomGenerator* generator = new RandomGenerator(); // could maybe be a object instead of a pointer
	double dx = 0, dy = -50; // radius of the initial polygon was 30
	bool simple, split, overroll;
	Translation* trans;
	int n = (*T).getNumberOfVertices();
	std::string filename;
	int performedTranslations = 0;

	for(int i = 0; i < iterations; i++){
		//filename = "output/triangulation" + std::to_string(i) + ".graphml";
		//(*T).print(filename.c_str());

		index = (*generator).getRandomIndex(n);

		dx = (*generator).getTranslationNormal(0, 5);
		dy = (*generator).getTranslationNormal(0, 5);
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
	}

	return performedTranslations;
}