#include <list>
#include <stdio.h>


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

	void addEdge(TEdge* e);

	void print(FILE* f){
		fprintf(f, "<node positionX=\"%f\" positionY=\"%f\" id=\"%d\" mainText=\"%d\"></node>\n", x * 20, y * 20, id, id);
	}

	int getID(){ return id;}
};

#endif