#include "polygonTransformer.h"

void transformPolygon(Polygon* p, int iterations){
	int n = (*p).getNumberOfVertices();
	int i, index;
	bool accepted;
	RandomGenerator *generator = new RandomGenerator();
	Point *original, *translated;
	int tries;
	
	printf("Start translation:\n");   	
	for(i = 0; i < iterations; i++){
		index = (*generator).getRandomIndex(n);
		original = (*p).getVertex(index);
		
		tries = 0;
		accepted = false;
		while(!accepted){
			translated = (*generator).translatePointNormal(original, 0.0, 10.0);
			(*p).replaceVertex(translated, index);
			accepted = checkSimplicityOfTranslation(p, index);
			tries++;
		}
		printf("Translation %d took %d tries \n", i, tries);
		
		free(original);	
	}
	
}

bool checkSimplicityOfTranslation(Polygon* p, int index){
	Point* newPoint = (*p).getVertex(index);
	Point* prevPoint = (*p).getVertex(index - 1);
	Point* nextPoint = (*p).getVertex(index + 1);
	int n = (*p).getNumberOfVertices();
	int i;
	Edge prev(prevPoint, newPoint);
	Edge next(newPoint, nextPoint);
	Edge e;
	int intersectionLevel = 0; // must stay be exactly 8 after the whole check...why not 6???
	
	for(i = 0; i <= n && intersectionLevel < 9; i++){
		e.setVertices((*p).getVertex(i - 1), (*p).getVertex(i));
		intersectionLevel += checkIntersection(prev, e);
		intersectionLevel += checkIntersection(e, next);		
	}
	printf("intersection level: %d \n", intersectionLevel);
	if(intersectionLevel < 9) return true;
	else return false;
}
