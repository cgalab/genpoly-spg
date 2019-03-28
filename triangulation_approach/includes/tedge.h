#include <list>
#include <stdio.h>

#ifndef __TEDGE_H_
#define __TEDGE_H_

class Triangle;
class Vertex;

#include "triangle.h"
#include "vertex.h"

class TEdge {

private:
	Vertex* v1;
	Vertex* v2;

	Triangle* t1;
	Triangle* t2;

	bool isPE; // polygon edge?
	bool isRE; // rectangle edge?

	int id;
	static int n;

public:
	TEdge(Vertex* V1, Vertex* V2);
	TEdge(Vertex* V1, Vertex* V2, bool PEdge, bool REdge);

	void setPEdge(bool isPEdge){
		isPE = isPEdge;
	}

	bool isPEdge(){ return isPE;}
	bool isREdge(){ return isRE;}

	void setTriangle(Triangle* t);

	void print(FILE* f);

	void print();

	Vertex* getV1();
	Vertex* getV2();
};

#endif