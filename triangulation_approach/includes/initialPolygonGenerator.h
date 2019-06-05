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
Triangulation* generateRegularPolygon(Settings &settings);
void initialTriangulationPseudoStar(Triangulation* T, int n);
void initialTriangulationStar(Triangulation* T, int n);
void initialTriangulationZigZag(Triangulation* T, int n);
void boxPolygon(Settings &settings, Triangulation* T, int startIndex);