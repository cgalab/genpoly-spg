#include "point.h"
#include "polygon.h"
#include <stdlib.h>
#include <math.h>

enum class RPShapes {RPS_FLAT, RPS_QUADRATIC};

Polygon* generateRectangularPolygon(int n, enum RPShapes type);
Polygon* generateRegularPolygon(int n);