#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "triangulation.h"
#include "polygonTransformer.h"
#include "statistics.h"
#include "settings.h"
#include "randomGenerator.h"

int main(){
	Settings settings;
	Triangulation* T;
	int translations, performed;
	RandomGenerator generator;

	readSettings(settings);

	T = generateRegularPolygon(settings);

	(*T).check();
	printf("Initial polygon with %d vertices in regular shape computed after %f seconds\n", settings.getInitialSize(), settings.elapsedTime());


	performed = transformPolygonByMoves(settings, generator, T, settings.getInitialTN());
	printf("Transformed initial polygon with %d of %d translations in %f seconds\n\n", performed, settings.getInitialTN(), settings.elapsedTime());

	(*T).check();

	(*T).print("triangulation_init.graphml");
	(*T).printPolygon("polygon_int.graphml");

	growPolygon(settings, generator, T);
	printf("Grew initial polygon to %d vertices afters %f seconds \n\n", settings.getTargetSize(), settings.elapsedTime());
	
	(*T).check();

	performed = transformPolygonByMoves(settings, generator, T, 1000000);
	printf("Transformed initial polygon with %d of %d translations in %f seconds\n\n", performed, 1000000, settings.elapsedTime());

	(*T).check();
	
	//(*T).print("triangulation.graphml");

	(*T).printPolygon("polygon.graphml");
}