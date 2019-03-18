#include <stdlib.h>
#include "point.h"
#include <vector>

#ifndef __POLYGON_H_
#define __POLYGON_H_

class Polygon{
	public:
   	std::vector<Point*> vertices;
		
		Polygon(int n){ vertices.reserve(n); }
   	void addVertex(Point* v){ vertices.push_back(v);}
   	int getNumberOfVertices(){ return vertices.size();}
   	Point* getVertex(int i){ return vertices[i]; }
   	void replaceVertex(Point* v, int i){ vertices[i] = v;}

};

#endif