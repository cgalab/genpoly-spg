#include "points.h"
#include "polygon.h"
#include <stdlib.h>

enum class RPShapes {RPS_FLAT, RPS_QUADRATIC};

Polygon* generateRectangularPolygon(int n, enum RPShapes type);