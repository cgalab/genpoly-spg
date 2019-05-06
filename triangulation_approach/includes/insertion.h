#include "vertex.h"
#include "triangulation.h"
#include "tedge.h"
#include "triangle.h"



#ifndef __INSERTION_H_
#define __INSERTION_H_

class Insertion{

private:

	Triangulation* T;

	int i;
	Vertex *v0, *v1;
	TEdge* e;

public:

	// Constructors
	Insertion(Triangulation* t, int index);

	// Others
	void execute();

	// Destructors
	~Insertion();

};

#endif