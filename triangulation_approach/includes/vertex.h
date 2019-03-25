#include <list>


#ifndef __VERTEX_H_
#define __VERTEX_H_

class TEdge;

#include "tedge.h"

class Vertex {

private:
	double x;
	double y;
	int id;
	std::list<TEdge*> edges;

public:
	Vertex(double X, double Y, int ID){ 
		x = X,
		y = Y;
		id = ID;
	}

};

#endif