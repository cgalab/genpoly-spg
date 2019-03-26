#include "triangulation.h"
#include "randomGenerator.h"
#include <stdio.h>
#include "tedge.h"
#include "vertex.h"
#include "timer.h"
#include "triangle.h"
#include <list>

void transformPolygon(Triangulation* T, int iterations, Timer t);
void checkSimplicityAndExecute(Triangulation* T, Vertex* newV, int index);
//bool checkSimplicityOfTranslation(Polygon* p, int index);