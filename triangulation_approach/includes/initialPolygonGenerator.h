#include "vertex.h"
#include "triangulation.h"
#include "tedge.h"
#include "triangle.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "settings.h"

enum class RPShapes {RPS_FLAT, RPS_QUADRATIC};

//Polygon* generateRectangularPolygon(int n, enum RPShapes type);
Triangulation* generateRegularPolygon();
void initialTriangulationPseudoStar(Triangulation* T);
void initialTriangulationZigZag(Triangulation* T);
void boxPolygon(Triangulation* T, int startIndex);