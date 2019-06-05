#include "vertex.h"
#include "triangulation.h"
#include "tedge.h"
#include "triangle.h"
#include "translation.h"
#include "randomGenerator.h"
#include <string>
#include "settings.h"

#ifndef __INSERTION_H_
#define __INSERTION_H_

class Insertion{

private:

	Triangulation* T;

	int i;
	Vertex *v0, *v1;
	Vertex *newV;
	TEdge* e;

public:

	// Constructors
	Insertion(Triangulation* t, int index);

	// Others
	bool lengthControll();
	void execute();
	void translate(Settings & settings, RandomGenerator* generator);
};

#endif