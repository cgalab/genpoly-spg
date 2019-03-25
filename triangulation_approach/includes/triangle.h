#include <list>

#ifndef __TRIANGLE_H_
#define __TRIANGLE_H_

class TEdge;

#include "tedge.h"

class Triangle {

private:
	TEdge* e0;
	TEdge* e1;
	TEdge* e2;

public:
	Triangle(TEdge* E0, TEdge* E1, TEdge* E2);
};

#endif