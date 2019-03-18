#include "polygonTransformer.h"

void transformPolygon(Polygon* p, int iterations){
	int n = (*p).getNumberOfVertices();
	int i, index;
	RandomGenerator *generator = new RandomGenerator();
	
	
	for(i = 0; i < iterations; i++){
		index = (*generator).getRandomIndex(n);
		printf("iteration: %d vertex index: %d \n", i, index);	
	}
	
}
