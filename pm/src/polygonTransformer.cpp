#include "polygonTransformer.h"

void transformPolygon(Polygon* p, int iterations){
	int n = (*p).getNumberOfVertices();
	int i, index;
	RandomGenerator *generator = new RandomGenerator();
	Point *original, *translated;
	
	
	for(i = 0; i < iterations; i++){
		index = (*generator).getRandomIndex(n);
		original = (*p).getVertex(index);
		translated = (*generator).translatePointNormal(original, 0.0, 10.0);
		(*p).replaceVertex(translated, index);
		
		free(original);	
	}
	
}
