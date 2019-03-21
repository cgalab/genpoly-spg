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
   	Point* getVertex(int i){ 
			int n = vertices.size();			
			
			if(i == -1){   		
   			return vertices[n - 1];
   		}else if(i == n){
				  return vertices[0]; 		
   		}else if(i > -1 && i < n){
				return vertices[i];   		
   		}else{
				return NULL;// TODO: some error handling   		
   		}
   	}
   	void replaceVertex(Point* v, int i){ vertices[i] = v;}

};

#endif