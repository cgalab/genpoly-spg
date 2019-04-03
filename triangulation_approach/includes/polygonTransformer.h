#include "triangulation.h"
#include "randomGenerator.h"
#include <stdio.h>
#include "tedge.h"
#include "vertex.h"
#include "timer.h"
#include "triangle.h"
#include <list>
#include <vector>

void transformPolygon(Triangulation* T, int iterations, Timer t);
bool checkSimplicityOfTranslation(Triangulation* T, int index, Vertex *oldV, Vertex* newV);