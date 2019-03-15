#include <stdlib.h>
#include "points.h"
#include <vector>

#ifndef __POLYGON_H_
#define __POLYGON_H_

class Polygon{
  public:
    std::vector<Point> vertices;

    void addVertex(Point p){ vertices.push_back(p);}
    int getNumberOfVertices(){ return vertices.size();}

};

#endif