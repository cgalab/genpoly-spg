#include "vertex.h"
#include "tedge.h"
#include "triangle.h"
#include <vector>

#ifndef __TRIANGULATION_H_
#define __TRIANGULATION_H_

class Triangulation {

private:
	std::vector<Vertex*> vertices;

public:
	Triangulation(int n){ vertices.reserve(n); }
	void addVertex(Vertex* v){
		vertices.push_back(v);
	}
	Vertex* getVertex(int index){
		return vertices[index];
	}
};

#endif