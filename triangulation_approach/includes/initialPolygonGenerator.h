#include "vertex.h"
#include "triangulation.h"
#include "tedge.h"
#include "triangle.h"
#include <stdlib.h>
#include <math.h>

enum class RPShapes {RPS_FLAT, RPS_QUADRATIC};

//Polygon* generateRectangularPolygon(int n, enum RPShapes type);
Triangulation* generateRegularPolygon(int n);